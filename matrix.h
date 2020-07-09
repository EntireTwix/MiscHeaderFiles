#pragma once
#include <stdexcept>
#include <ostream>

class Mat
{
private:
    double* members = nullptr;
    size_t sizeX = 0, sizeY = 0;
public:
    Mat() = default;
    explicit Mat(size_t x, size_t y) : sizeX(x), sizeY(y) 
    {
        members = new double[sizeX*sizeY]{0};
    }
    Mat(const Mat& mat)
    {
        sizeX = mat.sizeX;
        sizeY = mat.sizeY;
        members = new double[sizeX*sizeY];
        for(size_t i = 0; i < sizeX; ++i)
        for(size_t j = 0; j < sizeY; ++j)
            At(i, j) = mat.At(i, j);
    }
    Mat(Mat&& mat)
    {
        sizeX = mat.sizeX;
        sizeY = mat.sizeY;
        members = std::move(mat.members);
        mat.members = nullptr;
    }
    Mat operator=(const Mat& mat)
    {
        sizeX = mat.sizeX;
        sizeY = mat.sizeY;
        members = new double[sizeX*sizeY];
        for(size_t i = 0; i < sizeX; ++i)
        for(size_t j = 0; j < sizeY; ++j)
            At(i, j) = mat.At(i, j);
        return *this;
    }
    Mat operator=(Mat&& mat)
    {
        sizeX = mat.sizeX;
        sizeY = mat.sizeY;
        members = std::move(mat.members);
        mat.members = nullptr;
        return *this;
    }
    
    double& At(size_t x, size_t y) //indexing matrix
    {
        return members[(y*sizeX)+x];
    }
    double At(size_t x, size_t y) const //indexing matrix
    {
        return members[(y*sizeX)+x];
    }

    size_t SizeX() const
    {
        return sizeX;
    }
    size_t SizeY() const
    {
        return sizeY;
    }

    Mat Dot(const Mat& mat) const
    {
        Mat res(mat.sizeX, sizeY); //product dimensions are the x of both mats
		double amount;
		for(size_t i = 0; i < sizeY; ++i) //for each y on the A matrix 
		for(size_t j = 0; j < mat.sizeX; ++j) //for each x on the B matrix 
		{
			amount = 0;
			for(int k = 0; k < sizeX; ++k) //is equivelant to mat.sizeY() 
				amount+=At(k, i)*mat.At(j, k); 
			res.At(j, i) = amount;
		}
		return res;
    }

    friend std::ostream& operator<<(std::ostream& os, const Mat& mat)
    {
        for(size_t i = 0; i < mat.SizeX(); ++i)
        {
            for(size_t j = 0; j < mat.SizeY(); ++j)
            {
                os<<(double)mat.At(i, j)<<' ';
            }
            os<<'\n';
        }
        return os;
    }

    ~Mat()
    {
        delete[] members;
    }
};