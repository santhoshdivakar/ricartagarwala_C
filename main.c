#include <stdio.h>
#include <stdlib.h>
#include <syslog.h>
#include "client.h"
#include "server.h"
#include <sys/wait.h>
#include <unistd.h>
#include <string.h>

#define NODES 10

int *getNodeList(void) {
    FILE *fp;
    int nodeIndex;
    int *nodeList; 
    char *line;
    int read;
    size_t len=0;
    fp = fopen("portList.csv", "r");
    if (fp == NULL) {
        printf("Could not open the input file\n");
        exit(EXIT_FAILURE);
    }
    
    nodeList = (int *)malloc(NODES*(sizeof(int)));
    line=NULL;
    nodeIndex = 0;
    while ((read = getline(&line, &len, fp)) != -1) {
        printf("Retrieved line of length %zu :\n", read);
        printf("%s", line);
        nodeList[nodeIndex] = atoi((const char *)line);
        free(line);
        line=NULL;
        nodeIndex++;
        if (nodeIndex >= NODES) {
            break;
        }
    }
    printf("%d nodes read", nodeIndex);
    fclose(fp);
    return nodeList;
}

int main(int argc, char **argv) {
    char *nodeNum;
    int *nodeList;
    pid_t myPid;
    
    if (argc != 2) {
        printf("Syntax: \n %s <node_num>\n",argv[0]);
    }
    // Get the syslog setup
    openlog("RICART",LOG_PID, LOG_LOCAL1);
    nodeNum = argv[1];
    syslog(LOG_INFO,"%s  - %s",nodeNum, "Entered the program");

    //1a. Get the list of NODES
    nodeList = (int *)getNodeList();

    //2. Fork into client and server options
    myPid = fork();

    if (myPid == 0) {
        //3a. Run the client code.
        syslog(LOG_INFO,"%s  - %s",nodeNum, "Client Process started");
        return client(nodeNum, nodeList, NODES);
    } else {
        //3b. Run the server code
        syslog(LOG_INFO,"%s  - %s",nodeNum, "Server Process started");
        return server(atoi(nodeNum));
    }
    return 0;
}