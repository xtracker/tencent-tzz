#include <iostream>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>

#include "mysql_ops.h"
#include "seed.h"


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
    std::string sql_str = "select * from seed";
    mysql_query(&mysql, sql_str.c_str());
    MYSQL_RES *result = mysql_store_result(&mysql);
    std::vector<SeedIndex> ret;
    MYSQL_ROW row = NULL;
    row = mysql_fetch_row(result);
    while (NULL != row) {
        SeedIndex tmp;
        tmp._seed_id = atoi(row[0]);
        tmp._x = atof(row[4]);
        tmp._y = atof(row[5]);
        ret.push_back(tmp);
        row = mysql_fetch_row(result);
    }
}

