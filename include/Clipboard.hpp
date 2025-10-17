#ifndef CLIPBOARD_HPP
#define CLIPBOARD_HPP

#include <string>

class Clipboard {
private:
    std::string content;
    // bloquer instanciation directe
    Clipboard();

public:
    Clipboard(const Clipboard&) = delete;
    Clipboard& operator=(const Clipboard&) = delete;

    static Clipboard& getInstance();
    void setContent(const std::string& text);
    const std::string& getContent() const;
};

#endif // CLIPBOARD_HPP