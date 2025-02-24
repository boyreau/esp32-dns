/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   list.c                                                    +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/20 12:25:36 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/24 02:50:57 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "llist.h"

#include <stddef.h>
#include <stdlib.h>

void lst_add_back(linked_list **l, linked_list *new)
{
	while (*l != NULL)
		l = &((*l)->next);
	*l = new;
}

linked_list *lst_new(char *data)
{
	linked_list *n = malloc(sizeof(*n));

	n->label = data;
	n->next	 = NULL;
	return n;
}

void lst_add_new(linked_list **l, void *new)
{
	while (*l != NULL)
		l = &((*l)->next);
	*l = lst_new(new);
}
