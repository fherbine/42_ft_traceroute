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


void				dnslookup(char *host, t_sockaddr *addr, uint8_t ipver)
{
	t_addrinfo		hints;
	t_addrinfo		*first_info;

	ft_bzero(&hints, sizeof(t_addrinfo));
	hints.ai_family = AF_UNSPEC;
	hints.ai_socktype = SOCK_STREAM;
	hints.ai_flags |= AI_CANONNAME;
	if (getaddrinfo(host, NULL, &hints, &first_info))
		message_description_exit(host, \
								 "Name or service not known", EXIT_FAILURE);
	
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

void			reversedns4(t_sockaddr_in *addr, t_nameinfo *info)
{
	ft_bzero(info, sizeof(t_nameinfo));
	ft_bzero(info->host, NI_MAXHOST);
	if (getnameinfo((t_sockaddr *)addr, sizeof(t_sockaddr), info->host, NI_MAXHOST, NULL, 0, 0))
		exit(EXIT_FAILURE);
}