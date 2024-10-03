#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "dayt.c"
#include "options.c"

int main(int argc, char **argv)
{
    int c;

    char* add_aux = NULL;
    char* remove_aux = NULL;
    char* end_aux = NULL;

    while (1)
    {

        int option_index = 0;
        c = getopt_long(argc, argv, "a:r:e:", long_options, &option_index);

        if (c == -1)
            break;

        switch (c)
        {
        /* OPTIONS */
        case 'a':
            add_aux = malloc(sizeof(optarg));
            strcpy(add_aux, optarg);
            break;

        case 'r':
            remove_aux = malloc(sizeof(optarg));
            strcpy(remove_aux, optarg);
            break;

        case 'e':
            end_aux = malloc(sizeof(optarg));
            strcpy(end_aux, optarg);
            break;

        /* FLAGS */
        case 0:
            if (option_index < 8)
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
            {
                if(add_aux)
                    printf("day `%s` | add `%s`\n", days[i].name, add_aux);
                if(remove_aux)
                    printf("day `%s` | remove `%s`\n", days[i].name, remove_aux);
                if(end_aux)
                    printf("day `%s` | end `%s`\n", days[i].name, end_aux);
            }
        }

    if (verbose_flag)
    {
        printf("verbose\n");
    }

    if (help_flag)
    {
        printf("show help\n");
    }

    exit(0);
}