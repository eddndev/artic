#pragma once

#include "ASTNode.h"
#include <string>

namespace artic {

/**
 * @brief Base class for decorator nodes (@route, @layout, etc.)
 */
class Decorator : public ASTNode {
public:
    explicit Decorator(ASTNodeType type, SourceLocation location = SourceLocation())
        : ASTNode(type, location) {}

    virtual ~Decorator() = default;
};

/**
 * @brief @route("/path") decorator
 */
class RouteDecorator : public Decorator {
public:
    std::string path;

    explicit RouteDecorator(const std::string& path, SourceLocation location = SourceLocation())
        : Decorator(ASTNodeType::ROUTE_DECORATOR, location), path(path) {}

    std::string toString() const override {
        return "@route(\"" + path + "\")";
    }
};

/**
 * @brief @layout("name") decorator
 */
class LayoutDecorator : public Decorator {
public:
    std::string layoutName;

    explicit LayoutDecorator(const std::string& layoutName, SourceLocation location = SourceLocation())
        : Decorator(ASTNodeType::LAYOUT_DECORATOR, location), layoutName(layoutName) {}

    std::string toString() const override {
        return "@layout(\"" + layoutName + "\")";
    }
};

/**
 * @brief @utility decorator (marks utility declaration)
 */
class UtilityDecorator : public Decorator {
public:
    explicit UtilityDecorator(SourceLocation location = SourceLocation())
        : Decorator(ASTNodeType::UTILITY_DECORATOR, location) {}

    std::string toString() const override {
        return "@utility";
    }
};

} // namespace artic
