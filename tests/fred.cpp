#include "../include/kvstore.hpp"
#include "../include/vv.hpp"

int main() {

    kvstore::node a{"a", {}};
    kvstore::node b{"b", {}};
    kvstore::sibling_vals vals;

    kvstore::put(b, "key", "red", {});

    vals = kvstore::get(b, "key", {});
    for (const auto &val: vals) {
        std::cout << val.first << std::endl;
        vv::print(val.second);
    }

    kvstore::put(a, "key", "blue", {{"a", 0}});
    kvstore::put(a, "key", "blue", {{"a", 1}});
    kvstore::put(a, "key", "blue", {{"a", 2}});

    vals = kvstore::get(a, "key", {});
    for (const auto &val: vals) {
        std::cout << val.first << std::endl;
        vv::print(val.second);
    }


}