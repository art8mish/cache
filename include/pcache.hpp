#pragma once

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <queue>
#include <string>
#include <unordered_map>
#include <vector>

namespace cache {

template <typename key_t, typename page_t> class PerfectCache {
    using index_t = size_t;
    using IndexQueue = typename std::queue<index_t>;

    const size_t size_ = 0;

    using PageHash = typename std::unordered_map<key_t, page_t>;
    PageHash page_hash_{};

    struct CacheNode {
        IndexQueue indexes{};
        PageHash::iterator page_it{};
    };

    using Cache = typename std::unordered_map<key_t, CacheNode>;
    Cache cache_{};
    Cache::iterator furthest_it_{};

public:
    template <typename It> PerfectCache(const size_t size, It begin, It end) : size_{size} {
        size_t i = 0;
        for (It it = begin; it != end; ++it, ++i) {
            const key_t &key = *it;
            auto [cache_it, flg] = cache_.try_emplace(key);
            CacheNode &node = cache_it->second;
            node.indexes.push(i);
        }

        // erase keys, which is met once
        for (auto it = cache_.begin(), end = cache_.end(); it != end;) {
            if (it->second.indexes.size() == 1)
                it = cache_.erase(it);
            else
                ++it;
        }
        calc_furthest_it();
    }

private:
    bool cached(const Cache::const_iterator cache_it) const {
        return cache_it->second.page_it != page_hash_.end();
    }

    index_t furthest_index() const {
        return furthest_it_->second.indexes.front();
    }

    static index_t index(const Cache::const_iterator cache_it) {
        return cache_it->second.indexes.front();
    }

    void try_erase_page(const Cache::const_iterator cache_it) {
        if (cached(cache_it))
            page_hash_.erase(cache_it->second.page_it);
    }

    void calc_furthest_it() {
        index_t max_index = 0;
        typename Cache::iterator furthest_it{};
        for (auto it = cache_.begin(), end = cache_.end(); it != end; ++it) {
            index_t i = index(it);
            if (i > max_index) {
                max_index = i;
                furthest_it = it;
            }
        }
        furthest_it_ = furthest_it;
        return;
    }

    // inv: cache_it is correct
    void update_index(Cache::iterator cache_it) {
        assert(cache_it != cache_.end());
        CacheNode &node = cache_it->second;

        node.indexes.pop();
        if (node.indexes.empty()) {
            try_erase_page(cache_it);
            cache_.erase(cache_it);
            return;
        }

        if (index(cache_it) > furthest_index())
            furthest_it_ = cache_it;
        return;
    }

    // inv: free space in cache, page is not cached and cache_it is correct
    void insert_page(const Cache::iterator cache_it, page_t page) {
        assert(!full());
        assert(cache_it != cache_.end());
        assert(!cached(cache_it));

        auto [page_it, inserted] = page_hash_.emplace(cache_it->first, std::move(page));
        assert(inserted);
        cache_it->second.page_it = page_it;

        if (furthest_it_ == cache_.end() || index(cache_it) > furthest_index())
            furthest_it_ = cache_it;
        return;
    }

    void erase_furthest() {
        if (size() == 0)
            return;

        page_hash_.erase(furthest_it_->second.page_it);
        calc_furthest_it();
        return;
    }

    void dump(const std::string &msg = "") const {
        std::cout << "PerfectCache Dump (" + msg + "):\n";
        if (cache_.empty()) {
            std::cout << "Empty" << std::endl;
            return;
        }

        std::cout << "size        : " + std::to_string(size_) << '\n';
        std::cout << "furthest_key: " + std::to_string(furthest_it_->first) << '\n';

        std::cout << "page_hash: ";
        for (const auto &[key, page] : page_hash_)
            std::cout << std::to_string(key) + "(" + std::to_string(page) + ") ";
        std::cout << '\n';

        std::cout << "indexes:\n";
        for (auto it = cache_.begin(), end = cache_.end(); it != end; ++it)
            std::cout << std::to_string(it->first) + ": " + std::to_string(index(it)) << '\n';
        std::cout << std::endl;
    }

public:
    bool contains(const key_t &key) const {
        return cache_.contains(key);
    }

    bool full() const {
        return size() == size_;
    }

    size_t size() const {
        return page_hash_.size();
    }

    template <typename getter_t> bool lookup_update(const key_t &key, getter_t page_getter) {

        const auto cache_it = cache_.find(key);
        assert(cache_it != cache_.end());

        if (cached(cache_it)) {
            update_index(cache_it);
#ifndef NDEBUG
            dump(std::to_string(key) + " hit");
#endif
            return true; // hit
        }

        if (full() && (index(cache_it) < furthest_index())) {
            erase_furthest();
#ifndef NDEBUG
            dump(std::to_string(key) + " erase furthest");
#endif
        }

        if (full()) {
#ifndef NDEBUG
            dump(std::to_string(key) + " skip");
#endif
        } else {
            insert_page(cache_it, page_getter(key));
#ifndef NDEBUG
            dump(std::to_string(key) + " insert");
#endif
        }

        update_index(cache_it);
#ifndef NDEBUG
        dump(std::to_string(key));
#endif
        return false; // not hit
    }
};
} // namespace cache
