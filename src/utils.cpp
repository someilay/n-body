#include "utils.h"
#include <chrono>
#include <sys/stat.h>

inline bool file_exists(const std::string &name) {
    struct stat buffer{};
    return (stat(name.c_str(), &buffer) == 0);
}


sf::Font getArialFont(const std::string &path) {
    sf::Font font;
    if (file_exists(path)) {
        font.loadFromFile(path);
        return font;
    }
    if (file_exists("../" + path)) {
        font.loadFromFile("../" + path);
        return font;
    }
    std::fprintf(stderr, "File arialmt.ttf has not been found!");
    exit(1);
}


double timeSinceEpochMilliseconds() {
    using namespace std::chrono;
    uint64_t res = duration_cast<milliseconds>(system_clock::now().time_since_epoch()).count();
    return ((double) res) / 1000;
}


std::vector<double> operator + (std::vector<double> a, std::vector<double> b) {
    auto s = std::min(a.size(), b.size());
    std::vector<double> result(s);
    for (u_long i = 0; i < s; ++i) result[i] = a[i] + b[i];
    return result;
}


std::vector<double> operator * (std::vector<double> a, double v) {
    auto s = std::min(a.size(), a.size());
    std::vector<double> result(s);
    for (u_long i = 0; i < s; ++i) result[i] = a[i] * v;
    return result;
}
