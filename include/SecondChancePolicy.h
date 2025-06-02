#pragma once

#include "ReplacementPolicy.h"
#include <deque>

using namespace std;

class SecondChancePolicy : public ReplacementPolicy {
private:
    deque<ll> second_chance_queue;

public:
    SecondChancePolicy() = default;
    virtual ~SecondChancePolicy() = default;

    void on_page_load(ll page_number) override;

    void on_page_access(ll page_number) override;

    ll choose_victim(const vector<Page>& pages) override;

    void on_page_evict(ll page_number) override;
};
