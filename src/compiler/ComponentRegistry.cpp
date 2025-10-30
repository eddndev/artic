#include "ComponentRegistry.h"
#include <iostream>
#include <filesystem>

namespace artic {

Component* ComponentRegistry::loadComponent(const std::string& filePath) {
    // Resolve to absolute path
    std::string absPath;
    try {
        absPath = std::filesystem::absolute(filePath).string();
    } catch (const std::exception& e) {
        std::cerr << "Error resolving path '" << filePath << "': " << e.what() << std::endl;
        return nullptr;
    }

    // Check if already loaded
    auto it = m_pathToName.find(absPath);
    if (it != m_pathToName.end()) {
        // Already loaded, return cached component
        return m_components[it->second].get();
    }

    // Check for circular dependency
    if (isCircularDependency(absPath)) {
        std::cerr << "Circular dependency detected: " << absPath << std::endl;
        return nullptr;
    }

    // Mark as loading
    m_loading.insert(absPath);

    // Read file
    auto sourceOpt = FileSystem::readFile(absPath);
    if (!sourceOpt.has_value()) {
        std::cerr << "Failed to read file: " << absPath << std::endl;
        m_loading.erase(absPath);
        return nullptr;
    }
    std::string source = sourceOpt.value();

    // Lex
    Lexer lexer(source);
    std::vector<Token> tokens = lexer.tokenize();

    // Parse
    Parser parser(tokens);
    std::unique_ptr<Component> component;

    try {
        component = parser.parse();
    } catch (const ParseError& e) {
        std::cerr << "Parse error in " << absPath << ": " << e.what() << std::endl;
        m_loading.erase(absPath);
        return nullptr;
    }

    // Extract component name from file path
    std::string componentName = extractComponentName(absPath);

    // Load all imports for this component
    std::string baseDir = std::filesystem::path(absPath).parent_path().string();
    loadImports(component.get(), baseDir);

    // Cache component
    Component* componentPtr = component.get();
    m_components[componentName] = std::move(component);
    m_pathToName[absPath] = componentName;

    // Done loading
    m_loading.erase(absPath);

    return componentPtr;
}

Component* ComponentRegistry::getComponent(const std::string& componentName) const {
    auto it = m_components.find(componentName);
    if (it != m_components.end()) {
        return it->second.get();
    }
    return nullptr;
}

bool ComponentRegistry::hasComponent(const std::string& componentName) const {
    return m_components.find(componentName) != m_components.end();
}

void ComponentRegistry::loadImports(Component* component, const std::string& baseDir) {
    if (!component) {
        return;
    }

    // Load all named imports: use { Card, Button } from "./components"
    for (const auto& import : component->imports) {
        if (import->type == ASTNodeType::NAMED_IMPORT) {
            NamedImport* namedImport = static_cast<NamedImport*>(import.get());

            // Resolve import path relative to baseDir
            std::string importPath = resolvePath(namedImport->source, baseDir);

            // Check if path is a directory or file
            std::filesystem::path fsPath(importPath);

            if (std::filesystem::is_directory(fsPath)) {
                // Import from directory: use { Card } from "./components"
                // Load each named component from the directory
                for (const auto& name : namedImport->names) {
                    std::string componentFile = (fsPath / (name + ".atc")).string();

                    if (std::filesystem::exists(componentFile)) {
                        loadComponent(componentFile);
                    } else {
                        std::cerr << "Warning: Component file not found: " << componentFile << std::endl;
                    }
                }
            } else {
                // Import from specific file: use { Card } from "./Card.atc"
                // Ensure .atc extension
                if (!fsPath.has_extension()) {
                    importPath += ".atc";
                }

                if (std::filesystem::exists(importPath)) {
                    loadComponent(importPath);
                }
            }
        }
        // Note: ThemeImport (use theme "path") would be handled separately
        // For now, we focus on component imports
    }
}

std::string ComponentRegistry::resolvePath(const std::string& path, const std::string& baseDir) {
    std::filesystem::path fsPath(path);

    // If path is relative, resolve relative to baseDir
    if (fsPath.is_relative()) {
        fsPath = std::filesystem::path(baseDir) / fsPath;
    }

    try {
        return std::filesystem::absolute(fsPath).string();
    } catch (const std::exception& e) {
        std::cerr << "Error resolving path: " << e.what() << std::endl;
        return path;
    }
}

std::string ComponentRegistry::extractComponentName(const std::string& filePath) {
    std::filesystem::path fsPath(filePath);

    // Get filename without extension
    std::string filename = fsPath.stem().string();

    return filename;
}

bool ComponentRegistry::isCircularDependency(const std::string& filePath) const {
    return m_loading.find(filePath) != m_loading.end();
}

} // namespace artic
