#pragma once

#include <iostream>
#include <stdlib.h>
#include <string>
#include <fcgi_stdio.h>

#include <vector>

#include "seed.h"

enum RequestType {R_SEED, R_ID};

bool get(float &x, float &y, RequestType &type)
	
{
	char detail[256];
	std::string str = getenv("QUERY_STRING");
	if (str != "") {
		sscanf(str.c_str(), "x=%f&y=%f&%s", &x, &y, &detail);
	}
	else {
		x = y = 0;
		return false;
	}
	return true;
}

size_t seed_to_json(const std::vector<Seed> &seed_vec, char *json_str)
{
	size_t cur = 0;
	json_str[cur++] = '[';
	for (size_t i = 0; i< seed_vec.size(); ++i) {
		cur += sprintf(json_str + cur, "{\"Detail\":\"%s\",\"Images\":%s,\"Place\":\"%s\",\"Title\":\"%s\",\"UserName\":\"%s\",\"ViewImage\":\"%s\"},",
				seed_vec[i]._detail.c_str(),
				seed_vec[i]._image_url.c_str(),
				seed_vec[i]._place.c_str(),
				seed_vec[i]._title.c_str(),
				seed_vec[i]._user_name.c_str(),
				seed_vec[i]._view_image.c_str());
	}
	json_str[cur++] = ']';
	json_str[cur++] = '\0';
	return cur;
}
