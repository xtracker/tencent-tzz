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
    Seed get_seed(int seed_id);
    std::vector<Seed> get_seed_lists(const std::vector<int> &seed_ids);
    bool insert_seed(const Seed &seed);

    /*
     * operator of mysql conn
     */
    bool init_mysql();
    bool close_mysql();
private:
    MYSQL mysql;
};
