#ifndef KVSTORE_HPP
#define KVSTORE_HPP

#include <string>

#include "vv.hpp"

namespace kvstore {

    /*
     * TODO
     * Maybe these could be named better.
     */
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
        std::cout << "Putting key " << k << " and value " << v << " on node " << n.id << std::endl;

        vv::version_vec new_vv = ctx;
        vv::tick(new_vv, n.id);

        if (n.store[k].empty()) {
            std::cout << n.id << " has not stored key " << k << " before" << std::endl;
            n.store[k] = {std::make_pair(v, new_vv)};
            return;
        }

        /*
         * We need to keep siblings that are concurrent or newer.
         */
        sibling_vals new_siblings = {};
        for (const auto &sibling: n.store[k]) {
            if (!vv::descends(new_vv, sibling.second)) {
                new_siblings.emplace_back(sibling);
            }
        }
        new_siblings.emplace_back(std::make_pair(v, new_vv));
        n.store[k] = new_siblings;
    }

    /*
     * TODO
     * Only return values that aren't stale with respect to the provided
     * context.
     */
    sibling_vals get(const node &n, const std::string &k, const vv::version_vec &ctx) {
        std::cout << "Getting key " << k << " from node " << n.id << std::endl;
        return n.store.at(k);
    }
}

#endif