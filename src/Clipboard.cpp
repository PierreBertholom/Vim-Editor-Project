#include "Clipboard.hpp"

Clipboard::Clipboard() = default;

Clipboard& Clipboard::getInstance() {
    static Clipboard instance;
    return instance;
}

void Clipboard::setContent(const std::string &text) {
    this->content = text;
}

const std::string& Clipboard::getContent() const {
    return this->content;
}


