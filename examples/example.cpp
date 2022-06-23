#include <yapp.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    using namespace yapp;

    auto api = URL("https://api.github.com")
        / "repos" / "owner" / "repo" / "issues"
        %= {{"state", "open"}, {"sort", "created"}};

    std::cout << api.str() << std::endl;
    return 0;
}