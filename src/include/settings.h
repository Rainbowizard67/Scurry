#ifndef NETDATA_H
#define NETDATA_H

#include <yaml.h>
#include "scurry_utils.h"
#include "project_headers.h"

#define MAX_FILEPATH_SIZE 256

#define HT_SIZE 7


yaml_parser_t init_yaml(FILE *file);
void clean_parser(yaml_parser_t *parser, FILE *file);

#endif
