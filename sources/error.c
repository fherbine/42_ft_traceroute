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
	printf("\nUsage\n");
	printf("  ./ft_traceroute [options] <destination>\n");
	printf("\nOptions:\n");
	display_option("", "  dns name or ip address", "<destination>");
	display_option("-I", "use ICMP probes instead of UDP (by default).", "");
	display_option("-h", "print help and exit", "");
	exit(status);
}

void	message_description_exit(char *identifier, char *msg, int status)
{
	dprintf((!status) ? STDOUT : STDERR, "ft_traceroute: %s: %s\n", identifier, msg);
	exit(status);
}

void	invalid_option(char option_char)
{
	dprintf(STDERR, "ft_traceroute: invalid option -- '%c'\n", option_char);
	display_help(EXIT_FAILURE);
}

void	message_exit(char *msg, int status, uint8_t with_help)
{
	dprintf((!status) ? STDOUT : STDERR, "ft_traceroute %s\n", msg);
	if (with_help)
		display_help(status);
	exit(status);
}
