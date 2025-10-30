#pragma once

#include "frontend/parser/Parser.h"
#include "frontend/lexer/Lexer.h"
#include "frontend/ast/Component.h"
#include "shared/utils/FileSystem.h"
#include <string>
#include <memory>
#include <unordered_map>
#include <unordered_set>
#include <filesystem>

namespace artic {

/**
 * @brief Registry for loading and caching parsed components
 *
 * Handles:
 * - Loading .atc files from filesystem
 * - Parsing components
 * - Caching parsed components
 * - Resolving relative imports
 * - Detecting circular dependencies
 */
class ComponentRegistry {
public:
    ComponentRegistry() = default;

    /**
     * @brief Load and parse a component from file
     * @param filePath Absolute or relative path to .atc file
     * @return Parsed component or nullptr on error
     */
    Component* loadComponent(const std::string& filePath);

    /**
     * @brief Get a cached component by name
     * @param componentName Name of the component (from imports)
     * @return Component pointer or nullptr if not found
     */
    Component* getComponent(const std::string& componentName) const;

    /**
     * @brief Check if a component is loaded
     * @param componentName Name of the component
     * @return true if component is in registry
     */
    bool hasComponent(const std::string& componentName) const;

    /**
     * @brief Load all components imported by a component
     * @param component Component to load imports for
     * @param baseDir Base directory for resolving relative paths
     */
    void loadImports(Component* component, const std::string& baseDir);

    /**
     * @brief Get all loaded components
     * @return Map of component name -> component
     */
    const std::unordered_map<std::string, std::unique_ptr<Component>>& getAllComponents() const {
        return m_components;
    }

private:
    // Map: component name -> parsed component
    std::unordered_map<std::string, std::unique_ptr<Component>> m_components;

    // Map: absolute file path -> component name (for deduplication)
    std::unordered_map<std::string, std::string> m_pathToName;

    // Set of files currently being loaded (for circular dependency detection)
    std::unordered_set<std::string> m_loading;

    /**
     * @brief Resolve relative path to absolute path
     * @param path Path to resolve (can be relative or absolute)
     * @param baseDir Base directory for relative paths
     * @return Absolute path
     */
    std::string resolvePath(const std::string& path, const std::string& baseDir);

    /**
     * @brief Extract component name from file path
     * @param filePath Path to .atc file
     * @return Component name (e.g., "Card" from "Card.atc")
     */
    std::string extractComponentName(const std::string& filePath);

    /**
     * @brief Check if we're in a circular dependency
     * @param filePath File being loaded
     * @return true if circular dependency detected
     */
    bool isCircularDependency(const std::string& filePath) const;
};

} // namespace artic
