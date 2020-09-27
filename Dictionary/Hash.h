// By Fabio Nardelli
// Picola raccolta di funzioni hash per la classe Dictionary (ovviamente possono
// essere usate da qualunque classe).

#ifndef HASH_H
#define HASH_H

#include <cmath>
#include <string>
#include <cstring>
#include <functional>

template <typename K>
std::size_t getHash(const K &key);

// Per i tipi di dato diversi da quelli sotto, usa std::hash
template <typename K>
std::size_t getHash(const K &key)
{
    std::hash<K> hashFunction;
    return hashFunction(key);
}

// usa l'algoritmo proposto da Daniel J. Bernstein(noto anche come DJBX33A). 
template <typename K>
std::size_t getHash(const std::string &key) 
{
    std::size_t hash = 5381;
    std::size_t l = key.length();

    // hash << 5 == hash * 2^5, le operazioni bit a bit dovrebbero
    // garantire prestazioni migliori
    for (std::size_t i = 0; i < l; ++i)
        hash = ((hash << 5) + hash) + key[i];

    return hash;
}

// come sopra ma per stringhe C-style(char *)
template <typename K>
std::size_t getHash(const char *key) 
{
    std::size_t hash = 5381;
    std::size_t l = strlen(key);

    // hash << 5 == hash * 2^5, le operazioni bit a bit dovrebbero
    // garantire prestazioni migliori
    for (std::size_t i = 0; i < l; ++i)
        hash = ((hash << 5) + hash) + key[i];

    return hash;
}

#endif
