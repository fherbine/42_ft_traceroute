#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

/* libft */
# include "../libft/libft.h"

/* printf funcs */
# include <stdio.h>

/* addrinfo */
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>

/* exit */
# include <stdlib.h>

/* inet_{ntoa, pton} */
# include <sys/socket.h>
# include <netinet/in.h>
# include <arpa/inet.h>

/* defines & macros */
# define __NOP__(x) (void)x;

/** options defines **/
# define TRACERT_OPT_HELP 0x01
# define TRACERT_OPT_ICMP 0x02

/* typedefs */
typedef struct addrinfo t_addrinfo;
typedef struct sockaddr t_sockaddr;
typedef struct sockaddr_in t_sockaddr_in;
typedef struct sockaddr_in6 t_sockaddr_in6;
typedef struct msghdr t_msghdr;

/** structs **/
typedef struct		s_nameinfo
{
	char			server[NI_MAXSERV];
	char			host[NI_MAXHOST];
}					t_nameinfo;

typedef struct		s_traceroute
{
	char			*name_or_service;
	t_sockaddr_in	ipv4;
	uint8_t			options;
}					t_traceroute;


/* prototypes */

/** error.c **/
void	proper_exit(int status, t_traceroute *tracert);
void	display_help(int status);
void	message_description_exit(char *identifier, char *msg, int status);
void	invalid_option(char option_char);
void	message_exit(char *msg, int status, uint8_t with_help);

/* nslookup.c */
void	dnslookup(char *host, t_sockaddr *addr, uint8_t ipver);
void	reversedns4(t_sockaddr_in *addr, t_nameinfo *info);

/* parser.c */
void	parse(int argc, char **argv, t_traceroute *tracert);

/* main.c */
void 	free_tracert(t_traceroute *tracert);

#endif