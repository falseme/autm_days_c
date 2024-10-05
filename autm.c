#include "dayfiles.c"
#include "flags.c"

#include <time.h>

#define AUTM_VERSION "1.1.2"

static char **days_selected = NULL;
static size_t days_selected_len = 0;
static size_t days_selected_size = 0;

char *add_aux = NULL;
char *remove_aux = NULL;
char *end_aux = NULL;
char *undo_aux = NULL;
size_t rm_index_aux = 0;
size_t end_index_aux = 0;
size_t undo_index_aux = 0;

void manage_tasks(char *day);

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
                printf("Can't add a task with a name number\n");
            else
                add_aux = strcopy(optarg);
            break;

        case 'r':
            rm_index_aux = strtoul(optarg, NULL, 10);
            if (rm_index_aux == 0)
                remove_aux = strcopy(optarg);
            break;

        case 'e':
            end_index_aux = strtoul(optarg, NULL, 10);
            if (end_index_aux == 0)
                end_aux = strcopy(optarg);
            break;
        case 'u':
            undo_index_aux = strtoul(optarg, NULL, 10);
            if (undo_index_aux == 0)
                undo_aux = strcopy(optarg);
            break;

        /* FLAGS */
        case 0:
            if (option_index < 7)
                days_selected = strpush(days_selected, get_day_name(option_index), &days_selected_size, &days_selected_len);
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
    {
        for (int i = 0; i < days_selected_len; i++)
            manage_tasks(days_selected[i]);
    }
    else
    {
        time_t t = time(NULL);
        int tm_wday = localtime(&t)->tm_wday;
        load_today(get_day_name(tm_wday));
        manage_tasks("today");
    }

    if (help_flag)
    {
        printf("show help\n");
    }

    free(add_aux);
    free(remove_aux);
    free(end_aux);
    free(undo_aux);
    free(days_selected);

    exit(0);
}

void manage_tasks(char *day)
{
    if (add_aux)
        add_task(day, add_aux);
    if (end_aux)
        set_task_ended(day, end_aux, 1);
    if (end_index_aux > 0)
        set_task_ended_i(day, end_index_aux - 1, 1);
    if (undo_aux)
        set_task_ended(day, undo_aux, 0);
    if (undo_index_aux > 0)
        set_task_ended_i(day, undo_index_aux - 1, 0);
    if (remove_aux)
        remove_task_c(day, remove_aux);
    if (rm_index_aux > 0)
        remove_task_i(day, rm_index_aux - 1);
    if (!silent_flag)
        verbose_day(day);
}
