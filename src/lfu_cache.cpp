

#include "lfu_cache.hpp"
#include "api.hpp"

#include <ctime>
#include <iostream>
#include <vector>

int main() {
    unsigned size = 0;
    unsigned keys_amount = 0;

    std::cin >> size;
    std::cin >> keys_amount;

#ifndef NDEBUG
    std::cout << "LFU Cache";
    std::cout << "\ncache size: " << size;
    std::cout << "\nkeys amount: " << keys_amount;
    std::cout << "\ncache hits: ";

    auto start_time = std::clock();
#endif

    std::vector<unsigned> keys{};
    for (unsigned i = 0; i < keys_amount; i++) {
        unsigned key = 0;
        if (std::cin >> key)
            keys.push_back(key);
        else {
            std::cout << "Error: incorrect stdin" << std::endl;
            return 1;
        }
    }

    cache::LFUCache<unsigned, unsigned> lfu_cache{size};
    size_t hits = 0;
    for (unsigned &key : keys)
        if (lfu_cache.lookup_update<unsigned (*)(unsigned)>(key, slow_get_page) == true)
            hits++;

    std::cout << hits << std::endl;

#ifndef NDEBUG
    auto duration = std::clock() - start_time;
    std::cout << "runtime: " << duration << " us" << std::endl;
#endif

    return 0;
}