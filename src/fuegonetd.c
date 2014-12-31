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

#include "fuegonetd.h"
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <strings.h>
#include <unistd.h>
#include <pthread.h>
#include <libconfig.h>

void Help(int rcode) {

        printf("Usage:  fuegonetd\n");
        printf("\t\t -v - Be Verbose when running the Daemon\n");
        printf("\t\t -h - Display this manu\n");
        printf("\t\t -p - set the UDP port number (Default: 9777) \n");
        printf("\t\t -i - set the interface for the application socket (Default: Any)\n");
        printf("\t\t -m - speficied the requested multicast to be used\n");
        printf("\t\t -c - set the configuration File\n");
        printf("\n");

	exit(rcode);
}


typedef struct {
	unsigned int ver_opt:1;
	unsigned int port_opt:1;
	unsigned int int_opt:1;
	unsigned int conf_opt:1;
	unsigned int mcast_opt:1;
} opt_flags;

int main(int argc,char *argv[]) {

	config_t cfg;
	config_setting_t *setting;
	threadata *the_thread_data;
	int c,thread_prov_res, thread_req_res;
	pthread_t thread_provider, thread_requester;
	opt_flags optflags = { 0 , 0 , 0 , 0 , 0 };
	char *conf_file;
	int port_num;
	const char *int_name;

	the_thread_data = malloc(sizeof(threadata));

	while ((c = getopt (argc, argv ,"hvp:i:m:c:")) != -1 ) {

		switch(c) {
			case 'h':
					Help(0);
			break;

			case 'v':
					optflags.ver_opt = 1;
			break;

			case 'p':
					the_thread_data->thnetworkopt.port_num = atoi(optarg);
					optflags.port_opt = 1;
			break;

			case 'i':
					the_thread_data->thnetworkopt.interface_name = optarg;
					optflags.int_opt = 1;
			break;

			case 'm':
					the_thread_data->thnetworkopt.multi_addr = optarg;
					optflags.mcast_opt = 1;
			break;

			case 'c':
					conf_file = optarg;
					optflags.conf_opt = 1;
			break;

			case '?':

				if ( (optopt == 'p') || (optopt == 'i'))
                        	{
                           		fprintf (stderr," " "Option -%c requires an argument.\n", optopt);
                        	}
                        
				else if (isprint (optopt))
                        	{	
                                	fprintf (stderr, "Unknown option `-%c'.\n", optopt);
                        	}
                        
				else {
                           		fprintf (stderr, "Unknown option character `\\x%x'.\n", optopt);
                           		exit(EXIT_FAILURE);
                        	}
			break;

			default:
				fprintf(stderr,"worng argument given\n");
				exit(1);
		}
	}

	// check if the -c argument is been used , if not
	// then use the sysconfdir from the autotools to set the
	// configuration file 
	
	if ( optflags.conf_opt == 1 ) {
		config_init(&cfg);

		if (!config_read_file(&cfg, conf_file)) {
			fprintf(stderr,"%s:%d - %s\n", config_error_file(&cfg), config_error_line(&cfg) , config_error_text(&cfg));
			config_destroy(&cfg);
			return 1;

		}

		setting = config_lookup(&cfg , "network");
	
		if (setting != NULL) {
			config_setting_lookup_int(setting , "udp_port" , &the_thread_data->thnetworkopt.port_num);

			config_setting_lookup_string(setting , "interface" , &the_thread_data->thnetworkopt.interface_name);

			config_setting_lookup_string(setting , "multicast" , &the_thread_data->thnetworkopt.multi_addr);
		}
	}

	if (( optflags.port_opt == 0 ) && (!the_thread_data->thnetworkopt.port_num)){
		the_thread_data->thnetworkopt.port_num = 4321;
	}
	
	if (( optflags.int_opt == 0 ) && (!the_thread_data->thnetworkopt.interface_name)) {
		//the_thread_data->thnetworkopt.interface_name = (char*)calloc(5,sizeof(char*));
		the_thread_data->thnetworkopt.interface_name = strdup("ANY");
	}	

	if (( optflags.mcast_opt == 0 ) && (!the_thread_data->thnetworkopt.multi_addr)){
		//the_thread_data->thnetworkopt.multi_addr = (char*)calloc(16,sizeof(char*));
		the_thread_data->thnetworkopt.multi_addr = strdup("226.1.1.1");
	}

	if ( optflags.ver_opt == 1 ) {
			
			printf("UDP network port is: %d\n",the_thread_data->thnetworkopt.port_num);
                        printf("Network interface is: %s\n",the_thread_data->thnetworkopt.interface_name);
                        printf("the Multicast that will be used  is: %s\n",the_thread_data->thnetworkopt.multi_addr);
        }


	thread_prov_res = pthread_create(&thread_provider, NULL , providor_main , (void*)the_thread_data);

	if(thread_prov_res) {
		fprintf(stderr,"Error - pthread_create() return code: %d\n",thread_prov_res);
		exit(EXIT_FAILURE);
	}

	thread_req_res = pthread_create(&thread_requester, NULL , requester_main , (void*)the_thread_data);

	if(thread_req_res) {
		fprintf(stderr,"Error - pthread_create() return code: %d\n",thread_req_res);
		exit(EXIT_FAILURE);
	}

	pthread_join(thread_provider, NULL);
	pthread_join(thread_requester, NULL); 

	close(STDIN_FILENO);
	close(STDOUT_FILENO);
	close(STDERR_FILENO);

	free(the_thread_data);
	return 0;
}
