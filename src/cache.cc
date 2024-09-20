
#include "../include/cache.h"


class page_t {
    const key_t key_;

public:
    page_t(key_t key) : key_{key} {}
};

page_t *slow_get_page(key_t key, page_t *page_arr[]) {
    return page_arr[key];
}

struct LFUCacheNode {
    unsigned appl_cntr = 0;
    page_t *page = nullptr;
};


class LFUCache {
private:
    
    std::map<key_t, LFUCacheNode *> hash_tbl{};
    unsigned hit_cntr = 0; 
    size_t size_ = 0;
    

public:
    LFUCache(const size_t size) : size_{size} {
        if (size > MAX_CACHE_SIZE)
            throw std::invalid_argument("cache size is too large");

        std::array<LFUCacheNode, size> cache {};
    }

    page_t *proc_page(key_t key) {

        if (hash_tbl.count(key)) {
            LFUCacheNode *cache_node = hash_tbl[key];

            hit_cntr++;
            cache_node->appl_cntr++;
            return cache_node->page;
        }

        else {

        }


    }
};