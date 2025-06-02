#include "../include/LFUPolicy.h"
#include <limits>

void LFUPolicy::on_page_load(ll page_number) {
    freq[page_number] = 0;
}

void LFUPolicy::on_page_access(ll page_number) {
    freq[page_number]++;
}

ll LFUPolicy::choose_victim(const vector<Page>& pages) {
    ll victim_page_index = -1;
    ll min_freq = std::numeric_limits<ll>::max();
    for (auto const& it : freq) {
        if (it.second < min_freq) {
            min_freq = it.second;
            victim_page_index = it.first;
        }
    }
    return victim_page_index;
}

void LFUPolicy::on_page_evict(ll page_number) {
    freq.erase(page_number);
}
