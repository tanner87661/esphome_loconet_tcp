#pragma once

#include "esphome/core/component.h"
#include "esphome/core/automation.h"
#include <WiFiClient.h>
#include <string>

namespace esphome {
namespace loconet_tcp {

class LoconetTCP : public Component {
 public:
  void setup() override;
  void loop() override;
  void dump_config() override;

  void set_host(const std::string &host) { host_ = host; }
  void set_port(uint16_t port) { port_ = port; }

  void set_receive_trigger(Trigger<std::string> *trigger) { receive_trigger_ = trigger; }
  void set_sent_ok_trigger(Trigger<std::string> *trigger) { sent_ok_trigger_ = trigger; }

  // Action you can call from YAML: id(my_loconet).send("COMMAND")
  void send(const std::string &data);

 protected:
  std::string host_;
  uint16_t port_{1234};
  WiFiClient client_;
  std::string buffer_;
  uint32_t last_connect_attempt_{0};
  Trigger<std::string> *receive_trigger_{nullptr};
  Trigger<std::string> *sent_ok_trigger_{nullptr};

  void connect_();
  void process_line_(const std::string &line);
};

}  // namespace loconet_tcp
}  // namespace esphome