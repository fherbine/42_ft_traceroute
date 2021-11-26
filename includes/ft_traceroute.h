#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

/* libft */
# include "../libft/libft.h"
# include <errno.h>

/* printf funcs */
# include <stdio.h>

/* addrinfo, sendto */
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>

/* exit */
# include <stdlib.h>

/* inet_{ntoa, pton}, sendto, htons */
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

/* iphdr */
# include <netinet/ip.h>

/* icmphdr struct */
# include <netinet/ip_icmp.h>

/* timeval struct */
# include <sys/time.h>

/* defines & macros */

/** debug **/
# define __NOP__(x) (void)x;

/** general **/
# define MAXHOPS 30
# define PACKETSIZE 60
# define UDP_PORT 33434
# define UDP_HEADER_SIZE 8
# define PROBES_TIMEOUT 5

/** options defines **/
# define TRACERT_OPT_HELP 0x01
# define TRACERT_OPT_ICMP 0x02

/* typedefs */
typedef struct addrinfo t_addrinfo;
typedef struct sockaddr t_sockaddr;
typedef struct sockaddr_in t_sockaddr_in;
typedef struct sockaddr_in6 t_sockaddr_in6;
typedef struct msghdr t_msghdr;
typedef	double t_time;

/** structs **/
typedef struct		s_traceroute
{
	char			*name_or_service;
	t_sockaddr_in	ipv4;
	uint8_t			options;
	int				snd_socket;
	int				rcv_socket;
}					t_traceroute;


/* prototypes */

/** error.c **/
void		proper_exit(int status, t_traceroute *tracert);
void		display_help(int status);
void		message_description_exit(char *identifier, char *msg, int status);
void		invalid_option(char option_char);
void		message_exit(char *msg, int status, uint8_t with_help);

/* nslookup.c */
void		dnslookup(char *host, t_sockaddr *addr, uint8_t ipver);
void		reversedns4(t_sockaddr_in *addr, char **host);

/* parser.c */
void		parse(int argc, char **argv, t_traceroute *tracert);

/* main.c */
void 		free_tracert(t_traceroute *tracert);

/* traceroute.c */
void		ft_traceroute(t_traceroute *tracert);

/* utils.c */
uint16_t	compile_checksum(void *addr, ssize_t count);
t_time 		getnow(void);

/* debug.c */
void		print_buffer(void *buffer_addr, ssize_t len);

#endif