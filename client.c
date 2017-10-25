#include <time.h>
#include <unistd.h>
#include <string.h>
#include <stdlib.h>
#include "client.h"
#include "message.h"
#include <syslog.h>

#define TIME_LEN 20

int client(char *nodeNum, int *nodeList, int numOfNodes){
    int i,j;
    struct message msg;
    char *requestMessage;

    syslog(LOG_INFO,"%s  - %s",nodeNum, "Client Process Starting");
    for (i=0;i<20;i++){
        syslog(LOG_INFO,"%s  - %s: %d",nodeNum, "Client Process starting the approval process",i);
        //form the message
        msg.mtype = MSG_REQUEST;
        msg.nodeNum = atoi(nodeNum);
        msg.timeStamp = time(NULL);
        for (j=0;j<numOfNodes;j++){
            syslog(LOG_INFO,"%s  - %s %d",nodeNum, "Client Process Sending Request for critical Section to node:",nodeList[j]);
            if(sendRequest(nodeList[j], msg) != 0) {
                //serious shit happened here
            }
        }
        // Now we got all the approvals
        syslog(LOG_INFO,"%s  - %s %d",nodeNum, "Client Process Got all approvals:",i);
        // Run the Critical Section
        syslog(LOG_INFO,"%s  - %s",nodeNum, "Client Process start the critical section");
        sleep(20);

        // Send the finished message to the server of this node
        msg.mtype = MSG_FINISHED;
        if(sendRequest(atoi(nodeNum), msg) != 0) {
           //serious shit happened here
        }
        syslog(LOG_INFO,"%s  - %s",nodeNum, "Client Process Finish the critical section");
        // Out of Critical Section
        sleep(30);
    }
}