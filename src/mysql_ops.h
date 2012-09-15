/**
 *
 */

#pragma once

#include <vector>
#include <mysql/mysql.h>

class Seed;
class SeedIndex;
//struct MYSQL;

class SqlConn {
public:
    SqlConn();
    ~SqlConn() {
        close_mysql();
    }

    std::vector<SeedIndex> get_seed_index();
    bool get_seed(const int seed_id, Seed *ret);
    bool get_seed_list(const std::vector<int> &seed_ids,
                        std::vector<Seed> *ret);
    bool insert_seed(const Seed &seed);
    bool feed_seed_by_user(const int user_id, Seed *ret);

    /*
     * operator of mysql conn
     */
    bool init_mysql();
    bool close_mysql();
private:
    MYSQL mysql;
};
