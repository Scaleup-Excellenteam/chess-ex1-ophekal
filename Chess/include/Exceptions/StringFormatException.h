#pragma once
#include <exception>
#include <string>

//-----------------------------------------------------------------------------
// Custom Exception Class
//-----------------------------------------------------------------------------
class StringFormatException : public std::exception {
public:
    StringFormatException(const std::string& reason)
        : message("Invalid board string format: " + reason) {}

    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};