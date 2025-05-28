#pragma once
#include <exception>
#include <string>

//-----------------------------------------------------------------------------
// Custom Exception Class
//-----------------------------------------------------------------------------
class EmptyQueueException : public std::exception {
public:
    EmptyQueueException() : message("Cannot poll from an empty priority queue") {}

    const char* what() const noexcept override {
        return message.c_str();
    }

private:
    std::string message;
};