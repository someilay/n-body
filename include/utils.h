#ifndef N_BODY_UTILS_H
#define N_BODY_UTILS_H

#include <string>
#include <vector>
#include <SFML/Graphics.hpp>


inline bool file_exists(const std::string &name);


sf::Font getArialFont(const std::string &path = "fonts/arialmt.ttf");


double timeSinceEpochMilliseconds();


template<typename ... Args>
std::string string_format(const std::string &format, Args ... args) {
    int size_s = std::snprintf(nullptr, 0, format.c_str(), args ...) + 1; // Extra space for '\0'
    if (size_s <= 0) { throw std::runtime_error("Error during formatting."); }
    auto size = static_cast<size_t>( size_s );
    std::unique_ptr<char[]> buf(new char[size]);
    std::snprintf(buf.get(), size, format.c_str(), args ...);
    return {buf.get(), buf.get() + size - 1};
}


std::vector<double> operator + (std::vector<double> a, std::vector<double> b);


std::vector<double> operator * (std::vector<double> a, double v);


#endif //N_BODY_UTILS_H
