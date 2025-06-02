#include "../include/NRUPolicy.h"
#include <limits>
#include <iostream>

using namespace std;

void NRUPolicy::on_page_load(ll page_number) {}

void NRUPolicy::on_page_access(ll page_number) {}

ll NRUPolicy::choose_victim(const vector<Page>& pages) {
    vector<ll> class_0, class_1, class_2, class_3;
    for (ll i = 0; i < static_cast<ll>(pages.size()); i++) {
        if (pages[i].is_valid()) {
            bool R = pages[i].is_referenced();
            bool M = pages[i].is_modified();
            if (!R && !M)
                class_0.push_back(i);
            else if (!R && M)
                class_1.push_back(i);
            else if (R && !M)
                class_2.push_back(i);
            else
                class_3.push_back(i);
        }
    }
    vector<ll>* victim_class = nullptr;
    if (!class_0.empty())
        victim_class = &class_0;
    else if (!class_1.empty())
        victim_class = &class_1;
    else if (!class_2.empty())
        victim_class = &class_2;
    else
        victim_class = &class_3;
    ll victim_page_index = (*victim_class)[0];
    return victim_page_index;
}

void NRUPolicy::on_page_evict(ll page_number) {}
