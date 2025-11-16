

#include "lfu_cache.hpp"
#include "api.hpp"

#include <ctime>
#include <iostream>
#include <vector>

int main() {
    unsigned size = 0;
    unsigned keys_amount = 0;

    std::cin >> size;
    if (!std::cin.good()) {
        std::cout << "Error: incorrect size" << std::endl;
        return 1;
    }

    std::cin >> keys_amount;
    if (!std::cin.good()) {
        std::cout << "Error: incorrect keys amount" << std::endl;
        return 1;
    }

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

        std::cin >> key;
        if (!std::cin.good()) {
            std::cout << "Error: incorrect key" << std::endl;
            return 1;
        }
        keys.push_back(key);
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