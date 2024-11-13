
#include "../include/lfu_cache.hpp"
#include <vector>
#include <ctime> 

int main() {
    

    unsigned size        = 0;
    unsigned keys_amount = 0;

    
    // std::ifstream in("input/input.txt");
    // if (!in.is_open()) {
    //     std::cout << "error: incorrect input file" << std::endl;
    //     return 1;
    // }

    std::cin >> size;
    std::cin >> keys_amount;

    #ifdef DEBUG
    std::cout << "LFU Cache" << std::endl;
    std::cout << "cache size: "  << size << '\n' 
              << "keys amount: " << keys_amount << std::endl;
    std::cout << "cache hits: ";

    auto start_time = std::clock();
    #endif

    std::vector<unsigned> keys {};
    for (unsigned i = 0; i < keys_amount; i++) {
        unsigned key = 0;
        std::cin >> key;
        keys.push_back(key);
    }
    //fin.close();

    //std::cout << "keys: ";
    // for (unsigned& key : keys)
    //     std::cout << key << ' ';
    // std::cout << std::endl;

    cache::LFUCache<unsigned> cache {size, slow_get_page};
    
    for (unsigned& key : keys)
        cache.proc_page(key);


    
    std::cout <<  cache.hits() << std::endl;
    //std::cout << "runtime = " << std::clock()/1000.0 << std::endl;
    
    #ifdef DEBUG
    auto duration = std::clock() - start_time;
    std::cout << "runtime: " << duration << " us" << std::endl;
    #endif

    return 0;
}