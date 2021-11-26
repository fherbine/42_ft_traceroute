/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   nslookup.c                                         :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fherbine <fherbine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/29 16:55:25 by fherbine          #+#    #+#             */
/*   Updated: 2020/12/29 16:55:25 by fherbine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_traceroute.h"


void				dnslookup(char *host, t_sockaddr *addr, uint8_t ipver, t_traceroute *tracert)
{
	t_addrinfo		hints;
	t_addrinfo		*first_info;

	ft_bzero(&hints, sizeof(t_addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags |= AI_CANONNAME;
	if (getaddrinfo(host, NULL, &hints, &first_info))
	{
		message_description(host, "Name or service not known", 0, FALSE);
		printf("Cannot handle \"host\" cmdLine arg `%s' on position 1 (argc %d)\n", host, tracert->nos_index);
		exit(EXIT_FAILURE);
	}
	
	if (!first_info)
		exit(EXIT_FAILURE);

	t_addrinfo *p = NULL;

	p = first_info;
	while (p)
	{
		if (p->ai_family == ipver)
			ft_memcpy(addr, ((t_sockaddr *)p->ai_addr), sizeof(t_sockaddr_in));
		else if (p->ai_family == ipver)
			ft_memcpy(addr, ((t_sockaddr *)p->ai_addr), sizeof(t_sockaddr_in6));

		p = p->ai_next;
	}
	freeaddrinfo(first_info);
}

void			reversedns4(t_sockaddr_in *addr, char **host)
{
	*host = (char *)malloc(sizeof(char) * NI_MAXHOST);
	ft_bzero(*host, NI_MAXHOST);
	if (getnameinfo((t_sockaddr *)addr, sizeof(t_sockaddr), *host, NI_MAXHOST, NULL, 0, 0))
		ft_strcpy(*host, inet_ntoa(addr->sin_addr));
}