/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   pstring.h                                                 +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/20 21:43:12 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/23 02:09:08 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#ifndef PSTRING_H
#define PSTRING_H

#include <stdbool.h>
#include <stdint.h>

typedef char	 *pstr8_t;
typedef uint16_t *pstr16_t;
typedef uint32_t *pstr32_t;

uint8_t	 pstr8_len(pstr8_t str);
uint16_t pstr16_len(pstr16_t str);
uint32_t pstr32_len(pstr32_t str);

void	pstr8_rev(pstr8_t s);
pstr8_t pstr8_join(pstr8_t s1, pstr8_t s2);
pstr8_t pstr8_cat(pstr8_t s1, pstr8_t s2);

#endif
