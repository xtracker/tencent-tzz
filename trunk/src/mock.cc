#include "logic.h"
#include "seed.h"

#include <cstdio>
#include <vector>
#include <string>

#include <iostream>


int print_seed(Seed sd)
{
	return std::printf("%s\n", sd._title.c_str());
}
int main() {
    SeedGetter seed_getter;
    seed_getter.init_seed_index();
	std::vector<Seed> ret;
	std::vector<Pri_Site> ret_site;
	std::vector<Recommand> ret_rec;
//	seed_getter.search_nearby_seed(13.32f, 34.34f, &ret);
//    seed_getter.add_neww_seed("hello", "wooorld", 2, 111.11, 222.22);
//    ret.clear();
//    seed_getter.get_seed_of_user(1,&ret);
//
//	for (size_t i = 0; i < ret.size(); ++i)
//		print_seed(ret[i]);
////	seed_getter.init_seed_index();
	seed_getter.get_all_site(NULL);
	seed_getter.get_recommand_info(22.31f, 113.232f, &ret_rec);
	//seed_getter.get_site_router(22.5405f, 113.935f, &ret_site);
	//seed_getter.search_nearby_site(22.0f, 113.0f, &ret_site);
	std::cout << "site size " << ret_rec.size() << std::endl;
	return 0;
}
