#include "../include/SecondChancePolicy.h"

using namespace std;

void SecondChancePolicy::on_page_load(ll page_number) {
    second_chance_queue.push_back(page_number);
}

void SecondChancePolicy::on_page_access(ll page_number) {}

ll SecondChancePolicy::choose_victim(const vector<Page>& pages) {
    while (true) {
        if (second_chance_queue.empty()) {
            return -1;
        }
        ll page_index = second_chance_queue.front();
        second_chance_queue.pop_front();
        if (!pages[page_index].is_referenced()) {
            return page_index;
        }
        second_chance_queue.push_back(page_index);
    }
}

void SecondChancePolicy::on_page_evict(ll page_number) {}
