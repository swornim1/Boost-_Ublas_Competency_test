/**@file
 * @brief Contains the Matrix implementaion  with overloaded + operation using constexpr
 * 
 **/
#ifndef MATRIXCPP20_H
#define MATRIXCPP20_H
#include <cstddef>
#include <type_traits>
#include <tuple>
#include <array>
#include <iostream>
template <typename T, T... v> struct all_same_value {};

template <typename T, T v> struct all_same_value<T,v>
{
	static constexpr T value = v;
};


template <typename T, std::size_t N, std::size_t M> 
struct MatrixCPP20
{
	static_assert(N  != 0 && M != 0, "Matrix must have  positive dimensions");
	using size_type  = std::size_t;
	using value_type = T;
	static constexpr size_type column_size = N;
	static constexpr size_type row_size = M;

	T arrays[N][M];      		
	constexpr T* operator[](std::size_t i)
	{
		return arrays[i];
	}
	constexpr T const *operator[](std::size_t i) const { return arrays[i]; }	 
};

//Deduction guide for aggregate initialization
template <typename T, std::size_t M , std::size_t N>
MatrixCPP20(const T(&)[M][N])->MatrixCPP20<T,M,N>;

template <
	typename F, typename T, typename... Matrices,
	typename U = std::invoke_result_t<F, T, typename Matrices::value_type...>,
	std::size_t N = all_same_value<std::size_t, Matrices::column_size...>::value,
	std::size_t M = all_same_value<std::size_t, Matrices::row_size...>::value>
constexpr MatrixCPP20<U, N, M> elementwise(F f, const MatrixCPP20<T, N, M> & m, const Matrices &...matrices)
{
	MatrixCPP20<U,N,M> op_applied = { };
	for ( std::size_t i=0; i<N; i++)
	{
		for( std::size_t j=0; j<M; j++){
			op_applied[i][j] = std::apply(f, std::forward_as_tuple(m[i][j], matrices[i][j]...));
		}
	}
	return op_applied;	

}


template <typename T, std::size_t N, std::size_t M>
constexpr MatrixCPP20<T, N, M> operator+(const MatrixCPP20<T, N, M> &a, const MatrixCPP20<T, N, M> &b)
{
	return elementwise(std::plus<T>(),a,b);
}

template <typename T, std::size_t N, std::size_t M>
void print(MatrixCPP20<T, N, M> & a)
{
	for(int i =0; i<N; i++)
	{
		for(int j=0; j<M; j++)
		{
			std::cout<<a[i][j]<<"  ";		
		}
		std::cout<<'\n';
	}
}
#endif
