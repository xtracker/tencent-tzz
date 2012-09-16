#include "seed.h"
#include "logic.h"
#include "mysql_ops.h"

#include <stdio.h>
#include <algorithm>
#include <math.h>
#include <map>

#define PI      3.1415926
#define EARTH_RADIUS 6378.137

double radian(double d) {
    return d * PI / 180.0;
}

double _abs(double x) {
	return x > 0 ? x : -x;
}

double get_distance(double lat1, double lng1,
                    double lat2, double lng2) {
//    double radLat1 = radian(lat1);
//    double radLat2 = radian(lat2);
//    double a = radLat1 - radLat2;
//    double b = radian(lng1) -radian(lat2);
//
//    double dst = 2 * asin((sqrt(pow(sin(a/2),2) + cos(radLat1) * cos(radLat2) *
//                    pow(sin(b/2), 2))));
//
//    dst = dst * EARTH_RADIUS;
//    dst = round(dst * 10000) / 10000;
	double dst = _abs(lat1 - lat2) + _abs(lng1 - lng2);
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
        if (points[i].dis > 20000) break;
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
        const float x, const float y,
        const std::string place,
        const std::string view_image) {
    // get the date
    Seed seed;
    seed._title = title;
    seed._detail = detail;
    seed._user_id = user_id;
    seed._x = x;
    seed._y = y;
    seed._place = place;
    seed._view_image = view_image;

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

bool SeedGetter::get_all_site(std::vector<Pri_Site> *ret) {
    //_sql_conn->get_all_seed(ret);
	_site_ret.clear();
    _sql_conn->get_all_site(&_site_ret);
	printf("%d\n", _site_ret.size());
//    for (int i = 0; i < _site_ret.size(); i++) {
//        ret->push_back(_site_ret[i]);
//    }
    return true;
}

bool SeedGetter::search_nearby_site(int park_id,std::vector<Pri_Site> *ret) {
    for (int i = 0; i < _site_ret.size(); i++) {
        if (_site_ret[i]._park_id == park_id) {
            ret->push_back(_site_ret[i]);
        }
    }
    return true;
}

bool SeedGetter::search_nearby_site(float x, float y, std::vector<Pri_Site> *ret) {
    //_sql_conn->get_all_site(&_site_ret);
    std::vector<cmp_point> nearby_site;
    std::map<int, Pri_Site*> key_value;
    for (int i = 0; i < _site_ret.size(); i++) {
        cmp_point tmp;
        tmp.dis = get_distance(x, y, _site_ret[i]._x, _site_ret[i]._y);
        tmp.id = _site_ret[i]._site_id;
        nearby_site.push_back(tmp);
        key_value.insert(std::pair<int, Pri_Site*>(_site_ret[i]._site_id,
                    &_site_ret[i]));
    }
    sort(nearby_site.begin(), nearby_site.end(), cmp);
	printf("...size %d\n", nearby_site.size());
	printf("....dis %f\n", nearby_site[0].dis);
    if (nearby_site.size() == 0 || nearby_site[0].dis > 20000)
        return false;

    int parkid = key_value[nearby_site[0].id]->_park_id;
	printf("...parkid: %d\n", parkid);
    //std::vector<Pri_Site*> points;
    for (int i = 0; i < _site_ret.size(); i++) {
        if (_site_ret[i]._park_id != parkid) continue;
        //points.push_back(&_site_ret[i]);
        ret->push_back(_site_ret[i]);
    }
    return true;
}

struct router_point {
    float dis;
    int weight;
    int id;
};

int find_best_site(float x, float y, std::vector<Pri_Site*> &leaves) {
    float value = 0;
    int flag = 0;
    float belt = 0.4;
    for (int i = 0; i < leaves.size(); i++) {
        Pri_Site *next_site = leaves[i];
        float dis = get_distance(x, y, next_site->_x, next_site->_y);
        float val = (100.0f / dis) * belt + next_site->_score * (1 - belt);
        if (val > value) {
            flag = i;
            value = val;
        }
    }
    return flag;
}

void dfs_site(float x, float y, std::vector<Pri_Site*> &leaves, std::vector<int> *ret) {
   if (leaves.size() <= 0) return; 

   int flag = find_best_site(x, y, leaves);
   printf("...%d\n", leaves[flag]->_site_id);
   ret->push_back(leaves[flag]->_site_id);
   std::vector<Pri_Site *> nxt_leaves;
   for (int i = 0; i < leaves.size(); i++) {
        if (i != flag) {
            nxt_leaves.push_back(leaves[i]);
        }
   }
//   printf("next size:%d\n", nxt_leaves.size());
//   for (int i = 0; i < nxt_leaves.size(); i++) {
//	printf("%d ", nxt_leaves[i]->_site_id);
//   }
//   printf("\n");
   dfs_site(leaves[flag]->_x, leaves[flag]->_y, nxt_leaves, ret);
}

bool SeedGetter::get_site_router(const float x, const float y,
        std::vector<Pri_Site> *ret) {
    ret->clear();
    //1= get_all_site nearby
    std::vector<cmp_point> nearby_site;
    std::map<int, Pri_Site*> key_value;
    for (int i = 0; i < _site_ret.size(); i++) {
        cmp_point tmp;
        tmp.dis = get_distance(x, y, _site_ret[i]._x, _site_ret[i]._y);
        tmp.id = _site_ret[i]._site_id;
        nearby_site.push_back(tmp);
        key_value.insert(std::pair<int, Pri_Site*>(_site_ret[i]._site_id,
                    &_site_ret[i]));
    }
    sort(nearby_site.begin(), nearby_site.end(), cmp);
    if (nearby_site.size() == 0 || nearby_site[0].dis > 20000)
        return false;

    int parkid = key_value[nearby_site[0].id]->_park_id;
    std::vector<Pri_Site*> points;
    for (int i = 0; i < _site_ret.size(); i++) {
        if (_site_ret[i]._park_id != parkid) continue;
        points.push_back(&_site_ret[i]);
    }
    std::vector<int> ret_ids;
    dfs_site(x, y, points, &ret_ids);

    for (int i = 0; i < ret_ids.size(); i++) {
        ret->push_back(*key_value[ret_ids[i]]);
    }
    //vector<
    //2= dps_search_site_router
}

bool SeedGetter::get_park_info(const std::string &name, Park *ret) {
    _sql_conn->get_park(name, ret);
    return true;
}
