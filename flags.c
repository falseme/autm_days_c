/*
 * CMD FLAGS & OPTIONS
*/

#include <getopt.h>

static int silent_flag;
static int help_flag;
static int day_flag;

static struct option long_options[] = {
    /* FLAGS */
    {"mon", no_argument, &day_flag, 1},
    {"tue", no_argument, &day_flag, 2},
    {"wed", no_argument, &day_flag, 3},
    {"thu", no_argument, &day_flag, 4},
    {"fri", no_argument, &day_flag, 5},
    {"sat", no_argument, &day_flag, 6},
    {"sun", no_argument, &day_flag, 7},
    {"today", no_argument, &day_flag, 8},
    {"help", no_argument, &help_flag, 9},
    {"silent", no_argument, &silent_flag, 10},

    /* OPTIONS */
    {"add", required_argument, 0, 'a'},
    {"remove", required_argument, 0, 'r'},
    {"end", required_argument, 0, 'e'},
    {"undo", required_argument, 0, 'u'},

    /**/
    {0, 0, 0, 0}
};

int get_long_opt(int argc, char* const* argv, int *option_index)
{
    return getopt_long(argc, argv, "a:e:r:u:", long_options, option_index);
}

const char* get_day_name(int index)
{
    return long_options[index].name;
}

