/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   trie.h                                                    +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/20 17:27:47 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/03/02 20:27:50 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#ifndef TRIE_H
#define TRIE_H

#include "pstring.h"

#define DNS_CHARSET_LEN 38
// % is 37, it makes DNS_CHARSET a valid pstring
#define DNS_CHARSET "%abcdefghijklmnopqrstuvwxyz0123456789-."

typedef struct array_s
{
		uint8_t size;
		void   *children;
} array_t;

// I want to store a lot of strings in an optimized manner
// I also want a fast access time
// I don't care if building it is slow
typedef struct trie_s
{
		struct trie_s *children[DNS_CHARSET_LEN + 1];
} trie_t;

uint8_t trie_indexof(int c);
void	trie_add(struct trie_s *head, pstr8_t str, uint32_t ip);
void   *trie_get(struct trie_s *head, pstr8_t str);

#endif
