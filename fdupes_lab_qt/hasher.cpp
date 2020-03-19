/**
 * Получение хэша файла
 *
 * Copyright (c) 2020, Nikon Podgorny <student@cs.karelia.ru>
 *
 * This code is licensed under a WTFPL.
 */

#include <fstream>
#include <experimental/filesystem>
#include "hasher.h"

hashval get_hash(fs::path file_path) {
    ifstream ifs(file_path, ifstream::in|ifstream::binary);
    char b;
    unsigned long long hash;
    unsigned long long counter = 1;

    while(ifs.good()) {
        ifs.read(&b, 1);
        if(ifs.eof()) {
            break;
        }

        hash += (b * counter) % HASH_CONST;
        counter++;
    }

    return hash;
}
