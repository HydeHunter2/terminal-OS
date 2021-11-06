#pragma once

#include <string>
#include <vector>
#include <fstream>
#include <filesystem>

class FileSystem {
  public:
    struct File {
        File(const std::string& path);
        std::string path;
    };

    // use some pointer to data instead of string
    static void write(File& file, const std::string data);
    static std::string read(File& file);

    static std::vector<std::string> ls(const std::string path);
};
