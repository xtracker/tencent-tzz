#pragma once

#include <vector>
#include <string>

class Seed;
class SeedIndex;
class SqlConn;

class SeedGetter {
public:
    SeedGetter();
    ~SeedGetter();

    bool init_seed_index();
    bool search_nearby_seed(float x, float y, std::vector<Seed> *ret);
    bool add_neww_seed(const std::string &title,
            const std::string &detail, 
            const int user_id, 
            const float x, const float y);
    bool get_all_seed(std::vector<Seed> *);

private:
    bool get_nearby_seed_ids(float x, float y,
            std::vector<int> *ids);

private:
    std::vector<SeedIndex> _index;
    SqlConn *_sql_conn;
};
