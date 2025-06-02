#pragma once

#include "ReplacementPolicy.h"
#include <map>

using namespace std;

class LFUPolicy : public ReplacementPolicy {
private:
    map<ll, ll> freq;

public:
    LFUPolicy() = default;
    virtual ~LFUPolicy() = default;

    void on_page_load(ll page_number) override;

    void on_page_access(ll page_number) override;

    ll choose_victim(const vector<Page>& pages) override;

    void on_page_evict(ll page_number) override;
};
