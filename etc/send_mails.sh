#!/bin/bash

# Scripts sends all user_files constructed by newyear_roulette utility to
# corresponding emails.
#
# Usage: ./send_mails.sh users_dir/
#
# Author: I. Karbachinsky <igorkarbachinsky@mail.ru>

set -e
set -o pipefail

dir=$1

if [ ! -d $dir ]; then
    echo "$dir directory doesn't exist" >&2
    exit 1
fi

pushd $dir > /dev/null

for user_file in `ls`; do
    echo "Processing $user_file file"
    cat "$user_file" | mail -s "New Year's santa welcomes you!" $user_file
done

popd > /dev/null

echo "Done"
