/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   pstr8_cat.c                                               +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/23 01:08:49 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/23 02:38:21 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "pstring.h"

#include <string.h>

pstr8_t pstr8_cat(pstr8_t s1, pstr8_t s2)
{
	return memcpy(s1 + pstr8_len(s1) + 1, s2 + 1, pstr8_len(s2));
}
