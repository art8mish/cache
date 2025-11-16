#pragma once

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <list>
#include <string>
#include <unordered_map>

namespace cache {

template <typename key_t, typename page_t> class LFUCache {
    using KeyList = typename std::list<key_t>;
    using freq_t = unsigned int;

    const size_t size_ = 0;

    struct KeyNode {
        page_t page;
        freq_t freq;
        KeyList::iterator it;
    };

    using Cache = typename std::unordered_map<key_t, KeyNode>;
    using FreqTbl = typename std::unordered_map<freq_t, KeyList>;

    Cache cache_;
    FreqTbl freq_tbl_;

    freq_t min_freq_ = 0;
    static const freq_t start_freq_ = 1;

public:
    LFUCache(const size_t size) : size_{size} {
    }

private:
    // inv: page in cache
    page_t get_cached_page(Cache::iterator cache_it) {
        KeyNode &node = cache_it->second;

        const auto key_list_it = freq_tbl_.find(node.freq);
        assert(key_list_it != freq_tbl_.end());
        KeyList &key_list = key_list_it->second;

        key_list.erase(node.it);
        if (key_list.size() == 0) {
            freq_tbl_.erase(key_list_it);
            if (min_freq_ == node.freq)
                min_freq_ = node.freq + 1;
        }
        node.freq++;
        node.it = insert_freq_key(node.freq, cache_it->first);

        return node.page;
    }

    // inv: free space in cache, page not in cache
    void insert_page(const key_t &key, page_t page) {
        assert(!full());

        cache_.emplace(key, KeyNode{.page = std::move(page),
                                    .freq = start_freq_,
                                    .it = insert_freq_key(start_freq_, key)});

        if (min_freq_ == 0)
            min_freq_ = start_freq_;
        return;
    }

    KeyList::iterator insert_freq_key(const freq_t freq, const key_t &key) {
        const auto [key_list_it, flg] = freq_tbl_.try_emplace(freq);
        KeyList &key_list = key_list_it->second;
        key_list.push_front(key);
        return key_list.begin();
    }

    void erase_lfu() {
        if (size() == 0)
            return;

        const auto key_list_it = freq_tbl_.find(min_freq_);
        assert(key_list_it != freq_tbl_.end());
        KeyList &key_list = key_list_it->second;

        key_t &min_key = key_list.back();
        cache_.erase(min_key);
        key_list.pop_back();

        if (key_list.size() == 0) {
            freq_tbl_.erase(key_list_it);
            min_freq_ = 0;
            update_min_freq();
        }

        return;
    }

    void update_min_freq() {
        freq_t freq = min_freq_;
        for (const auto &elem : freq_tbl_)
            if (elem.first < freq || freq == 0)
                freq = elem.first;
        min_freq_ = freq;
    }

    void dump(const std::string &msg = "") const {
        std::cout << "LFUCache Dump (" + msg + "):\n";
        std::cout << "size     : " + std::to_string(size_) << '\n';
        std::cout << "min_freq : " + std::to_string(min_freq_) << '\n';
        std::cout << "cache:\n";
        for (const auto &[key, node] : cache_)
            std::cout << "\t" + std::to_string(key) + " : freq " + std::to_string(node.freq) +
                             " : page " + std::to_string(node.page) + " : it " +
                             std::to_string(*node.it) + '\n';

        std::cout << "freq_tbl:\n";
        for (const auto &[freq, key_list] : freq_tbl_) {
            std::cout << '\t' + std::to_string(freq) + " -> ";

            for (const auto &key : key_list)
                std::cout << std::to_string(key) + " ";
            std::cout << '\n';
        }
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
        return cache_.size();
    }

    template <typename getter_t> bool lookup_update(const key_t &key, getter_t page_getter) {
        const auto cache_it = cache_.find(key);
        if (cache_it != cache_.end()) {
            get_cached_page(cache_it);
#ifndef NDEBUG
            dump("key=" + std::to_string(key) + "(hit)");
#endif
            return true; // hit
        }

        if (full())
            erase_lfu();
        insert_page(key, page_getter(key));

#ifndef NDEBUG
        dump("key=" + std::to_string(key));
#endif
        return false; // not hit
    }
};
} // namespace cache
