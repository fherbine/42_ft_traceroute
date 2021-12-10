# include "../includes/json.h"

static void			free_json_list(t_json_list *lst);
static void 		free_json_dict(t_json_dict *dct);
static t_json_list	*parse_list(char *str);
static t_json_dict	*parse_dict(char *str);

static t_json_list *json_list_add_elem(t_json_list *lst, void *elem, uint8_t type)
{
	t_json_list *newelem = NULL;

	if (!(newelem = (t_json_list *)malloc(sizeof(t_json_list))))
		exit(EXIT_FAILURE);

	newelem->elem = elem;
	newelem->type = type;
	newelem->next = lst;

	return (newelem);
}


static t_json_dict *json_dict_add_elem(t_json_dict *dct, char *key, void *parameter, uint8_t param_type)
{
	t_json_dict *newelem = NULL;

	if (!(newelem = (t_json_dict *)malloc(sizeof(t_json_dict))))
		exit(EXIT_FAILURE);

	newelem->key = ft_strdup(key);
	newelem->parameter = parameter;
	newelem->param_type = param_type;
	newelem->next = dct;

	return (newelem);
}

static void free_json_list(t_json_list *lst)
{
	t_json_list *p = lst, *_tmp = lst;

	while (p) {
		switch (p->type)
		{
		case JSON_TYPE_STR:
			free(p->elem);
			break;

		case JSON_TYPE_LIST:
			free_json_list((t_json_list *)p->elem);
			break;

		case JSON_TYPE_DICT:
			free_json_dict((t_json_dict *)p->elem);
			break;
		
		default:
			break;
		}
		_tmp = p;
		p = p->next;
		free(_tmp);
	}
}

static void free_json_dict(t_json_dict *dct)
{
	t_json_dict *p = dct, *_tmp = dct;

	while (p) {
		switch (p->param_type)
		{
		case JSON_TYPE_STR:
			free(p->parameter);
			break;

		case JSON_TYPE_LIST:
			free_json_list((t_json_list *)p->parameter);
			break;

		case JSON_TYPE_DICT:
			free_json_dict((t_json_dict *)p->parameter);
			break;
		
		default:
			break;
		}
		free(p->key);
		_tmp = p;
		p = p->next;
		free(_tmp);
	}
}

static uint8_t rematch(char *str, const char *regstr)
{
	regex_t regex;
	int reti = regcomp(&regex, regstr, REG_EXTENDED);

	if (reti) {
		regfree(&regex);
		return (0);
	}
	
	reti = regexec(&regex, str, 0, NULL, 0);

	regfree(&regex);

	if (!reti)
		return (1);
	return (0);
}

static uint8_t get_json_root_type(char *str)
{
	if (rematch(str, JSON_ROOT_LIST))
		return (JSON_TYPE_LIST);
	else if (rematch(str, JSON_ROOT_DICT))
		return (JSON_TYPE_DICT);
	return (JSON_TYPE_ERROR);
}

static t_json_list *parse_list(char *str)
{
	t_json_list *out = NULL;
	uint32_t nested_level = 0;
	/* ignoring '[' / ']' */
	char *_str = str;
	_str[0] = ' ';
	_str[ft_strlen(_str)-1] = 0;
	_str = ft_strtrim(_str);

	char **splitted = ft_strsplit(_str, ',');
	free(_str);
	char *stack = NULL;

	for (uint64_t i = 0; splitted[i]; i++) {
		if (!nested_level) {
			if (stack) {
				switch (get_json_root_type(stack))
				{
				case JSON_TYPE_LIST:
					out = json_list_add_elem(out, (void *)parse_list(stack), JSON_TYPE_LIST);
					break;
				
				case JSON_TYPE_DICT:
					out = json_list_add_elem(out, (void *)parse_dict(stack), JSON_TYPE_DICT);
					break;
				
				default:
					return (NULL);
					break;
				}
				free(stack);
				stack = NULL;
			}

			if (rematch(splitted[i], JSON_NULL_REG))
				out = json_list_add_elem(out, NULL, JSON_TYPE_NULL);
			else if (rematch(splitted[i], JSON_BOOL_REG)) {
				uint8_t _bool = (ft_strcmp(splitted[i], "false") ? TRUE : FALSE);
				out = json_list_add_elem(out, &_bool, JSON_TYPE_BOOL);
			}
			else if (rematch(splitted[i], JSON_INT_REG)) {
				int64_t _int = ft_atoi(splitted[i]);
				out = json_list_add_elem(out, &_int, JSON_TYPE_INT);
			}
			else if (rematch(splitted[i], JSON_FLOAT_REG)) {
				double _f = atof(splitted[i]);
				out = json_list_add_elem(out, &_f, JSON_TYPE_FLOAT);
			}
			else if (rematch(splitted[i], JSON_STR_REG)) {
				char *elemstr = splitted[i];
				elemstr[0] = ' ';
				elemstr[ft_strlen(elemstr)-1] = 0;
				elemstr = ft_strtrim(elemstr);
				out = json_list_add_elem(out, &elemstr, JSON_TYPE_STR);
			}
			else
				return (NULL);
		} else {
			if (!stack)
				stack = ft_strdup("");
			stack = ft_strjoin(stack, splitted[i]);
		}

		if (ft_strchr(splitted[i], '{') || ft_strchr(splitted[i], '['))
			nested_level++;
		if (ft_strchr(splitted[i], '}') || ft_strchr(splitted[i], ']'))
			nested_level--;
	}

	ft_free_tab(splitted);
	return (out);
}

static t_json_dict *parse_dict(char *str)
{
	t_json_dict *out = NULL;
	uint32_t nested_level = 0;
	/* ignoring '[' / ']' */
	char *_str = str;
	_str[0] = ' ';
	_str[ft_strlen(_str)-1] = 0;
	_str = ft_strtrim(_str);

	char **splitted = ft_strsplit(_str, ','), **keyparam = NULL;
	char *stack = NULL, *strstack = NULL, *_p = NULL;

	for (uint64_t i = 0; splitted[i]; i++) {
		_p = ft_strtrim(splitted[i]);
		free(splitted[i]);
		splitted[i] = _p;

		keyparam = ft_strsplit(splitted[i], ':');

		for (int _i = 0; keyparam[_i]; _i++) {
			_p = ft_strtrim(keyparam[_i]);
			free(keyparam[_i]);
			keyparam[_i] = _p;
		}

		if (ft_tab_len(keyparam) >= 2) {
			char *_tab[3] = {keyparam[0], keyparam[1], NULL};
			char **keyp2 = ft_copy_tab(_tab);
			int ki = 2;
			while (keyparam[ki]) {
				keyp2[1] = ft_strjoin(keyp2[1], keyparam[ki]);
				ki++;
			}

			ft_free_tab(keyparam);
			keyparam = keyp2;
		}

		char *key = keyparam[0], *param = keyparam[1];

		char *_key = key;
		_key[0] = ' ';
		_key[ft_strlen(_key)-1] = 0;
		_key = ft_strtrim(_key);

		free(key);
		key = _key;
		keyparam[0] = key;


		if (!nested_level) {
			if (stack) {
				switch (get_json_root_type(stack))
				{
				case JSON_TYPE_LIST:
					out = json_dict_add_elem(out, key, (void *)parse_list(stack), JSON_TYPE_LIST);
					break;
				
				case JSON_TYPE_DICT:
					out = json_dict_add_elem(out, key, (void *)parse_dict(stack), JSON_TYPE_DICT);
					break;
				
				default:
					return (NULL);
					break;
				}
				free(stack);
				stack = NULL;
			}

			if (rematch(param, JSON_NULL_REG))
				out = json_dict_add_elem(out, key, NULL, JSON_TYPE_NULL);
			else if (rematch(param, JSON_BOOL_REG)) {
				uint8_t _bool = (!ft_strcmp(param, "false") ? FALSE : TRUE);
				out = json_dict_add_elem(out, key, &_bool, JSON_TYPE_BOOL);
			}
			else if (rematch(param, JSON_INT_REG)) {
				int64_t _int = ft_atoi(param);
				out = json_dict_add_elem(out, key, &_int, JSON_TYPE_INT);
			}
			else if (rematch(param, JSON_FLOAT_REG)) {
				double _f = atof(param);
				out = json_dict_add_elem(out, key, &_f, JSON_TYPE_FLOAT);
			}
			else if (rematch(param, JSON_STR_REG)) {
				char *elemstr = param;
				elemstr[0] = ' ';
				elemstr[ft_strlen(elemstr)-1] = 0;
				elemstr = ft_strtrim(elemstr);
				out = json_dict_add_elem(out, key, (void *)elemstr, JSON_TYPE_STR);
			}
			else {
				if (param[0] == '"') {
					strstack = ft_strdup(param);

					while (TRUE) 
					{
						i++;
						strstack = ft_strjoin(strstack, ",");
						strstack = ft_strjoin(strstack, splitted[i]);

						if (splitted[i][ft_strlen(splitted[i])-1] == '"') {
							char *elemstr = strstack;
							elemstr[0] = ' ';
							elemstr[ft_strlen(elemstr)-1] = 0;
							elemstr = ft_strtrim(elemstr);
							out = json_dict_add_elem(out, key, (void *)elemstr, JSON_TYPE_STR);
							free(strstack);
							strstack = NULL;
						}
						if (splitted[i][ft_strlen(splitted[i])-1] == '"')
							break ;
					}
				} else
					return (NULL);
			}
		} else {
			if (!stack)
				stack = ft_strdup("");
			stack = ft_strjoin(stack, splitted[i]);
		}

		if (ft_strchr(splitted[i], '{') || ft_strchr(splitted[i], '['))
			nested_level++;
		if (ft_strchr(splitted[i], '}') || ft_strchr(splitted[i], ']'))
			nested_level--;
		
		ft_free_tab(keyparam);
	}

	ft_free_tab(splitted);
	free(_str);
	return (out);
}

void free_json(t_json *json)
{
	switch (json->type)
	{
	case JSON_TYPE_LIST:
		free_json_list((t_json_list *)json->data);
		break;

	case JSON_TYPE_DICT:
		free_json_dict((t_json_dict *)json->data);
		break;
	
	default:
		break;
	}
}

t_json_dict *get_jdict_elem(t_json_dict *dct, char *key)
{
	t_json_dict *p = dct;

	while (p)
	{
		if (!ft_strcmp(p->key, key))
			break ;
		p = p->next;
	}
	return (p);
}

uint8_t parse_json_from_str(char *str, t_json *json)
{
	json->type = get_json_root_type(str);

	if (json->type == JSON_TYPE_LIST)
		json->data = (void *)parse_list(str);
	else if (json->type == JSON_TYPE_DICT)
		json->data = (void *)parse_dict(str);
	else {
		return (0);
	}
	
	if (json->data)
		return (1);
	return (0);
}

uint8_t parse_json_from_file(char *filename, t_json *json)
{
	FILE *fp;
	char read_buffer[JSON_DEFAULT_BUFFER];
	char *jsonstr = ft_strdup("");

	fp = fopen(filename, "r");
	while (fgets(read_buffer, JSON_DEFAULT_BUFFER, fp))
		jsonstr = ft_strjoin(jsonstr, read_buffer);
	
	uint8_t retval = parse_json_from_str(jsonstr, json);

	free(jsonstr);

	fclose (fp);
	return (retval);
}