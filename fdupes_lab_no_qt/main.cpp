/**
 * Аналог утилиты fdupes
 *
 * Copyright (c) 2020, Nikon Podgorny <student@cs.karelia.ru>
 *
 * This code is licensed under a WTFPL.
 */

#include <fstream>
#include <iostream>
#include <string>
#include <experimental/filesystem>

namespace fs = std::experimental::filesystem;
using namespace std;

int main(int argc, char const *argv[]) {
    if(argc < 2) {
        std::cerr << "no directories specified" << '\n';
        return 1;
    }

    fs::path start_path = argv[1];

    if(start_path.is_relative()) {
        start_path = fs::current_path() / argv[1];
    }

    if(!fs::exists(start_path)) {
        std::cerr << "no such directory " << start_path << '\n';
        return 1;
    }

    for(auto& p: fs::recursive_directory_iterator(start_path)) {
        // do some magic...
    }

    return 0;
}
