#include "../include/vv.hpp"

int main() {
    vv::version_vec v1;
    vv::tick(v1, "r1");
    std::cout << "v1:" << std::endl;
    vv::print(v1);

    vv::version_vec v2;
    vv::tick(v2, "r2");
    vv::tick(v2, "r2");
    std::cout << "v2:" << std::endl;
    vv::print(v2);

    auto v3 = vv::merge(v1, v2);
    std::cout << "v3:" << std::endl;
    vv::print(v3);

    /*
     * Test the descends relation.
     */
    assert(vv::descends({{"a", 3}, {"b", 2}}, {{"a", 2}, {"b", 2}}) == true);
    assert(vv::descends({{"a", 3}, {"b", 1}}, {{"a", 2}, {"b", 2}}) == false);
}