/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   list.c                                                    +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/20 12:25:36 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/20 12:32:31 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "llist.h"

#include <stddef.h>

void lst_add_back(linked_list **l, linked_list *new)
{
	while (*l != NULL)
		l = &((*l)->next);
	*l = new;
}
