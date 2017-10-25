# Ricart Agarwala algorithm implemented in C langauage


## About the program
The program runs on a single server. We can extend it to run on multiple servers with minimal change.
Each program consists of two portions -> 
(a) The server
(b) The client
The client , whenever it needs to go through the Critical-Section, it would send a request to all the other nodes.
The server listens to the port for requests and whenever a request is received it follows the protocol defined in RC algorithm.

### Compiling the program
`gcc -lpthread main.c message.c server.c client.c`

## Steps to run the setup
0. Setup the syslog.conf so that LOG_LOCAL5 points to /tmp/ricartAgarwala.log (otherwise it will follow default rules)
1. Set the port numbers in the portList.txt
2. Start each of the program with the port numbers in the above file as the parameter (./a.out 9111). Each instance needs a seperate port. 
3. We need to make sure that the instace did start correctly. A non-listening port would cause the entire setup to change
4. The logs of how the program runs can be found in /tmp/ricartAgarwala.log
5. Program ends after 20 requests to CS is done by each of the client

## Functions
---

### sendRequest
To check from a given node whether this request is approved by that node. 
The function sends the request to the given node and returns when it receives the reply from the node.
It would wait indefinitely to get the approval ( or till time-out is reached)
INPUT: nodeNum( char* )
OUTPUT: 0 in case of success and -1 in case of error. 

### client
This is the client portion of the program. This loops 20 times through to get into the CS
At start of each CS, it loops through the list of nodes and sends requests sequentially to each of them
It also sets the timer for its own server through a semaphore
Once it receives reply from each of the node, it goes into the CS and sleeps for 10 seconds
Once the sleep time is finished it resets the semaphore.
INPUT: semaphoreVar ( semctl )

### server
This is the server section of the program. This would wait on an event for any requests.
Once it gets a request it does the following
* Spawns a thread for each of the request received
* Checks if the semaphoreVar is set. If it is set, then it checks its time val is more than that of request. If it is less, then it sends reply.
* If semaphoreVar is set and its time is less than request, then it waits for the semaphoreVar to be reset
* If the above two conditions are not the case, then it replies immediately. 


