#include "../includes/ft_traceroute.h"

static size_t write_data(void *ptr, size_t size, size_t nmemb, struct url_data *data) {
    size_t index = data->size;
    size_t n = (size * nmemb);
    char* tmp;

    data->size += (size * nmemb);

#ifdef DEBUG
    fprintf(stderr, "data at %p size=%ld nmemb=%ld\n", ptr, size, nmemb);
#endif
    tmp = realloc(data->data, data->size + 1);

    if(tmp) {
        data->data = tmp;
    } else {
        if(data->data) {
            free(data->data);
        }
        fprintf(stderr, "Failed to allocate memory.\n");
        return 0;
    }

    memcpy((data->data + index), ptr, n);
    data->data[data->size] = '\0';

    return size * nmemb;
}

static char *http_get(char *url) {
	/* https://stackoverflow.com/a/13906020/12113451 */
    CURL *curl;

    struct url_data data;
    data.size = 0;
    if (!(data.data = malloc(GET_BUFFER_SIZE)))
		exit(EXIT_FAILURE);
    if(NULL == data.data) {
        fprintf(stderr, "Failed to allocate memory.\n");
        return NULL;
    }

    data.data[0] = '\0';

    CURLcode res;

    curl = curl_easy_init();
    if (curl) {
        curl_easy_setopt(curl, CURLOPT_URL, url);
        curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_data);
        curl_easy_setopt(curl, CURLOPT_WRITEDATA, &data);
        res = curl_easy_perform(curl);
        if(res != CURLE_OK) {
                fprintf(stderr, "curl_easy_perform() failed: %s\n",  
                        curl_easy_strerror(res));
        }

        curl_easy_cleanup(curl);

    }
    return data.data;
}

static void get_ip_location(t_sockaddr_in *ip, t_ip_location *iplocation)
{
	char *url = ft_strdup("http://ipinfo.io/");
	url = ft_strjoin(url, inet_ntoa(ip->sin_addr));
	char *jsonstr = http_get(url);
	t_json json;

	iplocation->city = ft_strdup("");
	iplocation->region = ft_strdup("");
	iplocation->country = ft_strdup("");

	if (parse_json_from_str(jsonstr, &json))
	{
		t_json_dict *city = get_jdict_elem((t_json_dict *)json.data, "city");
		t_json_dict *region = get_jdict_elem((t_json_dict *)json.data, "region");
		t_json_dict *country = get_jdict_elem((t_json_dict *)json.data, "country");

		if (city) {
			free(iplocation->city);
			iplocation->city = ft_strdup((char *)city->parameter);
		}
		if (region) {
			free(iplocation->region);
			iplocation->region = ft_strdup((char *)region->parameter);
		}
		if (country) {
			free(iplocation->country);
			iplocation->country = ft_strdup((char *)country->parameter);
		}
	}

	free(url);
	free(jsonstr);
	free_json(&json);
}

static void free_ip_location(t_ip_location *iploc)
{
	free(iploc->city);
	free(iploc->region);
	free(iploc->country);
}

char *get_ip_location_str(t_sockaddr_in *ip)
{
	t_ip_location iploc;
	char *ret = ft_strdup("");

	get_ip_location(ip, &iploc);

	if (ft_strcmp(iploc.city, ""))
		ret = ft_strjoin(ret, iploc.city);
	if (ft_strcmp(iploc.region, "")) {
		if (ft_strcmp(ret, ""))
			ret = ft_strjoin(ret, ", ");
		ret = ft_strjoin(ret, iploc.region);
	}
	if (ft_strcmp(iploc.country, "")) {
		if (ft_strcmp(ret, ""))
			ret = ft_strjoin(ret, ", ");
		ret = ft_strjoin(ret, iploc.country);
	}
	free_ip_location(&iploc);

	return (ret);
}