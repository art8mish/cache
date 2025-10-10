#pragma once

#include <algorithm>
#include <cassert>
#include <fstream>
#include <iostream>
#include <list>
#include <map>
#include <string>

namespace cache {

std::string LFU_LOG_PATH{"logs/lfuc_log.log"};

template <typename key_t, typename page_t> class LFUCache {
    using KeyList = typename std::list<key_t>;
    using ListIt = typename KeyList::iterator;
    using freq_t = unsigned int;

    const size_t size_ = 0;
    page_t (*page_getter_)(const key_t &) = nullptr;

    std::map<key_t, page_t> page_hash_;
    std::map<key_t, freq_t> freq_hash_;
    std::map<key_t, ListIt> key_hash_;

    std::map<freq_t, KeyList> cache_;

    freq_t min_freq_ = 0;

public:
    LFUCache(const size_t &size, page_t (*page_getter)(const key_t &))
        : size_{size}, page_getter_{page_getter} {
    }

private:
    // inv: page in cache
    page_t get_cached_page(const key_t &key) {
        assert(contains(key));
        freq_t &freq = freq_hash_[key];

        cache_[freq].erase(key_hash_[key]);
        if (cache_[freq].size() == 0) {
            cache_.erase(freq);
            if (min_freq_ == freq)
                min_freq_ = freq + 1;
        }
        freq++;
        insert_freq_key(freq, key);

        return page_hash_[key];
    }

    // inv: free space in cache, page not in cache
    void insert_page(const key_t &key, page_t page) {
        assert(!full() && !contains(key));

        freq_t start_freq = 1;
        freq_hash_[key] = start_freq;

        insert_freq_key(start_freq, key);

        if (min_freq_ == 0)
            min_freq_ = start_freq;

        page_hash_[key] = page;
        return;
    }

    void insert_freq_key(const freq_t &freq, const key_t &key) {
        if (!cache_.contains(freq))
            cache_[freq] = KeyList{};

        cache_[freq].push_front(key);
        key_hash_[key] = cache_[freq].begin();
    }

    void erase_lfu() {
        if (size() == 0)
            return;

        key_t &min_key = cache_[min_freq_].back();
        key_hash_.erase(min_key);
        page_hash_.erase(min_key);
        freq_hash_.erase(min_key);
        cache_[min_freq_].pop_back();

        if (cache_[min_freq_].size() == 0) {
            cache_.erase(min_freq_);
            min_freq_ = 0;
            update_min_freq();
        }

        return;
    }

    void update_min_freq() {
        for (const auto &elem : cache_)
            if (elem.first < min_freq_ || min_freq_ == 0)
                min_freq_ = elem.first;
    }

    void dump(const std::string &msg = "") const {
        std::ofstream fout(LFU_LOG_PATH, std::ios::app);
        if (!fout.is_open())
            return;

        fout << "LFUCache Dump (" + msg + "):" << std::endl;
        fout << "size     : " + std::to_string(size_) << std::endl;
        fout << "min_freq : " + std::to_string(min_freq_) << std::endl;
        fout << "freq_hash:" << std::endl;
        for (const auto &[key, freq] : freq_hash_)
            fout << "\t" + std::to_string(key) + ": " + std::to_string(freq) << std::endl;

        fout << "page_hash: ";
        for (const auto &[key, page] : page_hash_)
            if (key != page)
                fout << std::to_string(key) + "(" + std::to_string(page) + ") ";
        fout << std::endl;

        fout << "key_hash: ";
        for (const auto &[key, key_it] : key_hash_)
            if (key != *key_it)
                fout << std::to_string(key) + "(" + std::to_string(*key_it) + ")";
        fout << std::endl;

        fout << "cache:" << std::endl;
        for (const auto &[freq, key_list] : cache_) {
            fout << '\t' + std::to_string(freq) + " -> ";

            for (const auto &key : key_list)
                fout << std::to_string(key) + " ";
            fout << std::endl;
        }
        fout.close();
    }

public:
    bool contains(const key_t &key) const {
        return page_hash_.contains(key);
    }

    bool full() const {
        return size() == size_;
    }

    size_t size() const {
        return page_hash_.size();
    }

    page_t lookup_update(const key_t &key) {
        if (contains(key)) {
            [[maybe_unused]] page_t page = get_cached_page(key);
#ifndef NDEBUG
            dump("key=" + std::to_string(key) + "(hit)");
#endif
            return true; // hit
        }

        [[maybe_unused]] page_t page = page_getter_(key);

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
