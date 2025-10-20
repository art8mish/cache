
#include <gtest/gtest.h>

#include <fstream>

#include "api.hpp"
#include "pcache.hpp"

class TestPerfectCache : public ::testing::Test {
protected:
    using cache_t = typename cache::PerfectCache<unsigned, unsigned>;

    static const size_t data_size = 12;
    std::vector<unsigned> keys_{1, 2, 3, 4, 1, 2, 5, 1, 2, 4, 3, 4};

    cache_t cache_{4, keys_};
};

TEST_F(TestPerfectCache, ProcPage_Size) {
    ASSERT_EQ(0, cache_.size()); // init

    size_t out_size[data_size]{1, 2, 3, 4, 4, 4, 4, 3, 2, 2, 1, 0};

    for (unsigned i = 0; i < data_size; i++) {
        cache_.lookup_update<unsigned (*)(unsigned)>(keys_[i], slow_get_page);
        ASSERT_EQ(out_size[i], cache_.size());
    }
}

TEST_F(TestPerfectCache, ProcPage_Full) {
    ASSERT_EQ(false, cache_.full()); // init

    bool out_full[data_size]{false, false, false, true,  true,  true,
                             true,  false, false, false, false, false};

    for (unsigned i = 0; i < data_size; i++) {
        cache_.lookup_update<unsigned (*)(unsigned)>(keys_[i], slow_get_page);
        ASSERT_EQ(out_full[i], cache_.full());
    }
}

TEST_F(TestPerfectCache, ProcPage_Contains) {
    ASSERT_EQ(false, cache_.contains(0)); // init

    unsigned in_contains[data_size]{0, 2, 3, 2, 5, 4, 1, 5, 1, 3, 2, 3};
    bool out_contains[data_size]{false, true,  true,  true, false, true,
                                 true,  false, false, true, false, false};

    for (unsigned i = 0; i < data_size; i++) {
        cache_.lookup_update<unsigned (*)(unsigned)>(keys_[i], slow_get_page);
        ASSERT_EQ(out_contains[i], cache_.contains(in_contains[i]));
    }
}

TEST_F(TestPerfectCache, ProcPage_Hits) {
    size_t out_hits[data_size]{0, 0, 0, 0, 1, 2, 2, 3, 4, 5, 6, 7};

    size_t hits = 0;
    for (unsigned i = 0; i < data_size; i++) {
        if (cache_.lookup_update<unsigned (*)(unsigned)>(keys_[i], slow_get_page) == true)
            hits++;
        ASSERT_EQ(out_hits[i], hits);
    }
}

TEST(LFUCacheDataTests, DATA_004) {
    std::vector<unsigned> keys{4, 2,  1, 2, 5, 4, 1, 6, 3, 2,  10, 2, 9,  2, 7,
                               5, 10, 2, 6, 1, 0, 1, 2, 4, 10, 5,  9, 10, 2, 5};
    cache::PerfectCache<unsigned, unsigned> cache{5, keys};

    size_t hits = 0;
    for (auto &key : keys)
        if (cache.lookup_update<unsigned (*)(unsigned)>(key, slow_get_page) == true)
            hits++;

    ASSERT_EQ(18, hits);
}