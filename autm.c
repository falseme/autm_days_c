#include "dayfiles.c"
#include "flags.c"

static char **days = NULL;
static size_t day_count = 0;

void push_day(const char* day_name);

int main(int argc, char **argv)
{
    int c;

    char *add_aux = NULL;
    char *remove_aux = NULL;
    char *end_aux = NULL;

    while (1)
    {

        int option_index = 0;
        c = get_long_opt(argc, argv, &option_index);

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
                push_day(get_day_name(option_index));
            break;
        default:
            printf("unknown option: [%c]", c);
            abort();
        }
    }

    if (day_flag)
        for (int i = 0; i < day_count; i++)
        {
            printf("\n");
            if (add_aux)
                add_task(days[i], add_aux, verbose_flag);
            if (remove_aux)
                printf("DAY `%s` | REMOVE `%s`\n", days[i], remove_aux);
            if (end_aux)
                printf("DAY `%s` | END `%s`\n", days[i], end_aux);
        }

    if (help_flag)
    {
        printf("show help\n");
    }

    free(add_aux);
    free(remove_aux);
    free(end_aux);
    free(days);

    exit(0);
}

void push_day(const char *day_name)
{
    if (!days)
        days = (char **)malloc(sizeof(day_name));
    else
        days = (char **)realloc(days, sizeof(days) + sizeof(day_name));
    days[day_count] = malloc(sizeof(day_name));
    strcpy(days[day_count++], day_name);
}
