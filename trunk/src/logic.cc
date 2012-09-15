#include "seed.h"
#include "logic.h"
#include "mysql_ops.h"

#include <stdio.h>
#include <algorithm>

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
    for (int i = 0; i < _index.size(); i++) {
        printf("%d %f %f\n", _index[i]._seed_id,
                _index[i]._x, _index[i]._y);
    }
}

bool get_nearby_seed_ids(float x, float y,
        std::vector<int> *ids) {
    vector<cmp_point> points;
    for (int i = 0; i < _index.size(); i++) {
        float dis = (x - _index[i]._x) * (x - _index[i]._x) +
            (y - _index[i]._y) * (y - _index[i]._y);
        cmp_point tmp;
        tmp.dis = dis;
        tmp.id = _index[i]._seed_id;
        points.push_back(tmp);
    }
    sort(points.begin(), points.end(), cmp);
    for (int i = 0; i < 20 && i < points.size(); i++) {
        ids->push_back(points[i].id);
    }
    return true;
}

bool search_nearby_seed(float x, float y) {
    std::vector<int> ids;
    get_nearby_seed_ids(x, y, &ids);
    std::vector<Seed> ret;
    _sql_conn->get_seed_list(ids, &ret);

    // for test
    for (auto it = ret.begin(); it != ret->end(); it++) {
        Seed &cur = *it;
        printf("%s %s %s %s %f %f\n",
                it->_title,
                it->_detail,
                it->_image_url,
                it->_user_name,
                it->_x,
                it->_y);
    }
    return true;
}
