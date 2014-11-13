/*
 * Secret santa generator.
 * This utility gets list of users, shuffles them and for each user generates pair to be gifted.
 * Note: software considers love relations between users :) If some user A loves user B, he won't be able
 * to gift his lover. But be careful. Relations such as Swedish family is not supported yet :)
 *
 * Usage: ./newyear_roulette users-file
 *
 * Author: I.Karbachinsky <igorkarbachinsky@mail.ru>
 */

#include <iostream>
#include <memory>
#include <stdexcept>

#include "lib/user.hpp"
#include "lib/users_file_parser.hpp"
#include "lib/roulette.hpp"

using namespace roulette;
using namespace std;

int main(int argc, char *argv[]) {
    try {
        if (argc <= 1)
            throw runtime_error("Specify file with users!");

        string users_file(argv[1]);
        user_pointers_list_t users;

        users = UsersFileParser::parse_file(users_file);

        if (users.size() % 2 != 0)
            throw std::runtime_error("We have odd number of users!");

        Roulette roulette(users);

        roulette.gen_gift_relations();
        roulette.display_gift_relations();
        roulette.create_mail_files();
    }
    catch(runtime_error &e) {
        cerr << e.what() << endl;
        return 1;
    }

    return 0;
}