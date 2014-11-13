#ifndef NEWYEAR_ROULETTE_LIB_USER_HPP_
#define NEWYEAR_ROULETTE_LIB_USER_HPP_

#include <memory>
#include <vector>

namespace roulette {

class User;

typedef std::vector<std::shared_ptr<User> > user_pointers_list_t;

class User {
public:
    User(const std::string &name, const std::string &email) :
            name_(name),
            email_(email),
            is_busy_(false)
    {}

    /**
    * Checks whether user is already has user to be gifted
    */
    const bool is_busy() const {
        return is_busy_;
    }

    /**
    * Set user busy (this user is already gifted by someone)
    */
    const bool set_busy() const {
        is_busy_ = true;
    }

    /**
    * Checks if input user can be gifted by this user
    */
    bool can_be_gifted_by(const User &user) const {
        if (is_busy())
            return false;
        if (user == *this)
            return false;
        if (loved_user_ != nullptr && user == *loved_user_)
            return false;
        return true;
    }

    /**
    * Updates user to be gifted
    */
    void set_user_to_be_gifted(const std::shared_ptr<const User> user_ptr) {
        user_to_be_gifted_ = user_ptr;
        user_ptr->set_busy();
    }

    /**
    * set love relation for user
    */
    void set_loved(const std::shared_ptr<const User> user_ptr) {
        loved_user_ = user_ptr;
    }

    /**
    * Get user's name
    */
    const std::string get_name() const {
        return name_;
    }

    /**
    * Get user's email
    */
    const std::string get_email() const {
        return email_;
    }

    /**
    * Get user which will be gifted the gift by current user
    */
    const User get_user_to_be_gifted() const {
        auto user_ptr = user_to_be_gifted_.lock();
        return *user_ptr;
    }

    bool operator==(const User &other) const {
        return other.email_ == email_;
    }

private:
    const std::string name_;
    const std::string email_;
    mutable bool is_busy_;
    std::weak_ptr<const User> user_to_be_gifted_;
    std::shared_ptr<const User> loved_user_;
};

} // namespace roulette

#endif // NEWYEAR_ROULETTE_LIB_USER_HPP_