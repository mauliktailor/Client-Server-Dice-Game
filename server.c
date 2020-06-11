//Server
/*
Maulik Rajesh Tailor                    ID:105183659
Riddhi Harshadkumar Vaishnav            ID:110010465
*/
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 1212

void servicePlayers(int, int, int);
int main(int argc, char const *argv[])
{
    int game_id=1;
    int server_fd, new_socket1, valread1, new_socket2,valread2;
    struct sockaddr_in address;
    int addrlen = sizeof(address);
    char *hello = "Hello from server";

    // Creating socket file descriptor
    if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    address.sin_family = AF_INET;
    address.sin_addr.s_addr = INADDR_ANY;
    address.sin_port = htons( PORT );
    // attaching socket to the port
    if (bind(server_fd, (struct sockaddr *)&address, sizeof(address))<0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    // Listen with queue lenght for waiting connections = 3
    if (listen(server_fd, 3) < 0)
    {
            perror("listen");
            exit(EXIT_FAILURE);
    }
    while(1){
	//obtaining Socket descriptor of first client
	if ((new_socket1 = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
    	{
        	    perror("accept");
	            exit(EXIT_FAILURE);
	}
	printf("Got player 1 TOTO, Now wating for player 2 TITI....\n");
	//obtaining Socket descriptor of first client
    	if ((new_socket2 = accept(server_fd, (struct sockaddr *)&address, (socklen_t*)&addrlen))<0)
    	{
        	    perror("accept");
         	    exit(EXIT_FAILURE);
    	}
	printf("Got player 2 TITI\nLETS BEGIN!!!\n");
	//Statting new process to handle the game
	if(!fork()){
		printf("---------------------------NEW GAME---------------------------\n");
		servicePlayers(new_socket1, new_socket2,game_id);
		exit(0);
	}
	game_id++;
    }
    return 0;
}

//Method that handles the game
void servicePlayers(int p1, int p2, int game_id){
	char *readyMsg = "You can now play";
	int p1_total=0;
	int p2_total=0;
	int valread;
	char* go_winner= "Game over: you won the game\n";
        char* go_looser= "Game over: you lost the game\n";
	while(1){
        	char buffer_score1[10]={0};
	        char buffer_score2[10]={0};
		sleep(1);

		// Sending player 1 "You can play"
		send(p1 , readyMsg , strlen(readyMsg) , 0 );// TOTO can play

		// Reading the dice value of player 1 in buffer_score1
		valread = read(p1, buffer_score1,1024);

		//Checks if the client has teminated 99 is a special termination value.
		if(strcmp(buffer_score1,"99") == 0){
			printf("Game %d: TOTO quits the game!!\n", game_id);
			send(p2 , go_winner, strlen(go_winner) , 0 );
                        exit(0);
		}

		// Add dice value to total of player 2 after converting it to integer form string using atoi()
        	p1_total+=atoi(buffer_score1);

		//Check if the total exceeds 100 and declare winner
		if(p1_total>=100){
			send(p1 , go_winner, strlen(go_winner) , 0 );
			send(p2 , go_looser, strlen(go_looser) , 0 );
			exit(0);
		}
		sleep(1);

		// Sending player 2 "You can play"
		send(p2 , readyMsg , strlen(readyMsg) , 0 );// TITI can play

		// Reading the dice value of player 2 in buffer_score2
		valread = read(p2, buffer_score2,1024);

		//Checks if the client has teminated 99 is a special termination value.
		if(strcmp(buffer_score2,"99") == 0){
			printf("Game %d: TITI quits the game!!\n", game_id);
                        send(p1 , go_winner, strlen(go_winner) , 0 );
                        exit(0);
                }

		// Add dice value to total of player 2 after converting it to integer form string using atoi()
                p2_total+=atoi(buffer_score2);
		if(p2_total>=100){
                        send(p2 , go_winner, strlen(go_winner) , 0 );
                        send(p1 , go_looser, strlen(go_looser) , 0 );
			exit(0);
                }

		//Printing total of both players with game_id
		printf("Game %d: TOTO's total:%d \nGame %d:TITI's total:%d\n",game_id, p1_total, game_id, p2_total);
	}

	//Closing the client sockets
	close(p1);
	close(p2);
}
