#pragma once

#include <vector>
#include <string>

class Seed;
class SeedIndex;
class SqlConn;
class Pri_Site;
class Recommand;

class SeedGetter {
public:
    SeedGetter();
    ~SeedGetter();

    bool init_seed_index();
    bool search_nearby_seed(float x, float y, std::vector<Seed> *ret);
    bool add_neww_seed(const std::string &title,
            const std::string &detail, 
            const int user_id, 
            const float x, const float y,
            const std::string place, const std::string view);
    bool get_all_seed(std::vector<Seed> *);
    bool get_seed_of_user(const int, std::vector<Seed> *);

    // get router
    bool get_site_router(const float x, const float y,
            std::vector<Pri_Site> *ret);
    bool get_all_site(std::vector<Pri_Site> *ret);
    bool search_nearby_site(int, std::vector<Pri_Site> *ret);
    // get site
    bool search_nearby_site(float, float, std::vector<Pri_Site> *ret);

    bool get_recommand_info(float, float, std::vector<Recommand> *ret);

private:
    bool get_nearby_seed_ids(float x, float y,
            std::vector<int> *ids);
    //bool get_nearby_site(std::vector<

private:
    std::vector<SeedIndex> _index;
    std::vector<Pri_Site> _site_ret;
    SqlConn *_sql_conn;
};
