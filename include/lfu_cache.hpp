#pragma once

#include <iostream>
#include <fstream>

#include <string>
#include <map>
#include <list>
#include <algorithm>

#include <cassert>

unsigned slow_get_page(const unsigned& index) {
    return index;
}

namespace cache
{
    using key_t = unsigned int;

    template <typename page_t>
    class LFUCache {
    public:
        const char * const LFU_LOG_PATH = "logs/lfuc_log.txt";

    private:
        using KeyList = typename std::list<key_t>;
        using ListIt  = typename KeyList::iterator;
        using freq_t = unsigned int;

        const size_t size_ = 0;
        page_t (*page_getter_) (const key_t&) = nullptr;

        std::map<key_t, page_t> page_hash_;
        std::map<key_t, freq_t> freq_hash_;
        std::map<key_t, ListIt> key_hash_;

        std::map<freq_t, KeyList> cache_;

        freq_t min_freq_   = 0;
        unsigned hit_cntr_ = 0;

    public:
        LFUCache(const size_t &size, page_t (*page_getter)(const key_t&)) :
            size_{size}, page_getter_{page_getter} {}

    private:
        //inv: page in cache
        page_t get_cached_page(const key_t& key) {
            assert(contains(key));
            freq_t& freq = freq_hash_[key];

            cache_[freq].erase(key_hash_[key]);
            if (cache_[freq].size() == 0) {
                cache_.erase(freq);
                if (min_freq_ == freq)
                    min_freq_ = freq + 1;
            }
            freq++;
            insert_freq_key(freq, key);

            hit_cntr_++;
            return page_hash_[key];
        }

        //inv: free space in cache, page not in cache
        void insert_page(const key_t& key, page_t page) {
            assert(!full() && !contains(key));

            freq_t start_freq = 1;
            freq_hash_[key] = start_freq;

            insert_freq_key(start_freq, key);
            
            if (min_freq_ == 0)
                min_freq_ = start_freq;

            page_hash_[key] = page;
            return;
        }

        void insert_freq_key(const freq_t& freq, const key_t& key) {
            if (!cache_.contains(freq))
                cache_[freq] = KeyList{};

            cache_[freq].push_front(key);
            key_hash_[key] = cache_[freq].begin();
        }

        void erase_lfu() {
            if (size() == 0)
                return;

            key_t& min_key = cache_[min_freq_].back();
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
            for (const auto& elem: cache_)
                if (elem.first < min_freq_ || min_freq_ == 0)
                    min_freq_ = elem.first;
        }

        void dump(const std::string& msg = "") const{

            std::ofstream fout(LFU_LOG_PATH, std::ios::app);
            if (!fout.is_open())
                return;

            std::string dump_str{"LFUCache Dump (" + msg + "):\n"};
            dump_str.append("size     : " + std::to_string(size_) + '\n');
            dump_str.append("min_freq : " + std::to_string(min_freq_) + '\n');
            dump_str.append("freq_hash:\n");
            for (const auto& [key, freq] : freq_hash_)
                dump_str.append("\t" + std::to_string(key) + ": " \
                     + std::to_string(freq) + '\n');

            dump_str.append("page_hash: ");
            for (const auto& [key, page] : page_hash_)
                if (key != page)
                    dump_str.append(std::to_string(key) + "(" + std::to_string(page) + ") ");
            dump_str.append("\n");

            dump_str.append("key_hash: ");
            for (const auto& [key, key_it] : key_hash_)
                if (key != *key_it)
                    dump_str.append(std::to_string(key) + "(" + std::to_string(*key_it) + ") ");
            dump_str.append("\n");

            dump_str.append("cache:\n");
            for (const auto& [freq, key_list] : cache_) {
                dump_str.append('\t' + std::to_string(freq) + " -> ");

                for (const auto& key : key_list)
                    dump_str.append(std::to_string(key) + " ");
                dump_str.append("\n");
            }

            fout << dump_str << '\n' << std::endl;
            fout.close();
        }

    public:
        unsigned hits() const {
            return hit_cntr_;
        }

        bool contains(const key_t& key) const {
            return page_hash_.contains(key);
        }

        bool full() const {
            return size() == size_;
        }

        size_t size() const {
            return page_hash_.size();
        }

        page_t proc_page(const key_t& key) {

            if (contains(key)) {
                page_t page = get_cached_page(key);

                #ifndef NDEBUG
                dump("key=" + std::to_string(key) + "(hit)");
                #endif
                return page;
            }

            page_t page = slow_get_page(key);

            if (full()) 
                erase_lfu();

            insert_page(key, page);
            
            #ifndef NDEBUG
            dump("key=" + std::to_string(key));
            #endif
            return page;
        }
    };
} // namespace cache
