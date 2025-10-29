#include "shared/utils/FileSystem.h"
#include <fstream>
#include <sstream>

namespace artic {

std::optional<std::string> FileSystem::readFile(const std::filesystem::path& filePath) {
    try {
        // Check if file exists
        if (!std::filesystem::exists(filePath)) {
            return std::nullopt;
        }

        // Open file
        std::ifstream file(filePath, std::ios::in | std::ios::binary);
        if (!file.is_open()) {
            return std::nullopt;
        }

        // Read entire contents
        std::ostringstream contents;
        contents << file.rdbuf();

        file.close();

        return contents.str();

    } catch (const std::exception&) {
        return std::nullopt;
    }
}

bool FileSystem::writeFile(const std::filesystem::path& filePath, const std::string& contents) {
    try {
        // Create parent directories if they don't exist
        auto parentPath = filePath.parent_path();
        if (!parentPath.empty() && !std::filesystem::exists(parentPath)) {
            if (!createDirectory(parentPath)) {
                return false;
            }
        }

        // Open file for writing
        std::ofstream file(filePath, std::ios::out | std::ios::binary | std::ios::trunc);
        if (!file.is_open()) {
            return false;
        }

        // Write contents
        file << contents;

        file.close();

        return true;

    } catch (const std::exception&) {
        return false;
    }
}

bool FileSystem::fileExists(const std::filesystem::path& filePath) {
    try {
        return std::filesystem::exists(filePath) && std::filesystem::is_regular_file(filePath);
    } catch (const std::exception&) {
        return false;
    }
}

bool FileSystem::directoryExists(const std::filesystem::path& dirPath) {
    try {
        return std::filesystem::exists(dirPath) && std::filesystem::is_directory(dirPath);
    } catch (const std::exception&) {
        return false;
    }
}

bool FileSystem::createDirectory(const std::filesystem::path& dirPath) {
    try {
        if (std::filesystem::exists(dirPath)) {
            return std::filesystem::is_directory(dirPath);
        }

        return std::filesystem::create_directories(dirPath);

    } catch (const std::exception&) {
        return false;
    }
}

std::string FileSystem::getExtension(const std::filesystem::path& filePath) {
    return filePath.extension().string();
}

std::string FileSystem::getFileNameWithoutExtension(const std::filesystem::path& filePath) {
    return filePath.stem().string();
}

std::filesystem::path FileSystem::getAbsolutePath(const std::filesystem::path& filePath) {
    try {
        return std::filesystem::absolute(filePath);
    } catch (const std::exception&) {
        return filePath;
    }
}

std::filesystem::path FileSystem::joinPath(const std::filesystem::path& base, const std::string& relative) {
    return base / relative;
}

} // namespace artic
