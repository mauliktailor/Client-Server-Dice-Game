// Client
/*
Maulik Rajesh Tailor                    ID:105183659
Riddhi Harshadkumar Vaishnav            ID:110010465
*/
#include<sys/signal.h>
#include<stdlib.h>
#include<time.h>
#include <stdio.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <unistd.h>
#include <string.h>
#define PORT 1212
int sock;

// Handles Ctrl-C and Ctrl-Z signals in case of client termination in the middle of the game.
void myHandler(){
	printf("I quit the game\n");
	char * val = "99";

	// Send a special termination value 99 to server to indicate it's termination
	send(sock , val , strlen(val) , 0 );
	exit(0);
}

int main(int argc, char const *argv[])
{
    // Handeling the signals
    signal(SIGINT, myHandler);
    signal(SIGTSTP, myHandler);


    if(argc != 2){
    	printf("Usage: %s <Server's Ip Address>\n",argv[0]);
	exit(0);
    }
    int valread;
    struct sockaddr_in serv_addr;
    char buffer[1024] = {0};

    //socket creation
    if ((sock = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        printf("\n Socket creation error \n");
        return -1;
    }

    // Setting the server address and port
    serv_addr.sin_family = AF_INET;

    //converts the unsigned integer (Port number here) hostlong from host byte order to network byte order.
    serv_addr.sin_port = htons(PORT);

    // Convert IPv4 and IPv6 addresses given in arguement from text to  a  network address structure
    if(inet_pton(AF_INET, argv[1], &serv_addr.sin_addr)<=0)
    {
        printf("\nInvalid address/ Address not supported \n");
        return -1;
    }

    //connect client socket with server
    if (connect(sock, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0) 
    {
        printf("\nConnection Failed \n");
        return -1;
    }

    char *readyMsg = "You can now play";
    char *go_winner = "Game over: you won the game\n";
    long int ss=0;
    printf("player Ready!!!!!!!!!\n");
    while(1){
    	valread = read( sock , buffer, 1024);

	//compare if ready message
    	if(strcmp(buffer,readyMsg) == 0){
    		char dice[12];

		//Roll the dice values [1-10] and convert integer to string to send value to server using sprintf
		sprintf(dice, "%d", ((int)time(&ss)%10 + 1));
		printf("Dice rolled !!! Obtained: %s\n", dice);

		//Send the dice value to server
    		send(sock , dice , strlen(dice) , 0 );
    	}
	else{
		//compare if winner message
		if(strcmp(buffer, go_winner) == 0)
			printf("I won the game\n");
		else
			printf("I lost the game\n");
		break;
	}
    }
    return 0;
}
