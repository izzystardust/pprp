#include <stdint.h>
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>

#include "pakon.h"

struct pakon_raw_error *gen_error(int code, char *str) {
    struct pakon_raw_error *err = malloc(sizeof(struct pakon_raw_error));
    err->code = code;
    err->msg = str;
    return err;
}

struct pakon_raw_error * load_pakon_raw(struct pakon_raw *out, char *raw_file_name) {
    long file_len;
    FILE *raw_file;
    uint16_t *planar_pixels;

    raw_file = fopen(raw_file_name, "rb");
    fseek(raw_file, 0, SEEK_END);
    file_len = ftell(raw_file);

    if (file_len > MAX_RAW_FILE_SIZE_UINT8) {
        // TODO: details in error message :P
        return gen_error(P_ERR_FILESIZE_EXCEEDED, "file larger than max possible");
    }

    int width = file_len / (6*RAW_ROWS);
    fprintf(stderr, "width is %d\n", width);
    int header = file_len - (RAW_ROWS * width * 6);
    if (header && header != 16) {
        // TODO: details in error message :P
        return gen_error(P_ERR_BAD_HEADER_SIZE, "header is not 16 bytes?");
    }

    fseek(raw_file, header, SEEK_SET);
    planar_pixels = (uint16_t *) malloc(sizeof(char) * (file_len - header));
    fread((char *) planar_pixels, file_len, 1, raw_file);
    fclose(raw_file);

    out->rows = RAW_ROWS;
    out->columns = width;
    out->pixels = convert_from_planar(planar_pixels, out->rows * out->columns * 6);
    free(planar_pixels);
    return NULL;
}


uint16_t *convert_from_planar(uint16_t *planar_file, size_t len) {
    if (len % 3) {
        fprintf(stderr, "wat, len indivisible by 3?");
        return NULL;
    }
    uint16_t *out = malloc(sizeof(uint16_t) * len);
    for (int i = 0; i < len/3; ++i) {
        out[3*i + 0] = planar_file[i]; // Red component
        out[3*i + 1] = planar_file[len/3+i]; // Green component
        out[3*i + 2] = planar_file[2*len/3+i]; // Blue component
    }
    return out;
}