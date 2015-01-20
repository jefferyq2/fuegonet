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


#include <linux/kernel.h>
#include <linux/sys.h>
#include <stdio.h>
#include <sys/sysinfo.h>

int main ()
{
 /* Conversion constants. */
 const long minute = 60;
 const long hour = minute * 60;
 const long day = hour * 24;
 const double megabyte = 1024 * 1024;
 /* Obtain system statistics. */
 struct sysinfo si;
 sysinfo (&si);
 /* Summarize interesting values. */
 printf ("system uptime : %ld days, %ld:%02ld:%02ld\n",
     si.uptime / day, (si.uptime % day) / hour,
     (si.uptime % hour) / minute, si.uptime % minute);
 printf ("total RAM   : %5.1f MB\n", si.totalram / megabyte);
 printf ("free RAM   : %5.1f MB\n", si.freeram / megabyte);
 printf ("process count : %d\n", si.procs);
 return 0;
}

