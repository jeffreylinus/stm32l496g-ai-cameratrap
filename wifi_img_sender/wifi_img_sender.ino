#include <Arduino.h>
#include <Wire.h>
#include <WiFi.h>

extern "C" {
  #include "driver/sdmmc_host.h"
  #include "sdmmc_cmd.h"
  #include "esp_vfs_fat.h"
}

#include <dirent.h>
#include <sys/stat.h>
#include <unistd.h>

// ===================== USER CONFIG =====================

// I2C
constexpr uint8_t SLAVE_ADDR = 0x08;
constexpr int SDA_PIN = 4;
constexpr int SCL_PIN = 5;

// SDMMC pins for this board
constexpr gpio_num_t SD_CLK_PIN = GPIO_NUM_39;
constexpr gpio_num_t SD_CMD_PIN = GPIO_NUM_38;
constexpr gpio_num_t SD_D0_PIN  = GPIO_NUM_40;

const char* MOUNT_POINT = "/sdcard";

// Payload safety limit
constexpr uint32_t MAX_CROP_LEN = 200000;

// WiFi (adjust as needed)
const char* WIFI_SSID = "iPhone (281)";
const char* WIFI_PASS = "12345678";

WiFiServer httpServer(80);

// ===================== SD STATE =====================

static bool           g_sd_ready   = false;
static sdmmc_card_t*  g_sd_card    = nullptr;
static uint32_t       g_frame_idx  = 0;
static FILE*          g_fp         = nullptr;  // current frame file

// ===================== I2C RING BUFFER =====================
// single-producer (onReceive), single-consumer (loop)

constexpr size_t RX_Q_SIZE = 131072;   // power of 2
static uint8_t  rx_q[RX_Q_SIZE];
static volatile size_t rx_head = 0;
static volatile size_t rx_tail = 0;
static volatile uint32_t g_rx_dropped = 0;

static inline bool rx_queue_full() {
  return ((rx_head + 1) & (RX_Q_SIZE - 1)) == rx_tail;
}

static inline bool rx_queue_empty() {
  return rx_head == rx_tail;
}

static inline void rx_queue_push(uint8_t b) {
  size_t next = (rx_head + 1) & (RX_Q_SIZE - 1);
  if (next == rx_tail) {
    g_rx_dropped++;
    return;
  }
  rx_q[rx_head] = b;
  rx_head = next;
}

static inline bool rx_queue_pop(uint8_t &b) {
  if (rx_queue_empty()) return false;
  b = rx_q[rx_tail];
  rx_tail = (rx_tail + 1) & (RX_Q_SIZE - 1);
  return true;
}

// ===================== FRAME PARSER STATE =====================
//
// Protocol on I2C byte stream:
//
//   [8-byte header][crop_len bytes payload]
//
// header (little endian):
//   0-1: cw (uint16_t)
//   2-3: ch (uint16_t)
//   4-7: crop_len (uint32_t)
//
// For each frame, create: /sdcard/log_XXXXXX.bin
//   content = header(8) + payload(crop_len).
// =============================================================

enum class RxState {
  WAIT_HEADER,
  WAIT_PAYLOAD
};

static RxState   g_rx_state         = RxState::WAIT_HEADER;
static uint8_t   g_header[8];
static uint8_t   g_header_bytes     = 0;
static uint32_t  g_payload_expected = 0;
static uint32_t  g_payload_received = 0;

// fwd decls
static void reset_frame_state(bool delete_partial);
static bool sd_init();
static void unmount_sd();

// ===================== SD HELPERS =====================

static String make_frame_path(uint32_t idx) {
  char buf[64];
  snprintf(buf, sizeof(buf), "%s/log_%06lu.bin",
           MOUNT_POINT, (unsigned long)idx);
  return String(buf);
}

static void sd_close_frame_file() {
  if (g_fp) {
    fflush(g_fp);
    fclose(g_fp);
    g_fp = nullptr;
  }
}

static void sd_delete_file(const String& path) {
  if (unlink(path.c_str()) == 0) {
    Serial.printf("[SD] Deleted file %s\n", path.c_str());
  } else {
    Serial.printf("[SD] Failed to delete file %s\n", path.c_str());
  }
}

static bool sd_open_new_frame_file() {
  ++g_frame_idx;

  String path = make_frame_path(g_frame_idx);

  sd_close_frame_file(); // just in case

  g_fp = fopen(path.c_str(), "wb");
  if (!g_fp) {
    Serial.printf("[SD] fopen(%s) failed\n", path.c_str());
    return false;
  }

  Serial.printf("[I2C] New frame file: %s\n", path.c_str());
  return true;
}

static void reset_frame_state(bool delete_partial) {
  if (delete_partial && g_fp) {
    String path = make_frame_path(g_frame_idx);
    sd_close_frame_file();
    sd_delete_file(path);
  } else {
    sd_close_frame_file();
  }

  g_header_bytes     = 0;
  g_payload_expected = 0;
  g_payload_received = 0;
  g_rx_state         = RxState::WAIT_HEADER;
}

static bool sd_init() {
  sdmmc_host_t host = SDMMC_HOST_DEFAULT();
  host.flags = SDMMC_HOST_FLAG_1BIT;  // D0 only
  host.max_freq_khz = 400;            // safe init

  sdmmc_slot_config_t slot_config = SDMMC_SLOT_CONFIG_DEFAULT();
  slot_config.width   = 1;
  slot_config.clk     = SD_CLK_PIN;
  slot_config.cmd     = SD_CMD_PIN;
  slot_config.d0      = SD_D0_PIN;
  slot_config.gpio_cd = GPIO_NUM_NC;
  slot_config.gpio_wp = GPIO_NUM_NC;

  esp_vfs_fat_sdmmc_mount_config_t mount_config = {};
  mount_config.format_if_mount_failed = false;
  mount_config.max_files              = 8;
  mount_config.allocation_unit_size   = 16 * 1024;

  Serial.println("[SD] Mounting SDMMC...");

  esp_err_t ret = esp_vfs_fat_sdmmc_mount(
      MOUNT_POINT, &host, &slot_config, &mount_config, &g_sd_card);

  if (ret != ESP_OK) {
    Serial.printf("[SD] Mount failed: %s\n", esp_err_to_name(ret));
    return false;
  }

  Serial.println("[SD] Mount OK. Card info:");
  sdmmc_card_print_info(stdout, g_sd_card);
  return true;
}

static void unmount_sd() {
  if (!g_sd_ready) {
    Serial.println("[SD] Already unmounted.");
    return;
  }

  Serial.println("[SD] Unmounting SD card...");
  sd_close_frame_file();
  esp_vfs_fat_sdcard_unmount(MOUNT_POINT, g_sd_card);
  g_sd_card  = nullptr;
  g_sd_ready = false;
  Serial.println("[SD] Unmounted. It is now safe to remove the card.");
}

// ===================== I2C PARSER =====================

static void i2c_process_byte(uint8_t b) {
  if (!g_sd_ready) {
    return;
  }

  switch (g_rx_state) {

    case RxState::WAIT_HEADER: {
      g_header[g_header_bytes++] = b;

      if (g_header_bytes == 8) {
        uint16_t cw = (uint16_t)g_header[0] |
                      ((uint16_t)g_header[1] << 8);
        uint16_t ch = (uint16_t)g_header[2] |
                      ((uint16_t)g_header[3] << 8);

        g_payload_expected =
            (uint32_t)g_header[4] |
            ((uint32_t)g_header[5] << 8) |
            ((uint32_t)g_header[6] << 16) |
            ((uint32_t)g_header[7] << 24);

        Serial.printf(
          "[I2C] Header: cw=%u ch=%u crop_len=%u (raw len: %02X %02X %02X %02X)\n",
          cw, ch, g_payload_expected,
          g_header[4], g_header[5], g_header[6], g_header[7]
        );

        if (g_payload_expected == 0) {
          Serial.println("[I2C] Invalid crop_len, dropping frame and resyncing");
          reset_frame_state(true);
          return;
        }

        if (!sd_open_new_frame_file()) {
          Serial.println("[SD] ERROR: cannot open frame file, dropping");
          reset_frame_state(true);
          return;
        }

        // Write header to file
        size_t nw = fwrite(g_header, 1, 8, g_fp);
        if (nw != 8) {
          Serial.println("[SD] ERROR: fwrite header failed");
          reset_frame_state(true);
          return;
        }

        g_payload_received = 0;
        g_rx_state = RxState::WAIT_PAYLOAD;
      }
      break;
    }

    case RxState::WAIT_PAYLOAD: {
      if (g_fp) {
        uint8_t tmp = b;
        size_t nw = fwrite(&tmp, 1, 1, g_fp);
        if (nw != 1) {
          Serial.println("[SD] ERROR: fwrite payload failed, dropping frame");
          reset_frame_state(true);
          return;
        }
      }

      g_payload_received++;

      if (g_payload_received >= g_payload_expected) {
        String path = make_frame_path(g_frame_idx);
        Serial.printf(
          "[I2C] Frame complete: payload=%u bytes, file=%s\n",
          g_payload_received,
          path.c_str()
        );

        if (g_fp) {
          fflush(g_fp);
        }
        reset_frame_state(false);
      }
      break;
    }
  }
}

// ===================== I2C CALLBACK =====================

void onReceive(int len) {
  while (len-- > 0 && Wire.available()) {
    uint8_t b = Wire.read();
    rx_queue_push(b);
  }
}

// ===================== SERIAL COMMANDS =====================

static void handle_serial_commands() {
  if (!Serial.available()) return;

  String cmd = Serial.readStringUntil('\n');
  cmd.trim();
  cmd.toUpperCase();

  if (cmd == "STATUS") {
    Serial.printf(
      "[STATUS] state=%s, header_bytes=%u, payload_expected=%u, payload_received=%u, dropped=%lu, frame_idx=%lu, sd_ready=%d\n",
      (g_rx_state == RxState::WAIT_HEADER ? "WAIT_HEADER" : "WAIT_PAYLOAD"),
      g_header_bytes,
      g_payload_expected,
      g_payload_received,
      (unsigned long)g_rx_dropped,
      (unsigned long)g_frame_idx,
      g_sd_ready ? 1 : 0
    );
  } else if (cmd == "EJECT") {
    Serial.println("[CMD] EJECT: closing frame state and unmounting SD");
    reset_frame_state(false);
    unmount_sd();
  } else if (cmd == "MOUNT") {
    Serial.println("[CMD] MOUNT: trying to mount SD card");
    if (g_sd_ready) {
      Serial.println("[SD] Already mounted.");
    } else {
      g_sd_ready = sd_init();
      if (!g_sd_ready) {
        Serial.println("[SD] MOUNT failed.");
      } else {
        Serial.println("[SD] MOUNT OK.");
      }
    }
  } else {
    Serial.print("[CMD] Unknown: ");
    Serial.println(cmd);
    Serial.println("Commands: STATUS, EJECT, MOUNT");
  }
}

// ===================== TAR HELPERS (HTTP DOWNLOAD) =====================

static void tar_write_octal(char *field, size_t size, unsigned long value) {
  // size includes trailing NUL; use size-1 chars for digits
  char buf[32];
  snprintf(buf, sizeof(buf), "%0*lo", (int)(size - 1), value);
  size_t len = strlen(buf);
  if (len >= size) len = size - 1;
  memset(field, '0', size);
  memcpy(field + (size - 1 - len), buf, len);
  field[size - 1] = '\0';
}

static bool tar_write_file_header(WiFiClient &client,
                                  const char *name,
                                  unsigned long size) {
  uint8_t block[512];
  memset(block, 0, sizeof(block));

  // name
  strncpy((char*)block, name, 100);

  // mode
  memcpy(block + 100, "0000644", 7);

  // uid, gid
  memcpy(block + 108, "0000000", 7);
  memcpy(block + 116, "0000000", 7);

  // size
  tar_write_octal((char*)block + 124, 12, size);

  // mtime
  tar_write_octal((char*)block + 136, 12, 0);

  // typeflag
  block[156] = '0';

  // magic
  memcpy(block + 257, "ustar", 5);
  block[262] = ' ';
  block[263] = ' ';

  // checksum
  memset(block + 148, ' ', 8);
  unsigned long sum = 0;
  for (int i = 0; i < 512; ++i) {
    sum += block[i];
  }
  char chk[8];
  snprintf(chk, sizeof(chk), "%06lo", sum);
  memcpy(block + 148, chk, 6);
  block[154] = '\0';
  block[155] = ' ';

  size_t written = client.write(block, sizeof(block));
  return (written == sizeof(block));
}

static bool send_tar_of_logs(WiFiClient &client) {
  if (!g_sd_ready) {
    client.println("HTTP/1.1 500 Internal Server Error");
    client.println("Connection: close\r\n");
    client.println("SD not mounted");
    return false;
  }

  DIR *dir = opendir(MOUNT_POINT);
  if (!dir) {
    client.println("HTTP/1.1 500 Internal Server Error");
    client.println("Connection: close\r\n");
    client.println("Cannot open mount directory");
    return false;
  }

  // HTTP headers
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: application/x-tar");
  client.println("Content-Disposition: attachment; filename=logs.tar");
  client.println("Connection: close\r\n");

  bool any_sent = false;
  struct dirent *ent;

  while ((ent = readdir(dir)) != nullptr) {
    const char *name = ent->d_name;
    if (strcmp(name, ".") == 0 || strcmp(name, "..") == 0) continue;
    if (strncmp(name, "log_", 4) != 0) continue;

    char fullpath[128];
    snprintf(fullpath, sizeof(fullpath), "%s/%s", MOUNT_POINT, name);

    struct stat st;
    if (stat(fullpath, &st) != 0) continue;
    if (!S_ISREG(st.st_mode)) continue;

    unsigned long fsize = (unsigned long)st.st_size;

    if (!tar_write_file_header(client, name, fsize)) {
      closedir(dir);
      return false;
    }

    FILE* f = fopen(fullpath, "rb");
    if (!f) {
      closedir(dir);
      return false;
    }

    uint8_t buf[1024];
    unsigned long remaining = fsize;
    bool ok = true;

    while (remaining > 0) {
      size_t chunk = (remaining > sizeof(buf)) ? sizeof(buf) : remaining;
      size_t nr = fread(buf, 1, chunk, f);
      if (nr == 0) {
        ok = false;
        break;
      }
      size_t nw = client.write(buf, nr);
      if (nw != nr) {
        ok = false;
        break;
      }
      remaining -= nr;
    }

    // pad to 512-byte boundary
    if (ok) {
      size_t pad = (512 - (fsize % 512)) % 512;
      if (pad) {
        uint8_t zeros[512];
        memset(zeros, 0, pad);
        if (client.write(zeros, pad) != pad) {
          ok = false;
        }
      }
    }

    fclose(f);

    if (!ok) {
      closedir(dir);
      return false;
    }

    if (unlink(fullpath) == 0) {
      Serial.printf("[HTTP] Deleted sent file %s\n", fullpath);
    } else {
      Serial.printf("[HTTP] Failed to delete %s\n", fullpath);
    }

    any_sent = true;
  }

  closedir(dir);

  // end-of-archive: two 512-byte zero blocks
  uint8_t zeros[512];
  memset(zeros, 0, sizeof(zeros));
  if (client.write(zeros, sizeof(zeros)) != sizeof(zeros)) return false;
  if (client.write(zeros, sizeof(zeros)) != sizeof(zeros)) return false;

  if (!any_sent) {
    Serial.println("[HTTP] No log_*.bin files to send (empty tar sent)");
  }

  return true;
}

// ===================== HTTP HANDLER =====================

static void handle_http_client() {
  WiFiClient client = httpServer.available();
  if (!client) return;

  String req = client.readStringUntil('\r');
  client.readStringUntil('\n'); // consume '\n'

  Serial.print("[HTTP] Request: ");
  Serial.println(req);

  if (req.startsWith("GET /download")) {
    bool ok = send_tar_of_logs(client);
    client.stop();
    Serial.printf("[HTTP] /download done, success=%d\n", ok ? 1 : 0);
    return;
  }

  // default response
  client.println("HTTP/1.1 200 OK");
  client.println("Content-Type: text/plain");
  client.println("Connection: close\r\n");
  client.println("ESP32-S3 I2C→SD Logger");
  client.stop();
}

// ===================== SETUP / LOOP =====================

void setup() {
  Serial.begin(115200);
  delay(1000);
  Serial.println("\n=== ESP32-S3 I2C→SD LOGGER with HTTP ===");
  Serial.println("Serial commands: STATUS, EJECT, MOUNT");

  // SD
  g_sd_ready = sd_init();
  if (!g_sd_ready) {
    Serial.println("[SD] NOT READY – frames will be dropped");
  } else {
    Serial.println("[SD] READY");
  }

  // I2C slave
  bool ok = Wire.begin((uint8_t)SLAVE_ADDR);
  Serial.printf("[I2C] Wire.begin returned: %d\n", ok);
  Wire.onReceive(onReceive);
  Serial.printf("[I2C] Slave ready at 0x%02X on SDA=%d SCL=%d\n",
                SLAVE_ADDR, SDA_PIN, SCL_PIN);

  // WiFi
  WiFi.mode(WIFI_STA);
  WiFi.begin(WIFI_SSID, WIFI_PASS);
  Serial.print("[WiFi] Connecting");
  while (WiFi.status() != WL_CONNECTED) {
    Serial.print(".");
    delay(300);
  }
  Serial.println();
  Serial.printf("[WiFi] Connected, IP: %s\n",
                WiFi.localIP().toString().c_str());

  httpServer.begin();
  Serial.println("[HTTP] Server listening on port 80");
}

void loop() {
  handle_serial_commands();

  // drain I2C queue
  uint8_t b;
  while (rx_queue_pop(b)) {
    i2c_process_byte(b);
  }

  // periodic flush of open frame
  if (g_fp) {
    fflush(g_fp);
  }

  // monitor dropped bytes
  static uint32_t last_dropped = 0;
  if (g_rx_dropped != last_dropped) {
    Serial.printf("[I2C] DROPPED bytes in ring buffer: %lu\n",
                  (unsigned long)g_rx_dropped);
    last_dropped = g_rx_dropped;
  }

  handle_http_client();

  delay(1);
}
