*
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

int main (int argc , char *argv[]) {


	int reuse,r_read,sock;
	struct sockaddr_in lstn_interface; // local interface 
	struct sockaddr_in prov_interface;    // provider source interface
	socklen_t prov_len;
	char cmbuf[0x100];
	struct ip_mreq group; // the multicast group
	int gsize=0,prov_size;
	message *mymsg = malloc(sizeof(struct message_info));;

	sock = socket(AF_INET, SOCK_DGRAM, 0);

	if ( sock < 0 )
		 diewitherror("problem Openning Socket",1);
	else
		printf("openning Socket ... O.K.\n");

	/* 
  	 * setting up the the localinterface for reuse
	*/

	reuse = 1;

	if (setsockopt(sock , SOL_SOCKET , SO_REUSEADDR , (char*)&reuse , sizeof(reuse)) < 0 ) {

		diewitherror("unable to reuse the Address... exiting\n",1);
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
	lstn_interface.sin_port = htons(4321);
	lstn_interface.sin_addr.s_addr = INADDR_ANY;

	if (bind(sock , (struct sockaddr*)&lstn_interface, sizeof(lstn_interface))) {
	
		close(sock);
		diewitherror("unable to bind the Address or port ... exiting\n",2);
	}
	else {
		printf("Binding datagram socket ... O.K.\n");
	}

	/* 
 	 * setting the parameters to listen to a multicast address
 	 */	
	
	group.imr_multiaddr.s_addr = inet_addr("226.1.1.1");
	group.imr_interface.s_addr = inet_addr("192.168.122.11");

	if (setsockopt(sock , IPPROTO_IP , IP_ADD_MEMBERSHIP , (char*)&group , sizeof(group))) {

		close(sock);
		diewitherror("unable to join a multicast group ... exiting\n",2);
	}
	else {
		printf("joining to a multicast group successfully\n");
	}
	

	char src_ipaddr[32];

	gsize = sizeof(group);
	prov_size = sizeof(prov_interface);
		if((r_read = recvfrom(sock, mymsg, sizeof(*mymsg), 0,
			(struct sockaddr*) &prov_interface, &prov_size)) < 0 ) {
			close(sock);
			diewitherror("unable to read datagram message ... exiting\n",3);
		}

		else {
			//recvmsg(sock, &mh, 0);
		//	prov_len = sizeof(prov_interface);
		//	getpeername(sock , (struct sockaddr*) &prov_interface , &prov_len );
			//printf("Reading message from the server %s... O.K.\n",inet_ntoa(prov_interface.sin_addr));
			inet_ntop(AF_INET , &(prov_interface.sin_addr), src_ipaddr , 32);
			printf("Reading message from the server %s... O.K.\n",src_ipaddr);
			printf("the server spectrum is %i\n",mymsg->spectrum);
		}
	
	return 0;
}
