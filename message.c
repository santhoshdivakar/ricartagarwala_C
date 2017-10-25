/* This file is created to deal with the incoming and outgoing messages */
#include <stdlib.h>
#include <stdio.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <syslog.h>
#include <sys/socket.h>
#include <sys/types.h>
#include <netinet/in.h>
#include <netdb.h>
#include "message.h"



int sendRequest(int port, struct message msg){
    int sockfd = 0, n = 0;
    char recvBuff[10];
    struct sockaddr_in serv_addr;

    memset(recvBuff, '0',sizeof(recvBuff));

    printf("Sending Message\n");

    if((sockfd = socket(AF_INET, SOCK_STREAM, 0)) < 0)
    {
        syslog(LOG_INFO,"%d  - %s",port, "Could not create socket");
        return 1;
    } 

    memset(&serv_addr, '0', sizeof(serv_addr)); 

    serv_addr.sin_family = AF_INET;
    serv_addr.sin_port = htons(port); 

    if(inet_pton(AF_INET,"127.0.0.1", &serv_addr.sin_addr)<=0)
    {
        syslog(LOG_INFO,"%d  - %s %d",msg.nodeNum, "Could not create inet_pton for port:", port);
        return 1;
    } 
    printf("Connecting to server\n");
    if( connect(sockfd, (struct sockaddr *)&serv_addr, sizeof(serv_addr)) < 0)
    {
       syslog(LOG_INFO,"%d  - %s %d",msg.nodeNum, "send Request: Connect Failed for port:", port);
       // Here we assume that the other node doesnt exist
       return 0;
    }

    // write to the server for the request
    printf("\nclient Message ==> %ld\n", msg.timeStamp); 
    sendto(sockfd, &msg, sizeof(struct message), 0, (struct sockaddr *)&serv_addr, sizeof(struct sockaddr)); 

    while ( (n = read(sockfd, recvBuff, sizeof(recvBuff)-1)) > 0)
    {
        if(recvBuff[0] == '1') {
            break;
        } else {
            syslog(LOG_INFO,"%d  - %s %d",msg.nodeNum, "send Request: did not receive a good reply for port:", port);
        }
    } 

    close(sockfd);
    return 0;
}


