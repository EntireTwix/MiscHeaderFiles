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
    explicit Mat(size_t x, size_t y);
    Mat(const Mat& mat);
    Mat(Mat&& mat);
    Mat operator=(const Mat& mat);
    Mat operator=(Mat&& mat);
    
    double& At(size_t x, size_t y);
    double At(size_t x, size_t y) const;

    size_t SizeX() const;
    size_t SizeY() const;

    Mat Dot(const Mat& mat) const;
    Mat VecMult(const Mat& mat) const;
    Mat DoubleVecMult(const Mat& mat) const;
    Mat Transpose() const;
    Mat CompoundingVecMult(const Mat& mat) const;

    template <typename... Params>
    void Func(void (*func)(double&), Params... p);
    void Func(void (*func)(double&));
    
    template <typename... Params>
    Mat Func(double (*func)(double), Params... p) const; 
    Mat Func(double (*func)(double)) const;
    Mat Func(const Mat& mat, double (*func)(double, double) ) const;

    Mat operator+(const Mat& mat) const;
    Mat operator-(const Mat& mat) const;
    Mat operator*(const Mat& mat) const;

    double* begin()
    {
        return &members[0];
    }
    double* end()
    {
        return &members[sizeX*sizeY];
    }

    friend std::ostream& operator<<(std::ostream& os, const Mat& mat)
    {
        for(size_t i = 0; i < mat.SizeY(); ++i)
        {
            for(size_t j = 0; j < mat.SizeX(); ++j)
                os<<(double)mat.At(j, i)<<' ';
            os<<'\n';
        }
        return os;
    }

    ~Mat();
};





















inline Mat::Mat(size_t x, size_t y) : sizeX(x), sizeY(y) 
{
    members = new double[sizeX*sizeY]{0};
}
inline Mat::Mat(const Mat& mat)
{
    sizeX = mat.sizeX;
    sizeY = mat.sizeY;
    members = new double[sizeX*sizeY];

    for(size_t i = 0; i < sizeY; ++i)
    for(size_t j = 0; j < sizeX; ++j)
        At(j, i) = mat.At(j, i);
}
inline Mat::Mat(Mat&& mat)
{
    sizeX = mat.sizeX;
    sizeY = mat.sizeY;
    members = std::move(mat.members);
    mat.members = nullptr;
}
inline Mat Mat::operator=(const Mat& mat)
{
    sizeX = mat.sizeX;
    sizeY = mat.sizeY;
    members = new double[sizeX*sizeY];
    for(size_t i = 0; i < sizeY; ++i)
    for(size_t j = 0; j < sizeX; ++j)
        At(j, i) = mat.At(j, i);
    return *this;
}
inline Mat Mat::operator=(Mat&& mat)
{
    sizeX = mat.sizeX;
    sizeY = mat.sizeY;
    members = std::move(mat.members);
    mat.members = nullptr;
    return *this;
}
inline double& Mat::At(size_t x, size_t y) //indexing matrix
{
    if( (x >= sizeX) || (y >= sizeY) ) throw std::out_of_range("At: out of range, "+std::to_string(x)+' '+std::to_string(y));
    return members[(y*sizeX)+x];
}
inline double Mat::At(size_t x, size_t y) const //indexing matrix
{
    if( (x >= sizeX) || (y >= sizeY) ) throw std::out_of_range("At: out of range, "+std::to_string(x)+' '+std::to_string(y));
    return members[(y*sizeX)+x];
}

inline size_t Mat::SizeX() const
{
    return sizeX;
}
inline size_t Mat::SizeY() const
{
    return sizeY;
}

inline Mat Mat::Dot(const Mat& mat) const
{
    if(sizeY != 1) throw std::invalid_argument("input must be a vector for weight layer");
    if(sizeX != mat.sizeY) throw std::invalid_argument("y of input must equal x of weight");
     
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
inline Mat Mat::VecMult(const Mat& mat) const
{
    Mat res(sizeX, sizeY);
    if(mat.sizeX != sizeY) throw std::invalid_argument("sizeX of the argument must meet the sizeY of the calling object");
    for(size_t i = 0; i < sizeY; ++i)
    for(size_t j = 0; j < sizeX; ++j)
        res.At(j, i) = At(j, i)*mat.At(i,0);
    return res; 
}
inline Mat Mat::DoubleVecMult(const Mat& mat) const
{
    Mat res(sizeX, mat.sizeX);
    if(mat.sizeY != sizeY) throw std::invalid_argument("sizeY of both must be 1");
    for(size_t i = 0; i < mat.sizeX; ++i)
    for(size_t j = 0; j < sizeX; ++j)
        res.At(j, i) = At(j, 0)*mat.At(i,0);
    return res;
}
inline Mat Mat::Transpose() const
{
    Mat res(sizeY, sizeX);
    for(size_t i = 0; i < sizeY; ++i)
    for(size_t j = 0; j < sizeX; ++j)
        res.At(i, j) = At(j, i);
    return res;
}

template <typename... Params>
inline void Mat::Func(void (*func)(double&), Params... p)
{
    for(double* start = begin(); start != end(); ++start)
        func(*start, p...);
}
void Mat::Func(void (*func)(double&))
{
    for(double* start = begin(); start != end(); ++start)
        func(*start);
}
    
template <typename... Params>
inline Mat Mat::Func(double (*func)(double), Params... p) const
{
    Mat res(SizeX(), SizeY());
    for(size_t i = 0; i < sizeY; ++i)
    for(size_t j = 0; j < sizeX; ++j)
        res.At(j, i) = func(At(j, i), p...);
    return res;
}
inline Mat Mat::Func(double (*func)(double)) const
{
    Mat res(SizeX(), SizeY());
    for(size_t i = 0; i < sizeY; ++i)
    for(size_t j = 0; j < sizeX; ++j)
        res.At(j, i) = func(At(j, i));
    return res;
}
inline Mat Mat::Func(const Mat& mat, double (*func)(double, double) ) const
{
    if( (mat.sizeX != sizeX) || (mat.sizeY != sizeY) ) throw std::invalid_argument("matrices must be the same dimensions");
    Mat res(SizeX(), SizeY());
    for(size_t i = 0; i < sizeY; ++i)
    for(size_t j = 0; j < sizeX; ++j)
        res.At(j, i) = func(At(j, i), mat.At(j, i));
    return res;
}

inline Mat Mat::operator+(const Mat& mat) const
{
    if( (mat.SizeX() != SizeX()) || (mat.SizeY() != SizeY()) ) throw std::invalid_argument("operator+: parameter must match the dimensions of this");
    Mat res(SizeX(), SizeY());
    for(size_t i = 0; i < sizeY; ++i)
    for(size_t j = 0; j < sizeX; ++j)
        res.At(j, i) = At(j, i)+mat.At(j, i);
    return res;
}
inline Mat Mat::operator-(const Mat& mat) const
{
    if( (mat.SizeX() != SizeX()) || (mat.SizeY() != SizeY()) ) throw std::invalid_argument("operator-: parameter must match the dimensions of this");
    Mat res(SizeX(), SizeY());
    for(size_t i = 0; i < sizeY; ++i)
    for(size_t j = 0; j < sizeX; ++j)
        res.At(j, i) = At(j, i)-mat.At(j, i);
    return res;
}
inline Mat Mat::operator*(const Mat& mat) const
{
    if( (mat.SizeX() != SizeX()) || (mat.SizeY() != SizeY()) ) throw std::invalid_argument("operator*: parameter must match the dimensions of this");
    Mat res(SizeX(), SizeY());
    for(size_t i = 0; i < sizeY; ++i)
    for(size_t j = 0; j < sizeX; ++j)
        res.At(j, i) = At(j, i)*mat.At(j, i);
    return res;
}

inline Mat::~Mat()
{
    delete[] members;
}

//make initilizer list constructor
