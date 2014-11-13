/*
 * Secret santa generator.
 * This utility gets list of users, shuffles them and for each user generates pair to be gifted.
 * Note: software considers love relations between users :) If some user A loves user B, he won't be able
 * to gift his lover. But be careful. Relations such as Swedish family is not supported yet :)
 *
 * Usage: ./newyear_roulette users-file
 *
 * Copyright 2014 I.Karbachinsky <igorkarbachinsky@mail.ru>
 */

#include <iostream>
#include <memory>
#include <stdexcept>
#include <string>

#include "lib/user.hpp"
#include "lib/users_file_parser.hpp"
#include "lib/roulette.hpp"


namespace rou = roulette;


int main(int argc, char *argv[]) {
    try {
        if (argc <= 1)
            throw std::runtime_error("Specify file with users!");

        std::string users_file(argv[1]);
        rou::user_pointers_list_t users;

        users = rou::UsersFileParser::ParseFile(users_file);

        if (users.size() % 2 != 0)
            throw std::runtime_error("We have odd number of users!");

        rou::Roulette roulette(users);

        roulette.GenGiftRelations();
        roulette.DisplayGiftRelations();
        roulette.CreateMailFiles();
    }
    catch(std::runtime_error &e) {
        std::cerr << e.what() << std::endl;
        return 1;
    }

    return 0;
}
