/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   main.c                                             :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fherbine <fherbine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/10/31 10:57:48 by fherbine          #+#    #+#             */
/*   Updated: 2020/12/29 16:52:59 by fherbine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_traceroute.h"

static t_traceroute	*get_tracert_struct(void)
{
	t_traceroute *tracert = NULL;

	if (!(tracert = (t_traceroute *)malloc(sizeof(t_traceroute))))
		exit(EXIT_FAILURE);
	
	ft_bzero(tracert, sizeof(t_traceroute));
	return (tracert);
}

void free_tracert(t_traceroute *tracert)
{
	free(tracert);
}

int					main(int argc, char **argv)
{
	t_traceroute *tracert = get_tracert_struct();
	parse(argc, argv, tracert);
	dnslookup(tracert->name_or_service, (t_sockaddr *)&(tracert->ipv4), AF_INET);
	printf("IP is %s\n", inet_ntoa(tracert->ipv4.sin_addr));
	free_tracert(tracert);
	return (0);
}
