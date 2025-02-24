/* ************************************************************************** */
/*                                                                            */
/*                                                              ++            */
/*   dns_read_labels.c                                         +**+   +*  *   */
/*                                                             ##%#*###*+++   */
/*   By: aboyreau <bnzlvosnb@mozmail.com>                     +**+ -- ##+     */
/*                                                            # *   *. #*     */
/*   Created: 2025/02/22 18:14:49 by aboyreau          **+*+  * -_._-   #+    */
/*   Updated: 2025/02/24 02:55:53 by aboyreau          +#-.-*  +         *    */
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
	if (size == 0)
		return NULL;

	pstr8_t label = malloc(size + 1);
	memcpy(label, question, size + 1);
	return label;
}

static pstr8_t build_dns_from_labels(char *domain_name, char *label)
{
	char	dot[2];
	pstr8_t tmp;

	dot[0] = 1;
	dot[1] = '.';

	if (domain_name != NULL)
	{
		tmp			= domain_name;
		domain_name = pstr8_join(domain_name, dot);
		free(tmp);
	}
	tmp			= domain_name;
	domain_name = pstr8_join(domain_name, label);
	free(tmp);
	return domain_name;
}

pstr8_t dns_read_labels(char *message, size_t messagelen)
{
	pstr8_t label		= NULL;
	pstr8_t domain_name = NULL;
	size_t	index		= 0;

	while (1)
	{
		if (index != 0 && index - 1 > messagelen)
			break;

		label = dns_read_label(message + index);
		if (label == NULL)
			break;

		domain_name = build_dns_from_labels(domain_name, label);

		index += pstr8_len(label) + 1;
		free(label);
	}
	ESP_LOGI(TAG, "DNS labels parsed");
	return domain_name;
}
