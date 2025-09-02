#ifndef KVSTORE_HPP
#define KVSTORE_HPP

#include <string>

#include "vv.hpp"

namespace kvstore {
    using val = std::pair<std::string, vv::version_vec>;
    using sibling_vals = std::vector<val>;

    /*
     * Each node has an identifier and a mapping from keys to a set of
     * concurrent values.
     */
    typedef struct {
        std::string id;
        std::unordered_map<std::string, sibling_vals> store;
    } node;

    void put(node &n, const std::string &k, const std::string &v, const vv::version_vec &ctx) {
        if (n.store[k].empty()) {
            vv::version_vec vv = {};
            vv::tick(vv, n.id);
            n.store[k] = {std::make_pair(v, vv)};
            return;
        }

        sibling_vals new_siblings = {};
        for (const auto &sibling: n.store[k]) {
            if (vv::descends(sibling.second, ctx)) {
                new_siblings.emplace_back(sibling);
            }
        }
        vv::version_vec new_vv = {};
        vv::tick(new_vv, n.id);
        new_siblings.emplace_back(std::make_pair(v, new_vv));
    }

    sibling_vals get(const node &n, const std::string &k, const vv::version_vec &ctx) {
        return n.store.at(k);
    }
}

#endif