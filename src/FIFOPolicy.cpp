#include "../include/FIFOPolicy.h"
#include <iostream>

using namespace std;

void FIFOPolicy::on_page_load(ll page_number) {
    fifo_queue.push_back(page_number);
}

void FIFOPolicy::on_page_access(ll page_number) {}

ll FIFOPolicy::choose_victim(const std::vector<Page>& pages) {
    return (fifo_queue.empty() ? -1 : fifo_queue.front());
}

void FIFOPolicy::on_page_evict(ll page_number) {
    if (!fifo_queue.empty()) {
        fifo_queue.pop_front();
    }
}
