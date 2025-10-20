#pragma once

#include <algorithm>
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
    std::unordered_map<key_t, page_t> page_hash_{};
    std::unordered_map<key_t, IndexQueue> indexes_{};

    key_t furthest_key_ = 0;

public:
    PerfectCache(const size_t size, const std::vector<key_t> &keys) : size_{size} {
        size_t keys_amount = keys.size();
        for (size_t i = 0; i < keys_amount; i++) {
            const key_t &key = keys[i];
            if (!indexes_.contains(key))
                indexes_[key] = IndexQueue{};

            indexes_[key].push(i);
        }

        // erase keys, which is met once
        for (const key_t &key : keys)
            if (indexes_[key].size() == 1) {
                indexes_[key].pop();
                indexes_.erase(key);
            }
    }

private:
    void update_index(const key_t &key) {
        if (!is_met(key))
            return;

        indexes_[key].pop();

        if (indexes_[key].empty()) {
            indexes_.erase(key);
            page_hash_.erase(key);
            return;
        }

        if (next_i(key) > next_i(furthest_key_))
            furthest_key_ = key;
        return;
    }

    void calc_furthest_key() {
        index_t max_index = 0;
        key_t furthest_key = 0;
        for (auto &[key, page] : page_hash_) {
            if (!is_met(key))
                continue;

            index_t index = next_i(key);
            if (index > max_index) {
                max_index = index;
                furthest_key = key;
            }
        }
        furthest_key_ = furthest_key;
    }

    // inv: page in cache
    page_t get_cached_page(const key_t &key) {
        if (!contains(key))
            throw std::out_of_range("get_cached_page: key not found");
        return page_hash_[key];
    }

    // inv: free space in cache
    void insert_page(const key_t &key, page_t page) {
        if (full() || contains(key))
            return;

        page_hash_[key] = page;

        if (size() == 1 || next_i(key) > next_i(furthest_key_))
            furthest_key_ = key;
        return;
    }

    void erase_furthest() {
        if (size() == 0)
            return;

        page_hash_.erase(furthest_key_);
        calc_furthest_key();
        return;
    }

    void dump(const std::string &msg = "") {
        std::cout << "PerfectCache Dump (" + msg + "):\n";
        std::cout << "size        : " + std::to_string(size_) << '\n';
        std::cout << "furthest_key: " + std::to_string(furthest_key_) << '\n';

        std::cout << "page_hash: ";
        for (const auto &[key, page] : page_hash_) {
            std::cout << std::to_string(key);
            if (key != page)
                std::cout << "(" + std::to_string(page) + ")";
            std::cout << " ";
        }
        std::cout << '\n';

        std::cout << "indexes:\n";
        for (const auto &[key, index_q] : indexes_)
            std::cout << std::to_string(key) + ": " + std::to_string(next_i(key)) << '\n';
        std::cout << std::endl;
    }

    bool is_met(const key_t &key) const {
        return indexes_.contains(key);
    }

    index_t next_i(const key_t &key) {
        if (!is_met(key))
            throw std::out_of_range("next_i: key not found");
        index_t next_index = indexes_[key].front();
        return next_index;
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

    template <typename getter_t> bool lookup_update(const key_t &key, getter_t page_getter) {
        if (contains(key)) {
            [[maybe_unused]] page_t page = get_cached_page(key);
            update_index(key);

#ifndef NDEBUG
            dump(std::to_string(key) + " hit");
#endif
            return true; // hit
            // return page;
        }

        [[maybe_unused]] page_t page = page_getter(key);

        if (!is_met(key)) {
#ifndef NDEBUG
            dump(std::to_string(key) + " skip");
#endif
            return false; // not hit
            // return page;
        }

        else if (full() && next_i(key) < next_i(furthest_key_)) {
            erase_furthest();

#ifndef NDEBUG
            dump(std::to_string(key) + " erase furthest");
#endif
        }

        if (!full()) {
            insert_page(key, page);

#ifndef NDEBUG
            dump(std::to_string(key) + " insert");
#endif
        }
        update_index(key);

#ifndef NDEBUG
        dump(std::to_string(key));
#endif
        return false; // not hit
        // return page;
    }
};
} // namespace cache
