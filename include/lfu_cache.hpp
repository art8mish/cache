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
    std::unordered_map<key_t, KeyNode> cache_;
    std::unordered_map<freq_t, KeyList> freq_tbl_;

    freq_t min_freq_ = 0;

public:
    LFUCache(const size_t &size) : size_{size} {
    }

private:
    // inv: page in cache
    page_t get_cached_page(const key_t &key) {
        assert(contains(key));
        freq_t &freq = cache_[key].freq;

        freq_tbl_[freq].erase(cache_[key].it);
        if (freq_tbl_[freq].size() == 0) {
            freq_tbl_.erase(freq);
            if (min_freq_ == freq)
                min_freq_ = freq + 1;
        }
        freq++;
        insert_freq_key(freq, key);

        return cache_[key].page;
    }

    // inv: free space in cache, page not in cache
    void insert_page(const key_t &key, page_t page) {
        assert(!full() && !contains(key));

        freq_t start_freq = 1;
        cache_[key] = KeyNode{};
        cache_[key].freq = start_freq;

        insert_freq_key(start_freq, key);

        if (min_freq_ == 0)
            min_freq_ = start_freq;

        cache_[key].page = page;
        return;
    }

    void insert_freq_key(const freq_t &freq, const key_t &key) {
        if (!freq_tbl_.contains(freq))
            freq_tbl_[freq] = KeyList{};

        freq_tbl_[freq].push_front(key);
        cache_[key].it = freq_tbl_[freq].begin();
    }

    void erase_lfu() {
        if (size() == 0)
            return;

        key_t &min_key = freq_tbl_[min_freq_].back();
        cache_.erase(min_key);
        freq_tbl_[min_freq_].pop_back();

        if (freq_tbl_[min_freq_].size() == 0) {
            freq_tbl_.erase(min_freq_);
            min_freq_ = 0;
            update_min_freq();
        }

        return;
    }

    void update_min_freq() {
        for (const auto &elem : freq_tbl_)
            if (elem.first < min_freq_ || min_freq_ == 0)
                min_freq_ = elem.first;
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

    template <typename getter_t> page_t lookup_update(const key_t &key, getter_t page_getter) {
        if (contains(key)) {
            [[maybe_unused]] page_t page = get_cached_page(key);
#ifndef NDEBUG
            dump("key=" + std::to_string(key) + "(hit)");
#endif
            return true; // hit
        }

        [[maybe_unused]] page_t page = page_getter(key);

        if (full())
            erase_lfu();

        insert_page(key, page);

#ifndef NDEBUG
        dump("key=" + std::to_string(key));
#endif
        return false; // not hit
    }
};
} // namespace cache
