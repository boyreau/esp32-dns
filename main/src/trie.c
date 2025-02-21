/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   trie.c                                                    +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/20 22:24:39 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/21 00:38:55 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "pstring.h"
#include "trie.h"

#include <stdlib.h>

uint8_t indexof(pstring8_t set, char c)
{
	for (uint8_t i = 1; i <= *set; i++)
		if (set[i] == c)
			return i;
	return 0;
}

void trie_add(struct trie_s *head, pstring8_t str)
{
	for (uint8_t i = 1; i <= *str; i++)
	{
		if (head->children[i] == NULL)
		{
			head->children[i]		  = malloc(sizeof(trie_t));
			head->children[i]->letter = str[i];
		}
	}
}
