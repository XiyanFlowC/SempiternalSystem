#include "TypeParser.h"

int TypeParser::string2int(const char* p_input)
{
    int ret = 0;
    const char* loc = p_input;
    while (*loc != '\0' && isdigit(*loc))
    {
        ret *= 10;
        ret += *loc++ - '0';
    }
    if (*loc != '\0') throw new FormatIncorrectException(__FILE__, _STR_LINE_);//UNDONE: throw a exception!
    return ret;
}

FormatIncorrectException::FormatIncorrectException() : message("FormatIncorrectException") {}

FormatIncorrectException::FormatIncorrectException(std::string p_message) : message("FormatIncorrectException: ")
{
    message = p_message; 
}

FormatIncorrectException::FormatIncorrectException(std::string p_file, std::string p_line) : message("FormatIncorrectException:\n at file: ")
{
    message += p_file + " : " + p_line;
}
