all:	latency	thruput

latency:	client_latency server_latency

client_latency:	client_latency.c
	gcc client_latency.c -o client_latency -lrt

server_latency:	server_latency.c
	gcc server_latency.c -o server_latency

thruput:	client_thruput server_thruput

client_thruput:	client_thruput.c
	gcc client_thruput.c -o client_thruput -lrt

server_thruput:	server_thruput.c
	gcc server_thruput.c -o server_thruput
clean:
	rm -f server_latency server_thruput client_latency client_thruput
