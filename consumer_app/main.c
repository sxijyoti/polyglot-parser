#include <stdio.h>
#include "parser.h"

int main(int argc, char **argv) {
    const char *input = (argc > 1) ? argv[1] : "../../examples";

    polyparser_result *res = polyparser_parse_dir(input);
    if (!res) {
        fprintf(stderr, "Failed to parse directory: %s\n", input);
        return 1;
    }

    char *json = polyparser_to_json(res);
    if (!json) {
        fprintf(stderr, "Failed to export json\n");
        polyparser_free_res(res);
        return 1;
    }

    puts(json);

    polyparser_free_json(json);
    polyparser_free_res(res);
    return 0;
}
