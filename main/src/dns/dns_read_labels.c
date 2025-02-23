/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   dns_read_labels.c                                         +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/22 18:14:49 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/23 01:07:30 by aboyreau          +#-.-*  +         *    */
/*                                                     *-.. *   ++       #    */
/* ************************************************************************** */

#include "dns.h"
#include "esp_log.h"
#include "llist.h"
#include "pstring.h"

#include <cc.h>
#include <stdlib.h>
#include <string.h>
#include <sys/unistd.h>

extern const char *TAG;

static pstr8_t dns_read_label(char *question)
{
	uint8_t size = (uint8_t) question[0];
	// size		 = ntohs(size);
	if (size == 0)
		return NULL;

	ESP_LOGI(TAG, "Size expected: %d", size);

	pstr8_t label = malloc(size + 1);
	memcpy(label, question, size + 1);
	write(1, "read: ", 6);
	write(1, question + 1, *label);
	write(1, "\n", 1);
	return label;
}

int dns_read_labels(label_list **labels, char *message, size_t messagelen)
{
	size_t index = 0;

	while (1)
	{
		if (index != 0 && index - 1 > messagelen)
			break;

		pstr8_t question = dns_read_label(message + index);
		if (question == NULL)
			break;

		label_list *new = malloc(sizeof(*new));
		new->label		= question;
		new->next		= NULL;

		lst_add_back(labels, new);
		index += pstr8_len(question) + 1;
	}
	ESP_LOGI(TAG, "DNS labels parsed");
	return 0;
}
