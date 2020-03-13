/**
 * Аналог утилиты fdupes
 *
 * Copyright (c) 2020, Nikon Podgorny <student@cs.karelia.ru>
 *
 * This code is licensed under a WTFPL.
 */

/*
WARNING: the code that follows will make you cry; a safety pig is provided below for your benefit.

                         _
 _._ _..._ .-',     _.._(`))
'-. `     '  /-._.-'    ',/
   )         \            '.
  / _    _    |             \
 |  a    a    /              |
 \   .-.                     ;
  '-('' ).-'       ,'       ;
     '-;           |      .'
        \           \    /
        | 7  .__  _.-\   \
        | |  |  ``/  /`  /
       /,_|  |   /,_/   /
          /,_/      '`-'
*/

#include <fstream>
#include <iostream>
#include <string>
#include <experimental/filesystem>
#include <unordered_map>
#include <list>

namespace fs = std::experimental::filesystem;
using namespace std;

// Распределяет все файлы в указанной директории по из размеру
unordered_map<unsigned long, list<fs::path>> sort_by_size(fs::path start_path);

int main(int argc, char const *argv[]) {
    if(argc < 2) {
        cerr << "no directories specified" << '\n';
        return 1;
    }

    fs::path start_path = argv[1];

    if(start_path.is_relative()) {
        start_path = fs::current_path() / argv[1];
    }

    if(!fs::is_directory(start_path)) {
        cerr << start_path << " is not a directory" << '\n';
        return 1;
    }

    if(!fs::exists(start_path)) {
        cerr << "no such directory " << start_path << '\n';
        return 1;
    }

    unordered_map<unsigned long, list<fs::path>> files_by_size;

    files_by_size = sort_by_size(start_path);

    for(auto& [fsize, names] : files_by_size) {
        cout << fsize << " | " << names.size() << '\n';
    }

    return 0;
}

unordered_map<unsigned long, list<fs::path>> sort_by_size(fs::path start_path) {
    unordered_map<unsigned long, list<fs::path>> files_by_size;

    for(auto& p: fs::recursive_directory_iterator(start_path)) {
        if(!fs::is_regular_file(p)) {
            continue;
        }

        unsigned long fsize = file_size(p);

        if(files_by_size.count(fsize) > 0) {
            files_by_size[fsize].push_back(p);
        } else {
            list<fs::path> new_list = {p};
            files_by_size[fsize] = new_list;
        }
    }

    return files_by_size;
}
