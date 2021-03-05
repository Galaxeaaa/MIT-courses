#ifndef _IFS_H_
#define _IFS_H_

#include "image.h"
#include "matrix.h"
#include <cstring>

class IFS
{
    int n_trans;
    Matrix *M;
    float *probability;

public:
    IFS(char *input_file)
    {
        // open the file
        FILE *input = fopen(input_file, "r");
        assert(input != NULL);

        // read the number of transforms
        int num_transforms;
        fscanf(input, "%d", &num_transforms);
        M = new Matrix[num_transforms];
        probability = new float[num_transforms];

        // read in the transforms
        for (int i = 0; i < num_transforms; i++)
        {
            float p;
            fscanf(input, "%f", &p);
            Matrix m;
            m.Read3x3(input);
            // < DO SOMETHING WITH probability and m >
            readTransformation(i, m, p);
        }

        fclose(input);
    };
    ~IFS()
    {
        delete[] M;
        delete[] probability;
    }
    Matrix getMatrix(int i)
    {
        return M[i];
    }
    void readTransformation(int i, Matrix m, float p)
    {
        M[i] = m;
        probability[i] = p;
    }
    int getTransformationIndex(float p);
    void render(char *outputfile, int n_points, int n_iters, int size);
};

#endif