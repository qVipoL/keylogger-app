all:
	make release -C client
	make release -C server

clean:
	make clean -C client
	make clean -C server

run:
	gnome-terminal -- "./server/out/bin/keylogger-server" -t "Server"
	gnome-terminal -- "sudo ./client/out/bin/keylogger-client" -t "Client"