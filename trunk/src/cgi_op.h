#pragma once

#include <iostream>
#include <stdlib.h>
#include <string>
#include <fcgi_stdio.h>

#include <vector>

#include "seed.h"

enum RequestType {R_SEEDS, R_SITES, R_ROUTES, R_STRATEGY, R_POST, R_INVALID = 1024};

int get(float &x, float &y, RequestType &type, std::string &str)
{
	char detail[256];
	int ret_val = 0;
	int type_id = 0;
	str = getenv("QUERY_STRING");
	if (str != "") {
		ret_val = sscanf(str.c_str(), "x=%f&y=%f&type=%d", &x, &y, &type_id);
		type = (RequestType)type_id;
	}
	else {
		type = (RequestType)R_INVALID;
		x = y = 0;
		ret_val = 0;
	}
	return ret_val;
}

size_t seed_to_json(const std::vector<Seed> &seed_vec, char *json_str)
{
	size_t cur = 0;
	json_str[cur++] = '[';
	for (size_t i = 0; i< seed_vec.size(); ++i) {
		cur += sprintf(json_str + cur, "{\"Detail\":\"%s\",\"Image\":\"%s\",\"Place\":\"%s\",\"Title\":\"%s\",\"UserName\":\"%s\",\"ViewImage\":\"%s\"},",
				seed_vec[i]._detail.c_str(),
				seed_vec[i]._image_url.c_str(),
				seed_vec[i]._place.c_str(),
				seed_vec[i]._title.c_str(),
				seed_vec[i]._user_name.c_str(),
				seed_vec[i]._view_image.c_str());
	}
	if (seed_vec.size() != 0)
		--cur;
	json_str[cur++] = ']';
	json_str[cur++] = '\0';
	return cur;
}

size_t site_to_json(const std::vector<Pri_Site> &site_vec, char *json_str)
{
	size_t cur = 0;
	json_str[cur++] = '[';
	for (size_t i = 0; i< site_vec.size(); ++i) {
		cur += sprintf(json_str + cur, "{\"Name\":\"%s\",\"Detail\":\"%s\",\"Image\":\"%s\",\"Score\":\"%d\",\"Park_id\":\"%d\",\"Site_id\":\"%d\",\"X\":\"%f\",\"Y\":\"%f\"},",
				site_vec[i]._name.c_str(),
				site_vec[i]._detail.c_str(),
				site_vec[i]._image_url.c_str(),
				site_vec[i]._score,
				site_vec[i]._park_id,
				site_vec[i]._site_id,
				site_vec[i]._x,
				site_vec[i]._y);
	}
	if (site_vec.size() != 0)
		--cur;
	json_str[cur++] = ']';
	json_str[cur++] = '\0';
	return cur;
}

size_t stratety_to_json(const std::vector<Recommand> &sty_vec, char *json_str)
{
	size_t cur = 0;
	json_str[cur++] = '[';
	for (size_t i = 0; i < sty_vec.size(); ++i) {
		cur += sprintf(json_str + cur, "{\"Title\":\"%s\",\"Detail\":\"%s\"},",
				sty_vec[i]._title.c_str(),
				sty_vec[i]._detail.c_str());
	}
	if (sty_vec.size() != 0)
			--cur;
	json_str[cur++] = ']';
	json_str[cur++] = 0;
	return cur;
}
