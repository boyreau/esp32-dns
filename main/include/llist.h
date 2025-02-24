/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   llist.h                                                   +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/20 12:25:22 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/24 02:52:20 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#ifndef LLIST_H
#define LLIST_H

typedef struct s_list
{
		struct s_list *next;
		char		  *label;
} linked_list;

linked_list *lst_new(char *data);
void		 lst_add_back(linked_list **l, linked_list *new);
void		 lst_add_new(linked_list **l, void *new);

#endif
