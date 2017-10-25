/* https://gist.github.com/oleksiiBobko/43d33b3c25c03bcc9b2b */
#include <stdio.h>
#include <string.h>    //strlen
#include <stdlib.h>    //strlen
#include <syslog.h>
#include <sys/socket.h>
#include <arpa/inet.h> //inet_addr
#include <unistd.h>    //write
#include <pthread.h> //for threading , link with lpthread
#include "server.h"
#include "message.h"
 
//the thread function
void *connection_handler(void *);

int requestCounter=0;
int requestArray[20];
int nodeNum;
long globalLockTime;
 
int server(int myNode){
    int socket_desc , client_sock , c;
    struct sockaddr_in server , client;
    
    nodeNum = myNode;
    globalLockTime = 0;

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
    server.sin_port = htons( nodeNum );
     
    //Bind
    if( bind(socket_desc,(struct sockaddr *)&server , sizeof(server)) < 0)
    {
        //print the error message
        perror("bind failed. Error");
        return 1;
    }
    syslog(LOG_INFO,"%d  - %s",nodeNum, "Bind done");
     
    //Listen
    listen(socket_desc , 20);
     
    //3b1. Setup the listening port and listen to packets.
    syslog(LOG_INFO,"%d  - %s",nodeNum, "Server Process Listening");

    c = sizeof(struct sockaddr_in);
	pthread_t thread_id;
	
    while( (client_sock = accept(socket_desc, (struct sockaddr *)&client, (socklen_t*)&c)) )
    {
        syslog(LOG_INFO,"%d  - %s",nodeNum, "Server Process Accepted Client Connection");
         
        if( pthread_create( &thread_id , NULL ,  connection_handler , (void*) &client_sock) < 0)
        {
            perror("could not create thread");
            syslog(LOG_INFO,"%d  - %s",nodeNum, "Server Process Could not create thread");
            return 1;
        }
         
        //Now join the thread , so that we dont terminate before the thread
        //pthread_join( thread_id , NULL);
        syslog(LOG_INFO,"%d  - %s",nodeNum, "Server Process assigned handler");
    }
     
    if (client_sock < 0)
    {
        syslog(LOG_INFO,"%d  - %s",nodeNum, "Server Process accept failed");
        return 1;
    }
     
    return 0;
}
 
/*
 * This will handle connection for each client
 * */
void *connection_handler(void *socket_desc)
{
    //Get the socket descriptor
    int sock = *(int*)socket_desc;
    int read_size;
    char *message , client_message[2000], *replyMessage;
    struct message incomingMsg;
     
    syslog(LOG_INFO,"%d  - %s",nodeNum, "Server Process inside handler ");
    requestCounter++;
   
    //Receive a message from client
    while( (read_size = recv(sock , client_message , 2000 , 0)) > 0 )
    {
        //end of string marker
		client_message[read_size] = '\0';
		syslog(LOG_INFO,"%d  - %s",nodeNum, "Server Process received buffer ");

        //copy the message into the struct and read it.
        memcpy((void *)&incomingMsg, (const void *)client_message,sizeof(struct message));
        if (incomingMsg.mtype == MSG_FINISHED) {
            //release the lock here.
            syslog(LOG_INFO,"%d  - %s",nodeNum, "Server Process received FINISHED message ");
            globalLockTime = 0;
        } else {
            //request for the allow
            //check if the global lock is set for our client running the CS then wait
            while(globalLockTime != 0){
                if (incomingMsg.timeStamp < globalLockTime ) {
                    break;
                }
                sleep(1);
            }
            // if this is our client, we would note down the time in globalLockTime and reply immediately
            if (incomingMsg.nodeNum == nodeNum) {
                syslog(LOG_INFO,"%d  - %s",nodeNum, "Server Process received REQUEST of same client ");
                globalLockTime = incomingMsg.timeStamp;
            }
            replyMessage = MSG_ALLOW_STR;
            syslog(LOG_INFO,"%d  - %s %d",nodeNum, "Server Process Replied with ALLOW to ->", incomingMsg.nodeNum);
            write(sock , "1" , strlen(replyMessage));
        }
    }
     
   if(read_size <= 0){
        syslog(LOG_INFO,"%d  - %s",nodeNum, "Server Process read FAIL ");
    }
        
    return 0;
} 