#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <time.h>
#include <pthread.h>

#include "primitives.h"
#include "raytracing.h"

#define OUT_FILENAME "out.ppm"

#define ROWS 512
#define COLS 512

static void write_to_ppm(FILE *outfile, uint8_t *pixels,
                         int width, int height)
{
    fprintf(outfile, "P6\n%d %d\n%d\n", width, height, 255);
    fwrite(pixels, 1, height * width * 3, outfile);
}

static double diff_in_second(struct timespec t1, struct timespec t2)
{
    struct timespec diff;
    if (t2.tv_nsec-t1.tv_nsec < 0) {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec - 1;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec + 1000000000;
    } else {
        diff.tv_sec  = t2.tv_sec - t1.tv_sec;
        diff.tv_nsec = t2.tv_nsec - t1.tv_nsec;
    }
    return (diff.tv_sec + diff.tv_nsec / 1000000000.0);
}

int main()
{
    arg data;
    light_node lights = NULL;
    rectangular_node rectangulars = NULL;
    sphere_node spheres = NULL;

#include "use-models.h"

    struct timespec start, end;

    data.lights = lights;
    data.rectangulars = rectangulars;
    data.spheres = spheres;
    data.View = &view;
    data.col = COLS;
    data.row = ROWS;

    /* allocate by the given resolution */
    data.pixels = malloc(sizeof(unsigned char) * ROWS * COLS * 3);
    if (!data.pixels) exit(-1);
    printf("# Rendering scene\n");

    /* do the ray tracing with the given geometry */
    clock_gettime(CLOCK_REALTIME, &start);

    for(int i=0; i<4; i++) {
        if(pthread_create(&THREAD[i],NULL,(void*)raytracing,(void*)&data)) {
            printf("ERROR thread create!\n");
            pthread_exit(NULL);
        }
    }

    for(int i=0; i<4; i++)
        pthread_join(THREAD[i],NULL);

    clock_gettime(CLOCK_REALTIME, &end);
    {
        FILE *outfile = fopen(OUT_FILENAME, "wb");
        write_to_ppm(outfile, data.pixels, ROWS, COLS);
        fclose(outfile);
    }

    delete_rectangular_list(&data.rectangulars);
    delete_sphere_list(&data.spheres);
    delete_light_list(&data.lights);
    free(data.pixels);

    printf("Done!\n");
    printf("Execution time of raytracing() : %lf sec\n", diff_in_second(start, end));
    return 0;
}
