#ifndef VV_HPP
#define VV_HPP

#include <iostream>
#include <string>
#include <unordered_map>
#include <unordered_set>

namespace vv {

    /*
     * A version vector is a mapping from identifiers to clock values.
     */
    using version_vec = std::unordered_map<std::string, int>;

    /*
     * Increment the clock value for a given ID.
     *
     * TODO
     * Interfaces should be simpler than implementation. This function doesn't
     * do all that much.
     */
    void tick(version_vec &v, const std::string &id) {
        v[id]++;
    }

    /*
     * Synchronize two clocks, discarding obsolete knowledge.
     */
    version_vec merge(const version_vec &a, const version_vec &b) {
        version_vec merged = a;
        for (const auto &[id, clock]: b) {
            merged[id] = std::max(merged[id], clock);
        }
        return merged;
    }

    /*
     * Get the version vector that covers the given set of versions.
     */
    version_vec merge(const std::vector<version_vec> &vecs) {
        version_vec merged;
        for (const auto &vec: vecs) {
            for (const auto &[id, clock]: vec) {
                merged[id] = std::max(merged[id], clock);
            }
        }
        return merged;
    }

    auto descends(const version_vec &a, const version_vec &b) {
        std::unordered_set<std::string> ids;
        for (const auto &[id, clock]: a) {
            ids.emplace(id);
        }
        for (const auto &[id, clock]: b) {
            ids.emplace(id);
        }
        auto descends = false;
        for (const auto &id: ids) {
            auto a_clk = 0;
            auto b_clk = 0;
            if (a.find(id) != a.end()) a_clk = a.at(id);
            if (b.find(id) != b.end()) b_clk = b.at(id);
            if (a_clk < b_clk) return false;
            if (a_clk > b_clk) descends = true;
        }
        return descends;
    }

    void print(const version_vec &v) {
        for (const auto &[id, clock]: v) {
            std::cout << id << ":" << clock << ",";
        }
        std::cout << std::endl;
    }
}

#endif