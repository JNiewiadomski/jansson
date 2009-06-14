#ifndef JANSSON_H
#define JANSSON_H

#include <stdio.h>
#include <stdint.h>

/* types */

typedef enum {
    JSON_OBJECT,
    JSON_ARRAY,
    JSON_STRING,
    JSON_NUMBER,
    JSON_TRUE,
    JSON_FALSE,
    JSON_NULL
} json_type;

typedef struct {
    json_type type;
    unsigned long refcount;
} json_t;

#define json_typeof(json)      ((json)->type)
#define json_is_object(json)   (json && json_typeof(json) == JSON_OBJECT)
#define json_is_array(json)    (json && json_typeof(json) == JSON_ARRAY)
#define json_is_string(json)   (json && json_typeof(json) == JSON_STRING)
#define json_is_number(json)   (json && json_typeof(json) == JSON_NUMBER)
#define json_is_true(json)     (json && json_typeof(json) == JSON_TRUE)
#define json_is_false(json)    (json && json_typeof(json) == JSON_FALSE)
#define json_is_null(json)     (json && json_typeof(json) == JSON_NULL)

/* construction, destruction, reference counting */

json_t *json_object(void);
json_t *json_array(void);
json_t *json_string(const char *value);
json_t *json_number(double value);
json_t *json_true(void);
json_t *json_false(void);
json_t *json_null(void);

json_t *json_clone(json_t *json);

static inline json_t *json_incref(json_t *json)
{
    if(json)
        ++json->refcount;
    return json;
}

/* do not call json_delete directly */
void json_delete(json_t *json);

static inline void json_decref(json_t *json)
{
    if(json && --json->refcount == 0)
        json_delete(json);
}


/* getters, setters, manipulation */

json_t *json_object_get(const json_t *object, const char *key);
int json_object_set(json_t *object, const char *key, json_t *value);
int json_object_del(json_t *object, const char *key);
void *json_object_iter(json_t *object);
void *json_object_iter_next(json_t *object, void *iter);
const char *json_object_iter_key(void *iter);
json_t *json_object_iter_value(void *iter);

unsigned int json_array_size(const json_t *array);
json_t *json_array_get(const json_t *array, unsigned int index);
int json_array_set(json_t *array, unsigned int index, json_t *value);
int json_array_append(json_t *array, json_t *value);

const char *json_string_value(const json_t *json);
double json_number_value(const json_t *json);


/* loading, printing */

#define JSON_ERROR_TEXT_LENGTH  160

typedef struct {
    char text[JSON_ERROR_TEXT_LENGTH];
    int line;
} json_error_t;

json_t *json_load(const char *path, json_error_t *error);
json_t *json_loads(const char *input, json_error_t *error);
json_t *json_loadf(FILE *input, json_error_t *error);
json_t *json_loadfd(int fd, json_error_t *error);

#define JSON_INDENT(n)   (n & 0xFF)
#define JSON_SORT_KEYS   0x100

int json_dump(const json_t *json, const char *path, uint32_t flags);
char *json_dumps(const json_t *json, uint32_t flags);
int json_dumpf(const json_t *json, FILE *output, uint32_t flags);
int json_dumpfd(const json_t *json, int fd, uint32_t flags);

#endif