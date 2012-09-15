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
