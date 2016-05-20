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

#include <netdb.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <ifaddrs.h>


typedef struct args_flags {
	unsigned int argopt_v:1;
	unsigned int argopt_m:1;
	unsigned int argopt_p:1;
	unsigned int argopt_d:1;
	unsigned int argopt_t:1;
	unsigned int argopt_i:1;
	unsigned int argopt_n:1;
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

	printf("Usage:  simprequester\n");
    printf("\t\t -v - Be Verbose when running the command\n");
    printf("\t\t -h - Display this manu\n");
    printf("\t\t -m - Set the Multicast address (Default:226.1.1.1)\n");
    printf("\t\t -p - Set the Listening Port (Default:4321)\n");
    printf("\t\t -i - Set the interface to bind to (Default : ANY ) \n");
    printf("\t\t -d - Run the simrequester as a daemon (Default:Off)\n");
    printf("\n");

    exit(rcode);

}

int main (int argc , char *argv[]) {


	int family,s;
	int c,reuse,r_read,sock;
	struct sockaddr_in lstn_interface; // local interface
	struct sockaddr_in prov_interface;    // provider source interface
	struct sockaddr_in *sa;
	socklen_t prov_len;
	char cmbuf[0x100];
	char *mcast_addr, *intf_name;
	struct ip_mreq group; // the multicast group
	int port_num,gsize=0,prov_size;
	struct ifaddrs *ifap, *ifa;
	char ip_addr[NI_MAXHOST];

	argsflags myargflags = { 0 , 0 , 0 ,0 };

	while ((c = getopt (argc, argv, "vdhm:i:p:")) != -1)
		 switch (c) {
		 	 case 'v':
		 		 	 myargflags.argopt_v = 1;
		 	 break;

		 	 case 'i':
		 		 	 myargflags.argopt_i = 1;
		 		 	 intf_name = optarg;
		 	 break;

		 	 case 'd':
		 		 	 myargflags.argopt_d = 1;
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
		 		 if ( (optopt == 'm') || (optopt == 'i') || (optopt == 'p') ) {
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


	if ( myargflags.argopt_p == 0) {
			port_num = 4321;
		}

	if ( myargflags.argopt_m == 0) {
		mcast_addr = calloc(sizeof(char),sizeof("226.1.1.1"));
		strcpy(mcast_addr,"226.1.1.1");
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
	}


	message *mymsg = malloc(sizeof(struct message_info));

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	if ( sock < 0 )
		 diewitherror("problem Openning Socket",1,sock);
	else
		printf("openning Socket ... O.K.\n");

	/*
  	 * setting up the the localinterface for reuse
	*/

	reuse = 1;

	if (setsockopt(sock , SOL_SOCKET , SO_REUSEADDR , (char*)&reuse , sizeof(reuse)) < 0 ) {

		diewitherror("unable to reuse the Address... exiting\n",1,sock);
	}
	else {
		printf("Setting the IP Address for reuse .... O.K.\n");
	}


	/*
  	 * setting up all of the interfaces to listen to the requested port
  	 * for a multicast signal and binding the port
	*/

	memset((char*)&lstn_interface , 0 , sizeof(lstn_interface));

	lstn_interface.sin_family = AF_INET;
	lstn_interface.sin_port = htons(port_num);

	//if (myargflags.argopt_i == 1) {
	//}
	//else {
	//
	//	lstn_interface.sin_addr.s_addr = htonl(INADDR_ANY);;
	//
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
                                   printf("found selected interface : %s with IP: %s \n",ifa->ifa_name,ip_addr);
                                }

                                break;

                            }
                        }
                }

                lstn_interface.sin_addr.s_addr = inet_addr(ip_addr);

                freeifaddrs(ifap);
        }

	else {
		lstn_interface.sin_addr.s_addr = INADDR_ANY;
	}

	if (bind(sock , (struct sockaddr*)&lstn_interface, sizeof(lstn_interface))) {

		close(sock);
		diewitherror("unable to bind the Address or port ... exiting\n",2,sock);
	}
	else {
		printf("Binding datagram socket ... O.K.\n");
	}

	/*
 	 * setting the parameters to listen to a multicast address
 	 */

	//group.imr_multiaddr.s_addr = inet_addr(mcast_addr);
	group.imr_multiaddr.s_addr = inet_addr(mcast_addr);
	if (myargflags.argopt_i == 1) {
		group.imr_interface.s_addr = inet_addr(ip_addr);
	}
	else {
		group.imr_interface.s_addr = INADDR_ANY;
	}


	if (setsockopt(sock , IPPROTO_IP , IP_ADD_MEMBERSHIP , (char*)&group , sizeof(group))) {

		diewitherror("unable to join a multicast group ... exiting",2,sock);
	}
	else {
		printf("joining to a multicast group successfully\n");
	}


	char src_ipaddr[32];

	gsize = sizeof(group);
	prov_size = sizeof(prov_interface);

	do {

		if((r_read = recvfrom(sock, mymsg, sizeof(*mymsg), 0,
			(struct sockaddr*) &prov_interface, &prov_size)) < 0 ) {
			diewitherror("unable to read datagram message ... exiting\n",3,sock);
		}

		else {
			//recvmsg(sock, &mh, 0);
		//	prov_len = sizeof(prov_interface);
		//	getpeername(sock , (struct sockaddr*) &prov_interface , &prov_len );
			//printf("Reading message from the server %s... O.K.\n",inet_ntoa(prov_interface.sin_addr));
			inet_ntop(AF_INET , &(prov_interface.sin_addr), src_ipaddr , 32);
			printf("Reading message from the server %s... O.K.\n",src_ipaddr);
			printf("the server spectrum is %i\n",mymsg->spectrum);
			printf("the Text Message is :\"%s\" \n",mymsg->textmsg);
		}
	}

	while ( myargflags.argopt_d == 1 );

	if ( myargflags.argopt_m == 0) {
		free(mcast_addr);
	}
	return 0;
}

