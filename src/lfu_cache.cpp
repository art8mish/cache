
#include "../include/lfu_cache.hpp"
#include <vector>
#include <ctime>

int main()
{
    unsigned size = 0;
    unsigned keys_amount = 0;

    std::cin >> size;
    std::cin >> keys_amount;

    #ifndef NDEBUG
    std::cout << "LFU Cache" << std::endl;
    std::cout << "cache size: " << size << '\n'
              << "keys amount: " << keys_amount << std::endl;
    std::cout << "cache hits: ";

    auto start_time = std::clock();
    #endif

    std::vector<unsigned> keys{};
    for (unsigned i = 0; i < keys_amount; i++)
    {
        unsigned key = 0;
        std::cin >> key;
        keys.push_back(key);
    }

    cache::LFUCache<unsigned> cache{size, slow_get_page};

    for (unsigned &key : keys)
        cache.proc_page(key);

    std::cout << cache.hits() << std::endl;

    #ifndef NDEBUG
    auto duration = std::clock() - start_time;
    std::cout << "runtime: " << duration << " us" << std::endl;
    #endif

    return 0;
}