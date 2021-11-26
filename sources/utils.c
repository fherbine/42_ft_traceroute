/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   utils.c                                            :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: fherbine <fherbine@student.42.fr>          +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2020/12/30 20:30:26 by fherbine          #+#    #+#             */
/*   Updated: 2020/12/30 20:30:26 by fherbine         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/ft_traceroute.h"

uint16_t compile_checksum(void *addr, ssize_t count)
{
	/* Inspired by RFC1141 */
	uint32_t sum = 0;

	while( count > 1 )  {
		/*  This is the inner loop */
			sum += *(uint16_t *)(addr);
			addr += 2;
			count -= 2;
	}

		/*  Add left-over byte, if any */
	if( count > 0 )
			sum += *(uint8_t *)addr;

		/*  Fold 32-bit sum to 16 bits */
	while (sum>>16)
		sum = (sum & 0xffff) + (sum >> 16);

	return(~sum);
}

static t_time	timeval_to_ts(struct timeval dt)
{
	t_time ts;
	ts = ((double)dt.tv_sec * 1000) + ((double)dt.tv_usec / 1000);
	return (ts);
}

t_time getnow(void)
{
	struct timeval now;
	gettimeofday(&now, NULL);
	return (timeval_to_ts(now));
}