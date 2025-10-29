#pragma once

namespace artic {

/**
 * @brief Represents a location in the source code
 */
struct SourceLocation {
    int line;      // Line number (1-indexed)
    int column;    // Column number (1-indexed)
    int offset;    // Absolute character offset from start of file

    SourceLocation() : line(1), column(1), offset(0) {}

    SourceLocation(int line, int column, int offset)
        : line(line), column(column), offset(offset) {}

    /**
     * @brief Advance to next character
     */
    void advance(char c) {
        offset++;
        if (c == '\n') {
            line++;
            column = 1;
        } else {
            column++;
        }
    }
};

} // namespace artic
