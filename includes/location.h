#ifndef LOCATION_H
# define LOCATION_H

# include "../libft/libft.h"

# include <stdio.h>
# include <curl/curl.h>
# include <string.h>
# include <stdlib.h>
# include <sys/types.h>
# include <sys/socket.h>
# include <netdb.h>

# define GET_BUFFER_SIZE 4096

struct url_data {
    size_t size;
    char* data;
};

typedef struct 	s_ip_location {
	char		*city;
	char		*region;
	char		*country;
}				t_ip_location;

typedef struct sockaddr_in t_sockaddr_in;

char *get_ip_location_str(t_sockaddr_in *ip);

#endif