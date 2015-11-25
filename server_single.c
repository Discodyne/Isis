/*
    C socket server example
*/
 
#include<stdio.h>
#include<stdlib.h>//EXIT_FAILURE
#include<string.h>    //strlen
#include<sys/socket.h>
#include<arpa/inet.h> //inet_addr
#include<unistd.h>    //write
#include<dirent.h>//DIR

#include <sys/types.h>
#include <sys/stat.h>

 
int main(int argc , char *argv[])
{
    int socket_desc , client_sock , c , read_size;
    struct sockaddr_in server , client;
    char client_message[2000];
    DIR *dir;
    struct dirent *ent;
    struct stat st;/*структура для мета информации о файлах*/

    //Create socket
    socket_desc = socket(AF_INET , SOCK_STREAM , 0);
    if (socket_desc == -1)
    {
        printf("Could not create socket");
    }
    puts("Socket created");
     
    //Prepare the sockaddr_in structure
    server.sin_family = AF_INET;
    server.sin_addr.s_addr = INADDR_ANY;
    server.sin_port = htons( 8888 );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    puts("bind done");
     
    //Listen
    listen(socket_desc , 3);
     
    //Accept and incoming connection
    puts("Waiting for incoming connections...");
    c = sizeof(struct sockaddr_in);
     
    //accept connection from an incoming client
    client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c);
    if (client_sock < 0)
    {
        perror("accept failed");
        return 1;
    }
    puts("Connection accepted");
     
    //Receive a message from client
    while( (read_size = recv(client_sock , client_message , 2000 , 0)) > 0 )
    {
        if ((dir = opendir(client_message)) != NULL) {
 	 /* print all the files and directories within directory */
  	while ((ent = readdir (dir)) != NULL) {
                //Send the message back to client
           stat(ent->d_name, &st);
           char * birth_time = ctime(&st.st_ctime);
           write(client_sock , ent->d_name , strlen(ent->d_name));
           write(client_sock, birth_time, strlen(birth_time));
           write(client_sock,"\n",1);
    		// ("%s\n", ent->d_name);
  		}
  		closedir (dir);
	} else {
  	/* could not open directory */
  	perror ("");
  	return EXIT_FAILURE;
	}
    }
     
    if(read_size == 0)
    {
        puts("Client disconnected");
        fflush(stdout);
    }
    else if(read_size == -1)
    {
        perror("recv failed");
    }
     
    return 0;
}
