#ifndef JSON_H
# define JSON_H

# include "../libft/libft.h"

# include <regex.h>
# include <stdio.h>

# define JSON_TYPE_ERROR	255
# define JSON_TYPE_NULL		0
# define JSON_TYPE_BOOL		1
# define JSON_TYPE_INT		2
# define JSON_TYPE_FLOAT	3
# define JSON_TYPE_STR		4
# define JSON_TYPE_LIST		5
# define JSON_TYPE_DICT		6

# define JSON_NULL_REG	"^null$"
# define JSON_BOOL_REG	"^(true|false)$"
# define JSON_INT_REG	"^(-)?([0-9]+)$"
# define JSON_FLOAT_REG	"^(-)?([0-9]+)\\.([0-9]+)$"
# define JSON_STR_REG	"^\".*\"$"
# define JSON_LIST_REG	"^\\[.*\\]$"
# define JSON_DICT_REG	"^\\{.*\\}$"

# define JSON_ROOT_LIST JSON_LIST_REG
# define JSON_ROOT_DICT JSON_DICT_REG

# define JSON_DEFAULT_BUFFER 1024

typedef struct			s_json_dict {
	uint8_t				param_type;
	char				*key;
	void				*parameter;
	struct s_json_dict	*next;
}						t_json_dict;

typedef struct			s_json_list {
	uint8_t				type;
	void				*elem;
	struct s_json_list	*next;
}						t_json_list;

typedef struct			s_json {
	uint8_t				type;
	void				*data;
}						t_json;

void		free_json(t_json *json);
uint8_t		parse_json_from_str(char *str, t_json *json);
uint8_t		parse_json_from_file(char *filename, t_json *json);
t_json_dict	*get_jdict_elem(t_json_dict *dct, char *key);

#endif