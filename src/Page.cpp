#include "../include/Page.h"

using namespace std;

Page::Page() : last_access_time(0), frame_number(-1), valid(false), referenced(false), modified(false) {}

ll Page::get_last_access_time() const {
    return last_access_time;
}

ll Page::get_frame_number() const {
    return frame_number;
}

bool Page::is_valid() const {
    return valid;
}

bool Page::is_referenced() const {
    return referenced;
}

bool Page::is_modified() const {
    return modified;
}

void Page::set_last_access_time(ll last_access_time) {
    this->last_access_time = last_access_time;
}

void Page::set_frame_number(ll frame_number) {
    this->frame_number = frame_number;
}

void Page::set_valid(bool valid) {
    this->valid = valid;
}

void Page::set_referenced(bool referenced) {
    this->referenced = referenced;
}

void Page::set_modified(bool modified) {
    this->modified = modified;
}

void Page::reset() {
    last_access_time = 0;
    frame_number = -1;
    valid = false;
    referenced = false;
    modified = false;
}
