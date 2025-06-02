#include <iostream>
#include <iomanip>
#include "pagetable.h"

using namespace std;

using ll = long long;

bool is_valid_size(ll);

int main() {
    ll logical_memory_size, physical_memory_size, page_number, reference_count;
    int choice;

    cout << "=== Virtual Memory Paging Simulation ===\n\n";

    cout << "Enter the number of pages in logical memory space: ";
    cin >> logical_memory_size;

    if (!is_valid_size(logical_memory_size)) {
        cerr << "Error: Invalid number of logical pages.\n";
        return 1;
    }

    cout << "Enter the number of frames in physical memory space: ";
    cin >> physical_memory_size;

    if (!is_valid_size(physical_memory_size)) {
        cerr << "Error: Invalid number of physical frames.\n";
        return 1;
    }

    while (true) {
        cout << "\nSelect a page replacement algorithm:\n";
        cout << "1. Least Recently Used (LRU)\n";
        cout << "2. First In First Out (FIFO)\n";
        cout << "3. Least Frequently Used (LFU)\n";
        cout << "4. Not Recently Used (NRU)\n";
        cout << "5. Second Chance\n";
        cout << "6. Exit\n";
        cout << "Your choice: ";
        cin >> choice;

        if (choice == 6) {
            cout << "Goodbye!\n";
            break;
        }

        if (choice < 1 || choice > 6) {
            cerr << "Invalid choice. Please select a number between 1 and 6.\n";
            continue;
        }

        PageTable page_table(logical_memory_size, physical_memory_size);
        page_table.set_page_fault_count(0);

        ll page_hit_count = 0;

        cout << "Enter the number of page accesses: ";
        cin >> reference_count;

        ll page_references[reference_count];

        cout << "Enter the sequence of page numbers to be accessed (each must be between 0 and " << logical_memory_size - 1 << "):\n";

        for (ll i = 0; i < reference_count; i++) {
            cout << "\n[Input] Enter page number #" << i + 1 << ": ";
            cin >> page_references[i];
            page_number = page_references[i];
            cout << "Accessing page " << page_number << "...\n\n";
            if (page_number < 0 || page_number > logical_memory_size - 1) {
                cerr << "Invalid page number. Please enter a number between 0 and " << logical_memory_size - 1 << ".\n";
                i--;
                continue;
            }
            if (page_table.is_page_in_memory(page_number)) {
                page_hit_count++;
                page_table.access_page(page_number, choice);
            } else {
                page_table.load_page(page_number, choice);
                page_table.access_page(page_number, choice);
            }
        }

        int page_fault_count = page_table.get_page_fault_count();
        double hit_ratio = static_cast<double>(page_hit_count) / reference_count;
        double fault_ratio = static_cast<double>(page_fault_count) / reference_count;

        cout << "\n=== Simulation Result ===\n";
        cout << "Total page accesses: " << reference_count << "\n";
        cout << "Total page hits: " << page_hit_count << "\n";
        cout << "Total page faults: " << page_fault_count << "\n";
        cout << fixed << setprecision(2);
        cout << "Hit ratio: " << hit_ratio * 100 << "%\n";
        cout << "Fault ratio: " << fault_ratio * 100 << "%\n";
    }
    return 0;
}


bool is_valid_size(ll size) {
    return size >= 0;
}
