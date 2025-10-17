#ifndef BUFFER_HPP
#define BUFFER_HPP

#include <string>
#include <sys/types.h>

class Buffer {
private:
    std::string content;

public:
    Buffer();
    std::string getText(size_t start, size_t count) const;
    void insert(const std::string& text, size_t position);
    void remove(size_t start, size_t count);
    const std::string& getFullText() const;
    size_t getLength() const;
};

#endif //BUFFER_HPP
