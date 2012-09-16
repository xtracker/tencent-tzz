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
    ret->_user_id = atoi(row[3]);
    ret->_place = std::string(row[6]);
    ret->_view_image = std::string(row[7]);

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

bool SqlConn::insert_seed(const Seed &seed) {
    char sql_srt[100];
    sprintf(sql_srt, "insert into seed values (\"\",\"%s\",\"%s\",\"%d\",\"%f\",\"%f\", \"%s\", \"%s\")",
            seed._title.c_str(), seed._detail.c_str(),
            seed._user_id, seed._x, seed._y,
			seed._place.c_str(), seed._view_image.c_str());
    printf("%s\n", sql_srt);
    mysql_query(&mysql, sql_srt);
}

bool SqlConn::get_seed_by_user(const int user_id,
        std::vector<Seed> *ret) {
    char sql_str[100];
    sprintf(sql_str, "select * from seed where user_id = \"%d\"", user_id); 
    mysql_query(&mysql, sql_str);
    MYSQL_RES *result = mysql_store_result(&mysql);
    if (result == NULL)
        return false;    
    MYSQL_ROW row;
    row = mysql_fetch_row(result);
    while (row != NULL) {
        Seed seed_tmp;
        seed_tmp._title = std::string(row[1]);
        seed_tmp._detail = std::string(row[2]);
        seed_tmp._user_id = atoi(row[3]);
        seed_tmp._x = atof(row[4]);
        seed_tmp._y = atof(row[5]);
        seed_tmp._place = std::string(row[6]);
        seed_tmp._view_image = std::string(row[7]);

        feed_seed_by_user(user_id, &seed_tmp);
        ret->push_back(seed_tmp);
        row = mysql_fetch_row(result);
    }
    return true; 
}

bool SqlConn::seedrowcvt2seed(const MYSQL_ROW &row, Seed *seed) {
    seed->_title = std::string(row[1]);
    seed->_detail = std::string(row[2]);
    seed->_user_id = atoi(row[3]);
    seed->_x = atof(row[4]);
    seed->_y = atof(row[5]);
    //seed->_place = std::string(row[6]);
    //feed_seed_by_user(
}

bool SqlConn::get_all_site(std::vector<Pri_Site> *ret) {
    std::string sql_str = "select * from site";
    mysql_query(&mysql, sql_str.c_str());
    MYSQL_RES *result = mysql_store_result(&mysql);
    if (result == NULL)
        return ret;    
    MYSQL_ROW row;
    row = mysql_fetch_row(result);
    while (row != NULL) {
        Pri_Site site_tmp;
        site_tmp._x = atof(row[2]);
        site_tmp._y = atof(row[3]);
        site_tmp._name = std::string(row[1]);
        site_tmp._detail = std::string(row[4]);
        site_tmp._image_url = std::string(row[5]);
        site_tmp._park_id = atoi(row[6]);
        site_tmp._score = atoi(row[7]);
		site_tmp._site_id = atoi(row[0]);
        
        ret->push_back(site_tmp);
        row = mysql_fetch_row(result);
    }
    return true;
}

bool SqlConn::get_recommad(const int site_id, std::vector<Recommand> *ret) {
    char sql_str[100];
    sprintf(sql_str, "select * from strategy where site_id = \"%d\"", site_id); 
    mysql_query(&mysql, sql_str);
    MYSQL_RES *result = mysql_store_result(&mysql);
    if (result == NULL)
        return false;    
    MYSQL_ROW row;
    row = mysql_fetch_row(result);
    while (row != NULL) {
        Recommand recom;
        recom._title = std::string(row[2]);
        recom._detail = std::string(row[3]);

        ret->push_back(recom);
        row = mysql_fetch_row(result);
    }
    return true; 
}
