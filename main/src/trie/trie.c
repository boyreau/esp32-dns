/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   trie.c                                                    +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/20 22:24:39 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/02 13:05:53 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "pstring.h"
#include "trie.h"

#include <ctype.h>
#include <errno.h>
#include <esp_log.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>

uint8_t trie_indexof(int c)
{
	uint8_t i;

	if (!isalnum(c) && c != '-' && c != '.')
		return 0;

	if (c >= 'A' && c <= 'Z')
		c = c - ('A' - 'a');

	for (i = 1; DNS_CHARSET[i] != c; i++)
		;
	return i;
}

void trie_add(struct trie_s *head, const pstr8_t str, uint32_t ip)
{
	for (uint8_t i = 1; i <= *str; i++)
	{
		uint8_t index = trie_indexof(str[i]);
		if (head != NULL && head->children[index] == NULL)
			head->children[index] = calloc(sizeof(trie_t), 1);
		head = head->children[index];
	}
	head->children[0] = (void *) ip;
}

void trie_display(struct trie_s *head, uint8_t depth, char str[])
{
	if (head->children[0] != NULL)
	{
		str[depth] = 0;
		pstr8_rev(str);
		printf("%s\n", str);
	}
	for (uint8_t i = 0; i <= DNS_CHARSET_LEN; i++)
	{
		if (head->children[i] != NULL)
		{
			str[depth] = DNS_CHARSET[i];
			trie_display(head->children[i], depth + 1, str);
		}
	}
}

void *trie_get(struct trie_s *head, pstr8_t str)
{
	for (uint8_t i = 1; i <= pstr8_len(str); i++)
	{
		int index = trie_indexof(str[i]);
		if (head->children[index] == NULL)
			return NULL;
		head = head->children[index];
	}
	return head->children[0];
}
