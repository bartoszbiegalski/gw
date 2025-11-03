#pragma once

#include "core/types.h"

/// Base

class BaseException : public std::runtime_error
{
public:
    explicit BaseException(const std::string &msg)
        : std::runtime_error(msg) {}
};

class FileNotFoundException : public BaseException
{
public:
    explicit FileNotFoundException(const std::string &file)
        : BaseException("File not found: " + file) {}
};

/// GML File

class WrongExtensionException : public BaseException
{
public:
    explicit WrongExtensionException(const std::string &file)
        : BaseException("Wrong file extension: " + file) {}
};

class EmptyFileException : public BaseException
{
public:
    explicit EmptyFileException(const std::string &file)
        : BaseException("File is empty: " + file) {}
};

class FileFormatException : public BaseException
{
public:
    explicit FileFormatException(const std::string &msg)
        : BaseException("Wrong file format: " + msg) {}
};

// GML structure

class WrongRootElement : public BaseException
{
public:
    explicit WrongRootElement(const std::string &msg)
        : BaseException("Wrong class of root element; expected: " + msg) {}
};

class MissingRequiredttribute : public BaseException
{
public:
    explicit MissingRequiredttribute(const std::string &msg)
        : BaseException("Missing required attribute: " + msg) {}
};

// XML Parse

class XmlParseException : public BaseException
{
public:
    explicit XmlParseException(const std::string &msg)
        : BaseException("File " + msg + " is not valid XML") {}
};

// XSD Parse

class WrongXsdFileException : public BaseException
{
public:
    explicit WrongXsdFileException(const std::string &msg)
        : BaseException("File " + msg + " is not valid XSD file") {}
};

class XsdWrongContextException : public BaseException
{
public:
    explicit XsdWrongContextException(const std::string &msg)
        : BaseException("Cannot create XSD context from: " + msg) {}
};

class XsdParseException : public BaseException
{
public:
    explicit XsdParseException(const std::string &msg)
        : BaseException("File " + msg + " is not valid XSD") {}
};
