#include <stdio.h>
#include <stdlib.h>
#include <getopt.h>

/*
 * DAYS & TASKS
*/
typedef struct task_t
{
    char* name;
    int done;
} Task;

typedef struct day_t
{
    char* name;
    Task* tasks;
} Day;

static Day days[] = {
    {"monday", NULL},
    {"tuesday", NULL},
    {"wednesday", NULL},
    {"thursday", NULL},
    {"friday", NULL},
    {"saturday", NULL},
    {"sunday", NULL},
};

/*
 * FLAGS & OPTIONS
*/
static int verbose_flag;
static int help_flag;
static int day_flag;
int day_flags[8];

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
    {"verbose", no_argument, &verbose_flag, 10},

    /* CMD OPTIONS */
    {"add", required_argument, 0, 'a'},
    {"remove", required_argument, 0, 'r'},
    {"end", required_argument, 0, 'e'},

    /**/
    {0, 0, 0, 0}
};

int main(int argc, char **argv)
{
    int c;

    while (1)
    {

        int option_index = 0;
        c = getopt_long(argc, argv, "a:r:e:", long_options, &option_index);

        if (c == -1) break;

        switch (c)
        {
        /* OPTIONS */
        case 'a':
            printf("ADD `%s`\n", optarg);
            break;

        case 'r':
            printf("REMOVE `%s`\n", optarg);
            break;

        case 'e':
            printf("END `%s`\n", optarg);
            break;

        /* FLAGS */
        case 0:
            if(option_index < 8)
                day_flags[option_index] = 1;
                // day_flags[long_options[option_index].val] = 1;
            break;
        default:
            abort();
        }
    }

    if (day_flag)
        for (int i = 0; i < 8; i++)
        {
            if (day_flags[i])
                printf("day `%s`\n", days[i].name);
        }

    if(verbose_flag)
    {
        printf("verbose\n");
    }

    if (help_flag)
    {
        printf("show help\n");
    }

    exit(0);
}