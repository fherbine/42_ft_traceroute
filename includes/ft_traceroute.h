#ifndef FT_TRACEROUTE_H
# define FT_TRACEROUTE_H

/* libft */
# include "../libft/libft.h"

/* printf funcs */
# include <stdio.h>

/* exit */
# include <stdlib.h>

/* defines & macros */
# define __NOP__(x) (void)x;

/* prototypes */

/** error.c **/
void		proper_exit(int status);
void		display_help(int status);
void		message_description_exit(char *identifier, char *msg, int status);
void		invalid_option(char option_char);
void		message_exit(char *msg, int status, uint8_t with_help);



#endif