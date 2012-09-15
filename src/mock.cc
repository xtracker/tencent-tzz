#include "logic.h"
#include "seed.h"

#include <vector>
#include <string>

int main() {
    SeedGetter seed_getter;
    seed_getter.init_seed_index();
    std::vector<Seed> ret;
   seed_getter.search_nearby_seed(13.32f, 34.34f, &ret);
//    seed_getter.add_neww_seed("hello", "wooorld", 2, 111.11, 222.22);
//    ret.clear();
//    seed_getter.get_seed_of_user(1,&ret);
}
