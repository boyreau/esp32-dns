/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   pstr8_join.c                                              +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/23 00:46:34 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/24 02:22:06 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "pstring.h"

#include <stdlib.h>
#include <string.h>

pstr8_t pstr8_join(pstr8_t s1, pstr8_t s2)
{
	if (s1 == NULL)
		s1 = "";
	if (s2 == NULL)
		s2 = "";
	pstr8_t new = malloc(pstr8_len(s1) + pstr8_len(s2) + 1);

	((uint8_t *) new)[0] = (uint8_t) pstr8_len(s1) + pstr8_len(s2);

	memcpy(new + 1, s1 + 1, pstr8_len(s1));
	memcpy(new + pstr8_len(s1) + 1, s2 + 1, pstr8_len(s2));
	// pstr8_cat(new, s2);
	return new;
}
