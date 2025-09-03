#ifndef CLIENT_HPP
#define CLIENT_HPP

#include "kvstore.hpp"

typedef struct {
    std::unordered_map<std::string, vv::version_vec> cache;
} client_cache;

void put(client_cache &cache, kvstore::node &node, const std::string &k, const std::string &v) {
    auto vv = kvstore::put(node, k, v, cache.cache[k]);
    cache.cache[k] = vv;
}

/*
 * I think when we do a GET, the client should merge all the received versions
 * and cache that.
 */
auto get(client_cache &cache, kvstore::node &node, const std::string &k) {
    auto siblings = kvstore::get(node, k, cache.cache[k]);

    std::vector<vv::version_vec> versions;
    versions.reserve(siblings.size());
    for (const auto &sibling: siblings) {
        versions.push_back(sibling.second);
    }
    auto merged = vv::merge(versions);
    cache.cache[k] = merged;

    return siblings;
}

#endif