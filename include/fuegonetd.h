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


typedef struct {
	unsigned int ver_opt:1;
	unsigned int port_opt:1;
	unsigned int int_opt:1;
	unsigned int conf_opt:1;
	unsigned int mcast_opt:1;
} opt_flags;

typedef struct network_options {
        const char *multi_addr;
        int port_num;
        const char *interface_name;
} network_opt;

typedef struct thread_data {
        network_opt thnetworkopt;
        char **client_tbl;

} threadata ;


void *providor_main(void *table_clnt);

void *requester_main(void *table_clnt);
