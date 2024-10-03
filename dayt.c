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