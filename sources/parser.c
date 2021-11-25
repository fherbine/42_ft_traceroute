/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   parser.c                                           :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fherbine <fherbine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 20:15:10 by fherbine          #+#    #+#             */
/*   Updated: 2020/12/30 20:15:10 by fherbine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_traceroute.h"

static void	get_args(int start, int end, char **tab, t_traceroute **tracert)
{
	int i = start;
	int i2 = 1;

	while (i < end)
	{
		if (tab[i][0] != '-')
		{
			if ((*tracert)->name_or_service == NULL)
			{
				(*tracert)->name_or_service = tab[i];
				i++;
				continue;
			}
			else
				display_help(EXIT_FAILURE);
		}
		
		i2 = 1;
		while (tab[i][i2])
		{   
			char option = tab[i][i2];

			if (option == 'h')
				(*tracert)->options |= TRACERT_OPT_HELP;
			else if (option == 'I')
				(*tracert)->options |= TRACERT_OPT_ICMP;
			else
				invalid_option(option);
			i2++;
		}
		i++;
	}
}

void		parse(int argc, char **argv, t_traceroute *tracert)
{
	if (argc < 2)
		message_description_exit("usage error", \
								 "Destination address required", EXIT_FAILURE);
	get_args(1, argc, argv, &tracert);

	if (tracert->options & TRACERT_OPT_HELP)
		display_help(EXIT_SUCCESS);
}
