/*
 * Roulette class
 *
 * Copyright 2014 I.Karbachinsky <igorkarbachinsky@mail.ru>
 */

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
    explicit Roulette(user_pointers_list_t & users) : users_(users)
            {}

    /**
    *  Generates gift relations
    */
    void GenGiftRelations();

    /**
    *  Display gift relations to stdout
    */
    void DisplayGiftRelations() const;

    /**
     *  Create directory with mail files for each user
     */
    void CreateMailFiles() const;
private:
    user_pointers_list_t users_;
};

void Roulette::CreateMailFiles() const {
    if(!boost::filesystem::create_directory(kUsersFilesDir.c_str())) {
        std::cerr << "Failed to create directory. Maybe it' already exists? Continue running..." << std::endl;
    }

    // For each user creating file with his email
    for (auto i = users_.begin(); i != users_.end(); ++i) {
        const std::string user_file_name = kUsersFilesDir + (*i)->GetEmail();
        std::ofstream f(user_file_name);

        const boost::format mail = boost::format(kMailPattern)
                % (*i)->GetName()
                % (*i)->GetUserToBeGifted().GetName();

        f << mail;

        f.close();
    }
}

void Roulette::DisplayGiftRelations() const {
    for (auto i = users_.begin(); i != users_.end(); ++i) {
        std::cout << "User "
                  << (*i)->GetName()
                  << " will give a gift to "
                  << (*i)->GetUserToBeGifted().GetName()
                  << std::endl;
    }
}


void Roulette::GenGiftRelations() {
    // Randomly shuffling all the users
    size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(users_.begin(), users_.end(), std::default_random_engine(seed));

#ifdef DEBUG
    std::cout << "Shuffled: ";
    for (auto i=users_.begin(); i != users_.end(); ++i) {
        std::cout << (*i)->GetName()  << " " ;
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
                if ((*j)->CanBeGiftedBy(**i)) {
                    (*i)->SetUserToBeGifted(*j);
                    found = true;
                    break;
                }
            }

            start = users_.begin();
            end = i;
        } // while found

        if (!found) {
            throw std::runtime_error(
                "We've got some problems. Please check that you have more then 2 persons "
                "in your file and all the emails are coorect and different and try again!"
            );
        }
    }
}


} // namespace roulette

#endif // NEWYEAR_ROULETTE_LIB_ROULETTE_HPP_