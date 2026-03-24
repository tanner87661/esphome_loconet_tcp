#include "loconet_tcp.h"
#include "esphome/core/log.h"

namespace esphome {
namespace loconet_tcp {

static const char *const TAG = "loconet_tcp";

void LoconetTCP::setup() {
  this->connect_();
}

void LoconetTCP::loop() {
  if (!client_.connected()) {
    if (millis() - last_connect_attempt_ > 5000) {  // retry every 5 s – keeps connection "permanently on"
      this->connect_();
    }
    return;
  }

  while (client_.available()) {
    char c = client_.read();
    buffer_ += c;
    if (buffer_.size() >= 2 && buffer_.substr(buffer_.size() - 2) == "\r\n") {
      std::string line = buffer_.substr(0, buffer_.size() - 2);
      buffer_.clear();
      this->process_line_(line);
    }
  }
}

void LoconetTCP::connect_() {
  last_connect_attempt_ = millis();
  ESP_LOGI(TAG, "Connecting to %s:%d ...", host_.c_str(), port_);
  if (client_.connect(host_.c_str(), port_)) {
    ESP_LOGI(TAG, "TCP connected (permanent keep-alive)");
    buffer_.clear();
  } else {
    ESP_LOGW(TAG, "Connect failed, will retry");
  }
}

void LoconetTCP::process_line_(const std::string &line) {
  ESP_LOGV(TAG, "Received: %s", line.c_str());

  if (line.rfind("RECEIVE", 0) == 0 && receive_trigger_) {
    receive_trigger_->trigger(line);
  } else if (line.rfind("SENT OK", 0) == 0 && sent_ok_trigger_) {
    sent_ok_trigger_->trigger(line);
  }
}

void LoconetTCP::send(const std::string &data) {
  if (client_.connected()) {
    client_.print(data);
    client_.print("\r\n");
    client_.flush();
    ESP_LOGD(TAG, "Sent: %s", data.c_str());
  } else {
    ESP_LOGW(TAG, "Cannot send – not connected");
  }
}

void LoconetTCP::dump_config() {
  ESP_LOGCONFIG(TAG, "LoConet TCP Client:");
  ESP_LOGCONFIG(TAG, "  Host: %s", host_.c_str());
  ESP_LOGCONFIG(TAG, "  Port: %d", port_);
}

}  // namespace loconet_tcp
}  // namespace esphome