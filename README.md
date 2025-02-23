| Supported Targets | ESP32 |
| ----------------- | ----- |


# DNS Server with ad-blocking

The application is an UDP DNS Server on port 53, supporting ad-blocking and custom hosts.

## Implemented features

It currently accepts and decodes queries.
A prefix tree optimized for DNS caching is built-in.

## Incoming features

- answering queries
- recursive querying to a configurable remote DNS server
- web interface with a configuration panel

## How to use

Run it on ESP hardware
```
idf.py build flash monitor
```

The local IP address should be displayed. Query it using
```
dig @<ip address of the board> github.com
```

No answers are sent yet.

## Hardware Required

I only tested it on an ESP32-WROOM32.

## Configure the project

```
idf.py menuconfig
```

Configure Wi-Fi or Ethernet under "Example Connection Configuration" menu. See "Establishing Wi-Fi or Ethernet Connection" section in [examples/protocols/README.md](../../README.md) for more details.

## Build and Flash

Build the project and flash it to the board, then run monitor tool to view serial output:

```
idf.py -p PORT flash monitor
```

(To exit the serial monitor, type ``Ctrl-]``.)
