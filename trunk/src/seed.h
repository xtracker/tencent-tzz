#pragma once

#include <string>

class Seed {
public:
    std::string _title;
    std::string _detail;
    std::string _image_url;
    std::string _user_name;
    std::string _place;
    std::string _view_image;
    int _user_id;
    float _x;
    float _y;
};

class SeedIndex {
public:
    float _x;
    float _y;
    int _seed_id;
};

class Pri_Site {
public:
    float _x;
    float _y;
    std::string _name;
    std::string _detail;
    std::string _image_url;
    int _park_id;
    int _score;
    int _site_id;
};
