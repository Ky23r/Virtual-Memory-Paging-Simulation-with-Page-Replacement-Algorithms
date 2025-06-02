#pragma once

#include "Page.h"
#include "ReplacementPolicy.h"
#include <vector>
#include <memory>

using namespace std;

using ll = long long;

class PageTable {
private:
    ll current_time;
    ll page_fault_count;
    ll access_counter;

    static constexpr int RESET_INTERVAL = 10;

    vector<Page> page_table;
    vector<ll> free_frame_list;
    unique_ptr<ReplacementPolicy> policy;

    ll replace_page();
    ll evict_page(ll victim_page_index);
    void reset_reference_bits();

public:
    PageTable(ll logical_memory_size, ll physical_memory_size, unique_ptr<ReplacementPolicy> policy);

    bool is_page_in_memory(ll page_number) const;

    void access_page(ll page_number);

    void load_page(ll page_number);

    void display_page_table() const;

    ll get_page_fault_count() const;

    void set_page_fault_count(ll count);
};
