/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   dns.c                                                     +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/17 22:58:38 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/01 20:10:22 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "dns.h"

const char *dns_classes[] = {
	"  ",
	"IN",
	"CS",
	"CH",
	"HS",
};

const char *dns_types[] = {
	"",
	"A",
	"NS",
	"MD",
	"MF",
	"CNAME",
	"SOA",
	"MB",
	"MG",
	"MR",
	"NuLL",
	"WKS",
	"PTR",
	"HINFO",
	"MINFO",
	"MX",
	"TXT",
};
