/*
 * the fuegonet is an open source project for Enterprise applicaion
 * interface without a coordinator server
 * Copyright (C) yyyy  Oren Oichman
 *
 * This program is free software; you can redistribute it and/or
 * modify it under the terms of the GNU General Public License
 * as published by the Free Software Foundation; either version 2
 * of the License, or (at your option) any later version.
 *
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 *
 * You should have received a copy of the GNU General Public License
 * along with this program; if not, write to the Free Software
 * Foundation, Inc., 51 Franklin Street, Fifth Floor, Boston, MA  02110-1301, USA.
 *
 * Gnomovision version 1, Copyright (C) Oren Oichman
 * Gnomovision comes with ABSOLUTELY NO WARRANTY; for details
 * type `fuegonetctl -h'.  This is free software, and you are welcome
 * to redistribute it under certain conditions; type `canoectl -h'
 * for details.
 */


#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <strings.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>

typedef struct args_flags {
	unsigned int argopt_v:1;
	unsigned int argopt_m:1;
	unsigned int argopt_p:1;
	unsigned int argopt_n:1;
	unsigned int argopt_t:1;
	unsigned int argopt_i:1;
} argsflags;

typedef struct message_info {
	unsigned short serverload;
	unsigned int isavailable:1;
	int spectrum;
	char textmsg[1024];
} message ;



void diewitherror(char *data,int rcode,int sock) {

	fprintf(stderr,"%s\n",data);

	if ( sock >= 0){
		close(sock);
	}

	exit(rcode);
}

void Help(int rcode) {

	printf("Usage:  simprovider\n");
    printf("\t\t -v - Be Verbose when running the command\n");
    printf("\t\t -h - Display this manu\n");
    printf("\t\t -m - Set the Multicast address ( Default : 226.1.1.1 )\n");
    printf("\t\t -p - Set the Listening Port ( Default : 4321 )\n");
    printf("\t\t -i - Set the interface to bind to (Default : ANY ) \n");
    printf("\t\t -t - Text message to send over the multicast( Default : Hello Multicast )\n");
    printf("\n");

    exit(rcode);

}

int main(int argc,char *argv[]) {


	int port_num,c,r_send,sock;
	int family,s;
	struct in_addr serv_interface;
	struct sockaddr_in *sa;
	struct sockaddr_in groupSock;
	char *mcast_addr, *tmptext=NULL;
	char *intf_name;
	message mymsg;
	struct ifaddrs *ifap, *ifa;
	char ip_addr[NI_MAXHOST];
	
	argsflags myargflags = { 0 , 0 , 0 ,0 ,0 , 0};

	memset(&mymsg , 0 , sizeof(mymsg));
	mymsg.serverload = 10;
	mymsg.isavailable = 1;
	mymsg.spectrum = 50;

	while ((c = getopt (argc, argv, "vhi:m:p:t:")) != -1)
		 switch (c) {
		         case 'i':
		    	 	 myargflags.argopt_i = 1;
		    	 	 intf_name = optarg;
			 break;

		 	 case 'v':
		 		 	 myargflags.argopt_v = 1;
		 	 break;

		 	 case 't':
		 		 	 myargflags.argopt_t = 1;
		 		 	 tmptext = optarg;
		         break;

		 	 case 'm':
		 	 	 	 myargflags.argopt_m = 1;
		 	 	 	 mcast_addr = optarg;
		 	 break;

		 	 case 'p':
		 		 	 myargflags.argopt_p = 1;
		 		 	 port_num = atoi(optarg);
		 	 break;

		 	 case 'h':
		 		 	 Help(0);
		 	 break;

		 	 case '?':
		 		 if ( (optopt == 'm') || (optopt == 'n') || (optopt == 'p') ) {
                          fprintf (stderr," " "Option -%c requires an argument.\n", optopt);
		 		 }

		 		 else if (isprint (optopt)) {
		 			 fprintf (stderr, "Unknown option `-%c'.\n", optopt);
		 		 }

		 		 else {
		 			 fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
		 			 exit(EXIT_FAILURE);
		 		 }
		 	 break;

		 	 default:
                               Help(1);
             break;

		 }

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	if ( sock < 0 )
		 diewitherror("problem Openning Socket",1,sock);
	else if ( myargflags.argopt_v == 1 ) {
		printf("openning Socket ... O.K.\n");
	}

	if ( myargflags.argopt_p == 0) {
		port_num = 4321;
	}

	if ( myargflags.argopt_m == 0) {
		mcast_addr = calloc(sizeof(char),sizeof("226.1.1.1"));
		strcpy(mcast_addr,"226.1.1.1");
	}

	if (myargflags.argopt_t == 1) {
		strncpy(mymsg.textmsg,tmptext,strlen(tmptext));
	}
	else {
		strncpy(mymsg.textmsg,"Hello Multicast",strlen("Hello Multicast"));

	}

	if (myargflags.argopt_n == 0) {

	}
	if ( myargflags.argopt_v == 1) {

		if ( myargflags.argopt_m == 0) {
			printf("using default Multicast address: 226.1.1.1 \n");
		}
		else {
			printf("using selected Multicast address: %s\n",mcast_addr);
		}

		if (myargflags.argopt_p == 0) {
			printf("using default Port: 4321 \n");
		}
		else {
			printf("using selected Port: %d\n",port_num);
		}

		if (myargflags.argopt_i == 1) {
			printf("using the selected interface: %s\n",intf_name);
		}

		else {
			printf("using all interfaces by default configuration\n");
		}

		if (myargflags.argopt_t == 1) {
			printf("using custom text message: %s \n",mymsg.textmsg);
		}
		else {
			printf("using Default Text Message : \"Hello Multicast\" \n");
		}
	}


	memset((char*)&groupSock , 0 , sizeof(groupSock));

	groupSock.sin_family = AF_INET;
	//groupSock.sin_addr.s_addr = inet_addr("226.1.1.1");
	groupSock.sin_addr.s_addr = inet_addr(mcast_addr);
	//groupSock.sin_port = htons(4321);
	groupSock.sin_port = htons(port_num);

	if (myargflags.argopt_i == 1) {
		getifaddrs (&ifap);
		for (ifa = ifap; ifa; ifa = ifa->ifa_next) {
			if (strcmp(intf_name,ifa->ifa_name) == 0) {

				family = ifa->ifa_addr->sa_family;

				if (family == AF_INET || family == AF_INET6) {

					s = getnameinfo(ifa->ifa_addr,
                    				(family == AF_INET) ? sizeof(struct sockaddr_in) :
						sizeof(struct sockaddr_in6),ip_addr, NI_MAXHOST, NULL, 0, NI_NUMERICHOST);

					if (s != 0) {
                				printf("getnameinfo() failed: %s\n", gai_strerror(s));
						diewitherror("unable to bind the requested inteface",3,sock);
					}
				if (myargflags.argopt_v == 1 ) {
					printf("found selected interface : %s\n",ifa->ifa_name);
				}

				break;

				}
			}
		}

		serv_interface.s_addr = inet_addr(ip_addr);
		if (myargflags.argopt_v == 1 ) {
			printf("using IP Addres : %s\n",ip_addr);
		}
	}

	else {
		serv_interface.s_addr = htonl(INADDR_ANY);
	}

	if(setsockopt(sock, IPPROTO_IP , IP_MULTICAST_IF , (char*)&serv_interface, sizeof(serv_interface)) < 0 ) {
		diewitherror("Setting Local interface error - unable to bind with  multicast",2,sock);
	}
	else {
		printf("Setting the local interface is O.K\n");
	}



	r_send = sendto(sock , (message*)&mymsg , sizeof(mymsg) , 0 ,
				    (struct sockaddr *)&groupSock , sizeof(groupSock));

	if ( r_send < 0 ) {
		diewitherror("Problem sending the Struct over multicast",3,sock);
	}

	else {

		if ( myargflags.argopt_v == 1) {
			printf("sending the struct over multicast was successful\n");
		}
	}

	close(sock);
	return 0;
}

