/* ************************************************************************** */
/*                                                                            */
/*                                                        :::      ::::::::   */
/*   UtilsParser.cpp                                    :+:      :+:    :+:   */
/*                                                    +:+ +:+         +:+     */
/*   By: ael-qori <ael-qori@student.1337.ma>        +#+  +:+       +#+        */
/*                                                +#+#+#+#+#+   +#+           */
/*   Created: 2024/12/28 13:44:03 by ael-qori          #+#    #+#             */
/*   Updated: 2025/01/06 14:45:24 by ael-qori         ###   ########.fr       */
/*                                                                            */
/* ************************************************************************** */

#include "../includes/UtlisParser.hpp"


std::string itoa(size_t n)
{
    std::ostringstream oss;

    oss << n;
    return oss.str();
}

std::string trim(const std::string& str) {
    size_t start = str.find_first_not_of(" \t\n\r\f\v");
    if (start == std::string::npos) return "";
    size_t end = str.find_last_not_of(" \t\n\r\f\v");
    return str.substr(start, end - start + 1);
}


std::vector<std::string> splitString(const std::string& input, const std::string& delimiters)
{
    std::vector<std::string> tokens;
    std::string token;
    size_t start = 0;
    
    for (size_t i = 0; i < input.length(); ++i) {
        if (delimiters.find(input[i]) != std::string::npos) {
            if (!token.empty()) {
                tokens.push_back(token);
                token.clear();
            }
        }
        else token += input[i];
    }
    if (!token.empty()) tokens.push_back(token);
    return tokens;
}
void    Error(int count, ...)
{
    std::string     error;
    int             i = -1;
    va_list         args;
    char    *tmp;
    
    va_start(args, count);
    tmp = (char *)va_arg(args, char *);
    if (tmp == NULL) return ;
    error += BOLDRED + std::string(tmp) + RESET;
    while (++i < (count - 1))
    {
        tmp = (char *)va_arg(args, char *);
        if (tmp == NULL) return ;
        error += " " + std::string(tmp);
    }
    throw std::runtime_error(error);
}

bool is_number(std::string str, int index)
{
    while (str[++index]) if (!isdigit(str[index])) return false;
    return true;
}

bool is_ipaddress(std::string str, int index)
{
    std::vector<std::string>    Array;
    int                         countDots = 0;
    int                         byte;

    while (str[++index]) if (str[index] == '.') countDots++;
    if (countDots != 3) return false;
    Array = splitString(str, ".");
    if (Array.size() != 4 && ((index = INDEX) || true)) return false;
    while (++index < Array.size() && ((byte = atoi(Array[index].c_str()) )|| true)) if(Array[index] != itoa(byte) || byte < 0 || byte > 255) return false;
    return true;
}

bool is_valid_server_name(std::string str, int index)
{
    while (++index < str.size())
    {
        if (str[index] == '-' && (index == 0 || index == str.size() - 1)) return false;
        if (str[index] != '-' && (isalpha(str[index]) == false && isdigit(str[index]) == false)) return false;
    }
    return true;
}

bool is_hostname(std::string str, int index)
{
    std::vector<std::string>    Array;
    int                         checkDots;

    while (str[++index])if(str[index] == '.' && (index == 0 || index == str.size() - 1 || str[index - 1] == '.' || str[index + 1] == '.')) return false;
    Array = splitString(str, ".");
    index = INDEX;
    while (++index < Array.size())if (is_valid_server_name(Array[index], INDEX) == false) return false;
    return true;
}

bool is_valid_size(std::string str, int index)
{
    std::vector<std::string>    Array;
    std::string                 tmp;
    
    while (str[++index]) if (isalnum(str[index]) == false) return false;
    tmp = str[index - 1];    
    str[index - 1] = ',';
    str += tmp;
    Array = splitString(str, ",");
    if (Array.size() != 2) return false;
    if ((Array[0] != itoa(atoi(Array[0].c_str()))) || (Array[1] != "K" && Array[1] != "M")) return false;
    return true;
}

bool is_statuscode(std::string str, int index)
{
    int                         status;

    status = atoi(str.c_str());
    if (itoa(status) != str) return false;
    if (status < 100 || status > 999) return false;
    return true;
}

bool is_duplicated(std::vector<std::string> vec, std::string v, int index)
{
    while (++index < vec.size()) if (vec[index] == v) return false;
    return true;
}