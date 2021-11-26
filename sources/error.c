/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   error.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fherbine <fherbine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 19:50:18 by fherbine          #+#    #+#             */
/*   Updated: 2020/12/30 19:50:18 by fherbine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_traceroute.h"

static void		display_option(char *letter, char *description, char *placeholder)
{
	printf("  %s %-16s%s\n", letter, placeholder, description);
}

void		display_help(int status)
{
	printf("Usage\n");
	printf("  ./ft_traceroute [ -I ] <destination>\n");
	printf("\nOptions:\n");
	display_option("", "  dns name or ip address", "<destination>");
	display_option("-I", "Use ICMP ECHO for tracerouting", "");
	display_option("-h", "print help and exit", "");
	exit(status);
}

void	message_description_exit(char *identifier, char *msg, int status)
{
	message_description(identifier, msg, status, TRUE);
}

void	message_description(char *identifier, char *msg, int status, uint8_t with_exit)
{
	dprintf((!status) ? STDOUT : STDERR, "%s: %s\n", identifier, msg);
	if (with_exit)
		exit(status);
}

void	invalid_option(char option_char, int argc)
{
	dprintf(STDERR, "Bad option `-%c' (argc %d)\n", option_char, argc);
	exit(EXIT_FAILURE);
}

void	message_exit(char *msg, int status, uint8_t with_help)
{
	dprintf((!status) ? STDOUT : STDERR, "%s\n", msg);
	if (with_help)
		display_help(status);
	exit(status);
}
