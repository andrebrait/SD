#include <rpc/rpc.h>
#include "file.h"

char **servefile_1_svc(char **fname, struct svc_req *req) {
    FILE *fp;
    long lSize;
    static char *buffer;

    fp = fopen(*fname, "r");
    if (!fp) perror(*fname), exit(1);

    fseek(fp, 0L, SEEK_END);
    lSize = ftell(fp);
    rewind(fp);

    /* allocate memory for entire content */
    buffer = calloc(1, (size_t) lSize + 1);

    if (!buffer) fclose(fp), fputs("memory alloc fails", stderr), exit(1);

    /* copy the file into the buffer */
    if (1 != fread(buffer, (size_t) lSize, 1, fp))
        fclose(fp), free(buffer), fputs("entire read fails", stderr), exit(1);

    /* do your work here, buffer is a string contains the whole text */
    fclose(fp);
    return &buffer;
}
