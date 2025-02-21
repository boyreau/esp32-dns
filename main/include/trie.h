/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   trie.h                                                    +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/20 17:27:47 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/20 23:11:47 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#ifndef TRIE_H
#define TRIE_H

#include "pstring.h"
#define DNS_CHARSET_LEN 37
#define DNS_CHARSET		"abcdefghijklmnopqrstuvwxyz0123456789-"

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
		struct trie_s *children[DNS_CHARSET_LEN];
		uint8_t		   letter;
} trie_t;

void trie_add(trie_t *head, pstring8_t str);

#endif
