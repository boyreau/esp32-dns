/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   main.c                                                    +**+   +*  *   */
/*                                                             ##%#*###*+++   */ /*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/21 12:50:25 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/22 17:15:49 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "pstring.h"
#include "trie.h"

#include <fcntl.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>
#include <unistd.h>

#define ADDR_LEN	8
#define BUFFER_SIZE 512

int main(void)
{
	trie_t head = {0};
	FILE  *f	= fopen("./adservers.txt", "r");

	if (f == NULL)
	{
		perror("fopen");
		exit(1); // NOLINT
	}
	if (setvbuf(f, NULL, _IOLBF, 0))
	{
		fclose(f);
		perror("setvbuf");
		exit(1); // NOLINT
	}
	while (!ferror(f) && !feof(f))
	{
		char line[BUFFER_SIZE]; // Max DNS 255 + address 8 + whitespace 1 +
								// newline 1 will fit.
		fgets(line, BUFFER_SIZE, f);
		if (line[0] == '#')
			continue;
		line[ADDR_LEN - 1] = (char) strlen(line + ADDR_LEN);
		pstr8_rev(line + ADDR_LEN - 1);
		trie_add(&head, line + ADDR_LEN - 1);
		write(1, line, strlen(line));
	}
	char str[64];
	str[63] = '\0';
	trie_display(&head, 0, str);
}
