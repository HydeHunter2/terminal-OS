#include "file_system.hpp"

FileSystem::File::File(const std::string& path) : path(path) {}

void FileSystem::write(File& file, const std::string data) {
    std::filesystem::create_directories("OS");
    std::ofstream f("OS/" + file.path);
    f << data;
    f.close();
}
std::string FileSystem::read(File& file) {
    std::filesystem::create_directories("OS");
    std::ifstream f("OS/" + file.path);

    f.seekg(0, std::ios::end);
    size_t size = f.tellg();
    std::string buffer(size, ' ');
    f.seekg(0);
    f.read(&buffer[0], size); 
    f.close();

    return buffer;
}


std::vector<std::string> FileSystem::ls(const std::string path) {
    std::filesystem::create_directories("OS");

    std::vector<std::string> files;
    for (const auto& file : std::filesystem::directory_iterator("OS/" + path)) {
        files.push_back(file.path().filename());
    }
    return files;
}
