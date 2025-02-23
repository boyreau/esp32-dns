/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   llist.h                                                   +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/20 12:25:22 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/20 12:30:50 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#ifndef LLIST_H
#define LLIST_H

typedef struct s_list
{
		struct s_list *next;
		char		  *label;
} label_list;

void lst_add_back(label_list **l, label_list *new);

#endif
