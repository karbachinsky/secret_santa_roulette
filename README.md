# Secret Santa Roulette

Going to have a new year party with your friends? Secret Santa utility is that what you need.
This utility allows to generate gift relations for secret santa game.
Each of your friend will have a person to gift. Roulette allows to markdown love relations between friends in special file to prevent
uninteresting gift relations.

### How to build?
1. Download project from github.
2. To build the utility you have to check whether *cmake* and *boost* library are installed.
   (Correct *CMakeLists.txt* if your boost library is not in default */usr/local/boost/* and don't forget to build *boost.filesystem* library).
2. Go to the project directory and run *cmake*.
3. Run *make*.

```sh
$ cmake .
$ make
```
### How to run?
1. Construct friends file. You can see pattern in *etc/users.pattern* (Don't forget to sign correct emails).
2. Run *./newyear_roulette <PATH_TO_USERS_FILE>*.
```sh
./newyear_roulette ./etc/users.pattern
```
3. Checkout output and look for just created *users/* directory with pretty email files for each user.
4. Now you can send emails by hand or using utility *etc/send_mails.sh*. Just run this script specifying *users/* directory like this:
```sh
./etc/send_mails.sh users/
```
5. Enjoy!

### How to change email pattern?
Unfortunately in this version you'll have to change pattern right in code (See lib/roulette.hpp).

###Bugs and Features?
Feel free to write me via email.

### TODO
1. Shift from code to pluggable file.
2. Use program_options.
