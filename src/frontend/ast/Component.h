#pragma once

#include "ASTNode.h"
#include "Decorator.h"
#include "Import.h"
#include "Props.h"
#include "Utility.h"
#include "Template.h"
#include <vector>
#include <memory>
#include <string>

namespace artic {

/**
 * @brief Root AST node representing a complete .atc file
 *
 * Structure (in order):
 * 1. Metadata (decorators)
 * 2. Imports
 * 3. Props (optional, only for components)
 * 4. Styles (utility declarations)
 * 5. Template (required, must be last)
 */
class Component : public ASTNode {
public:
    // Metadata
    std::unique_ptr<RouteDecorator> route;       // Optional @route
    std::unique_ptr<LayoutDecorator> layout;     // Optional @layout

    // Imports
    std::vector<std::unique_ptr<Import>> imports;

    // Props (optional, only for reusable components)
    std::unique_ptr<PropsBlock> props;

    // Styles
    std::vector<std::unique_ptr<UtilityDeclaration>> utilities;

    // Template (required)
    std::vector<std::unique_ptr<TemplateNode>> templateNodes;

    Component(SourceLocation location = SourceLocation())
        : ASTNode(ASTNodeType::COMPONENT, location) {}

    /**
     * @brief Add import
     */
    void addImport(std::unique_ptr<Import> import) {
        imports.push_back(std::move(import));
    }

    /**
     * @brief Add utility declaration
     */
    void addUtility(std::unique_ptr<UtilityDeclaration> utility) {
        utilities.push_back(std::move(utility));
    }

    /**
     * @brief Add template node
     */
    void addTemplateNode(std::unique_ptr<TemplateNode> node) {
        templateNodes.push_back(std::move(node));
    }

    /**
     * @brief Check if component has route (is a page)
     */
    bool hasRoute() const {
        return route != nullptr;
    }

    /**
     * @brief Check if component has layout
     */
    bool hasLayout() const {
        return layout != nullptr;
    }

    /**
     * @brief Get route path (empty if no route)
     */
    std::string getRoutePath() const {
        return route ? route->path : "";
    }

    /**
     * @brief Get layout name (empty if no layout)
     */
    std::string getLayoutName() const {
        return layout ? layout->layoutName : "";
    }

    std::string toString() const override {
        std::string result = "Component {\n";

        // Metadata
        if (route) {
            result += "  " + route->toString() + "\n";
        }
        if (layout) {
            result += "  " + layout->toString() + "\n";
        }

        // Imports
        if (!imports.empty()) {
            result += "\n  Imports:\n";
            for (const auto& imp : imports) {
                result += "    " + imp->toString() + "\n";
            }
        }

        // Utilities
        if (!utilities.empty()) {
            result += "\n  Utilities:\n";
            for (const auto& util : utilities) {
                result += "    " + util->toString() + "\n";
            }
        }

        // Template
        if (!templateNodes.empty()) {
            result += "\n  Template:\n";
            for (const auto& node : templateNodes) {
                result += "    " + node->toString() + "\n";
            }
        }

        result += "}";
        return result;
    }
};

} // namespace artic
