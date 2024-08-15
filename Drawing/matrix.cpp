#include <cfloat>
#include <cmath>
#include <cstdlib>
#include "matrix.h"
#include <iostream>

using namespace std;

Vector::Vector() : data (new double[3])
{
	data[0] = 0;
	data[1] = 0;
	data[2] = 1.0;
}

Vector::Vector(const Vector& oldVector) : data (new double[3])
{
    for (int i = 0; i < 3; ++i) {
        data[i] = oldVector.data[i];
    }
}

Vector::Vector(const double x, const double y) : data (new double[3]) // allocates space for the vector but still have to initialize it
{
	data[0] = x;
	data[1] = y;
	data[2] = 1.0;
}


Vector::~Vector()
{
	delete[] data;
}

double& Vector::operator[](int index) const
{
	return data[index]; //gets data out of the vector
}

double* Vector::getData() const
{
    return data;
}

Matrix::Matrix() 
{
    data = new double*[3];

    for (int i = 0; i < 3; i++) 
    {
        data[i] = new double[3];

        for (int j = 0; j < 3; j++) 
        {
            data[i][j] = 0.0;
        }
        data[i][i] = 1.0;
    }
}

Matrix::Matrix(const Matrix& oldMatrix) 
{
    data = new double*[3];

    for (int i = 0; i < 3; ++i) 
    {
        data[i] = new double[3];

        for (int j = 0; j < 3; ++j) 
        {
            data[i][j] = oldMatrix.data[i][j];
        }
    }
}


Matrix::~Matrix()
{
	for (int i = 0; i < 3; i++) {
		delete[] data[i];
	}
    delete[] data;
}

Matrix* Matrix::multiply(const Matrix* otherMatrix) const
{
    Matrix* result = new Matrix();

    for (int i = 0; i < 3; ++i)
    {
        for (int j = 0; j < 3; ++j)
        {
            double sum = 0.0;
            for (int k = 0; k < 3; ++k)
            {
                sum += data[i][k] * (*otherMatrix)[k][j];
            }
            (*result)[i][j] = sum;
        }
    }

    return result;
}

Vector* Matrix::multiply(const Vector* theVector) const
{
    Vector* result = new Vector();

    for (int i = 0; i < 3; ++i)
    {
        double sum = 0.0;
        for (int j = 0; j < 3; ++j)
        {
            sum += data[i][j] * (*theVector)[j];
        }
        (*result)[i] = sum;
    }

    return result;
}

double* Matrix::operator[](int index) const
{
	return data[index];
}

Matrix* Matrix::translation(double deltaX, double deltaY)
{
	Matrix* translationMatrix = new Matrix();
    
    // Not working
    // translationMatrix->data[0][0] = 1.0;
    // translationMatrix->data[0][1] = 0.0;
    // translationMatrix->data[0][2] = deltaX;
    // translationMatrix->data[1][0] = 0.0;
    // translationMatrix->data[1][1] = 1.0;
    // translationMatrix->data[1][2] = deltaY;
    // translationMatrix->data[2][0] = 0.0;
    // translationMatrix->data[2][1] = 0.0;
    // translationMatrix->data[2][2] = 1.0;

for (int i = 0; i < 3; i++) {
    for (int j = 0; j < 3; j++){
        if (i == j) {
            (*translationMatrix)[i][j] = 1.0;
        } else {
            (*translationMatrix)[i][j] = 0.0;
        }
    }
}

(*translationMatrix)[0][2] = deltaX;
(*translationMatrix)[1][2] = deltaY;

return translationMatrix;

}

Matrix* Matrix::rotation(double theta)
{
    Matrix* rotationMatrix = new Matrix();

    double cosTheta = cos(theta);
    double sinTheta = sin(theta);

    (*rotationMatrix)[0][0] = cosTheta;
    (*rotationMatrix)[0][1] = -sinTheta;
    (*rotationMatrix)[1][0] = sinTheta;
    (*rotationMatrix)[1][1] = cosTheta;
    (*rotationMatrix)[2][2] = 1.0;

    return rotationMatrix;
}

Matrix* Matrix::shearing(double shearXY, double shearYX)
{
    Matrix* shearingMatrix = new Matrix();

    (*shearingMatrix)[0][0] = 1.0;
    (*shearingMatrix)[0][1] = shearXY;
    (*shearingMatrix)[1][0] = shearYX;
    (*shearingMatrix)[1][1] = 1.0;
    (*shearingMatrix)[2][2] = 1.0;

    return shearingMatrix;
}



Matrix* Matrix::scaling(double scaleX, double scaleY)
{
    Matrix* scalingMatrix = new Matrix();

    (*scalingMatrix)[0][0] = scaleX;
    (*scalingMatrix)[1][1] = scaleY;

    return scalingMatrix;
}


Matrix* Matrix::getInverse() const
{
	Matrix* answer = new Matrix();
	double det = getDeterminant();

	answer->data[0][0] = -data[1][2] * data[2][1] + data[1][1] * data[2][2];
	answer->data[0][1] = data[0][2] * data[2][1] - data[0][1] * data[2][2];
	answer->data[0][2] = -data[0][2] * data[1][1] + data[0][1] * data[1][2];
	answer->data[1][0] = data[1][2] * data[2][0] - data[1][0] * data[2][2];
	answer->data[1][1] = -data[0][2] * data[2][0] + data[0][0] * data[2][2];
	answer->data[1][2] = data[0][2] * data[1][0] - data[0][0] * data[1][2];
	answer->data[2][0] = -data[1][1] * data[2][0] + data[1][0] * data[2][1];
	answer->data[2][1] = data[0][1] * data[2][0] - data[0][0] * data[2][1];
	answer->data[2][2] = -data[0][1] * data[1][0] + data[0][0] * data[1][1];

	for (int i = 0; i<3; i++)
		for (int j = 0; j<3; j++)
			answer->data[i][j] /= det;

	return answer;
}

double Matrix::getDeterminant() const
{
	return  -data[0][2] * data[1][1] * data[2][0] + data[0][1] * data[1][2] * data[2][0] +
		data[0][2] * data[1][0] * data[2][1] - data[0][0] * data[1][2] * data[2][1] -
		data[0][1] * data[1][0] * data[2][2] + data[0][0] * data[1][1] * data[2][2];
}
