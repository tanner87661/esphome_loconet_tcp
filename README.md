loconet\_tcp/

├── components/

│   └── loconet\_tcp/

│       ├── loconet\_tcp.h

│       ├── loconet\_tcp.cpp

│       └── loconet\_tcp.py

└── README.md   (optional – copy the text below)



Example yaml



esphome:

&#x20; name: loconet-esp8266

&#x20; platform: ESP8266

&#x20; board: nodemcuv3   # or d1\_mini etc.



wifi:

&#x20; ssid: "YourWiFi"

&#x20; password: "password"



external\_components:

&#x20; - source: github://tanner87661/esphome_loconet\_tcp

&#x20;   components: \[loconet\_tcp]



loconet\_tcp:

&#x20; id: my\_loconet

&#x20; host: ln\_overTCP          # your TCP server IP

&#x20; port: ln\_overTCP\_port     # your server port



&#x20; on\_receive:               # fires on any line that STARTS WITH "RECEIVE"

&#x20;   - logger.log:

&#x20;       format: "RECEIVE callback → %s"

&#x20;       args: \['x']

&#x20;   - script.execute: handle\_receive   # or any automation you want



&#x20; on\_sent\_ok:               # fires on any line that STARTS WITH "SENT OK"

&#x20;   - logger.log:

&#x20;       format: "SENT OK received → %s"

&#x20;       args: \['x']

&#x20;   - light.turn\_on: my\_led   # example



\# Optional: send a command from anywhere (button, automation, etc.)

button:

&#x20; - platform: template

&#x20;   name: "Send Test to Server"

&#x20;   on\_press:

&#x20;     - lambda: |-

&#x20;         id(my\_loconet).send("SEND 85 7A");



interval:

&#x20; - interval: 30s

&#x20;   then:

&#x20;     - lambda: |-

&#x20;         id(my\_loconet).send("SEND 85 7A");   # send opc\_busy to keep-alive

