#include <iostream>
#include <vector>
#include <cstdio>
#include <cstdlib>
#include <fcgi_stdio.h>
#include <unistd.h>

#include "seed.h"
#include "logic.h"
#include "cgi_op.h"
using namespace std;

char buf[4096] = {"[{\"Detail\":\"daoci\",\"Image\":\"http://12.jpg\",\"Place\":\"tencent\",\"Title\":\"wo\",\"UserName\":\"tommy\",\"ViewImage\":\"http://122.jpg\"},]"};

int print_html(const char *p)
{
	return printf("Content-type: text/html\r\n\r\n%s", p);
}
int main()
{
	float x, y;
	bool ok = false;
	RequestType id;
	SeedGetter seed_getter;
    std::vector<Seed> ret;
	while (FCGI_Accept() >= 0) {
		seed_getter.init_seed_index();
		ret.clear();
		if (get(x, y, id)) {
			seed_getter.search_nearby_seed(x, y ,&ret);
			if (ret.size() != 0) {
				seed_to_json(ret, buf);
			}
				print_html(buf);
		} else {
				printf("Content-type: text/html\r\n"
                   "\r\n"
                   "<title>FastCGI Hello!</title>"
                   "<h1>FastCGI Hello!</h1>"
                   "Request number %d running on host <i>%s</i>\n",
                    1, getenv("QUERY_STRING"));
			}
	}

    /* End of response loop */
    return 0;
}

