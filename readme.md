
# Cache

## Описание

Проект представляет из себя реализации двух кешей для сравнения LFU и Perfect. На вход подаётся размер хеша, количество входных страниц и ключи для каждой страницы в заданном формате, пример: 
```
4 12
1 2 3 4 1 2 5 1 2 4 3 4
```
> Примеры наборов данных можно найти в папке `./data/`

В качестве выходных данных выдаётся количество попаданий (хитов).

## Сборка

Зависимости проекта (c++20):

```
sudo apt install -y \
    cmake \
    build-essential \
    libgtest-dev \
```

Сборка проекта осуществляется следующими командами: 

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
cmake --build build
```

Запуск файлов:
* `build/lfu_cache` - LFU cache
* `build/lfuc_tests` - LFU cache tests
* `build/pcache`    - Perfect cache
* `build/pc_tests`  - Perfect cache tests

Запуск на примерах из папки `./data`:
```bash
./build/lfu_cache < data/004.dat
```

Запуск тестов:
```bash
ctest --test-dir build
```



## Отладка

Чтобы скомпилировать программы в режиме отладки, следует скомпилировать в режиме `Debug`

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Debug
cmake --build build
```
