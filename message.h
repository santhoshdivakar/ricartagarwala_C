#define MSG_REQUEST 0
#define MSG_FINISHED 1
#define MSG_ALLOW_STR "1"

struct message
{
    int     mtype; // REQUEST=0 ; REPLY=1; FINISHED=3
    int     nodeNum;
    long     timeStamp; // contains the timestamp
};

int sendRequest(int port, struct message);