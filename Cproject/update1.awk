#!/usr/bin/awk -f

BEGIN {
    FS = " ";
}

{
    if (ARGC == 2) {
        if (index($0, ARGV[1]) == 1) {
            if (length($0) > 0) {
                print $0;
            }
        }
    } else if (ARGC == 5) {
        if (index($0, ARGV[2]) == 1) {
            printf("%s %d %d\n", ARGV[3], ARGV[4], ARGV[5]);
        } else {
            print $0;
        }
    } else if (ARGC == 3) {
        if (ARGV[2] == "u") {
            if (index($0, ARGV[1]) == 1) {
                printf("%s %d %d\n", ARGV[1], ARGV[3], ARGV[4]);
            } else {
                print $0;
            }
        } else if (ARGV[2] == "d") {
            if (index($0, ARGV[1]) != 1) {
                print $0;
            }
        }
    }
}

END {
}
