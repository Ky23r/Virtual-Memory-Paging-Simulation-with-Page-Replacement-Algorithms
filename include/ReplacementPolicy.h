#pragma once

#include <vector>
#include "Page.h"

using namespace std;

using ll = long long;

class ReplacementPolicy {
public:
    virtual ~ReplacementPolicy() = default;

    virtual void on_page_load(ll page_number) = 0;

    virtual void on_page_access(ll page_number) = 0;

    virtual ll choose_victim(const vector<Page>& pages) = 0;

    virtual void on_page_evict(ll page_number) = 0;
};
