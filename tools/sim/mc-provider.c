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
#include <strings.h>
#include <arpa/inet.h>
#include <netinet/in.h>
#include <sys/types.h>
#include <sys/socket.h>

typedef struct message_info {
	unsigned short serverload;
	unsigned int isavailable:1;
	unsigned int spectrum;
} message ;



void diewitherror(char *data,int rcode) {

	fprintf(stderr,"%s\n",data);
	exit(rcode);
}

int main(int argc,char *argv[]) {

	int r_send,sock;
	struct in_addr serv_interface;
	struct sockaddr_in groupSock;
	message mymsg;
	
	sock = socket(AF_INET, SOCK_DGRAM, 0);

	if ( sock < 0 )
		 diewitherror("problem Openning Socket",1);
	else
		printf("openning Socket ... O.K.\n");

	/* 
 	 * setting up the multicast IP Address
 	 * for server usage
 	 */

	memset((char*)&groupSock , 0 , sizeof(groupSock));
	groupSock.sin_family = AF_INET;
	groupSock.sin_addr.s_addr = inet_addr("226.1.1.1");
	groupSock.sin_port = htons(4321);

	serv_interface.s_addr = inet_addr("192.168.122.11");
	if(setsockopt(sock, IPPROTO_IP , IP_MULTICAST_IF , (char*)&serv_interface, sizeof(serv_interface)) < 0 ) {
		diewitherror("Setting Local interface error",2);
	}
	else {
		printf("Setting the local interface is O.K\n");
	}


	memset(&mymsg , 0 , sizeof(mymsg));

	mymsg.serverload = 10;
 	mymsg.isavailable = 1;
        mymsg.spectrum = 50;
	
	r_send = sendto(sock , (message*)&mymsg , sizeof(mymsg) , 0 , (struct sockaddr *)&groupSock , sizeof(groupSock));

	if ( r_send < 0 )
		diewitherror("Problem sending the Struct over multicast",3);
	else
		printf("sending the struct over multicast was successful\n");

	close(sock);
	return 0;
}
