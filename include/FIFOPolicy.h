#pragma once

#include "ReplacementPolicy.h"
#include <deque>

using namespace std;

class FIFOPolicy : public ReplacementPolicy {
private:
    deque<ll> fifo_queue;

public:
    FIFOPolicy() = default;
    virtual ~FIFOPolicy() = default;

    void on_page_load(ll page_number) override;

    void on_page_access(ll page_number) override;

    ll choose_victim(const vector<Page>& pages) override;

    void on_page_evict(ll page_number) override;
};
