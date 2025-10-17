#include "Buffer.hpp"

Buffer::Buffer() : content("") {}

void Buffer::insert(const std::string& text, size_t position) {
    if (position <= content.length()) {
        content.insert(position, text);
    }
}

void Buffer::remove(size_t start, size_t count) {
    if (start < content.length()) {
        content.erase(start, count);
    }
}

std::string Buffer::getText(size_t start, size_t count) const {
    if (start < content.length()) {
        return content.substr(start, count);
    }
    return "";
}

const std::string& Buffer::getFullText() const {
    return content;
}

size_t Buffer::getLength() const {
    return content.length();
}