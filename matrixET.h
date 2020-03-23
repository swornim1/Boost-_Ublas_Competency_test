/**@file 
 * @brief Contains the Matrix implementaion  with overloaded + operation using expression templates
 *
 */


#ifndef MATRIXET_H
#define MATRIXET_H

#include <iostream>
#include <vector>
template <typename E>
class expr
	{
		public:
			
			int operator() (size_t x, size_t y) const
			{
				return static_cast<E const &> (*this)(x,y);
			}
			int rowCount() const { return static_cast<E const&> (*this).rowCount(); } 
	
			int colCount() const { return static_cast<E const&> (*this).colCount(); }
				
	};

template <typename t>
class MatrixET : public expr<MatrixET<t>>
{
	private:
		std::vector<std::vector<t>> container_;
		int row;
		int col;
	public:
		t operator()(size_t x, size_t y) const {
			return container_[x][y];
		}
		t &operator()(size_t x, size_t y) {
			return container_[x][y];}
	        
		int rowCount() const { return row; }
                int colCount() const { return col; }

		MatrixET(std::vector<std::vector<t>> matrix): container_(matrix), row(matrix.size()), col(matrix[0].size()){ };

		template <typename E>
		MatrixET(expr<E> const& exp) : row(exp.rowCount()), col(exp.colCount())   
		{
			for (size_t i=0; i< row; i++)
			{
				std::vector<t> temp;
				for(size_t j =0; j< col; j++)
				{	
					temp.push_back(exp(i,j));
				}
				container_.push_back(temp);
			}
		}
};

template <typename E1, typename E2>
class MatrixSum : public expr<MatrixSum<E1,E2>>
{
	E1 const& _u;
	E2 const& _v;

	public:
		MatrixSum(E1 const & u, E2 const& v): _u(u), _v(v) {}
		double operator()(size_t x, size_t y) const { 
			return _u(x,y) + _v(x,y) ; }
		int rowCount() const { return _v.rowCount(); }
		int colCount() const { return _v.colCount(); }
};

template <typename E1, typename E2>
MatrixSum<E1, E2>  
operator+(expr<E1> const &u, expr<E2> const &v)
{
	return MatrixSum<E1, E2>(*static_cast<const E1*>(&u), * static_cast<const E2*>(&v));
}

template<typename T>
void print(MatrixET<T> input)
{
	for(int i=0; i< input.rowCount(); i++)
	{
		for(int j=0; j < input.colCount(); j++)
		{
			std::cout<<input(i,j)<<"  "; 	
		}
			std::cout<<'\n';	
	}

}
#endif


