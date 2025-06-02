#pragma once

#include "ReplacementPolicy.h"

using namespace std;

class NRUPolicy : public ReplacementPolicy {
public:
    NRUPolicy() = default;
    virtual ~NRUPolicy() = default;

    void on_page_load(ll page_number) override;

    void on_page_access(ll page_number) override;

    ll choose_victim(const vector<Page>& pages) override;

    void on_page_evict(ll page_number) override;
};
