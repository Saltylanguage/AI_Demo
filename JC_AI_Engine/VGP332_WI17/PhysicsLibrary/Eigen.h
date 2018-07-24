#include <math.h>
#include "X\Inc\XEngine.h"

/* Eigen-decomposition for symmetric 3x3 real matrices.
Public domain, copied from the public domain Java library JAMA. */

// Disclaimer: I have no idea what this library is doing.
class Eigen
{
public:	
	static double Hypotenuse(double x, double y) { return sqrt(x*x + y*y); }
	static void Tred2(double V[3][3], double d[3], double e[3]);
	static void Tql2(double V[3][3], double d[3], double e[3]);
	static void eigen_decomposition(double A[3][3], double V[3][3], double d[3]);
	static void eigen_decomposition(X::Math::Matrix& A, X::Math::Matrix& V, X::Math::Vector3& d);
};



/* Symmetric matrix A => eigenvectors in columns of V, corresponding
eigenvalues in d. */

