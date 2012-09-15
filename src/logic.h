#pragma once

#include <vector>

class Seed;
class SeedIndex;
class SqlConn;

class SeedGetter {
public:
    SeedGetter();
    ~SeedGetter();

    bool init_seed_index();
    bool search_nearby_seed(float x, float y);

private:
    bool get_nearby_seed_ids(float x, float y,
            std::vector<int> *ids);

private:
    std::vector<SeedIndex> _index;
    SqlConn *_sql_conn;
};
