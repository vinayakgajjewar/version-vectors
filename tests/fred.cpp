#include "../include/kvstore.hpp"
#include "../include/vv.hpp"

int main() {
    vv::version_vec va;
    vv::version_vec vb;

    vv::tick(vb, "b");

    vv::tick(va, "a");
    vv::tick(va, "a");
    vv::tick(va, "a");

    if (vv::descends(va, vb)) {
        std::cout << "va descends vb" << std::endl;
    } else {
        std::cout << "va does not descend vb" << std::endl;
    }

    if (vv::descends(vb, va)) {
        std::cout << "vb descends va" << std::endl;
    } else {
        std::cout << "vb does not descend va" << std::endl;
    }
}