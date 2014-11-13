#!/bin/sh

# Scripts sends all user_files constructed by newyear_roulette utility to
# corresponding emails.
#
# Usage: ./send_mails.sh users_dir/
#
# Author: I. Karbachinsky

set -e
#set -o pipefail

dir=$1

for user_file in `ls`; do
    echo $user_file
    #cat $user_file | mail -s 'New Year's santa welcomes you!' $user_file
done