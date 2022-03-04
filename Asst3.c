#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <sys/socket.h>
#include <netdb.h>
#include <pthread.h>

#define BACKLOG 5

// the argument we will pass to the connection-handler threads
struct connection {
    struct sockaddr_storage addr;
    socklen_t addr_len;
    int fd;
};

int server(char *port);
void *echo(void *arg);

int isPunctuation(char c){
    if(c == '!' || c == '?' || c == '.'){
        return 1;
    } else {
        return 0;
    }
}

void printError(char string[]){

	

    if(string[5] == '0' && string[6] == 'C' && string[7] == 'T'){
        char *str = "M0CT - message 0 content was not correct (i.e. should be \"Knock, knock.\"";
		printf("%s\n", str);
        return;
    } else
    if(string[5] == '0' && string[6] == 'L' && string[7] == 'N'){
        char *str = "M0LN - message 0 length value was incorrect (i.e. should be 13 characters long)";
        printf("%s\n", str);
		return;
    } else
    if(string[5] == '0' && string[6] == 'F' && string[7] == 'T'){
        char *str = "M0FT - message 0 format was broken (did not include a message type, missing or too many '|'";
        printf("%s\n", str);
		return;
    } else
    if(string[5] == '1' && string[6] == 'C' && string[7] == 'T'){
        char *str = "M1CT - message 1 content was not correct (i.e. should be \"Who's there?\"";
		printf("%s\n", str);
        return;
    } else
    if(string[5] == '1' && string[6] == 'L' && string[7] == 'N'){
        char *str = "M1LN - message 1 length value was incorrect (i.e. should be 12 characters long)";
		printf("%s\n", str);
        return;
    } else
    if(string[5] == '1' && string[6] == 'F' && string[7] == 'T'){
        char *str = "M1FT - message 1 format was broken (did not include a message type, missing or too many '|'";
		printf("%s\n", str);
        return;
    } else
    if(string[5] == '2' && string[6] == 'C' && string[7] == 'T'){
        char *str = "M2CT - message 2 content was not correct (i.e. missing punctuation)";
		printf("%s\n", str);
        return;
    } else
    if(string[5] == '2' && string[6] == 'L' && string[7] == 'N'){
        char *str = "M2LN - message 2 length value was incorrect (i.e. should be the length of the message)";
		printf("%s\n", str);
        return;
    } else
    if(string[5] == '2' && string[6] == 'F' && string[7] == 'T'){
        char *str = "M2FT - message 2 format was broken (did not include a message type, missing or too many '|'";
		printf("%s\n", str);
        return;
    } else
    if(string[5] == '3' && string[6] == 'C' && string[7] == 'T'){
        char *str = "M3CT - message 3 content was not correct (i.e. should contain message 2 with \", who\" tacked on)";
		printf("%s\n", str);
        return;
    } else
    if(string[5] == '3' && string[6] == 'L' && string[7] == 'N'){
        char *str = "M3LN - message 3 length value was incorrect (i.e. should be M2 length plus six)";
		printf("%s\n", str);
        return;
    } else
    if(string[5] == '3' && string[6] == 'F' && string[7] == 'T'){
        char *str = "M3FT - message 3 format was broken (did not include a message type, missing or too many '|'";
		printf("%s\n", str);
        return;
    } else
    if(string[5] == '4' && string[6] == 'C' && string[7] == 'T'){
        char *str = "M4CT - message 4 content was not correct (i.e. missing punctuation)";
		printf("%s\n", str);
        return;
    } else
    if(string[5] == '4' && string[6] == 'L' && string[7] == 'N'){
        char *str = "M4LN - message 4 length value was incorrect (i.e. should be the length of the message)";
		printf("%s\n", str);
        return;
    } else
    if(string[5] == '4' && string[6] == 'F' && string[7] == 'T'){
        char *str = "M4FT - message 4 format was broken (did not include a message type, missing or too many '|'";
		printf("%s\n", str);
        return;
    } else
    if(string[5] == '5' && string[6] == 'C' && string[7] == 'T'){
        char *str = "M5CT - message 5 content was not correct (i.e. missing punctuation)";
		printf("%s\n", str);
        return;
    } else
    if(string[5] == '5' && string[6] == 'L' && string[7] == 'N'){
        char *str = "M5LN - message 5 length value was incorrect (i.e. should be the length of the message)";
		printf("%s\n", str);
        return;
    } else
    if(string[5] == '5' && string[6] == 'F' && string[7] == 'T'){
        char *str = "M5FT - message 5 format was broken (did not include a message type, missing or too many '|'";
		printf("%s\n", str);
        return;
    } 

	exit(0);
}

int checkCorrectFormat(char string[]){ //-1 if it's a failed format, -2 if it's a failed length. 1 if it's a correct format, 2 if it's correct format, but ERR
	int barCounter = 0;
	int numOfChars = 0;
	int numOfSpace = 0;
	if(string[0] == 'R' && string[1] == 'E' && string[2] == 'G'){
		int i = 3;
		while(barCounter != 3){ //now we're going to iterate through the string
			if(string[i] == '|'){ //but if we're just at a bar, move on
				barCounter++;
				i++;
			} else {
				if(barCounter == 1){
					char curr[10];
					int j = 0;
					
					int numOfNum = 0;

					while(string[i] != '|'){
						if(string[i] < '0' || string[i] > '9'){ //we have a non number in here. return.
							return -1;
						}
						if(string[i] == '\0'){
							return -1;
						}
						curr[j] = string[i]; //else, let's copy our numbers into curr
						numOfNum++;
						i++;
						j++;
					}
					if(numOfNum == 0){
						return -1;
					}
					curr[j] = '\0'; //make sure the end of curr has an ending
					numOfSpace = atoi(curr); //turn curr (which is a number but in chars) into an actual integer.

				} else
				if(barCounter == 2){
					while(string[i] != '|'){
						if(string[i] == '\0'){
							return -1;
						}
						numOfChars++;
						i++;
					}
				} 

			}

		}

		if(string[i] != '\0'){
			return -1;
		}

		if(numOfSpace != numOfChars){
			return -2;
		}

		return 1;
		
	} else 
    if(string[0] == 'E' && string[1] == 'R' && string[2] == 'R'){ //incorrect format.
		return 2;
	} else {
        return -1;
    }
}

int returnNumber(char string[]){ //only use this when we are sure that the string is correctly formatted
	int i = 3;

	char curr[10];
	int j = 0;

	int numOfSpace;

	while(string[i] != '|'){
		curr[j] = string[i]; //else, let's copy our numbers into curr
		j++;
		i++;
	}

	curr[j] = '\0'; //make sure the end of curr has an ending
	numOfSpace = atoi(curr); //turn curr (which is a number but in chars) into an actual integer.

	return numOfSpace;

}


char *removePunctuation(char string[]){

    int i = 0;
    while(string[i] != '\0'){
        i++;
    }

    string[i - 1] = '\0';
    return string;
    
}

char *returnString(char string[]){
	int barCounter = 0;
	char *ret = (char*)malloc(50 * sizeof(char));
	int i = 3;


	while(barCounter != 3){ //now we're going to iterate through the string
		if(string[i] == '|'){ //but if we're just at a bar, move on
			barCounter++;
			i++;
		} else {
			if(barCounter == 2){
				int j = 0;
				while(string[i] != '|'){
					ret[j] = string[i];
					i++;
					j++;
				}
				ret[j] = '\0'; //make sure the end of curr has an ending
			} else
			if(barCounter == 3){ //we are done
				return ret; 
			} else {
				i++;
			}
		}
	}
	return ret; 
}

void printString(char string[]){
	
	for(int i = 0; i < strlen(string); i++){
		printf("'%c'", string[i]);
	}

	if(string[strlen(string)] == '\0'){
		printf("'/0'");
	} else


	if(string[strlen(string) + 1] == '\0'){
		printf("'/0'");
	} else

	if(string[strlen(string) + 2] == '\0'){
		printf("'/0'");
	} 

	printf("\n");
	return;
}

int main(int argc, char **argv)
{
	if (argc != 2) {
		printf("Usage: %s [port]\n", argv[0]);
		exit(EXIT_FAILURE);
	}

    (void) server(argv[1]);
    return EXIT_SUCCESS;
}


int server(char *port)
{
    struct addrinfo hint, *address_list, *addr;
    struct connection *con;
    int error, sfd;
    pthread_t tid;

    // initialize hints
    memset(&hint, 0, sizeof(struct addrinfo));
    hint.ai_family = AF_UNSPEC;
    hint.ai_socktype = SOCK_STREAM;
    hint.ai_flags = AI_PASSIVE;
    	// setting AI_PASSIVE means that we want to create a listening socket

    // get socket and address info for listening port
    // - for a listening socket, give NULL as the host name (because the socket is on
    //   the local host)
    error = getaddrinfo(NULL, port, &hint, &address_list);
    if (error != 0) {
        fprintf(stderr, "getaddrinfo: %s\n", gai_strerror(error));
        return -1;
    }

    // attempt to create socket
    for (addr = address_list; addr != NULL; addr = addr->ai_next) {
        sfd = socket(addr->ai_family, addr->ai_socktype, addr->ai_protocol);
        
        // if we couldn't create the socket, try the next method
        if (sfd == -1) {
            continue;
        }

        // if we were able to create the socket, try to set it up for
        // incoming connections;
        // 
        // note that this requires two steps:
        // - bind associates the socket with the specified port on the local host
        // - listen sets up a queue for incoming connections and allows us to use accept
        if ((bind(sfd, addr->ai_addr, addr->ai_addrlen) == 0) &&
            (listen(sfd, BACKLOG) == 0)) {
            break;
        }

        // unable to set it up, so try the next method
        close(sfd);
    }

    if (addr == NULL) {
        // we reached the end of result without successfuly binding a socket
        fprintf(stderr, "Could not bind\n");
        return -1;
    }

    freeaddrinfo(address_list);

    // at this point sfd is bound and listening
    printf("Waiting for connection\n");
    for (;;) {
    	// create argument struct for child thread
		con = malloc(sizeof(struct connection));
        con->addr_len = sizeof(struct sockaddr_storage);
        	// addr_len is a read/write parameter to accept
        	// we set the initial value, saying how much space is available
        	// after the call to accept, this field will contain the actual address length
        
        // wait for an incoming connection
        con->fd = accept(sfd, (struct sockaddr *) &con->addr, &con->addr_len);
        	// we provide
        	// sfd - the listening socket
        	// &con->addr - a location to write the address of the remote host
        	// &con->addr_len - a location to write the length of the address
        	//
        	// accept will block until a remote host tries to connect
        	// it returns a new socket that can be used to communicate with the remote
        	// host, and writes the address of the remote hist into the provided location
        
        // if we got back -1, it means something went wrong
        if (con->fd == -1) {
            perror("accept");
            continue;
        }

		// spin off a worker thread to handle the remote connection
        error = pthread_create(&tid, NULL, echo, con);

		// if we couldn't spin off the thread, clean up and wait for another connection
        if (error != 0) {
            fprintf(stderr, "Unable to create thread: %d\n", error);
            close(con->fd);
            free(con);
            continue;
        }

		// otherwise, detach the thread and wait for the next connection request
        pthread_detach(tid);
    }

    // never reach here
    return 0;
}

void *echo(void *arg)
{
    char host[100], port[10];
    struct connection *c = (struct connection *) arg;
    int error;

	// find out the name and port of the remote host
    error = getnameinfo((struct sockaddr *) &c->addr, c->addr_len, host, 100, port, 10, NI_NUMERICSERV);
    	// we provide:
    	// the address and its length
    	// a buffer to write the host name, and its length
    	// a buffer to write the port (as a string), and its length
    	// flags, in this case saying that we want the port as a number, not a service name
    if (error != 0) {
        fprintf(stderr, "getnameinfo: %s", gai_strerror(error));
        close(c->fd);
        return NULL;
    }

    printf("[%s:%s] connection\n", host, port);


//STEP 1====================================================================

 
    char string[22] = {'R', 'E', 'G', '|', '1', '3', '|', 'K', 'n', 'o', 'c', 'k', ',', ' ', 'k', 'n', 'o', 'c', 'k', '.', '|', '\0'};
    write(c->fd, string, strlen(string) + 1); //Here we are sending "Knock Knock" (or it should be)

//=====================================================================
//STEP 2=====================================================================

    char string1[200];

    read(c->fd, string1, 200); //Here we are receiving "Who's there"... or an error message.

    if(strcmp("D", string1) == 0){
        close(c->fd);
        return NULL;
    }

    printf(">>%s\n", string1);

    if(checkCorrectFormat(string1) == 1){
        if(strcmp("Who's there?", returnString(string1)) == 0){

            char setup_line[200] = {'R', 'E', 'G', '|', '4', '|', 'B', 'o', 'o', '.', '|', '\0'};
            write(c->fd, setup_line, strlen(setup_line) + 1); //Here we are sending "<setupline>" (or it should be)

        } else {
            char setup_line[200] = {'E', 'R', 'R', '|', 'M', '1', 'C', 'T', '|', '\0'};
            write(c->fd, setup_line, strlen(setup_line) + 1); //sending the content error above
        }
    } else
    if(checkCorrectFormat(string1) == 2){ //we've received an error. print out the appropriate desc.
        printError(string1);
        write(c->fd, "D", 2);
        close(c->fd);
        return NULL;
    } else
    if(checkCorrectFormat(string1) == -1){
        char setup_line[200] = {'E', 'R', 'R', '|', 'M', '1', 'F', 'T', '|', '\0'};
        write(c->fd, setup_line, strlen(setup_line) + 1); //sending the format error above
    } else
    if(checkCorrectFormat(string1) == -2){
        char setup_line[200] = {'E', 'R', 'R', '|', 'M', '1', 'L', 'N', '|', '\0'};
        write(c->fd, setup_line, strlen(setup_line) + 1); //sending the lengtherror above
    }

//=====================================================================
//STEP 3=====================================================================    
    
    char string2[200];


    read(c->fd, string2, 200); //Here we are receiving "<setup_line>, who?"... or an error message.

    if(strcmp("D", string2) == 0){
        close(c->fd);
        return NULL;
    }

    printf(">>%s\n", string2);


    if(checkCorrectFormat(string2) == 1){

        if(strcmp(returnString(string2), "Boo, who?") == 0){ //checks here
            
            char string3[200] = {'R', 'E', 'G', '|', '1', '9', '|', 'W', 'h', 'y', ' ', 'a', 'r', 'e', ' ', 'y', 'o', 'u', ' ', 'c', 'r', 'y', 'i', 'n', 'g', '?', '|', '\0'};
            write(c->fd, string3, strlen(string3) + 1); //Here we are sending "<punchline><punc.>" (or it should be)

        } else {
            char string3[200] = {'E', 'R', 'R', '|', 'M', '3', 'C', 'T', '|', '\0'};
            write(c->fd, string3, strlen(string3) + 1); //sending the content error above
        }
    } else
    if(checkCorrectFormat(string2) == 2){ //we've received an error. print out the appropriate desc.
        printError(string2);
        write(c->fd, "D", 2);
        close(c->fd);
        return NULL;
    } else
    if(checkCorrectFormat(string2) == -1){
        char string3[200] = {'E', 'R', 'R', '|', 'M', '3', 'F', 'T', '|', '\0'};
        write(c->fd, string3, strlen(string3) + 1); //sending the format error above
    } else
    if(checkCorrectFormat(string2) == -2){
        char string3[200] = {'E', 'R', 'R', '|', 'M', '3', 'L', 'N', '|', '\0'};
        write(c->fd, string3, strlen(string3) + 1); //sending the lengtherror above
    }
//=====================================================================
//STEP 4=====================================================================  

    char string4[200];

    read(c->fd, string4, 200); //Here we are receiving "<a/d/s><punct>"... or an error message.

    if(strcmp("D", string4) == 0){
        close(c->fd);
        return NULL;
    }

    printf(">>%s\n", string4);

    if(checkCorrectFormat(string4) == 1){
        if(isPunctuation(string4[strlen(string4) - 1]) == 1){ //checks here

        } else {
            char string5[200] = {'E', 'R', 'R', '|', 'M', '5', 'C', 'T', '|', '\0'};
            write(c->fd, string5, strlen(string5) + 1); //sending the content error above
        }
    } else
    if(checkCorrectFormat(string4) == 2){ //we've received an error. print out the appropriate desc.
        printError(string4);
        write(c->fd, "D", 2);
        close(c->fd);
        return NULL;
    } else
    if(checkCorrectFormat(string4) == -1){
        char string5[200] = {'E', 'R', 'R', '|', 'M', '5', 'F', 'T', '|', '\0'};
        write(c->fd, string5, strlen(string5) + 1); //sending the format error above
    } else
    if(checkCorrectFormat(string4) == -2){
        char string5[200] = {'E', 'R', 'R', '|', 'M', '5', 'L', 'N', '|', '\0'};
        write(c->fd, string5, strlen(string5) + 1); //sending the lengtherror above
    }  


    printf("[%s:%s] waiting for next connection...\n", host, port);

    close(c->fd);
    free(c);
    return NULL;
}