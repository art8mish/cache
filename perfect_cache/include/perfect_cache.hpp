#pragma once

#include <iostream>
#include <fstream>

#include <string>
#include <map>
#include <queue>
#include <vector>
#include <algorithm>


unsigned slow_get_page(const unsigned index) {
    return index;
}

namespace cache
{
    using key_t = unsigned int;

    template <typename page_t>
    class PerfectCache {
    public:
        const char *LOG_PATH = "perfect_cache/log/pc_log.txt";

    private:
        // using KeyList = typename std::list<key_t>;
        // using ListIt  = typename KeyList::iterator;
        //using MapIt  = typename std::unordered_map<key_t, page_t>::iterator;
        using index_t = size_t;
        using IndexQueue = typename std::queue<index_t>;

        const size_t size_ = 0;
        page_t (*page_getter_) (key_t) = nullptr;

        std::map<key_t, page_t> page_hash_ {};
        //std::map<key_t, IndexIt> indexes_ {};
        std::map<key_t, IndexQueue> indexes_ {};
        //std::unordered_set <key_t> black_list{};
        //std::map<key_t, freq_t> freq_hash_;
        //std::map<key_t, ListIt> key_hash_;

        //std::map<freq_t, std::list<key_t>> cache_;

        key_t furthest_key_ = 0;
        unsigned hit_cntr_  = 0;

    public:
        PerfectCache(const size_t size, page_t (*page_getter)(const key_t), 
                     const std::vector<key_t>& keys) :
            size_{size}, page_getter_{page_getter} {
                
                size_t keys_amount = keys.size();
                for (size_t i=0; i < keys_amount; i++) {
                    if (!indexes_.contains(keys[i]))
                        indexes_[keys[i]] = IndexQueue{};

                    indexes_[keys[i]].push(i);
                }

                //erase keys, which is met once
                for (const key_t& key : keys)
                    if (indexes_[key].size() == 1){
                        indexes_[key].pop();
                        indexes_.erase(key);
                    }

                // for (auto& [key, index_q] : indexes_) {
                //     indexes_ = 
                // }

                #ifdef DEBUG
                dump("init");
                #endif 
            }

    private:
        void update_index(const key_t& key) {
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

        key_t calc_furthest_key() {
            index_t max_index  = 0;
            key_t furthest_key = furthest_key_;
            for (auto& [key, page] : page_hash_) {
                if (!is_met(key))
                    continue;
                
                index_t index = next_i(key);
                if (index > max_index) {
                    max_index    = index;
                    furthest_key = key;
                }
            }

            return furthest_key;
        }

        //inv: page in cache
        page_t get_cached_page(const key_t& key) {
            if (!contains(key))
                throw std::out_of_range("get_cached_page: key not found");

            hit_cntr_++;
            return page_hash_[key];
        }

        //inv: free space in cache
        void insert_page(const key_t& key, page_t page) {
            if (size() == size_ || contains(key))
                return;

            page_hash_[key] = page;

            if (size() == 1 || 
                next_i(key) > next_i(furthest_key_))
                furthest_key_ = key;
            return;
        }

        void erase_furthest() {
            if (size() == 0)
                return;

            page_hash_.erase(furthest_key_);
            key_t new_furthest_key = calc_furthest_key();

            if (new_furthest_key == furthest_key_)
                furthest_key_ = 0;
            else
                furthest_key_ = new_furthest_key;
            return;
        }


        // #ifdef DEBUG
        // dump("key=" + std::to_string(key));
        // #endif
        void dump(const std::string msg = ""){
            
            std::string dump_str{"PerfectCache Dump (" + msg + "):\n"};
            dump_str.append("size        : " + std::to_string(size_) + '\n');
            dump_str.append("furthest_key: " + std::to_string(furthest_key_) + '\n');
    
            dump_str.append("page_hash: ");
            for (const auto& [key, page] : page_hash_) {
                dump_str.append(std::to_string(key));
                if (key != page)
                    dump_str.append("(" + std::to_string(page) + ")");
                 dump_str.append(" ");
            }
            dump_str.append("\n");

            dump_str.append("indexes:\n");
            for (const auto& [key, index_q] : indexes_) {
                dump_str.append(std::to_string(key) + ": " + std::to_string(next_i(key)) + "\n");
                // for (const auto& index : index_q)
                //     dump_str.append(index + " ");
                // dump_str.append("\n");
            }

            //std::cout << dump_str << std::endl;
            std::ofstream fout(LOG_PATH, std::ios::app);
            if (fout.is_open())
                fout << dump_str << '\n' << std::endl;
            fout.close();
        }

        bool is_met(const key_t& key) const {
            return indexes_.contains(key);
        }

        index_t next_i(const key_t& key) {
            if (!is_met(key))
                throw std::out_of_range("next_i: key not found");
            index_t next_index = indexes_[key].front();
            return next_index;
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
                update_index(key);
                #ifdef DEBUG
                dump(std::to_string(key) + " hit");
                #endif
                return page;
            }

            page_t page = slow_get_page(key);

            if (!is_met(key)) {
                #ifdef DEBUG
                dump(std::to_string(key) + " skip");
                #endif
                return page;
            }

            else if (full() &&
                     next_i(key) < next_i(furthest_key_)) {
                erase_furthest();
                #ifdef DEBUG
                dump(std::to_string(key) + " erase furthest");
                #endif
            }

            if (!full()) {
                insert_page(key, page);
                #ifdef DEBUG
                dump(std::to_string(key) + " insert");
                #endif
            }
            update_index(key);

            #ifdef DEBUG
            dump(std::to_string(key));
            #endif
            return page;
        }
    };
} // namespace cache
