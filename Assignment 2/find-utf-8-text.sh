#! /bin/bash

# prepend . or ./ to input if necessary
# this makes the names in current directory identical
# this also takes care of input with leading special character
add_dot_slash() {
    for input in $@
    do

    # check first character of input
    if [ ${input:0:1} = '.' ]; then
            echo "$input"
        elif [ ${input:0:1} = '/' ]; then
        echo ".$input"
    else
            echo "./$input"
        fi
    
    done
}

# return 0 (true) if arg1 is utf8 file
# return 1 (false) if arg1 is not utf8 file
is_utf() {

    # check if arg1 contains null byte
    num_lines=$( sed -n "/\x00/p" $1 | wc -l )

    # -s to check if arg1 file content is not empty
    if ([ $num_lines -eq 0 ] && [ -s $1 ] ); then

    # check if arg1 file contains non-asc character
    # check for match
    file=$( grep -Pl "[^\x00-\x7F]" $1 )
    if [ ! -z $file ]; then
        return 0
    fi
    fi

    # arg1 is not asc
    return 1
}

# takes files or directories as arguments
# return only utf files
# if an argument is a directory, it will recursively
# go through the files in subdirectories
find_utf() {
    for input in $@
    do

    # check if input is directory 
    if [ -d $input ]; then

            files=$( find $input -type f )

        # recursively check if files in the directory are asc
            for file in $files
            do

        if is_utf $file; then
            echo "$file" 
        fi
        
            done
    
    else
    
        if is_utf $input; then
        echo "$input"
            fi

    fi

    done
}

# split the argument based on new line instead of space
OIFS="$IFS"
IFS=$'\n'

# disable * and ?
set -f

inputs=$( add_dot_slash $@ )

files=$( find_utf $inputs )

files=$( echo "$files" | sort -u )

# restore settings
IFS=$OIFS
set +f

if [ ! -z "$files" ]; then
    echo "$files"
fi
