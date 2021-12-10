/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   debug.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fherbine <fherbine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2021/10/21 13:52:49 by fherbine          #+#    #+#             */
/*   Updated: 2021/10/21 13:52:49 by fherbine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_traceroute.h"

void print_buffer(void *buffer_addr, ssize_t len)
{
	ssize_t count = 0;

	while (count < len)
	{
		if (count % 8 == 0)
			printf("%02ld | " , (len / 8) - (count / 8));

		printf("%02X ", *((uint8_t *)buffer_addr++));

		if ((count + 1) % 8 == 0 && count)
			printf("\n");

		count++;
	}
	printf("\n");
}

void print_json_dict(t_json *json)
{
	char types[7][255] = {
		"null_type",
		"bool_type",
		"int_type",
		"float_type",
		"string_type",
		"list_type",
		"dict_type"
	};
	
	t_json_dict *p = (t_json_dict *)json->data;

	while (p)
	{
		printf("%s %s %p", p->key, types[p->param_type], p->parameter);
		switch (p->param_type)
		{
		case JSON_TYPE_NULL:
			printf("\t(null)");
			break;

		case JSON_TYPE_BOOL:
			printf("\t%s", (*((uint8_t *)p->parameter)) ? "true": "false");
			break;

		case JSON_TYPE_INT:
			printf("\t%ld", (*((int64_t *)p->parameter)));
			break;

		case JSON_TYPE_FLOAT:
			printf("\t%lf", (*((double *)p->parameter)));
			break;

		case JSON_TYPE_STR:
			printf("\t%s", (char *)p->parameter);
			break;
		default:
			break;
		}
		p = p->next;
		printf("\n");
	}
	
}