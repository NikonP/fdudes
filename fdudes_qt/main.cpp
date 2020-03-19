/**
 * Аналог утилиты fdupes с использованием библиотек QT
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

#include <unordered_map>
#include <list>
#include <iostream>

#include <QDir>
#include <QFile>
#include <QDirIterator>

#include "hasher.h"

using namespace std;

// Пути к файлам по ключам типа unsigned long long
typedef unordered_map<unsigned long long, list<QString>> ll_path_map;

// Распределяет все файлы в указанной директории по размеру
ll_path_map sort_by_size(QString start_path);

// Распределяет все файлы в указанной директории по хэшу
ll_path_map sort_by_hash(list<QString> paths);

// Сравнивает файлы по байтам
bool compare_by_bytes(QString file_1, QString file_2);

int main(int argc, char const *argv[]) {
    // Проверяем есть ли аргумент
    if(argc < 2) {
        cerr << "no directories specified" << '\n';
        exit(EXIT_FAILURE);
    }

    // Получаем путь
    QString path = argv[1];

    // Превращаем путь в абсолютный
    path = QFileInfo(path).absoluteFilePath();

    // Проверяем существование пути
    if(!QFileInfo(path).exists()) {
        cerr << "no such directory " << path.toStdString() << '\n';
        exit(EXIT_FAILURE);
    }

    // Проверяем является ли путь директорией
    if(!QFileInfo(path).isDir()) {
        cerr << path.toStdString() << " is not a directory" << '\n';
        exit(EXIT_FAILURE);
    }

    // Получаем все файлы отсортированные по размеру
    ll_path_map files_by_size = sort_by_size(path);

    // Для каждой "корзины" сортируем по хэшам
    for(auto [fsize, paths_by_size] : files_by_size) {
        if(paths_by_size.size() > 1) {
            // Получаем все файлы отсортированные по хэшам
            ll_path_map files_by_hash = sort_by_hash(paths_by_size);

            // В каждой "корзине" сравниваем файлы по байтам
            for(auto [hash, paths_by_hash] : files_by_hash) {
                while(paths_by_hash.size() > 1) {
                    // Если РАЗНЫЕ ФАЙЛЫ имеют ОДИН ХЭШ ничего не произойдет
                    // НО теоретически в одной "корзине" может быть несколько "корзин" дубликатов
                    // Поэтому проверем всё
                    QString front = paths_by_hash.front();
                    paths_by_hash.pop_front();

                    list<QString> dups;
                    dups.push_back(front);

                    auto it = paths_by_hash.begin();
                    while(it != paths_by_hash.end()) {
                        if(compare_by_bytes(front, *it)) {
                            dups.push_back(*it);
                            it = paths_by_hash.erase(it);
                        }
                    }

                    // Выводим найденные дубликаты
                    unsigned long long dup_num = 0;
                    if(dups.size() > 1) {
                        cout << dups.size() << " duplicates found:\n";
                        for(QString p : dups) {
                            cout << "[" << dup_num++ << "] " << p.toStdString() << '\n';
                        }
                    }
                    cout << '\n';
                }
            }
        }
    }
    return 0;
}

ll_path_map sort_by_size(QString start_path) {
    ll_path_map files_by_size;

    QDirIterator it(start_path, QDir::Files, QDirIterator::Subdirectories);
    while (it.hasNext()) {
        QFile f(it.next());

        unsigned long long fsize = f.size();

        if(files_by_size.count(fsize) > 0) {
            files_by_size[fsize].push_back(f.fileName());
        } else {
            list<QString> new_list = {f.fileName()};
            files_by_size[fsize] = new_list;
        }
    }

    return files_by_size;
}

ll_path_map sort_by_hash(list<QString> paths) {
    ll_path_map files_by_hash;

    for(QString p : paths) {
        hashval hash = get_hash(p);
        if(files_by_hash.count(hash) > 0) {
            files_by_hash[hash].push_back(p);
        } else {
            list<QString> new_list = {p};
            files_by_hash[hash] = new_list;
        }
    }

    return files_by_hash;
}

bool compare_by_bytes(QString path_1, QString path_2) {
    QFile file_1(path_1);
    QFile file_2(path_2);

    if(!file_1.open(QIODevice::ReadOnly)) {
        cerr << "Can't read file " << path_1.toStdString() << endl;
        exit(EXIT_FAILURE);
    }

    if(!file_2.open(QIODevice::ReadOnly)) {
        cerr << "Can't read file " << path_2.toStdString() << endl;
        exit(EXIT_FAILURE);
    }

    char byte_1;
    char byte_2;

    while(!file_1.atEnd() && !file_2.atEnd()) {
        file_1.read(&byte_1, 1);
        file_2.read(&byte_2, 1);

        if(byte_1 != byte_2) {
            return false;
        }
    }

    if(file_1.atEnd() != file_2.atEnd()) {
        return false;
    }

    return true;
}
