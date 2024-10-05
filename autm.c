#include "dayfiles.c"
#include "flags.c"

#include <time.h>

#define AUTM_VERSION "1.1.1"

static char **days = NULL;
static size_t day_count = 0;

void push_day(const char *day_name);
void manage_tasks(char *day, char *add, char *remove, char *end, char *undo);

int main(int argc, char **argv)
{
    int c;

    char *add_aux = NULL;
    char *remove_aux = NULL;
    char *end_aux = NULL;
    char *undo_aux = NULL;

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
        case 'u':
            undo_aux = malloc(sizeof(optarg));
            strcpy(undo_aux, optarg);
            break;

        /* FLAGS */
        case 0:
            if (option_index < 7)
                push_day(get_day_name(option_index));
            break;
        default:
            printf("unknown option: [%c]", c);
            abort();
        }
    }

    if (version_flag)
    {
        printf("Autumn Days C\nautm - %s\n", AUTM_VERSION);
    }

    if (day_flag)
        for (int i = 0; i < day_count; i++)
            manage_tasks(days[i], add_aux, remove_aux, end_aux, undo_aux);
    else
    {
        time_t t = time(NULL);
        int tm_wday = localtime(&t)->tm_wday;
        load_today(get_day_name(tm_wday));
        manage_tasks("today", add_aux, remove_aux, end_aux, undo_aux);
    }

    if (help_flag)
    {
        printf("show help\n");
    }

    free(add_aux);
    free(remove_aux);
    free(end_aux);
    free(undo_aux);
    free(days);

    exit(0);
}

// TODO: rewrite mallocs
void push_day(const char *day_name)
{
    if (!days)
        days = (char **)malloc(sizeof(day_name));
    else
        days = (char **)realloc(days, sizeof(days) + sizeof(day_name));
    days[day_count] = malloc(sizeof(day_name));
    strcpy(days[day_count++], day_name);
}

void manage_tasks(char *day, char *add, char *remove, char *end, char *undo)
{
    if (add)
        add_task(day, add);
    if (remove)
        remove_task(day, remove);
    if (end)
        set_task_ended(day, end, 1);
    if (undo)
        set_task_ended(day, undo, 0);
    if (!silent_flag)
        verbose_day(day);
}
