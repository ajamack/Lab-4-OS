#include <netinet/in.h> //structure for storing address information 
#include <stdio.h> 
#include <string.h>
#include <stdlib.h> 
#include <sys/socket.h> //for socket APIs 
#include <sys/types.h> 
#include "list.h"

#define PORT 9001
#define ACK "ACK"
  
int main(int argc, char const* argv[]) 
{ 
    //(void)argc; (void)argv;
  
	int n, val, idx;
    // create server socket similar to what was done in 
    // client program 
    int servSockD = socket(AF_INET, SOCK_STREAM, 0); 
    //if (servSockD < 0) { perror("socket"); exit(1); }

  
    // string store data to recv/send to/from client 
    char buf[1024];
	char sbuf[1024];
    char* token;

    // define server address 
    struct sockaddr_in servAddr; 
    //memset(&servAddr, 0, sizeof(servAddr));

  
	  // list
	list_t *mylist;


    servAddr.sin_family = AF_INET; 
    servAddr.sin_port = htons(PORT); 
    servAddr.sin_addr.s_addr = INADDR_ANY; 
  
    // bind socket to the specified IP and port 
    bind(servSockD, (struct sockaddr*)&servAddr, sizeof(servAddr));
  
    // listen for connections 
    listen(servSockD, 1); 
  
    // integer to hold client socket. 
    int clientSocket = accept(servSockD, NULL, NULL); 

    mylist = list_alloc();  // create the list

    while(1){
        // recvs messages from client socket 
        n = recv(clientSocket, buf, sizeof(buf), 0);
        buf[n] = '\0';

        if(n > 0){
            token = strtok(buf, " ");
    
						if(strcmp(token,"exit") == 0){
							  list_free(mylist);
                              sprintf(sbuf, "%s", "bye");
                              send(clientSocket, sbuf, strlen(sbuf), 0);
							  exit(0);
						}
						else if(strcmp(token,"get_length") == 0){
								// Make a Call
								val = list_length(mylist);
								sprintf(sbuf,"%s%d", "Length = ", val);
						}
						else if(strcmp(token,"add_front") == 0){
							  token = strtok(NULL, " ");  // get next token (value)
								val = atoi(token);
								// Make a Call
								sprintf(sbuf,"%s%d", ACK, val);
								list_add_to_front(mylist,val);
						}
						else if(strcmp(token,"remove_position") == 0){
							  token = strtok(NULL, " ");
								idx = atoi(token);
								// Make a Call
								val = list_remove_at_index(mylist,idx);
								sprintf(sbuf,"%s%d", ACK, val);
						}
						else if(strcmp(token,"print") == 0){
							  sprintf(sbuf,"%s", listToString(mylist));
						}
						// ADD THE OTHER OPERATIONS
                        else if(strcmp(token, "add back") == 0){
                            token = strtok(NULL, " ");
                            val = atoi(token);
                            list_add_to_back(my_list, val);
                            sprintf(sbuf, "%s %d", ACK, val);
                        }
                        else if(strcmp(token, "add position") == 0){
                            char *tidx = strtok(NULL, " ");
                            char *tval = strtok(NULL, " ");
                            if (!tidx || !tval) sprintf(sbuf, "Error usage");
                            else {
                                idx = atoi(tidx);
                                val = atoi(tval);
                                int ok = list_add_to_index(my_list, idx, val);
                                if (ok) sprintf(sbuf, "%s %d", ACK, val);)
                                else    sprintf(sbuf, "ERR index");
                            }
                        }
                        else if(strcmp(token,"remove_back") == 0){
                            val = list_remove_from_back(mylist);
                            sprintf(sbuf, "%d", val);
                        }
                        else if(strcmp(token,"remove_front") == 0){
                            val = list_remove_from_front(mylist);
                            sprintf(sbuf, "%d", val);
                        }
                        else if(strcmp(token,"get") == 0){
                            token = strtok(NULL, " ");
                            if (!token) sprintf(sbuf, "ERR usage: get <index>");
                            else {
                                idx = atoi(token);
                                val = list_get_elem_at(mylist, idx);
                                sprintf(sbuf, "%d", val);
                            }
                        }
                        else if(strcmp(token,"menu") == 0){                           // CHANGED (ADDED tiny handler)
                            sprintf(sbuf, "OK"); // harmless stub so client doesn't hang
                        }
                        else {
                            sprintf(sbuf,"ERR unknown");
                        }



                // send's messages to client socket 
            send(clientSocket, sbuf, strlen(sbuf), 0);
        }
				memset(buf, '\0', 1024);
    }
  
    return 0; 
}
