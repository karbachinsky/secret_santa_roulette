#ifndef NEWYEAR_ROULETTE_LIB_ROULETTE_HPP_
#define NEWYEAR_ROULETTE_LIB_ROULETTE_HPP_

#include <iostream>
#include <memory>
#include <chrono>

#include "boost/format.hpp"
#include "boost/filesystem.hpp"

#include "user.hpp"


namespace roulette {

const std::string kMailPattern =
        "Привет, %1%!\n"
        "\n"
        "Тебя приветствует минский дед мороз!\n"
        "В этот знаменательный день тебе повезло стать секретным "
        "санта-клаусом и осчастливить твоего друга по имени %2%.\n"
        "\n"
        "Сделай его праздник незабываемым!\n"
        "\n"
        "PS: Кстати, раскрывать себя не обязательно.\n";

const std::string kUsersFilesDir = "users/";

class Roulette {
public:
    Roulette(user_pointers_list_t & users) : users_(users)
            {}

    /**
    *  Generates gift relations
    */
    void gen_gift_relations();

    /**
    *  Display gift relations to stdout
    */
    void display_gift_relations() const;

    /**
     *  Create directory with mail files for each user
     */
    void create_mail_files() const;
private:
    user_pointers_list_t users_;
};

void Roulette::create_mail_files() const {
    using namespace std;

    if(!boost::filesystem::create_directory(kUsersFilesDir.c_str())) {
        std::cerr << "Failed to create directory. Maybe it' already exists? Continue running..." << std::endl;
    }

    // For each user creating file with his email
    for (auto i = users_.begin(); i != users_.end(); ++i) {
        const string user_file_name = kUsersFilesDir + (*i)->get_email();
        ofstream f(user_file_name);

        const boost::format mail = boost::format(kMailPattern)
                % (*i)->get_name()
                % (*i)->get_user_to_be_gifted().get_name();

        f << mail;

        f.close();
    }
}

void Roulette::display_gift_relations() const {
    for (auto i = users_.begin(); i != users_.end(); ++i) {
        std::cout << "User "
                  << (*i)->get_name()
                  << " will give a gift to "
                  << (*i)->get_user_to_be_gifted().get_name()
                  << std::endl;
    }
}


void Roulette::gen_gift_relations() {
    // Randomly shuffling all the users
    size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(users_.begin(), users_.end(), std::default_random_engine(seed));

#ifdef DEBUG
    std::cout << "Shuffled: ";
    for (auto i=users_.begin(); i != users_.end(); ++i) {
        std::cout << (*i)->get_name()  << " " ;
    }
    std::cout << std::endl << std::endl;
#endif

    // Iteratively generating gifting pairs
    for (auto i=users_.begin(); i != users_.end(); ++i) {
        bool found = false;
        auto start = i+1;
        auto end = users_.end();

        size_t num_iterations = 0;
        while(!found && num_iterations++ < 2 ) {
            for (auto j = start; j != end; ++j) {
                if ((*j)->can_be_gifted_by(**i)) {
                    (*i)->set_user_to_be_gifted(*j);
                    found = true;
                    break;
                }
            }

            start = users_.begin();
            end = i;
        } // while found

        if (!found) {
            throw std::runtime_error("We've got some problems. Please try again!");
        }
    }
}


} // namespace roulette

#endif // NEWYEAR_ROULETTE_LIB_ROULETTE_HPP_