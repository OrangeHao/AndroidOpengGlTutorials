//
// Created by chenzihao on 2019/6/26.
//

#include "matrix.h"


/* [matrixIdentity] */
void matrixIdentityFunction(float* matrix)
{
    if(matrix == NULL)
    {
        return;
    }

    matrix[0] = 1.0f;
    matrix[1] = 0.0f;
    matrix[2] = 0.0f;
    matrix[3] = 0.0f;
    matrix[4] = 0.0f;
    matrix[5] = 1.0f;
    matrix[6] = 0.0f;
    matrix[7] = 0.0f;
    matrix[8] = 0.0f;
    matrix[9] = 0.0f;
    matrix[10] = 1.0f;
    matrix[11] = 0.0f;
    matrix[12] = 0.0f;
    matrix[13] = 0.0f;
    matrix[14] = 0.0f;
    matrix[15] = 1.0f;
}
/* [matrixIdentity] */
/* [matrixTranslate] */
void matrixTranslate(float* matrix, float x, float y, float z)
{
    float temporaryMatrix[16];
    matrixIdentityFunction(temporaryMatrix);
    temporaryMatrix[12] = x;
    temporaryMatrix[13] = y;
    temporaryMatrix[14] = z;
    matrixMultiply(matrix,temporaryMatrix,matrix);
}
/* [matrixTranslate] */

/* [matrixMultiply] */
void matrixMultiply(float* destination, float* operand1, float* operand2)
{
    float theResult[16];
    int row, column = 0;
    int i,j = 0;
    for(i = 0; i < 4; i++)
    {
        for(j = 0; j < 4; j++)
        {
            theResult[4 * i + j] = operand1[j] * operand2[4 * i] + operand1[4 + j] * operand2[4 * i + 1] +
                                   operand1[8 + j] * operand2[4 * i + 2] + operand1[12 + j] * operand2[4 * i + 3];
        }
    }

    for(int i = 0; i < 16; i++)
    {
        destination[i] = theResult[i];
    }
}
/* [matrixMultiply] */

/* [matrixFrustum] */
void matrixFrustum(float* matrix, float left, float right, float bottom, float top, float zNear, float zFar)
{
    float temp, xDistance, yDistance, zDistance;
    temp = 2.0 *zNear;
    xDistance = right - left;
    yDistance = top - bottom;
    zDistance = zFar - zNear;
    matrixIdentityFunction(matrix);
    matrix[0] = temp / xDistance;
    matrix[5] = temp / yDistance;
    matrix[8] = (right + left) / xDistance;
    matrix[9] = (top + bottom) / yDistance;
    matrix[10] = (-zFar - zNear) / zDistance;
    matrix[11] = -1.0f;
    matrix[14] = (-temp * zFar) / zDistance;
    matrix[15] = 0.0f;
}
/* [matrixFrustum] */

/* [matrixPerspective] */
/*
 * The first parameter that we supply is the matrix that we want to do the projection with. This is like most of the functions we have already described. The second is the Field of View. This is angle that you should be able to see through. 45 Degrees is pretty standard but when you have finished the tutorial feel free to adjust to see the effect. It is the best way to learn.

The next parameter is the aspect ratio. This is usually the width / height of your screen or surface. You wouldn't need this parameter if all screen ratios were the same but unfortunately they are not. This helps the application to look correct on a variety of devices. Which is very important considering the vast array of different devices that are available in the mobile space.

The final two parameters are called zNear and zFar. zNear is how close an object has to be to the camera before it disappears and gets clipped. zFar is the opposite; how far away an object should be from the camera before it is no longer drawn.
 * */
void matrixPerspective(float* matrix, float fieldOfView, float aspectRatio, float zNear, float zFar)
{
    float ymax, xmax;
    ymax = zNear * tanf(fieldOfView * M_PI / 360.0);
    xmax = ymax * aspectRatio;
    matrixFrustum(matrix, -xmax, xmax, -ymax, ymax, zNear, zFar);
}
/* [matrixPerspective] */

/* [matrixRotate] */
void matrixRotateX(float* matrix, float angle)
{
    float tempMatrix[16];
    matrixIdentityFunction(tempMatrix);

    tempMatrix[5] = cos(matrixDegreesToRadians(angle));
    tempMatrix[9] = -sin(matrixDegreesToRadians(angle));
    tempMatrix[6] = sin(matrixDegreesToRadians(angle));
    tempMatrix[10] = cos(matrixDegreesToRadians(angle));
    matrixMultiply(matrix, tempMatrix, matrix);
}

void matrixRotateY(float *matrix, float angle)
{
    float tempMatrix[16];
    matrixIdentityFunction(tempMatrix);

    tempMatrix[0] = cos(matrixDegreesToRadians(angle));
    tempMatrix[8] = sin(matrixDegreesToRadians(angle));
    tempMatrix[2] = -sin(matrixDegreesToRadians(angle));
    tempMatrix[10] = cos(matrixDegreesToRadians(angle));
    matrixMultiply(matrix, tempMatrix, matrix);
}

void matrixRotateZ(float *matrix, float angle)
{
    float tempMatrix[16];
    matrixIdentityFunction(tempMatrix);

    tempMatrix[0] = cos(matrixDegreesToRadians(angle));
    tempMatrix[4] = -sin(matrixDegreesToRadians(angle));
    tempMatrix[1] = sin(matrixDegreesToRadians(angle));
    tempMatrix[5] = cos(matrixDegreesToRadians(angle));
    matrixMultiply(matrix, tempMatrix, matrix);
}
/* [matrixRotate] */

/* [matrixScale] */
void matrixScale(float* matrix, float x, float y, float z)
{
    float tempMatrix[16];
    matrixIdentityFunction(tempMatrix);

    tempMatrix[0] = x;
    tempMatrix[5] = y;
    tempMatrix[10] = z;
    matrixMultiply(matrix, tempMatrix, matrix);
}
/* [matrixScale] */

float matrixDegreesToRadians(float degrees)
{
    return M_PI * degrees / 180.0f;
}