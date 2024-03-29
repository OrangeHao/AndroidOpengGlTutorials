//
// Created by chenzihao on 2019/6/26.
//


#ifndef MATRIX_H
#define MATRIX_H

#include <cmath>

/**
 * \brief Takes a 4 * 4 and sets the elements to the Identity function.
 * \param[out] matrix Pointer to an array of a 4 * 4 matrix. Must be at least 16 elements long.
 */
extern void matrixIdentityFunction(float *matrix);

/**
 * \brief Takes in a 4 * 4 matrix and translates it by the vector defined by x y and z
 * \param[in,out] matrix Pointer to an array of a 4 * 4 matrix. Must be at least 16 elements long
 * \param[in] x X component of the translation vector.
 * \param[in] y Y component of the translation vector.
 * \param[in] z Z component of the translation vector.
 */
extern void matrixTranslate(float *matrix, float x, float y, float z);

/**
 * \brief Takes 2 matrices and multiplies them together. Then stores the result in a third matrix.
 * \param[out] destination A pointer to the array the result of the multiplication should be stored.
 * \param[in] operand1 A pointer to the first matrix to be multiplied.
 * \param[in] operand2 A pointer to the second matrix to be multiplied.
 */
extern void matrixMultiply(float *destination, float *operand1, float *operand2);

/**
 * \brief Create a viewing frustum and store the result in the first parameter. This is usually called by matrixPerspective.
 * \param[out] matrix Destination of the frustum.
 * \param[in] left Left side of the frustum.
 * \param[in] right Right side of the frustum.
 * \param[in] bottom Bottom of the frustum.
 * \param[in] top Top of the frustum.
 * \param[in] zNear Near visible distance of the frustum.
 * \param[in] zFar Far visible distance of the frustum.
 */
extern void matrixFrustum(float *matrix, float left, float right, float bottom, float top, float zNear, float zFar);

/**
 * \brief Create a perspective projection matrix and store the results in the first parameter.
 * \param[out] matrix Destination of the perspective projection matrix.
 * \param[in] fieldOfView The field of view in degrees. How far you expect the camera to see in degrees.
 * \param[in] aspectRatio The aspect ratio of your viewport.
 * \param[in] zNear How close objects can be to the camera before they disappear.
 * \param[in] zFar How far away objects can be before they are no longer drawn.
 */
extern void matrixPerspective(float *matrix, float fieldOfView, float aspectRatio, float zNear, float zFar);

/**
 * \brief Rotates a matrix around the x axis by a given angle.
 * \param[in,out] matrix A pointer to the matrix to be rotated
 * \param[in] angle A float representing the angle to rotate by in degrees.
 */
extern void matrixRotateX(float *matrix, float angle);

/**
 * \brief Rotates a matrix around the y axis by a given angle.
 * \param[in,out] matrix A pointer to the matrix to be rotated
 * \param[in] angle A float representing the angle to rotate by in degrees.
 */
extern void matrixRotateY(float *matrix, float angle);

/**
 * \brief Rotates a matrix around the Z axis by a given angle.
 * \param[in,out] matrix A pointer to the matrix to be rotated
 * \param[in] angle A float representing the angle to rotate by in degrees.
 */
extern void matrixRotateZ(float *matrix, float angle);

/**
 * \brief Scales a matrix by a given factor in the x, y and z axis
 * \param[in,out] matrix A pointer to the matrix to be scaled.
 * \param[in] x Scaling factor in the X axis.
 * \param[in] y Scaling factor in the Y axis.
 * \param[in] z Scaling factor in the Z axis.
 */
extern void matrixScale(float *matrix, float x, float y, float z);

/**
 * \brief Function to convert degrees into Radians
 * \param[in] degrees Angle to be converted.
 * \return Converted angle in Radians.
 */
extern float matrixDegreesToRadians(float degrees);


#endif //OPENGLDEMOJAVA_MATRIX_H
