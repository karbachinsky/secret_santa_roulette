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
#include <algorithm>
#include <iterator>

#include "boost/format.hpp"
#include "boost/filesystem.hpp"

#include "user.hpp"


namespace roulette {

// FIXME
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
        std::cerr << "Failed to create directory" << kUsersFilesDir.c_str() << ". Maybe it already exists? Continue running..." << std::endl;
    }

    // For each user creating file with his email
    for (auto i = users_.begin(); i != users_.end(); ++i) {
        const std::string user_file_name = kUsersFilesDir + (*i)->GetEmail();
        std::ofstream f(user_file_name);

        const boost::format mail = boost::format(kMailPattern)
                % (*i)->GetName()
                % (*i)->GetUserToBeGifted()->GetName();

        f << mail;

        f.close();
    }
}

void Roulette::DisplayGiftRelations() const {
    for (auto i = users_.begin(); i != users_.end(); ++i) {
        std::cout << "User "
                  << (*i)->GetName()
                  << " will give a gift to "
                  << (*i)->GetUserToBeGifted()->GetName()
                  << std::endl;
    }
}


void Roulette::GenGiftRelations() {
    // Randomly shuffling all the users
    size_t seed = std::chrono::system_clock::now().time_since_epoch().count();
    std::shuffle(users_.begin(), users_.end(), std::default_random_engine(seed));

    size_t num_users = users_.size();
    size_t num_users_with_pairs = 0;

#ifdef DEBUG
    std::cout << "Shuffled: ";
    for (auto i=users_.begin(); i != users_.end(); ++i) {
        std::cout << (*i)->GetName()  << " " ;
    }
    std::cout << std::endl << std::endl;
#endif

    // Iteratively generating gifting pairs
    for (auto i=users_.begin(); i != users_.end(); ++i) {
        if ((*i)->GetUserToBeGifted())
            continue;
        bool found = false;
        auto start = i + 1;
        auto end = users_.end();

        size_t num_iterations = 0;
        while (!found && num_iterations++ < 2) {
            for (auto j = start; j != end; ++j) {
                if ((*j)->CanBeGiftedBy(*i)) {
                    (*i)->SetUserToBeGifted(*j);
                    found = true;
                    ++num_users_with_pairs;
                    break;
                }
            }

            start = users_.begin();
            end = i;
        } // while found

        if (!found) {
            throw std::runtime_error(
                    "We've got some problems. Please check that you have more then 2 persons "
                            "in your file and all the emails are correct and different and try again!"
            );
        }

        // Heuristic case when we have 4 users with 1 pair and two single persons.
        // To prevent situation with 2 users from one pair remained, two single persons
        // should choose persons from love pair. Of course we can randomly shuffle until
        // it will be ok, but it's not effective.
        if (4 == num_users - num_users_with_pairs) {
            user_pointers_list_t four_users;

            for (auto j = users_.begin(); j != users_.end(); ++j) {
                if (!(*j)->GetUserToBeGifted())
                    four_users.push_back(*j);
            }

            size_t num_love_relations = 0;
            std::vector<size_t> love_indexes;
            std::vector<size_t> single_indexes;
            for (auto j = four_users.begin(); j != four_users.end(); ++j) {
                for (auto k = j+1; k != four_users.end(); ++k) {
                    if ((*j)->GetLoved() == *k) {
                        love_indexes.push_back(std::distance(four_users.begin(), j));
                        love_indexes.push_back(std::distance(four_users.begin(), k));
                        ++num_love_relations;
                        continue;
                    }
                }
            }

            if (1 == num_love_relations) {
                size_t s = 0, love_index = 0, single_index = 0;
                for (auto j = four_users.begin(); j != four_users.end(); ++j) {
                    if (std::find(love_indexes.begin(), love_indexes.end(), s++) == love_indexes.end()) {
                        (*j)->SetUserToBeGifted(four_users[love_indexes[love_index]]);
                        ++love_index;
                    }
                }
            }
        } // check 4
    }
}


} // namespace roulette

#endif // NEWYEAR_ROULETTE_LIB_ROULETTE_HPP_
