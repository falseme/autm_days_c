/*
 * DAYS, TASKS & FILES
 */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define COLOR_RED "\x1b[31m"
#define COLOR_GREEN "\x1b[32m"
#define COLOR_YELLOW "\x1b[33m"
#define COLOR_BLUE "\x1b[34m"
#define COLOR_MAGENTA "\x1b[35m"
#define COLOR_CYAN "\x1b[36m"
#define COLOR_RESET "\x1b[0m"

/**
 * Daily Task data
 */
typedef struct task_t
{
    char *name;
    int ended;
} Task;

/**
 * Day data
 */
typedef struct day_t
{
    char *name;
    Task **task_list;
    size_t task_size;
} Day;

/**
 * Serialize the Day data into the given file. (Does not close the file)
 *
 * @param fd The file where the day data will be serialized
 * @param day The day data
 *
 * File format:
 * 1. Day name byte size [size_t]
 * 2. Day name [(1.) * char]
 * 3. Day task list length [size_t]
 * 4. Day Task List:
 * 4.1. Task name byte size [size_t]
 * 4.2. Task name [(4.1.) * char]
 * 4.3. Task ended condition [int]
 */
void serialize_day(FILE *fd, Day *day)
{
    size_t day_name_size = (strlen(day->name) + 1) * sizeof(char);
    fwrite(&day_name_size, sizeof(size_t), 1, fd);  // 1.
    fwrite(day->name, day_name_size, 1, fd);        // 2.
    fwrite(&day->task_size, sizeof(size_t), 1, fd); // 3.

    // 4.
    for (size_t i = 0; i < day->task_size; i++)
    {
        size_t task_name_size = (strlen(day->task_list[i]->name) + 1) * sizeof(char);
        fwrite(&task_name_size, sizeof(size_t), 1, fd);         // 4.1.
        fwrite(day->task_list[i]->name, task_name_size, 1, fd); // 4.2.
        fwrite(&day->task_list[i]->ended, sizeof(int), 1, fd);  // 4.3.
    }
}

/**
 * Deserialize the day data from the given file and store it into the given day struct.
 * (Does not close the file)
 *
 * @param fd The day file
 * @param day The day struct
 *
 * File format:
 * 1. Day name byte size [size_t]
 * 2. Day name [(1.) * char]
 * 3. Day task list length [size_t]
 * 4. Day Task List:
 * 4.1. Task name byte size [size_t]
 * 4.2. Task name [(4.1.) * char]
 * 4.3. Task ended condition [int]
 */
void deserialize_day(FILE *fd, Day *day)
{
    if (!day)
        day = malloc(sizeof(Day));
    size_t day_name_size;
    fread(&day_name_size, sizeof(size_t), 1, fd); // 1.
    day->name = malloc(day_name_size);
    fread(day->name, day_name_size, 1, fd); // 2.
    size_t task_len;
    fread(&task_len, sizeof(size_t), 1, fd); // 3.
    day->task_size = task_len;

    // 4.
    day->task_list = (Task **)malloc(sizeof(Task) * task_len);
    for (size_t i = 0; i < task_len; i++)
    {
        day->task_list[i] = (Task *)malloc(sizeof(Task));
        size_t task_name_size;
        fread(&task_name_size, sizeof(size_t), 1, fd); // 4.1.
        day->task_list[i]->name = malloc(sizeof(task_name_size));
        fread(day->task_list[i]->name, task_name_size, 1, fd); // 4.2.
        fread(&day->task_list[i]->ended, sizeof(int), 1, fd);  // 4.3.
    }
}

/**
 * Saves the given day data into a file with the same name.
 *
 * @param day The day data
 * @param fday The day file name
 */
void save_day(Day *day, const char *fday)
{
    FILE *fd = fopen(fday, "wb");
    if (!fd)
    {
        printf("[ERR] couldn't open day file [%s]", day->name);
        return;
    }

    serialize_day(fd, day);
    fclose(fd);
}

/**
 * Loads and retrieves the day data looking for a file with the same name.
 *
 * @param name The first 3 characters of a day. Eg. `mon` (monday).
 */
Day *load_day(const char *name)
{
    Day *day = malloc(sizeof(Day));

    FILE *fd = fopen(name, "rb");
    if (!fd)
    {
        day->name = malloc(sizeof(name));
        strcpy(day->name, name);
        day->task_size = 0;
        day->task_list = NULL;
    }
    else
    {
        deserialize_day(fd, day);
        fclose(fd);
    }

    return day;
}

/**
 * Loads the current day file.
 *
 * @param wday The current day of the week.
 */
void load_today(const char *wday)
{
    Day *today = load_day("today");
    if (strcmp(today->name, wday) != 0)
    {
        free(today);
        today = load_day(wday);
        save_day(today, "today");
    }
    printf(COLOR_BLUE "\nTODAY: " COLOR_MAGENTA "[%s]", today->name);
    free(today);
}

/**
 * Adds a task to the day task list.
 *
 * @param name The day name
 * @param task_name The daily task name
 * @param silent Do not verbose if `true`
 */
void add_task(char *name, char *task_name)
{
    Day *day = load_day(name);
    size_t i = day->task_size++;

    if (!day->task_list)
        day->task_list = (Task **)malloc(sizeof(Task));
    else
    {
        for (size_t i = 0; i < day->task_size - 1; i++)
        {
            if (strcmp(day->task_list[i]->name, task_name) == 0)
            {
                printf(COLOR_MAGENTA "\n[%s]" COLOR_RED " task \"%s\" already exists" COLOR_RESET, day->name, task_name);
                free(day);
                return;
            }
        }
        day->task_list = (Task **)realloc(day->task_list, sizeof(Task) * (day->task_size));
    }

    day->task_list[i] = (Task *)malloc(sizeof(Task));
    day->task_list[i]->name = (char *)malloc(sizeof(task_name));
    strcpy(day->task_list[i]->name, task_name);
    day->task_list[i]->ended = 0;

    save_day(day, name);
    free(day);
}

/**
 * Removes a task from the given day.
 *
 * @param day The day data struct
 * @param index The task index
 * @param task_name The task name
 * @return 1 if could remove it, 0 otherwise
 */
int remove_task(Day *day, size_t index, const char *task_name)
{
    // ERR
    if (index >= day->task_size)
        return 0;
    // MOVE TO THE LEFT
    for (size_t i = index + 1; i < day->task_size; i++)
    {
        day->task_list[i - 1]->name = day->task_list[i]->name;
        day->task_list[i - 1]->ended = day->task_list[i]->ended;
    }
    // RESIZE
    day->task_size--;
    free(day->task_list[day->task_size]);
    day->task_list = (Task **)realloc(day->task_list, sizeof(Task) * day->task_size);
    return 1;
}

/**
 * Loads the day file, search for the given task and removes it.
 *
 * @param name The day name
 * @param task_name The daily task name
 */
void remove_task_c(char *name, const char *task_name)
{
    Day *day = load_day(name);
    size_t index;
    for (index = 0; index < day->task_size; index++)
        if (strcmp(task_name, day->task_list[index]->name) == 0)
            break;

    if (remove_task(day, index, task_name))
        save_day(day, name);
    else
        printf(COLOR_MAGENTA "\n[%s]" COLOR_RED " task \"%s\" not found" COLOR_RESET, name, task_name);
    free(day);
}

/**
 * Loads the day file and removes the task in the given index.
 *
 * @param name The day name
 * @param index The task index
 */
void remove_task_i(char *name, size_t index)
{
    Day *day = load_day(name);
    if (remove_task(day, index, "index -r"))
        save_day(day, name);
    else
        printf(COLOR_MAGENTA "\n[%s]" COLOR_RED " task index \"%ld\" not found" COLOR_RESET, name, index + 1);
    free(day);
}

/**
 * Ends or Undo a task from the task list.
 *
 * @param name The day name
 * @param task_name The daily task name
 * @param eu End [1] or Undo [0]
 */
void set_task_ended(char *name, char *task_name, int eu)
{
    Day *day = load_day(name);
    for (size_t i = 0; i < day->task_size; i++)
    {
        if (strcmp(task_name, day->task_list[i]->name) == 0)
        {
            day->task_list[i]->ended = eu;
            save_day(day, name);
            free(day);
            return;
        }
    }
    printf(COLOR_MAGENTA "\n[%s]" COLOR_RED " task \"%s\" not found" COLOR_RESET, name, task_name);
    free(day);
}

void set_task_ended_i(char *name, size_t index, int eu)
{
    Day *day = load_day(name);
    if (index >= day->task_size)
    {
        printf(COLOR_MAGENTA "\n[%s]" COLOR_RED " task index \"%ld\" not found" COLOR_RESET, name, index + 1);
        free(day);
        return;
    }
    day->task_list[index]->ended = eu;
    save_day(day, name);
    free(day);
}

/**
 * Prints the given day in a readable format
 */
void prettyprint(Day *day)
{
    printf("\n" COLOR_MAGENTA "[%s]" COLOR_RESET " Task List:\n", day->name);
    for (size_t i = 0; i < day->task_size; i++)
    {
        if (day->task_list[i]->ended)
            printf("\n %ld." COLOR_GREEN "[%c]" COLOR_RESET " %s", i + 1, 'X', day->task_list[i]->name);
        else
            printf("\n %ld." COLOR_RED "[%c]" COLOR_RESET " %s", i + 1, '_', day->task_list[i]->name);
    }
    printf("\n");
}

/**
 * Loads the day and prettyprints it.
 */
void verbose_day(char *day_name)
{
    Day *day = load_day(day_name);
    prettyprint(day);
    free(day);
}
