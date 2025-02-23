/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   pstring_join.c                                            +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/23 00:46:34 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/23 01:08:41 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "pstring.h"

#include <stdlib.h>
#include <string.h>

pstr8_t pstr8_join(pstr8_t s1, pstr8_t s2)
{
	pstr8_t new = malloc(pstr8_len(s1) + pstr8_len(s2) + 1);
	memcpy(new + 1, s1 + 1, pstr8_len(s1));
	pstr8_cat(new, s2);
	return new;
}
