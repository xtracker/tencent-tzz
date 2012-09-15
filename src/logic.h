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

private:
    std::vector<SeedIndex> _index;
    SqlConn *_sql_conn;
};
