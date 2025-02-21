/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   pstring.h                                                 +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/20 21:43:12 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/20 23:11:52 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#ifndef PSTRING_H
#define PSTRING_H

#include <stdbool.h>
#include <stdint.h>
typedef char *pstring8_t;

uint8_t pstrlen(pstring8_t str);

#endif
