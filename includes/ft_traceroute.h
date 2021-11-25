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

/* defines & macros */
# define __NOP__(x) (void)x;

/* typedefs */
typedef struct addrinfo t_addrinfo;
typedef struct sockaddr t_sockaddr;
typedef struct sockaddr_in t_sockaddr_in;
typedef struct sockaddr_in6 t_sockaddr_in6;
typedef struct msghdr t_msghdr;

typedef struct	s_nameinfo
{
	char		server[NI_MAXSERV];
	char		host[NI_MAXHOST];
}				t_nameinfo;


/* prototypes */

/** error.c **/
void		proper_exit(int status);
void		display_help(int status);
void		message_description_exit(char *identifier, char *msg, int status);
void		invalid_option(char option_char);
void		message_exit(char *msg, int status, uint8_t with_help);

/* nslookup.c */
void				dnslookup(char *host, t_sockaddr *addr, uint8_t ipver);
void				reversedns4(t_sockaddr_in *addr, t_nameinfo *info);

#endif