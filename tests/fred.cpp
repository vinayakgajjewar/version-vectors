#include "../include/client.hpp"

int main() {

    kvstore::node a;
    a.id = "a";

    kvstore::node b;
    b.id = "b";

    client_cache c1, c2;

    put(c1, b, "k", "red");

    put(c2, a, "k", "blue");
    put(c2, a, "k", "blue");
    put(c2, a, "k", "blue");

    kvstore::sync(a, b);

    auto vals = get(c1, a, "k");
    std::cout << "Results for key k:" << std::endl;
    for (const auto &val: vals) {
        std::cout << val.first << ": ";
        vv::print(val.second);
    }

    put(c1, a, "k", "green");

    kvstore::sync(a, b);
}