#include <iostream>
#include <vector>
#include <iomanip>
#include <cstdlib>
#include <ctime>

using namespace std;

using ll = long long;

const int RESET_INTERVAL = 10;

class PageTable {
private:
    ll current_time;
    ll page_fault_count;
    ll access_counter = 0;

    class Page {
    public:
        ll last_access_time;
        ll frame_number;
        int is_valid, referenced, modified;

        Page() : last_access_time(0), frame_number(-1), is_valid(0), referenced(0), modified(0) {}
    };

    vector<Page> page_table;
    vector<ll> free_frame_list;
    vector<ll> fifo_queue;
    vector<pair<ll, ll>> lfu_list;
    vector<ll> second_chance_queue;

    ll replace_page(int choice);
    ll evict_page(ll victim_page_index);
    void reset_reference_bits();

public:
    PageTable(ll logical_memory_size, ll physical_memory_size);

    bool is_page_in_memory(ll page_number);
    void access_page(ll page_number, int choice);
    void load_page(ll page_number, int choice);
    void display_page_table();
    ll get_page_fault_count();
    void set_page_fault_count(int count);
};


PageTable::PageTable(ll logical_memory_size, ll physical_memory_size) : current_time(1), page_fault_count(0) {
    for (ll i = physical_memory_size - 1; i >= 0; i--) {
        free_frame_list.push_back(i);
    }
    page_table.resize(logical_memory_size);
}


bool PageTable::is_page_in_memory(ll page_number) {
    return page_table[page_number].is_valid;
}


void PageTable::access_page(ll page_number, int choice) {
    ll frame = page_table[page_number].frame_number;

    page_table[page_number].last_access_time = current_time++;
    page_table[page_number].referenced = 1;

    // Simulate memory access type with 40% probability for write operations.
    // If the result is less than 4 (out of 10), it's a write (modified = 1); otherwise, it's a read.

    page_table[page_number].modified = (rand() % 10 < 4) ? 1 : 0;

    if (choice == 3) {
        for (auto& entry : lfu_list) {
            if (entry.first == page_number) {
                entry.second++;
                break;
            }
        }
    }

    access_counter++;

    if ((choice == 4 || choice == 5) && access_counter % RESET_INTERVAL == 0) {
        reset_reference_bits();
        cout << "[Info] Reference bits reset.\n";
    }

    display_page_table();

    cout << "Page " << page_number << " accessed in frame " << frame << ".\n\n";
}


void PageTable::load_page(ll page_number, int choice) {
    ll frame;

    if (!free_frame_list.empty()) {
        frame = free_frame_list.back();
        free_frame_list.pop_back();
    } else {
        frame = replace_page(choice);
    }

    page_table[page_number].frame_number = frame;
    page_table[page_number].is_valid = 1;

    cout << "Page Fault: Loaded page " << page_number << " into frame " << frame << ".\n";

    page_fault_count++;

    switch (choice) {
        case 2:
            fifo_queue.push_back(page_number);
            break;
        case 3:
            lfu_list.push_back({ page_number, 1 });
            break;
        case 5:
            second_chance_queue.push_back(page_number);
            break;
    }
}


ll PageTable::replace_page(int choice) {
    ll frame_to_free = -1;

    switch (choice) {
        case 1: {
            ll earliest_time = current_time;
            ll victim_page_index = -1;
            for (ll j = 0; j < page_table.size(); j++) {
                if (page_table[j].is_valid == 1) {
                    if (page_table[j].last_access_time < earliest_time) {
                        earliest_time = page_table[j].last_access_time;
                        victim_page_index = j;
                    }
                }
            }
            frame_to_free = evict_page(victim_page_index);
            break;
        }

        case 2: {
            ll victim_page_index = fifo_queue.front();
            fifo_queue.erase(fifo_queue.begin());
            frame_to_free = evict_page(victim_page_index);
            break;
        }

        case 3: {
            ll least_frequent_index = 0;
            for (ll i = 1; i < lfu_list.size(); i++) {
                if (lfu_list[i].second < lfu_list[least_frequent_index].second) {
                    least_frequent_index = i;
                }
            }
            ll victim_page_index = lfu_list[least_frequent_index].first;
            lfu_list.erase(lfu_list.begin() + least_frequent_index);
            frame_to_free = evict_page(victim_page_index);
            break;
        }

        case 4: {
            vector<ll> class_0, class_1, class_2, class_3;
            for (ll i = 0; i < page_table.size(); i++) {
                if (page_table[i].is_valid) {
                    int R = page_table[i].referenced;
                    int M = page_table[i].modified;
                    if (R == 0 && M == 0)
                        class_0.push_back(i);
                    else if (R == 0 && M == 1)
                        class_1.push_back(i);
                    else if (R == 1 && M == 0)
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
            frame_to_free = evict_page(victim_page_index);
            break;
        }

        case 5: {
            while (true) {
                ll candidate_page = second_chance_queue.front();
                second_chance_queue.erase(second_chance_queue.begin());
                if (page_table[candidate_page].referenced == 0) {
                    frame_to_free = evict_page(candidate_page);
                    break;
                } else {
                    page_table[candidate_page].referenced = 0;
                    second_chance_queue.push_back(candidate_page);
                }
            }
            break;
        }

        default:
            return -1;
    }
    return frame_to_free;
}


void PageTable::display_page_table() {
    cout << "\n==== PAGE TABLE ====\n";
    cout << "+--------------+----------------+-------------+--------------------+--------------+------------+\n";
    cout << "|  Page Index  |  Frame Number  |  Valid Bit  |  Last Access Time  |  Referenced  |  Modified  |\n";
    cout << "+--------------+----------------+-------------+--------------------+--------------+------------+\n";
    for (int i = 0; i < page_table.size(); i++) {
        cout << "| " << setw(13) << left << i
            << "| " << setw(15) << page_table[i].frame_number
            << "| " << setw(12) << (page_table[i].is_valid ? "YES" : "NO")
            << "| " << setw(19) << page_table[i].last_access_time
            << "| " << setw(13) << (page_table[i].referenced ? "YES" : "NO")
            << "| " << setw(11) << (page_table[i].modified ? "YES" : "NO")
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


void PageTable::set_page_fault_count(int count) {
    page_fault_count = count;
}


ll PageTable::get_page_fault_count() {
    return page_fault_count;
}


void PageTable::reset_reference_bits() {
    for (auto& page : page_table) {
        page.referenced = 0;
    }
}


ll PageTable::evict_page(ll victim_page_index) {
    ll frame_to_free = page_table[victim_page_index].frame_number;
    page_table[victim_page_index].is_valid = 0;
    page_table[victim_page_index].frame_number = -1;
    page_table[victim_page_index].referenced = 0;
    page_table[victim_page_index].modified = 0;
    page_table[victim_page_index].last_access_time = 0;
    return frame_to_free;
}
