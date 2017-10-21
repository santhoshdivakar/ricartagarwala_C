#include <stdio.h>
#include <syslog.h>

int main(int argc, char **argv) {
    char *nodeNum;
    if (argc != 2) {
        printf("Syntax: \n %s <node_num>\n",argv[0]);
    }
    // Get the syslog setup
    openlog("RICART",LOG_PID, LOG_USER);
    nodeNum = argv[1];
    syslog(LOG_INFO,"%s  - %s","Entered the program",nodeNum);
    return 0;
}