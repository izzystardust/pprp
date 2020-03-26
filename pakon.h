#pragma once

#include <stdio.h>
//#include <stdbool.h>
#include <stdint.h>

// optional 16 byte header
#define HEADER 16
#define RAW_ROWS 2000
#define MAX_HEIGHT 3000
#define MAX_RAW_FILE_SIZE_UINT8 (HEADER + 6*RAW_ROWS*MAX_HEIGHT)

struct pakon_raw {
    int rows;
    int columns;
    uint16_t *pixels;
};

struct pakon_raw_error {
    char *msg;
    int code;
};

enum pakon_error_code {
    P_ERR_FILESIZE_EXCEEDED = 1,
    P_ERR_BAD_HEADER_SIZE,
};

struct pakon_raw_error *
load_pakon_raw(
    struct pakon_raw *out,
    char *raaw_file_name
);

uint16_t *convert_from_planar(uint16_t *planar_file, size_t len);