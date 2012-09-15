#include "seed.h"
#include "logic.h"
#include "mysql_ops.h"

#include <stdio.h>
#include <algorithm>
#include <math.h>

#define PI      3.1415926
#define EARTH_RADIUS 6378.137

double radian(double d) {
    return d * PI / 180.0;
}

double get_distance(double lat1, double lng1,
                    double lat2, double lng2) {
    double radLat1 = radian(lat1);
    double radLat2 = radian(lat2);
    double a = radLat1 - radLat2;
    double b = radian(lng1) -radian(lat2);

    double dst = 2 * asin((sqrt(pow(sin(a/2),2) + cos(radLat1) * cos(radLat2) *
                    pow(sin(b/2), 2))));

    dst = dst * EARTH_RADIUS;
    dst = round(dst * 10000) / 10000;
    return dst;
}

// print func, for test
void print_ret_list(const std::vector<Seed> *ret) {
    for (int i = 0; i < ret->size(); i++) {
        const Seed *it = &(ret->at(i));
        printf("%s %s %s %s %f %f\n",
                it->_title.c_str(),
                it->_detail.c_str(),
                it->_image_url.c_str(),
                it->_user_name.c_str(),
                it->_x,
                it->_y);
    }
}

struct cmp_point {
    float dis;
    int id;
};

bool cmp(const cmp_point &a, const cmp_point &b) {
    return a.dis < b.dis;
}

SeedGetter::SeedGetter() {
    printf("...seed getter begin\n");
    _sql_conn = new SqlConn();
}
SeedGetter::~SeedGetter() {
    delete _sql_conn;
}

bool SeedGetter::init_seed_index() { 
    _index = _sql_conn->get_seed_index();
    // for test
    printf("build search index...\n");
    for (int i = 0; i < _index.size(); i++) {
        printf("%d %f %f\n", _index[i]._seed_id,
                _index[i]._x, _index[i]._y);
    }
}

bool SeedGetter::get_nearby_seed_ids(float x, float y,
        std::vector<int> *ids) {
    std::vector<cmp_point> points;
    for (int i = 0; i < _index.size(); i++) {
//        float dis = (x - _index[i]._x) * (x - _index[i]._x) +
//            (y - _index[i]._y) * (y - _index[i]._y);
        float dis = get_distance(x, y, _index[i]._x, _index[i]._y);
        printf("%f\n",dis);
        cmp_point tmp;
        tmp.dis = dis;
        tmp.id = _index[i]._seed_id;
        points.push_back(tmp);
    }
    sort(points.begin(), points.end(), cmp);
    for (int i = 0; i < 20 && i < points.size(); i++) {
        if (points[i].dis > 1000) break;
        ids->push_back(points[i].id);
    }
    return true;
}

bool SeedGetter::search_nearby_seed(float x, float y,
        std::vector<Seed> *ret) {
    std::vector<int> ids;
    get_nearby_seed_ids(x, y, &ids);
    _sql_conn->get_seed_list(ids, ret);

    return true;
}

bool SeedGetter::add_neww_seed(const std::string &title, 
        const std::string &detail, 
        const int user_id, 
        const float x, const float y) {
    // get the date
    Seed seed;
    seed._title = title;
    seed._detail = detail;
    seed._user_id = user_id;
    seed._x = x;
    seed._y = y;

    _sql_conn->insert_seed(seed);
}

bool SeedGetter::get_all_seed(std::vector<Seed> *ret) {
    std::vector<int> ids;
    for (int i = 0; i < _index.size(); i++) {
        ids.push_back(_index[i]._seed_id);
    }

    _sql_conn->get_seed_list(ids, ret);
    return true;
}

bool SeedGetter::get_seed_of_user(const int user_id, std::vector<Seed> *ret) {
    _sql_conn->get_seed_by_user(user_id, ret);
    print_ret_list(ret);
    return true;
}
