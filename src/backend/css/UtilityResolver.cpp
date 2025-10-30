#include "backend/css/UtilityResolver.h"
#include <sstream>

namespace artic {

UtilityResolver::UtilityResolver() {
    initSpacing();
    initColors();
    initFontSizes();
    initPropertyMap();
}

void UtilityResolver::initSpacing() {
    // Tailwind spacing scale (0-96)
    m_spacing["0"] = "0";
    m_spacing["1"] = "0.25rem";
    m_spacing["2"] = "0.5rem";
    m_spacing["3"] = "0.75rem";
    m_spacing["4"] = "1rem";
    m_spacing["5"] = "1.25rem";
    m_spacing["6"] = "1.5rem";
    m_spacing["8"] = "2rem";
    m_spacing["10"] = "2.5rem";
    m_spacing["12"] = "3rem";
    m_spacing["16"] = "4rem";
    m_spacing["20"] = "5rem";
    m_spacing["24"] = "6rem";
    m_spacing["32"] = "8rem";
    m_spacing["40"] = "10rem";
    m_spacing["48"] = "12rem";
    m_spacing["56"] = "14rem";
    m_spacing["64"] = "16rem";
}

void UtilityResolver::initColors() {
    // Blue palette
    m_colors["blue-50"] = "#eff6ff";
    m_colors["blue-100"] = "#dbeafe";
    m_colors["blue-200"] = "#bfdbfe";
    m_colors["blue-300"] = "#93c5fd";
    m_colors["blue-400"] = "#60a5fa";
    m_colors["blue-500"] = "#3b82f6";
    m_colors["blue-600"] = "#2563eb";
    m_colors["blue-700"] = "#1d4ed8";
    m_colors["blue-800"] = "#1e40af";
    m_colors["blue-900"] = "#1e3a8a";

    // Gray palette
    m_colors["gray-50"] = "#f9fafb";
    m_colors["gray-100"] = "#f3f4f6";
    m_colors["gray-200"] = "#e5e7eb";
    m_colors["gray-300"] = "#d1d5db";
    m_colors["gray-400"] = "#9ca3af";
    m_colors["gray-500"] = "#6b7280";
    m_colors["gray-600"] = "#4b5563";
    m_colors["gray-700"] = "#374151";
    m_colors["gray-800"] = "#1f2937";
    m_colors["gray-900"] = "#111827";

    // Red palette
    m_colors["red-500"] = "#ef4444";
    m_colors["red-600"] = "#dc2626";
    m_colors["red-700"] = "#b91c1c";

    // Green palette
    m_colors["green-500"] = "#22c55e";
    m_colors["green-600"] = "#16a34a";
    m_colors["green-700"] = "#15803d";

    // Common colors
    m_colors["white"] = "#ffffff";
    m_colors["black"] = "#000000";
    m_colors["transparent"] = "transparent";
}

void UtilityResolver::initFontSizes() {
    // Font size + line height
    m_fontSizes["xs"] = {"0.75rem", "1rem"};
    m_fontSizes["sm"] = {"0.875rem", "1.25rem"};
    m_fontSizes["base"] = {"1rem", "1.5rem"};
    m_fontSizes["lg"] = {"1.125rem", "1.75rem"};
    m_fontSizes["xl"] = {"1.25rem", "1.75rem"};
    m_fontSizes["2xl"] = {"1.5rem", "2rem"};
    m_fontSizes["3xl"] = {"1.875rem", "2.25rem"};
    m_fontSizes["4xl"] = {"2.25rem", "2.5rem"};
    m_fontSizes["5xl"] = {"3rem", "1"};
    m_fontSizes["6xl"] = {"3.75rem", "1"};
}

void UtilityResolver::initPropertyMap() {
    // Padding
    m_propertyMap["p"] = {"padding"};
    m_propertyMap["px"] = {"padding-left", "padding-right"};
    m_propertyMap["py"] = {"padding-top", "padding-bottom"};
    m_propertyMap["pt"] = {"padding-top"};
    m_propertyMap["pr"] = {"padding-right"};
    m_propertyMap["pb"] = {"padding-bottom"};
    m_propertyMap["pl"] = {"padding-left"};

    // Margin
    m_propertyMap["m"] = {"margin"};
    m_propertyMap["mx"] = {"margin-left", "margin-right"};
    m_propertyMap["my"] = {"margin-top", "margin-bottom"};
    m_propertyMap["mt"] = {"margin-top"};
    m_propertyMap["mr"] = {"margin-right"};
    m_propertyMap["mb"] = {"margin-bottom"};
    m_propertyMap["ml"] = {"margin-left"};

    // Width & Height
    m_propertyMap["w"] = {"width"};
    m_propertyMap["h"] = {"height"};

    // Background
    m_propertyMap["bg"] = {"background-color"};

    // Text
    m_propertyMap["text"] = {"color"}; // text:blue-500 → color
    m_propertyMap["font"] = {"font-weight"}; // font:bold → font-weight

    // Flex
    m_propertyMap["flex"] = {"display"};
    m_propertyMap["justify"] = {"justify-content"};
    m_propertyMap["items"] = {"align-items"};
    m_propertyMap["gap"] = {"gap"};

    // Border
    m_propertyMap["rounded"] = {"border-radius"};
    m_propertyMap["border"] = {"border-width"};

    // Display
    m_propertyMap["grid"] = {"display"};
    m_propertyMap["block"] = {"display"};
    m_propertyMap["inline"] = {"display"};
}

std::vector<std::string> UtilityResolver::resolve(const std::string& property, const std::string& value) {
    std::vector<std::string> declarations;

    auto cssProps = getCssProperties(property);
    if (cssProps.empty()) {
        return declarations; // Unknown property
    }

    // Special cases
    if (property == "w" || property == "h") {
        // Width/height special values
        if (value == "full") {
            for (const auto& cssProp : cssProps) {
                declarations.push_back(cssProp + ": 100%");
            }
        } else if (value == "screen") {
            for (const auto& cssProp : cssProps) {
                declarations.push_back(cssProp + ": 100vw");
            }
        } else {
            std::string resolved = resolveSpacing(value);
            for (const auto& cssProp : cssProps) {
                declarations.push_back(cssProp + ": " + resolved);
            }
        }
        return declarations;
    }

    if (property == "flex") {
        declarations.push_back("display: flex");
        return declarations;
    }

    if (property == "grid") {
        declarations.push_back("display: grid");
        return declarations;
    }

    // Check text:size BEFORE text:color (font-size takes priority)
    if (property == "text" && m_fontSizes.count(value)) {
        // Font size
        auto [fontSize, lineHeight] = resolveFontSize(value);
        declarations.push_back("font-size: " + fontSize);
        declarations.push_back("line-height: " + lineHeight);
        return declarations;
    }

    if (property == "bg" || property == "text") {
        // Color properties
        std::string resolved = resolveColor(value);
        for (const auto& cssProp : cssProps) {
            declarations.push_back(cssProp + ": " + resolved);
        }
        return declarations;
    }

    if (property == "font") {
        // Font weight
        if (value == "bold") {
            declarations.push_back("font-weight: 700");
        } else if (value == "semibold") {
            declarations.push_back("font-weight: 600");
        } else if (value == "medium") {
            declarations.push_back("font-weight: 500");
        } else if (value == "normal") {
            declarations.push_back("font-weight: 400");
        }
        return declarations;
    }

    if (property == "rounded") {
        // Border radius
        if (value == "none") {
            declarations.push_back("border-radius: 0");
        } else if (value == "sm") {
            declarations.push_back("border-radius: 0.125rem");
        } else if (value == "md" || value.empty()) {
            declarations.push_back("border-radius: 0.375rem");
        } else if (value == "lg") {
            declarations.push_back("border-radius: 0.5rem");
        } else if (value == "xl") {
            declarations.push_back("border-radius: 0.75rem");
        } else if (value == "full") {
            declarations.push_back("border-radius: 9999px");
        }
        return declarations;
    }

    // Default: spacing properties (padding, margin, gap)
    std::string resolved = resolveSpacing(value);
    for (const auto& cssProp : cssProps) {
        declarations.push_back(cssProp + ": " + resolved);
    }

    return declarations;
}

bool UtilityResolver::isValidProperty(const std::string& property) const {
    return m_propertyMap.count(property) > 0;
}

std::vector<std::string> UtilityResolver::getCssProperties(const std::string& property) const {
    auto it = m_propertyMap.find(property);
    if (it != m_propertyMap.end()) {
        return it->second;
    }
    return {};
}

std::string UtilityResolver::resolveSpacing(const std::string& value) {
    auto it = m_spacing.find(value);
    if (it != m_spacing.end()) {
        return it->second;
    }
    return value; // Return as-is if not found
}

std::string UtilityResolver::resolveColor(const std::string& value) {
    auto it = m_colors.find(value);
    if (it != m_colors.end()) {
        return it->second;
    }
    return value; // Return as-is if not found
}

std::pair<std::string, std::string> UtilityResolver::resolveFontSize(const std::string& value) {
    auto it = m_fontSizes.find(value);
    if (it != m_fontSizes.end()) {
        return it->second;
    }
    return {"1rem", "1.5rem"}; // Default
}

} // namespace artic
