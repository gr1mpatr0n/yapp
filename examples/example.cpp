#include <yapp.hpp>
#include <iostream>

int main(int argc, char* argv[]) {
    using namespace yapp;

    URL url("https://apple.com");

    url.parse_input("\t\t\t\r\rhttps://www.apple.com\t\t\t");

    return 0;
}