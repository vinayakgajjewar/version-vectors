#ifndef KVSTORE_HPP
#define KVSTORE_HPP

#include <string>

#include "vv.hpp"

namespace kvstore {

    /*
     * TODO
     * Maybe these could be named better.
     *
     * TODO
     * Maybe these should be typedefs
     */
    using versioned_val = std::pair<std::string, vv::version_vec>;

    /*
     * TODO
     * Maybe this type definition isn't necessary?
     */
    using sibling_vals = std::vector<versioned_val>;

    /*
     * Each node has an identifier and a mapping from keys to a set of
     * concurrent values.
     */
    typedef struct {
        std::string id;
        std::unordered_map<std::string, sibling_vals> store;
    } node;

    auto put(node &n, const std::string &k, const std::string &v, const vv::version_vec &ctx) {
        std::cout << "Putting key " << k << " and value " << v << " on node " << n.id << std::endl;

        vv::version_vec new_vv = ctx;
        vv::tick(new_vv, n.id);
        std::cout << "Creating new version: ";
        vv::print(new_vv);

        if (n.store[k].empty()) {
            std::cout << n.id << " has not stored key " << k << " before" << std::endl;
            n.store[k] = {std::make_pair(v, new_vv)};
            return new_vv;
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
        return new_vv;
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

    /*
     * TODO
     *
     * I'm just syncing a key at a time for ease of implementation. When we
     * sync, we probably want to sync all the keys at once.
     */
    void sync_key(node &n1, node &n2, const std::string &k) {
        std::cout << "Syncing key " << k << " between " << n1.id << " and " << n2.id << std::endl;
        std::vector<versioned_val> all;
        auto n1_sibs = n1.store[k];
        auto n2_sibs = n2.store[k];
        all.insert(all.end(), n1_sibs.begin(), n1_sibs.end());
        all.insert(all.end(), n2_sibs.begin(), n2_sibs.end());

        std::vector<versioned_val> concurrent;
        for (const auto &val: all) {
            auto descended = false;
            for (const auto &comp: all) {
                if (vv::descends(comp.second, val.second)) {
                    descended = true;
                    break;
                }
            }
            if (!descended) {
                concurrent.push_back(val);
            }
        }

        std::cout << "Concurrent siblings for " << k << ":" << std::endl;
        for (const auto &val: concurrent) {
            std::cout << val.first << ": ";
            vv::print(val.second);
        }

        n1.store[k] = concurrent;
        n2.store[k] = concurrent;
    }
}

#endif