
#include "pcache.hpp"
#include "api.hpp"

#include <ctime>

int main() {
    unsigned size = 0;
    unsigned keys_amount = 0;

    std::cin >> size;
    std::cin >> keys_amount;

#ifndef NDEBUG
    std::cout << "Perfect Cache" << std::endl;
    std::cout << "cache size: " << size << '\n' << "keys amount: " << keys_amount << std::endl;

    std::cout << "cache hits: ";
    auto start_time = std::clock();
#endif

    std::vector<unsigned> keys{};
    for (unsigned i = 0; i < keys_amount; i++) {
        unsigned key = 0;
        std::cin >> key;
        keys.push_back(key);
    }

    cache::PerfectCache<unsigned, unsigned> pc_cache{size, slow_lookup_update, keys};
    size_t hits = 0;
    for (unsigned &key : keys)
        if (pc_cache.lookup_update(key) == true)
            hits++;

    std::cout << hits << std::endl;

#ifndef NDEBUG
    auto duration = (std::clock() - start_time);
    std::cout << "runtime: " << duration << " us" << std::endl;
#endif

    return 0;
}