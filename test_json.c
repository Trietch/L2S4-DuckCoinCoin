/* vim: set et ts=4
 *
 * Copyright (C) 2015 Mirko Pasqualetti  All rights reserved.
 * https://github.com/udp/json-parser
 *
 * Redistribution and use in source and binary forms, with or without
 * modification, are permitted provided that the following conditions
 * are met:
 *
 * 1. Redistributions of source code must retain the above copyright
 *   notice, this list of conditions and the following disclaimer.
 *
 * 2. Redistributions in binary form must reproduce the above copyright
 *   notice, this list of conditions and the following disclaimer in the
 *   documentation and/or other materials provided with the distribution.
 *
 * THIS SOFTWARE IS PROVIDED BY THE AUTHOR AND CONTRIBUTORS ``AS IS'' AND
 * ANY EXPRESS OR IMPLIED WARRANTIES, INCLUDING, BUT NOT LIMITED TO, THE
 * IMPLIED WARRANTIES OF MERCHANTABILITY AND FITNESS FOR A PARTICULAR PURPOSE
 * ARE DISCLAIMED.  IN NO EVENT SHALL THE AUTHOR OR CONTRIBUTORS BE LIABLE
 * FOR ANY DIRECT, INDIRECT, INCIDENTAL, SPECIAL, EXEMPLARY, OR CONSEQUENTIAL
 * DAMAGES (INCLUDING, BUT NOT LIMITED TO, PROCUREMENT OF SUBSTITUTE GOODS
 * OR SERVICES; LOSS OF USE, DATA, OR PROFITS; OR BUSINESS INTERRUPTION)
 * HOWEVER CAUSED AND ON ANY THEORY OF LIABILITY, WHETHER IN CONTRACT, STRICT
 * LIABILITY, OR TORT (INCLUDING NEGLIGENCE OR OTHERWISE) ARISING IN ANY WAY
 * OUT OF THE USE OF THIS SOFTWARE, EVEN IF ADVISED OF THE POSSIBILITY OF
 * SUCH DAMAGE.
 */

#include <stdio.h>
#include <stdlib.h>
#include <sys/stat.h>

#include "defines.h"
#include "json.h"

/*
 * Test for json.c
 *
 * Compile (static linking) with
 *         gcc -o test_json -I.. test_json.c ../json.c -lm
 *
 * Compile (dynamic linking) with
 *         gcc -o test_json -I.. test_json.c -lm -ljsonparser
 *
 * USAGE: ./test_json <json_file>
 */


static void process_value(json_value* value, int depth, FILE *file_temp);

static void process_object(json_value* value, int depth, FILE *file_temp)
{
        int length, x;
        if (value == NULL) {
                return;
        }
        length = value->u.object.length;
        for (x = 0; x < length; x++) {
                fprintf(file_temp, "%s\n", value->u.object.values[x].name);
                process_value(value->u.object.values[x].value, depth+1, file_temp);
        }
}

static void process_array(json_value* value, int depth, FILE *file_temp)
{
        int length, x;
        if (value == NULL) {
                return;
        }
        length = value->u.array.length;
        for (x = length-1; x >= 0; --x) {
                process_value(value->u.array.values[x], depth, file_temp);
        }
}

void process_value(json_value *value, int depth, FILE *file_temp)
{
    if (value == NULL) {
               return;
       }
        switch (value->type) {
                case json_none:
                        fprintf(file_temp, "none\n");
                        break;
                case json_object:
                        process_object(value, depth+1, file_temp);
                        break;
                case json_array:
                        process_array(value, depth+1, file_temp);
                        break;
                case json_integer:
                        fprintf(file_temp, "%ld\n", value->u.integer);
                        break;
                case json_double:
                        fprintf(file_temp, "%f\n", value->u.dbl);
                        break;
                case json_string:
                        fprintf(file_temp, "%s\n", value->u.string.ptr);
                        break;
                case json_boolean:
                        fprintf(file_temp, "%d\n", value->u.boolean);
                        break;
				default:
						break;
        }

}

void get_json_value(char *filename)
{
        FILE *fp;
        struct stat filestatus;
        int file_size;
        char* file_contents;
        json_char* json;
        json_value* value;
		FILE *file_temp = fopen(JSON_TEMP, "w+");

        if ( stat(filename, &filestatus) != 0) {
                fprintf(stderr, "File %s not found\n", filename);
               	exit(-1);
        }
        file_size = filestatus.st_size;
        file_contents = (char*)malloc(filestatus.st_size);
        if ( file_contents == NULL) {
                fprintf(stderr, "Memory error: unable to allocate %d bytes\n", file_size);
                exit(-1);
        }

        fp = fopen(filename, "rt");
        if (fp == NULL) {
                fprintf(stderr, "Unable to open %s\n", filename);
                fclose(fp);
                free(file_contents);
                exit(-1);
        }
        if ( fread(file_contents, file_size, 1, fp) != 1 ) {
                fprintf(stderr, "Unable t read content of %s\n", filename);
                fclose(fp);
                free(file_contents);
                exit(-1);
        }
        fclose(fp);

        json = (json_char*)file_contents;

        value = json_parse(json,file_size);

        if (value == NULL) {
                fprintf(stderr, "Unable to parse data\n");
                free(file_contents);
                exit(-1);
        }
		
        process_value(value, 0, file_temp);

        json_value_free(value);
        free(file_contents);
		fclose(file_temp);
		printf("Json read as %s\n", JSON_TEMP);
}
