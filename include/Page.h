#pragma once

#include <cstdint>

using namespace std;

using ll = long long;

class Page {
private:
    ll last_access_time;
    ll frame_number;
    bool valid;
    bool referenced;
    bool modified;

public:
    Page();

    ll get_last_access_time() const;
    ll get_frame_number() const;
    bool is_valid() const;
    bool is_referenced() const;
    bool is_modified() const;

    void set_last_access_time(ll last_access_time);
    void set_frame_number(ll frame_number);
    void set_valid(bool valid);
    void set_referenced(bool referenced);
    void set_modified(bool modified);

    void reset();
};
