#ifndef UTIL_H
#define UTIL_H

constexpr int get_string_size(const char * input,int i = 0) {
    return input[i] == '\0' ? i : get_string_size(input,i+1);
}

constexpr int get_string_size(const wchar_t* input, int i = 0){
    return input[i] == L'\0' ? i * 2 : get_string_size(input, i + 1);
}

constexpr uint32_t leftrotate(uint32_t x, int c) {
    return c == 0 ? x : (x << c) | (x >> (32-c));
}

constexpr uint32_t rightrotate(uint32_t x, int c) {
    return c == 0 ? x : (x >> c) | (x << (32-c));
}

constexpr uint64_t rightrotate64(uint64_t x, int c) {
    return c == 0 ? x : (x >> c) | (x << (64-c));
}

template<typename T>
constexpr unsigned char get_byte(T input, int i) {
    return (input >> 8*i) & 0xff;
}


template<>
constexpr unsigned char get_byte(const char * input, int i) {
    return input[i];
}

template<>
constexpr unsigned char get_byte(const wchar_t* input, int i) {
    int index = i / 2;
    int offset = i % 2;
    uint16_t value = static_cast<uint16_t>(input[index]);
    return static_cast<unsigned char>((value >> (offset * 8)) & 0xFF);
}

template<>
constexpr unsigned char get_byte(char * input, int i) {
    return input[i];
}

template<>
constexpr unsigned char get_byte(wchar_t* input, int i) {
    return ((const char*)input)[i];
}

template<>
unsigned char get_byte(std::string input, int i) {
    return input[i];
}


template<typename T>
constexpr size_t get_size(T input) {
    return sizeof(T);
}

template<>
constexpr size_t get_size(const char * input) {
    return get_string_size(input);
}

template<>
constexpr size_t get_size(const wchar_t* input) {
    return get_string_size(input);
}

template<>
constexpr size_t get_size(char * input) {
    return get_string_size(input);
}

template<>
constexpr size_t get_size(wchar_t* input) {
    return get_string_size(input);
}


template<>
size_t get_size(std::string input) {
    return input.size();
}

constexpr uint32_t endianess_convertion(uint32_t value) {
    return (get_byte<uint32_t>(value,0) << 8*3) +
      (get_byte<uint32_t>(value,1) << 8*2) +
      (get_byte<uint32_t>(value,2) << 8) +
      (get_byte<uint32_t>(value,3)); 
}

#endif

