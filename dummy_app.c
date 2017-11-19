#include <inttypes.h>
#include <stdio.h>
#include <string.h>

#define APP_NAME    "Dummy application"
#define APP_VERSION "0.0.1"

typedef enum {
    LOG_LEVEL_INFO = 0,
    LOG_LEVEL_ERROR,
    LOG_LEVEL_MAX
} log_levels_t;

const char *log_level_strings[LOG_LEVEL_MAX] = {
    "INFO",
    "ERROR"
};

#define LOG(_level, _fmt, ...) \
    printf("[%s] " _fmt "\n", log_level_strings[(_level)], ##__VA_ARGS__)

#define LOGI(_fmt, ...) LOG(LOG_LEVEL_INFO, _fmt, ##__VA_ARGS__)
#define LOGE(_fmt, ...) LOG(LOG_LEVEL_ERROR, _fmt, ##__VA_ARGS__)

typedef enum {
    APP_RESULT_OK = 0,
    APP_RESULT_ERROR
} app_result_t;

typedef enum {
    INPUT_ARG_EXEC_NAME = 0,
    INPUT_ARG_PARAM,
    INPUT_ARG_NUM,
} input_arg_t;

typedef enum {
    PARAM_FIRST = 0,
    PARAM_HELP = PARAM_FIRST,
    PARAM_NUM,
    PARAM_STR,
    PARAM_INVALID,
    PARAM_TOO_LONG
} param_t;

const char *param_strings[PARAM_NUM] = {
    "help"
};

const char *param_desc_strings[PARAM_NUM] = {
    "Print this help message"
};

#define PARAM_PREFIX "--"
#define PARAM_PREFIX_LEN 2

#define MAX_PARAM_LENGTH 20

param_t get_parameter_type(char *input)
{
    param_t res = PARAM_INVALID;
    uint32_t itr;
    char *param;

    if (strncmp(PARAM_PREFIX, input, PARAM_PREFIX_LEN)) {
        return (strlen(input) <= MAX_PARAM_LENGTH) ?  PARAM_STR : PARAM_TOO_LONG;
    }

    if (strlen(input) <= PARAM_PREFIX_LEN) {
       return PARAM_INVALID;
    }

    param = input + PARAM_PREFIX_LEN;
    for (itr = PARAM_FIRST; itr < PARAM_NUM; itr++) {
        if (!strcmp(param, param_strings[itr])) {
            res = itr;
            break;
        }
    }

    return res;
}

app_result_t print_help(void)
{
    uint32_t itr;

    LOGI("%s v%s", APP_NAME, APP_VERSION);
    LOGI("");
    LOGI("Print out command line parameter with maximum legnth of %d,", MAX_PARAM_LENGTH);
    LOGI("except of when parameter prefix is \"%s\":", PARAM_PREFIX);
    for (itr = PARAM_FIRST; itr < PARAM_NUM; itr++) {
        LOGI("    %s%-10s %s", PARAM_PREFIX, param_strings[itr], param_desc_strings[itr]);
    }

    return APP_RESULT_OK;
}

app_result_t print_parameter(char *parameter)
{
    LOGI("Input: \"%s\"", parameter);

    return APP_RESULT_OK;
}

app_result_t handle_invalid_parameter(char *parameter)
{
    LOGE("Invalid parameter \"%s\"", parameter);

    return APP_RESULT_ERROR;
}

app_result_t handle_parameter_too_long(void)
{
    LOGE("Parameter longer than %d", MAX_PARAM_LENGTH);

    return APP_RESULT_ERROR;
}

app_result_t handle_parameter(char *parameter)
{
    param_t param_type;
    app_result_t res = APP_RESULT_ERROR;

    param_type = get_parameter_type(parameter);
    switch(param_type) {
        case PARAM_HELP:
            res = print_help();
            break;
        case PARAM_STR:
            res = print_parameter(parameter);
            break;
        case PARAM_INVALID:
            res = handle_invalid_parameter(parameter);
            break;
        case PARAM_TOO_LONG:
            res = handle_parameter_too_long();
        default:
            break;
    }

    return res;
}

int main(int argc, char *argv[])
{
    if (argc != INPUT_ARG_NUM) {
        LOGE("Number of input parameters must be %d", INPUT_ARG_NUM);
        return APP_RESULT_ERROR;
    }

    return handle_parameter(argv[INPUT_ARG_PARAM]);
}
