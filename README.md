# Client-Server-Dice-Game

Steps to run Client/Server dice game:

1) Compile client and server program by following commands in respective machines:

	gcc client.c -o client
	gcc server.c -o server
Note: Program will also work if both program are in same machine.

2) Run Server out file by the following command:

	./server

3) Run Client out file by the following command:

	./client <Server's Ip Address>

Note: If the client and server are in the same machine you can either give
server's IP address or 127.0.0.1 (the standard address for IPv4 loopback traffic)

4) Run another client by the same command to start the two-player game.

Note: You can also start multiple two-player palyer dice games in parallel. The game will be differed by game_id
i.e. "Game 1" , "Game 2", etc.
