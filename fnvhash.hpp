#ifndef FNVHASH_HPP
#define FNVHASH_HPP

int fnvhash(const char* str) {
    const int PRIME = 0x01000193;
    int hval = 0x811c9dc5;

    for (size_t i = 0; str[i] != '\0'; ++i) {
        hval = (hval * PRIME) ^ (str[i] & 0xFF);
    }

    return hval;
}

#endif /* end of include guard: FNVHASH_HPP */
