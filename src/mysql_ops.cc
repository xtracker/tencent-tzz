#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mysql_ops.h"
#include "seed.h"


SqlConn::SqlConn() {
    printf("....init sqlConn\n");
    init_mysql();
    printf("....init sqlConn ok\n");
}

bool SqlConn::init_mysql() {
    mysql_init(&mysql);    
    if (mysql_real_connect(&mysql, "localhost", "root", "", "tencent",
                    0, NULL, 0) == NULL) {
    }
}

bool SqlConn::close_mysql() {
    mysql_close(&mysql);
}

std::vector<SeedIndex> SqlConn::get_seed_index() {
    std::vector<SeedIndex> ret;
    std::string sql_str = "select * from seed";
    mysql_query(&mysql, sql_str.c_str());
    MYSQL_RES *result = mysql_store_result(&mysql);
    if (result == NULL)
        return ret;    
    MYSQL_ROW row;
    row = mysql_fetch_row(result);
    while (row != NULL) {
        SeedIndex tmp;
        tmp._seed_id = atoi(row[0]);
        tmp._x = atof(row[4]);
        tmp._y = atof(row[5]);
        ret.push_back(tmp);
        row = mysql_fetch_row(result);
    }
}

bool SqlConn::get_seed(const int seed_id, Seed *ret) {
//    std::string sql_str = std::string("select * from seed where id = \"") +
//        seed_id + std::string("\"");
    char sql_str[100];
    sprintf(sql_str, "select * from seed where id = \"%d\"", seed_id);
    mysql_query(&mysql, sql_str);
    MYSQL_RES *result = mysql_store_result(&mysql);
    if (result == NULL)
        return false;    
    int row_count = mysql_num_rows(result);
    if (row_count != 1) {
        return false;
    }
    MYSQL_ROW row;
    row = mysql_fetch_row(result);
    
    ret->_x = atof(row[4]);
    ret->_y = atof(row[5]);
    ret->_title = std::string(row[1]);
    ret->_detail = std::string(row[2]);

    feed_seed_by_user(atoi(row[3]), ret);
    return true;
}

bool SqlConn::get_seed_list(const std::vector<int> &seed_ids,
                            std::vector<Seed> *ret) {
    for (int i = 0; i < seed_ids.size(); i++) {
        Seed tmp;
        get_seed(seed_ids[i], &tmp);
        ret->push_back(tmp);
    }
    return true;
}


bool SqlConn::feed_seed_by_user(const int user_id, Seed *ret) {
//    std::string sql_str = std::string("select * from seed where id = \"") + 
//        user_id + std::string("\"");
    char sql_str[100];
    sprintf(sql_str, "select * from users where id = \"%d\"", user_id);
    mysql_query(&mysql, sql_str);
    MYSQL_RES *result = mysql_store_result(&mysql);
    if (result == NULL)
        return false;    
    int row_count = mysql_num_rows(result);
    if (row_count != 1) {
        return false;
    }
    MYSQL_ROW row;
    row = mysql_fetch_row(result);
    
    ret->_user_name = std::string(row[1]);
    ret->_image_url = std::string(row[2]);
    return true;
}
