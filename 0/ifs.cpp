#include "ifs.h"
#include <time.h>

int IFS::getTransformationIndex(float p)
{
    float p_upb = 0;
    for (int i = 0; i < n_trans; i++)
    {
        p_upb += probability[i];
        if (p <= p_upb)
            return i;
    }
    return -1;
}

void IFS::render(char *outputfile, int n_points, int n_iters, int size)
{
    int width = size, height = size;
    Vec3f color(0.24, 0.48, 0.27);
    Vec3f white(1.0, 1.0, 1.0);
    Image img(width, height);
    img.SetAllPixels(white);

    float coordinate_x = 0.0;
    float coordinate_y = 0.0;
    Vec3f iteratedPoint(0, 0, 0);

    srand(time(NULL));
    for (int i = 0; i < n_points; i++)
    {
        coordinate_x = rand() / (double) RAND_MAX;
        coordinate_y = rand() / (double) RAND_MAX;
        iteratedPoint.Set(coordinate_x, coordinate_y, 0.0);
        for (int j = 0; j < n_iters; j++)
        {
            int indexProbability = 0;
            float probability = rand() / (double) RAND_MAX;
            int transIndex = getTransformationIndex(probability);
            Matrix m = getMatrix(transIndex);
            m.Transform(iteratedPoint);
        }
        coordinate_x = iteratedPoint[0];
        coordinate_y = iteratedPoint[1];
        if (coordinate_x >= 0 && coordinate_x < 1 && coordinate_y >= 0 && coordinate_y < 1)
        {
            img.SetPixel(coordinate_x * width, coordinate_y * height, color);
        }
    }
    img.SaveTGA(outputfile);
}