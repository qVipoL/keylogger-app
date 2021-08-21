## Keylogger App
Keylogger, client and server application. Written in C with raw_sockets.

### Settings:
- server/src/main.c - change NET_INTERFACE_IDX and PORT
- client/src/main.c - change NET_INTERFACE_IDX, SRC params and DEST params.
- to get NET_INTERFACE_IDX: <code>ip link show</code>

### Usage:
- Compile: <code>make</code>
- Run Server: <code>sudo bash run_server.sh</code>
- Run Client: <code>sudo bash run_client.sh</code>