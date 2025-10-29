#pragma once

#include <string>
#include <filesystem>
#include <optional>

namespace artic {

/**
 * @brief File system utilities for Artic compiler
 */
class FileSystem {
public:
    /**
     * @brief Read entire file contents
     * @param filePath Path to file
     * @return File contents, or std::nullopt if error
     */
    static std::optional<std::string> readFile(const std::filesystem::path& filePath);

    /**
     * @brief Write contents to file
     * @param filePath Path to file
     * @param contents Contents to write
     * @return true if successful, false otherwise
     */
    static bool writeFile(const std::filesystem::path& filePath, const std::string& contents);

    /**
     * @brief Check if file exists
     * @param filePath Path to file
     * @return true if exists, false otherwise
     */
    static bool fileExists(const std::filesystem::path& filePath);

    /**
     * @brief Check if directory exists
     * @param dirPath Path to directory
     * @return true if exists, false otherwise
     */
    static bool directoryExists(const std::filesystem::path& dirPath);

    /**
     * @brief Create directory (and parent directories if needed)
     * @param dirPath Path to directory
     * @return true if successful, false otherwise
     */
    static bool createDirectory(const std::filesystem::path& dirPath);

    /**
     * @brief Get file extension
     * @param filePath Path to file
     * @return Extension (e.g., ".atc")
     */
    static std::string getExtension(const std::filesystem::path& filePath);

    /**
     * @brief Get file name without extension
     * @param filePath Path to file
     * @return Filename without extension
     */
    static std::string getFileNameWithoutExtension(const std::filesystem::path& filePath);

    /**
     * @brief Get absolute path
     * @param filePath Path to file
     * @return Absolute path
     */
    static std::filesystem::path getAbsolutePath(const std::filesystem::path& filePath);

    /**
     * @brief Join paths
     * @param base Base path
     * @param relative Relative path
     * @return Joined path
     */
    static std::filesystem::path joinPath(const std::filesystem::path& base, const std::string& relative);
};

} // namespace artic
