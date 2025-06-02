#include "../include/LRUPolicy.h"
#include <limits>
#include <iostream>

using namespace std;

void LRUPolicy::on_page_load(ll page_number) {}

void LRUPolicy::on_page_access(ll page_number) {}

ll LRUPolicy::choose_victim(const vector<Page>& pages) {
    ll victim_page_index = -1;
    ll earliest = numeric_limits<ll>::max();
    for (ll i = 0; i < static_cast<ll>(pages.size()); i++) {
        if (pages[i].is_valid()) {
            ll last_access_time = pages[i].get_last_access_time();
            if (last_access_time < earliest) {
                earliest = last_access_time;
                victim_page_index = i;
            }
        }
    }
    return victim_page_index;
}

void LRUPolicy::on_page_evict(ll page_number) {}
