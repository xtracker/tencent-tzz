#include "logic.h"

int main() {
    SeedGetter seed_getter;
    seed_getter.init_seed_index();
    seed_getter.search_nearby_seed(12.32f, 109.34f);
}
