#ifndef NEWYEAR_ROULETTE_LIB_USERS_FILE_PARSER_HPP_
#define NEWYEAR_ROULETTE_LIB_USERS_FILE_PARSER_HPP_

#include <iostream>
#include <fstream>
#include <memory>
#include <sstream>
#include <stdexcept>

#include "user.hpp"


namespace roulette {

class UsersFileParser {
public:
    /**
    *  Parses input file with clusters of users
    */
    static user_pointers_list_t parse_file(const std::string & file_name);
};

user_pointers_list_t UsersFileParser::parse_file(const std::string & file_name) {
    // Getting users from file
    std::ifstream f(file_name);
    std::string tmp_email, tmp_name;
    user_pointers_list_t users;
    auto prev_user = std::shared_ptr<User>(nullptr);

    if (!f.is_open())
        throw std::runtime_error("Failed to open users file!");

    std::string line;
    while (std::getline(f, line)) {
        if ( line.empty() ) {
            prev_user = nullptr;
            continue;
        }

        std::istringstream buf(line);
        buf >> tmp_name >> tmp_email;

        User user(tmp_name, tmp_email);

        std::shared_ptr<User> user_ptr = std::make_shared<User>(user);

        if (prev_user) {
            user_ptr->set_loved(prev_user);
            prev_user->set_loved(user_ptr);
        }

        users.push_back(user_ptr);
        prev_user = user_ptr;
    }

    f.close();

    return users;
}

} // namespace roulette

#endif // NEWYEAR_ROULETTE_LIB_USERS_FILE_PARSER_HPP_