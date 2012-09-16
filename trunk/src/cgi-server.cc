#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <fcgi_stdio.h>
#include <unistd.h>

#include <json/json.h>
#include "seed.h"
#include "logic.h"
#include "cgi_op.h"
using namespace std;

char buf[4096]; // = {"[{\"Detail\":\"daoci\",\"Image\":\"http://12.jpg\",\"Place\":\"tencent\", \
	\"Title\":\"wo\",\"UserName\":\"tommy\",\"ViewImage\":\"http://122.jpg\"},]"};

int print_html(const char *p)
{
	return printf("Content-type: text/html\r\n\r\n%s", p);
}

int trim_utf8(char *s)
{
	int tmp;
	char *p = s, *q = s;
	while (*p) {
	
		if (*p != '%')
			*q++ = *p++;
		else {
			++p;
			sscanf(p, "%2x", &tmp);
			*q++ = (char)tmp;
			p += 2;
		}
	}
	*q++ = 0;
	return (int)(q - s);
}
int main()
{
	float x, y;
	bool ok = false;
	int p_len = 0;
	RequestType type;
	std::string params;
	SeedGetter seed_getter;
    std::vector<Seed> ret_seed;
	std::vector<Pri_Site> ret_site;
	std::vector<Recommand> ret_rec;
	while (FCGI_Accept() >= 0) {
		ret_seed.clear();
		ret_site.clear();
		if (p_len = get(x, y, type, params)) {
			if (type == R_SEEDS) {
				seed_getter.init_seed_index();
				seed_getter.search_nearby_seed(x, y ,&ret_seed);
				seed_to_json(ret_seed, buf);
			}
			if (type == R_SITES) {
				seed_getter.get_all_site(NULL);
				seed_getter.search_nearby_site(x, y, &ret_site);
				site_to_json(ret_site, buf);
			}
			if (type == R_ROUTES) {
			
				seed_getter.get_all_site(NULL);
				seed_getter.get_site_router(x, y, &ret_site);
				site_to_json(ret_site, buf);
			}
			if (type == R_POST) {

				trim_utf8(const_cast<char *>(params.c_str()));

				params = params.substr(params.rfind("=") + 1, params.length());
			
				seed_getter.add_neww_seed("彩蛋", params.c_str(), 1, x, y,"","");
			}
			if (type == R_STRATEGY) {
			
				seed_getter.get_recommand_info(x, y, &ret_rec);
				stratety_to_json(ret_rec, buf);
				//if (ret)
			}
			print_html(buf);
		} else {

			fread(buf, atoi(getenv("CONTENT_LENGTH")), 1, stdin);
			print_html(buf);
		
		}

	}

    /* End of response loop */
    return 0;
}

