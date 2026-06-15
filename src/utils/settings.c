#include "../include/settings.h"
#include <yaml.h>

//Start structure settings
struct handler_settings {
    char doc_root[MAX_FILEPATH_SIZE];
    char default_path[MAX_FILEPATH_SIZE];
    unsigned short keep_alive;
    //TODO, default timeout
};

typedef struct handler_settings handler_settings_t;

struct network_settings {
    unsigned short port_number;
    bool dual_stack_enabled;
    char ipv4_address[16];
    char ipv6_address[46];
    unsigned short backlog;
    unsigned short max_events;
};

typedef struct network_settings network_settings_t;

struct settings {
    network_settings_t* network;
    handler_settings_t* handler;
};
typedef struct settings settings_t;
//End structure settings

//Start CYAML schema settings and format
static const cyaml_schema_field_t ns_fields_schema[] = {
    CYAML_FIELD_UINT("Port_Number", CYAML_FLAG_DEFAULT, network_settings_t, port_number),
    CYAML_FIELD_STRING("IPv4_Address", CYAML_FLAG_DEFAULT, network_settings_t, ipv4_address, 0),
    CYAML_FIELD_UINT("Backlog", CYAML_FLAG_DEFAULT, network_settings_t, backlog),
    CYAML_FIELD_UINT("Max_Events", CYAML_FLAG_DEFAULT, network_settings_t, max_events),
    CYAML_FIELD_END
};

static const cyaml_schema_field_t hs_fields_schema[] = {
    CYAML_FIELD_STRING("Doc_Root", CYAML_FLAG_POINTER, handler_settings_t, doc_root, 0),
    CYAML_FIELD_STRING("Default_Path", CYAML_FLAG_POINTER, handler_settings_t, default_path, 0),
    CYAML_FIELD_UINT("Keep_Alive", CYAML_FLAG_DEFAULT, handler_settings_t, keep_alive),
    CYAML_FIELD_END
};

static const cyaml_schema_field_t setting_fields_schema[] = {
    CYAML_FIELD_MAPPING("Network", CYAML_FLAG_POINTER, settings_t, network, ns_fields_schema),
    CYAML_FIELD_MAPPING("Handler", CYAML_FLAG_POINTER, settings_t, handler, hs_fields_schema),
    CYAML_FIELD_END
};

static const cyaml_schema_value_t schema = {
    CYAML_VALUE_MAPPING(CYAML_FLAG_POINTER, settings_t, setting_fields_schema),
};
//End CYAML schema settings and format

struct settings_t parse_yaml(yaml_parser_t *parser) {
    yaml_token_t token;

    do {
        yaml_parser_scan(&parser, &token);
        switch(token.type)) {
            case YAML_STREAM_START_TOKEN:
            case YAML_STREAM_END_TOKEN:
            case YAML_KEY_TOKEN:
            case YAML_VALUE_TOKEN:

            default:
                yaml_token_delete(&token)
        }
    } while (token.type != YAML_STREAM_END_TOKEN);

    yaml_token_delete(&token);
}

/*
Function that initializes the yaml parser with the given file fd and returns the parser.
Return yaml_parser_t, Input FILE *file
*/
yaml_parser_t init_yaml(FILE *file) {
    yaml_parser_t parser;

    if(!yaml_parser_initialize(&parser)) {
        perror("Error yaml_parser_initialize");
        exit(EXIT_FAILURE);
    }
    if(file == NULL) {
        perror("Error fopen");
        exit(EXIT_FAILURE);
    }

    yaml_parser_set_input_file(&parser, file);

    return parser;
}

/*
Function that cleans up the yaml parser and closes the file.
Return void, Input yaml_parser_t *parser & FILE *file
*/
void clean_parser(yaml_parser_t *parser, FILE *file) {
    if(fclose(file) != 0) {
        perror("Error fclose");
        exit(EXIT_FAILURE);
    }
    yaml_parser_delete(parser);
}
