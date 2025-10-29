#pragma once

#include <string>
#include <unordered_map>
#include <vector>

namespace artic {

/**
 * @brief Resolves Tailwind-like utility tokens to CSS properties
 *
 * Examples:
 *   px:4 → padding-left: 1rem; padding-right: 1rem;
 *   bg:blue-500 → background-color: #3b82f6;
 *   text:lg → font-size: 1.125rem; line-height: 1.75rem;
 */
class UtilityResolver {
public:
    UtilityResolver();

    /**
     * @brief Resolve a utility token to CSS declaration(s)
     * @param property Property name (px, bg, text, etc.)
     * @param value Value (4, blue-500, lg, etc.)
     * @return Vector of CSS declarations (property: value;)
     */
    std::vector<std::string> resolve(const std::string& property, const std::string& value);

    /**
     * @brief Check if property is valid
     */
    bool isValidProperty(const std::string& property) const;

    /**
     * @brief Get CSS property name(s) for a utility property
     */
    std::vector<std::string> getCssProperties(const std::string& property) const;

private:
    // Spacing values (px, py, p, m, etc.)
    std::unordered_map<std::string, std::string> m_spacing;

    // Color palette
    std::unordered_map<std::string, std::string> m_colors;

    // Font sizes
    std::unordered_map<std::string, std::pair<std::string, std::string>> m_fontSizes;

    // Property mappings
    std::unordered_map<std::string, std::vector<std::string>> m_propertyMap;

    /**
     * @brief Initialize spacing scale (0-96)
     */
    void initSpacing();

    /**
     * @brief Initialize color palette
     */
    void initColors();

    /**
     * @brief Initialize font sizes
     */
    void initFontSizes();

    /**
     * @brief Initialize property mappings
     */
    void initPropertyMap();

    /**
     * @brief Resolve spacing value (4 → 1rem)
     */
    std::string resolveSpacing(const std::string& value);

    /**
     * @brief Resolve color value (blue-500 → #3b82f6)
     */
    std::string resolveColor(const std::string& value);

    /**
     * @brief Resolve font size (lg → 1.125rem + line-height)
     */
    std::pair<std::string, std::string> resolveFontSize(const std::string& value);
};

} // namespace artic
