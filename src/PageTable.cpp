#include "../include/PageTable.h"
#include "../include/ReplacementPolicy.h"
#include <iostream>
#include <iomanip>
#include <cstdlib>

using namespace std;

PageTable::PageTable(ll logical_memory_size, ll physical_memory_size, std::unique_ptr<ReplacementPolicy> policy) :
    current_time(1),
    page_fault_count(0),
    access_counter(0),
    page_table(logical_memory_size),
    policy(move(policy)) {
    for (ll i = physical_memory_size - 1; i >= 0; --i) {
        free_frame_list.push_back(i);
    }
}

bool PageTable::is_page_in_memory(ll page_number) const {
    return page_table[page_number].is_valid();
}

void PageTable::access_page(ll page_number) {
    ll frame = page_table[page_number].get_frame_number();
    ll new_time = current_time++;
    page_table[page_number].set_last_access_time(new_time);
    page_table[page_number].set_referenced(true);
    page_table[page_number].set_modified((rand() % 10 < 4 ? true : false));

    policy->on_page_access(page_number);

    access_counter++;
    if (access_counter % RESET_INTERVAL == 0) {
        reset_reference_bits();
        cout << "[Info] Reference bits have been reset after " << RESET_INTERVAL << " accesses.\n";
    }

    display_page_table();

    cout << "Page " << page_number << " accessed in frame " << frame << ".\n\n";
}

void PageTable::load_page(ll page_number) {
    ll frame;

    if (!free_frame_list.empty()) {
        frame = free_frame_list.back();
        free_frame_list.pop_back();
    } else {
        frame = replace_page();
    }

    page_table[page_number].set_frame_number(frame);
    page_table[page_number].set_valid(true);

    cout << "Page Fault: Loaded page " << page_number << " into frame " << frame << ".\n";

    page_fault_count++;

    policy->on_page_load(page_number);
}

ll PageTable::replace_page() {
    ll victim = policy->choose_victim(page_table);
    ll free_frame = evict_page(victim);
    policy->on_page_evict(victim);
    return free_frame;
}

ll PageTable::evict_page(ll victim_page_index) {
    ll free_frame = page_table[victim_page_index].get_frame_number();
    page_table[victim_page_index].reset();
    return free_frame;
}

void PageTable::reset_reference_bits() {
    for (auto& page : page_table) {
        page.set_referenced(false);
    }
}

void PageTable::display_page_table() const {
    cout << "\n==== PAGE TABLE ====\n";
    cout << "+--------------+----------------+-------------+--------------------+--------------+------------+\n";
    cout << "|  Page Index  |  Frame Number  |  Valid Bit  |  Last Access Time  |  Referenced  |  Modified  |\n";
    cout << "+--------------+----------------+-------------+--------------------+--------------+------------+\n";
    for (int i = 0; i < page_table.size(); i++) {
        cout << "| " << setw(13) << left << i
            << "| " << setw(15) << page_table[i].get_frame_number()
            << "| " << setw(12) << (page_table[i].is_valid() ? "YES" : "NO")
            << "| " << setw(19) << page_table[i].get_last_access_time()
            << "| " << setw(13) << (page_table[i].is_referenced() ? "YES" : "NO")
            << "| " << setw(11) << (page_table[i].is_modified() ? "YES" : "NO")
            << "|\n";
        cout << "+--------------+----------------+-------------+--------------------+--------------+------------+\n";
    }

    cout << "\n==== FREE FRAME POOL ====\n";
    cout << "+---------------------+\n";
    cout << "|     Frame Index     |\n";
    cout << "+---------------------+\n";
    for (int frame : free_frame_list) {
        cout << "| " << setw(20) << left << frame << "|\n";
        cout << "+---------------------+\n";
    }
}

ll PageTable::get_page_fault_count() const {
    return page_fault_count;
}

void PageTable::set_page_fault_count(ll count) {
    page_fault_count = count;
}
