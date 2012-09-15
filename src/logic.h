#pragma once

#include <vector>
#include <string>

class Seed;
class SeedIndex;
class SqlConn;
class Site;

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
    bool get_seed_of_user(const int, std::vector<Seed> *);

    bool get_site_router(const float x, const float y);
    bool get_all_site(std::vector<Site> *ret);

private:
    bool get_nearby_seed_ids(float x, float y,
            std::vector<int> *ids);

private:
    std::vector<SeedIndex> _index;
    std::vector<Site> _site_ret;
    SqlConn *_sql_conn;
};
