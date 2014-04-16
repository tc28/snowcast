#include <inttypes.h>
#include <netdb.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>


#include <stdio.h>
#include <unistd.h>
#include <time.h>
#include <errno.h>
#include <sys/stat.h>
#include <dirent.h>
#include <pwd.h>
#include <stdlib.h>
#include <string.h>
#include <pwd.h>
#include <fcntl.h>

#include <sys/time.h>
#include <sys/types.h>

#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/un.h>




#define BUFFERSIZE 1024



//i) no welcome message
//ii) udp stuff


int main(int argc, char **argv) {
	/* TODO: everything!	*/
	
	//the first argument is ./snowcast_client itself
	if (argc != 4) {
		fprintf(stderr, "Usage: ./snowcast_client host serverport udpport\n");
		exit(1);
	}

	char *sname = argv[1];
	
	//don't know if I need this
	//char *cname = "tc28";
	
	char *server_port = argv[2];	
	
	//conversion
	uint16_t udp_port = atoi(argv[3]);
	
	struct sockaddr_in server_addr;
	int sock;
	struct hostent *hostinfo;
		
	// set up socket for TCP
	if ((sock = socket(PF_INET, SOCK_STREAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}

	// Step 2: find the internet address of the server
	if ((hostinfo = gethostbyname(sname)) == 0) {
		fprintf(stderr, "Host %s does not exist\n", argv[1]);
		exit(1);
	}
	
	memset(&server_addr, 0, sizeof(server_addr));
	server_addr.sin_family = PF_INET;
	memcpy(&server_addr.sin_addr, hostinfo->h_addr_list[0],
		hostinfo->h_length);
	server_addr.sin_port = htons(atoi(server_port));

	// Step 3: connect to the server
	if (connect(sock, (struct sockaddr *)&server_addr,
		sizeof(server_addr)) < 0) {
		perror("connect");
		exit(1);
	}
	
	
	
	//////////////////////////////
	

	struct sockaddr_in client_name;
	int udp_sock;
	

	if ((udp_sock = socket(PF_INET, SOCK_DGRAM, 0)) < 0) {
		perror("socket");
		exit(1);
	}
	
	//Step 2: set up a sockaddr structure to contain the
	//name we want to assign to the socket 
	client_name.sin_family = PF_INET;//change
	client_name.sin_addr.s_addr = htonl(INADDR_ANY);
	client_name.sin_port = htons(udp_port);
	
	//i guess the problem is here....
	//Step 3: bind the name to the socket 
	if (bind(udp_sock, (struct sockaddr *)&client_name, sizeof(client_name)) < 0) {
		perror("bind");
		exit(1);
	}
	//change something regarding the client_name struct
	//do we need to do any other steps???

	//do I need to set up servername?
	//Step 4: set up server's name 
	/*
	server_name.sun_family = PF_UNIX;//
	strcpy(server_name.sun_path, sname);
	server_len = strlen(sname) + sizeof(server_name.sun_family);
	*/
	
	//int total_station_num;
	
	char buf[BUFFERSIZE];	
	char music_buf[1024];
	int hello_has_sent = 0;
	while(1) {
        //TODO: call select, read, and process_data
		fd_set my_fdset;
		FD_ZERO(&my_fdset);
		FD_SET(sock, &my_fdset);
		//I think 0 stands for STDIN
		FD_SET(0, &my_fdset);
		FD_SET(udp_sock, &my_fdset);
	  
		//for write purpose
		fd_set my_fdset2;
		FD_ZERO(&my_fdset2);
		FD_SET(sock, &my_fdset2);
		FD_SET(1, &my_fdset2);
			
		struct timeval my_timeval = {0, 0};
		fcntl(sock, F_SETFL, O_NONBLOCK);
		
		int ready = select(999999999, &my_fdset, &my_fdset2, NULL, &my_timeval);
		if (ready == -1) {
			//perror(strerror(errno));
			continue;
		} else {
			//meaning the tcp socket for read is ready
			
			if ((hello_has_sent == 0) && (FD_ISSET(sock, &my_fdset2) != 0)) {
				
				uint8_t command_type = 0;
				if (write(sock, &command_type, 1) < 0) {
					perror("less than 0\n");
				}
				
				udp_port = htons(udp_port);
				write(sock, &udp_port, 2);
				
				hello_has_sent = 1;
			}			
			
			//
			if ((FD_ISSET(udp_sock, &my_fdset) != 0)) {
				//printf("music ready\n");
				memset(music_buf, 0, 1024);
				read(udp_sock, music_buf, 1024);
				//write(1, music_buf, 1024);			
			}
			
			if (FD_ISSET(sock, &my_fdset) != 0) {
				//printf("socket is ready to receive\n");
				
				
				memset(buf, 0, BUFFERSIZE);
				int n =0;
				if ((n = read(sock, buf, BUFFERSIZE)) <= 0) {
				      perror("fatal\n");
				      exit(1);
				}
				
				int type = buf[0];
				int size = buf[1];
				//size = ntohs(size);
				printf("number of s:%d\n", size);
				
				
				
				for (int i = 2; i < 2 + size; i++) {
					fprintf(stderr, "%c", buf[i]);
				}
				fprintf(stderr, "\n");
				
				if (type == 2) {
					break;
				}
				
			}
			
			//meaning the STDIN for read is ready
			if ((FD_ISSET(0, &my_fdset) != 0) && (FD_ISSET(sock, &my_fdset2) != 0)) {
				
				memset(buf, 0, BUFFERSIZE);
				read(0, buf, BUFFERSIZE);
				
				for (unsigned int i = 0; i < strlen(buf); i++) {
					if(buf[i] == '\n') {
						buf[i] = '\0';
					}
				}
				
				if ((strcmp(buf, "q") == 0) | (strcmp(buf, "quit") == 0)) {
					break;
				} else if ((atoi(buf) > 0) | (strcmp(buf, "0") == 0)) {
					uint16_t station_number = atoi(buf);
					station_number = htons(station_number);
					uint8_t command_type = 1;
					write(sock, &command_type, 1);
					write(sock, &station_number, 2);
					
				} else if (strcmp(buf, "") == 0) {
					
				} else {
					fprintf(stderr, "invalid command.\n");
				}
				
			}
		}
	}

	return 0;
}
