#include "dayfiles.c"
#include "flags.c"

#include <time.h>

#define AUTM_VERSION "1.1.2"

static char **days = NULL;
static size_t day_count = 0;

char *add_aux = NULL;
char *remove_aux = NULL;
char *end_aux = NULL;
char *undo_aux = NULL;
size_t rm_index_aux = 0;
size_t end_index_aux = 0;
size_t undo_index_aux = 0;

void push_day(const char *day_name);
void manage_tasks(char *day, char *add, char *remove, char *end, char *undo);

int main(int argc, char **argv)
{
    int c;

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
            if (strtoul(optarg, NULL, 10) > 0 || strcmp(optarg, "0") == 0)
            {
                printf("Can't add a task with a name number\n");
                break;
            }
            add_aux = malloc(sizeof(char) * (strlen(optarg) * 1));
            strcpy(add_aux, optarg);
            break;

        case 'r':
            rm_index_aux = strtoul(optarg, NULL, 10);
            if (rm_index_aux == 0)
            {
                remove_aux = malloc(sizeof(char) * (strlen(optarg) * 1));
                strcpy(remove_aux, optarg);
            }
            break;

        case 'e':
            end_index_aux = strtoul(optarg, NULL, 10);
            if (end_index_aux == 0)
            {
                end_aux = malloc(sizeof(char) * (strlen(optarg) * 1));
                strcpy(end_aux, optarg);
            }
            break;
        case 'u':
            undo_index_aux = strtoul(optarg, NULL, 10);
            if (undo_index_aux == 0)
            {
                undo_aux = malloc(sizeof(char) * (strlen(optarg) * 1));
                strcpy(undo_aux, optarg);
            }
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
        days = (char **)realloc(days, sizeof(days) + sizeof(day_name)); // HERE
    days[day_count] = malloc(sizeof(day_name));
    strcpy(days[day_count++], day_name);
}

void manage_tasks(char *day, char *add, char *remove, char *end, char *undo)
{
    if (add)
        add_task(day, add);
    if (remove)
        remove_task_c(day, remove);
    if (rm_index_aux > 0)
        remove_task_i(day, rm_index_aux - 1);
    if (end)
        set_task_ended(day, end, 1);
    if (end_index_aux > 0)
        set_task_ended_i(day, end_index_aux - 1, 1);
    if (undo)
        set_task_ended(day, undo, 0);
    if (undo_index_aux > 0)
        set_task_ended_i(day, undo_index_aux - 1, 0);
    if (!silent_flag)
        verbose_day(day);
}
