/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   pstring_rev.c                                             +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/23 02:16:41 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/23 02:19:42 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "pstring.h"

void pstr8_rev(pstr8_t s)
{
	uint8_t l = 1;
	uint8_t r = pstr8_len(s) - 1;

	while (l < r)
	{
		s[l] = (char) (s[l] ^ s[r]);
		s[r] = (char) (s[l] ^ s[r]);
		s[l] = (char) (s[l] ^ s[r]);

		l++;
		r--;
	}
}
