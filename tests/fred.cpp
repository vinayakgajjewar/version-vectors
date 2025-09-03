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

    kvstore::sync_key(a, b, "k");
}