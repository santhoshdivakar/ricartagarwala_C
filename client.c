#include <stdio.h>
#include <syslog.h>

const NODES=10
int main(int argc, char **argv) {
    char *nodeNum;
    int requestDeferred[NODES]; // to store the requests.
    if (argc != 2) {
        printf("Syntax: \n %s <node_num>\n",argv[0]);
    }
    // Get the syslog setup
    openlog("RICART",LOG_PID, LOG_USER);
    nodeNum = argv[1];
    syslog(LOG_INFO,"%s  - %s",nodeNum, "Entered the program");
    //1. Create the semaphore for communication between the server and the client
    //2. Fork into client and server options
    myPid = fork();
    if (myPid == 0) {
        //3a. Run the client code.
        syslog(LOG_INFO,"%s  - %s",nodeNum, "Client Process started");
    } else {
        //3b. Run the server code
        syslog(LOG_INFO,"%s  - %s",nodeNum, "Server Process started");
        //3b1. Setup the listening port and listen to packets.
        syslog(LOG_INFO,"%s  - %s",nodeNum, "Server Process Listening");
        //3b2. Setup the thread for each of the receiving packets

            
        }
    }
    return 0;
}