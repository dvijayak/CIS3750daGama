#ifndef MatrixF_hpp
#define MatrixF_hpp

#include "global.hpp"

#include "Vector3F.hpp"

// INFO: For using templates and friends, see http://isocpp.org/wiki/faq/templates#template-friends

template <size_t M, size_t N>
class MatrixF;

template <size_t M, size_t N>
std::ostream& operator<< (std::ostream&, const MatrixF<M, N>&);

// Useful typedefs
typedef MatrixF<3, 1> ColMatrix3F;
typedef MatrixF<1, 3> RowMatrix3F;
typedef MatrixF<2, 2> Matrix2F;
typedef MatrixF<3, 3> Matrix3F;
typedef MatrixF<4, 4> Matrix4F;
	
template <size_t M, size_t N>
class MatrixF
{
public:
	typedef std::array<float, M*N> values_type;

public:
	// By default, we construct the identity matrix
	MatrixF ();
	// Construct a matrix filled with the given float
	MatrixF (const float);
	// Construct a matrix with the given values
	MatrixF (const values_type&);
	// Construct a matrix from the given 3D vector
	MatrixF (const Vector3F&);
	MatrixF (const MatrixF<M, N>& other) : m_nRows(other.m_nRows), m_nCols(other.m_nCols) { *this = other; }
	MatrixF& operator= (const MatrixF<M, N>&);
	~MatrixF () {}

	// Standard arithmetic ops
	const MatrixF operator+ (const MatrixF<M, N>&) const;
	const MatrixF operator- (const MatrixF<M, N>&) const;
	const MatrixF operator* (const float) const;
	const MatrixF operator/ (const float) const;
	// NOTE: Matrix-matrix multiplication has been specially implemented via template
	// specialization later in this file

	// Accesors/Mutators
	inline void FillValues (const float value) { for (float& val: m_values) { val = value;	} }
	inline const values_type GetValues () const { return m_values; }
	inline float GetDeterminant () const { return m_determinant; }
	// Indexing operators...really important
	inline float operator[] (size_t i) const { assert(i < M*N); return m_values[i]; } // ASSERT: Array out of bounds. I prefer this over an exception since this should be a developer error, not a runtime error.
	// A convenience for double-array like indexing and setting
	// NOTE: When efficiency is prime, operator[] may be preferably (need to confirm)
	inline float& operator() (size_t i, size_t j) { assert(i < M && j < N); return m_values[i*N + j]; } // ASSERT: Ditto

	friend std::ostream& operator<< <> (std::ostream&, const MatrixF<M, N>&);

private:
	// Must be called whenever the matrix values change
	void Update ();

	// Calculate the determinant of the matrix
	void ComputeDeterminant ();

	const size_t m_nRows;
	const size_t m_nCols;

	values_type m_values{}; // value-initialization
	float m_determinant;
};

template <size_t M, size_t N>
MatrixF<M, N>::MatrixF ()
	: m_nRows(M)
	, m_nCols(N)
{
	// ASSERT: It makes no sense to have no rows or no columns (i.e., size 0)
	assert(m_nRows && m_nCols);

	int i = 0;
	for (float& val: m_values)
	{
		if (m_nRows == m_nCols)
		{
			val = i % (m_nRows+1) ? 0.0 : 1.0;
		}
		else
		{
			val = 0.0;
		}

		++i;
	}

	Update();
}

template <size_t M, size_t N>
MatrixF<M, N>::MatrixF (const float value)
	: m_nRows(M)
	, m_nCols(N)
{
	// ASSERT: It makes no sense to have no rows or no columns (i.e., size 0)
	assert(m_nRows && m_nCols);

	FillValues(value);

	Update();
}

template <size_t M, size_t N>
MatrixF<M, N>::MatrixF (const MatrixF<M, N>::values_type& values)
	: m_nRows(M)
	, m_nCols(N)
{
	// ASSERT: It makes no sense to have no rows or no columns (i.e., size 0)
	assert(m_nRows && m_nCols);
	// Also, note that equality of dimensions is guaranteed via template specialization

	std::copy( values.begin(), values.end(), m_values.begin() );
	
	Update();
}

template <size_t M, size_t N>
MatrixF<M, N>::MatrixF (const Vector3F& v)
	: m_nRows(M)
	, m_nCols(N)
{
	// ASSERT: A Vector3F must only be understood as a 3 x 1 or 1 x 3 matrix
	assert( (m_nRows == 3 && m_nCols == 1) || (m_nRows == 1 && m_nCols == 3) );

	m_values[0] = v.GetX();
	m_values[1] = v.GetY();
	m_values[2] = v.GetZ();

	Update();
}


template <size_t M, size_t N>
MatrixF<M, N>& MatrixF<M, N>::operator= (const MatrixF<M, N>& other)
{
	// ASSERT: Both matrices must be of identical dimensions for a reliable copy
	// Technically, this is already guaranteed due to template specialization
	assert(m_nRows == other.m_nRows && m_nCols == other.m_nCols);

	std::copy( other.m_values.begin(), other.m_values.end(), m_values.begin() );

	Update();

	return *this;
}


template <size_t M, size_t N>
void MatrixF<M, N>::Update ()
{
	ComputeDeterminant();
}

template <size_t M, size_t N>
inline void MatrixF<M, N>::ComputeDeterminant ()
{
	m_determinant = 0.0;
}


template <size_t M, size_t N>
const MatrixF<M, N> MatrixF<M, N>::operator+ (const MatrixF<M, N>& other) const
{
	MatrixF<M, N>::values_type values;
	for (size_t i = 0; i < M*N; ++i)
	{
		values[i] = m_values[i] + other.m_values[i];
	}
	return MatrixF<M, N>(values);
}

template <size_t M, size_t N>
const MatrixF<M, N> MatrixF<M, N>::operator- (const MatrixF<M, N>& other) const
{
	MatrixF<M, N>::values_type values;
	for (size_t i = 0; i < M*N; ++i)
	{
		values[i] = m_values[i] - other.m_values[i];
	}
	return MatrixF<M, N>(values);
}

// Ye ol' multiplication between matrices
template <size_t M, size_t N, size_t O>
const MatrixF<M, O> operator* (const MatrixF<M, N>& A, const MatrixF<N, O>& B)
{
	// We use the beauty of template specialization to implement two matrix
	// multiplication properties at compile-time:
	// 1) Two matrices A and B can only be multipled iff A.m_nCols == B.m_nRows
	// 2) The resultant matrix of the above operation is of dimensions A.m_nCols x B.m_nRows
	//
	// Note that this operator overload is separated from the class. This is needed
	// in order to avoid template parameter redeclaration.

	// INFO: Why use 'typename' here? See http://stackoverflow.com/questions/5524744/typedef-inside-template-class-doesnt-work
	typename MatrixF<M, N>::values_type values_A = A.GetValues();
	typename MatrixF<N, O>::values_type values_B = B.GetValues();
	typename MatrixF<M, O>::values_type values;
	for (size_t i = 0; i < M; ++i)
	{
		for (size_t j = 0; j < O; ++j)
		{
			float sum = 0.0;
			for (size_t k = 0; k < N; ++k)
			{
				sum += values_A[i*N + k] * values_B[k*O + j];
			}
			values[i*O + j] = sum;
		}
	}
	return MatrixF<M, O>(values);
}

template <size_t M, size_t N>
const MatrixF<M, N> MatrixF<M, N>::operator* (const float scalar) const
{
	MatrixF<M, N>::values_type values;
	for (size_t i = 0; i < M*N; ++i)
	{
		values[i] = m_values[i] * scalar;
	}
	return MatrixF<M, N>(values);
}

template <size_t M, size_t N>
const MatrixF<M, N> MatrixF<M, N>::operator/ (const float scalar) const
{
	MatrixF<M, N>::values_type values;
	for (size_t i = 0; i < M*N; ++i)
	{
		values[i] = m_values[i] / scalar;
	}
	return MatrixF<M, N>(values);
}


template <size_t M, size_t N>
std::ostream& operator<< (std::ostream& os, const MatrixF<M, N>& m)
{
	os << std::endl;
	os << "[ Determinant = " << m.m_determinant << "  ";
	
	size_t i = 0;
	for (const float val: m.m_values)
	{
		if (i % m.m_nCols == 0)
		{
			os << std::endl << "  ";
		}

		os << val << " ";

		i++;
	}
	os << "]";

	return os;
}

#endif
