// matrix_mult_no_pntrs.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"
#include <hpx/hpx_main.hpp>
#include <hpx/include/lcos.hpp>
#include <hpx/include/actions.hpp>
#include <hpx/include/components.hpp>
#include <hpx/include/iostreams.hpp>
#include <hpx/parallel/algorithms/for_loop.hpp>

#include <boost/format.hpp>
#include <boost/container/vector.hpp>

#include <list>
#include <set>
#include <mutex>
#include <iostream>
#include <istream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>

#include <hpx/hpx_init.hpp>
#include <hpx/hpx.hpp>
#include <hpx/util/lightweight_test.hpp>

#include <hpx/include/parallel_for_loop.hpp>

#include <algorithm>
#include <cstddef>
#include <numeric>
#include <string>
#include <utility>
#include <vector>

#include "test_utils.hpp"

///////////////////////////////////////////////////////////////////////////////





bool debug = false;

std::vector<double> get_col(const std::vector< std::vector<double> >& data,
	int col);

double dot_product(const std::vector<double> row,
	const std::vector<double> columns);

void usage(char* func_name) {
	std::cout << "usage: " << func_name << " double1 double2 double3 double4"
		<< std::endl;
}

void rules() {
	std::cout << "For matrix multiplication to be possible, with matrices" <<
		" X(x_1,x_2) and Y(y_1,y_2), for X*Y x_2 MUST equal y_1, and" <<
		" vice versa" << std::endl;
}


double dot_product(const std::vector<double> row,
	const std::vector<double> columns) {
	double sum = 0;
	for (int i = 0; i < row.size(); i++) {
		sum += row.at(i) * columns.at(i);
	}
	return sum;
}
HPX_PLAIN_ACTION(dot_product, dot_product_action);

std::vector< std::vector<double> > matrix_gen(int dim_on, int dim_tw) {
	//std::cout << dim_on << " " << dim_tw << std::endl;
	std::vector< std::vector<double > > data;
	data.reserve(dim_on);
	//std::cout << data.capacity() << std::endl;
	for (int i = 0; i < dim_on; i++) {
		std::vector<double > k;
		data.push_back(k);
		data.at(i).reserve(dim_tw);
	}
	return data;
}

std::vector< std::vector < double > >
rand_filler(int dim_one, int dim_two) {
	std::vector< std::vector< double > > data =
		matrix_gen(dim_one, dim_two);
	for (int i = 0; i < data.capacity(); i++) {
		data.at(0);
		data.at(i).clear();
		if(debug)
			std::cout << "[";
		for (int j = 0; j < data.at(0).capacity(); j++) {
			data.at(i).push_back((double)(rand() % 100));
			if(debug)
				std::cout << data.at(i).at(j) << " ";
		}
		if(debug)
			std::cout << "]" << std::endl;

	}
	return data;
}
HPX_PLAIN_ACTION(rand_filler, rand_filler_action);

std::vector< double > get_col(const std::vector< std::vector< double > >& data,
	int col)
{
	std::vector< double > column;
	column.reserve(data.capacity());
	for (int i = 0; i < data.capacity(); i++)
	{
		column.push_back(data.at(i).at(col));
	}
	return column;
}



template <typename ExPolicy, typename IteratorTag>
void test_for_loop(ExPolicy && policy, IteratorTag)
{
	static_assert(
		hpx::parallel::is_execution_policy<ExPolicy>::value,
		"hpx::parallel::is_execution_policy<ExPolicy>::value");

	typedef std::vector<std::vector<double>>::iterator base_iterator;
	typedef test::test_iterator<base_iterator, IteratorTag> iterator;
	//IteratorTag = std::forward_iterator_tag()
	//iterator = test::test_iterator<std::vector<std::vector<double>>::iterator, std::forward_iterator_tag()>
	/*
	
	
	
	
	std::vector<std::vector<double>> c(10);
	for (int i = 0; i < c.size(); i++) {
		std::vector<double> k;
		for (int j = i; j < 5; j++) {
			k.push_back(j);
		}
		c.push_back(k);
	}
	std::cout << "First round!" << std::endl;
	for (int i = 0; i < c.size(); i++) {
		std::cout << i << "\n\t" << c.at(i).size() << std::endl;
		
	}
	int f;
	hpx::parallel::for_loop(
		std::forward<ExPolicy>(policy),
		iterator(boost::begin(c)), iterator(boost::end(c)),
		[](iterator it)
	{
		//(*it).clear();
		
		
	});
	std::cout << "Second Round!" << std::endl;
	for (int i = 0; i < c.size(); i++) {
		std::cout << i << "\t" << c.at(i).size() << std::endl;		
	}
	
	std::cin.ignore();
	std::cin.ignore();
	
	*/
	
	
	
	/*
	std::cout << "After par-for, c.size() = " << c.size() << std::endl;
	for (int i = 0; i < c.size(); i++) {
		std::cout << c.at(0).at(0) << std::endl;
	}
	std::cin >> f;
	
	*/

}


///////////////////////////////////////////////////////////////////////////////


template <typename ExPolicy, typename IteratorTag>
std::vector< std::vector < double > > matrix_foreman_serial(
	std::vector< std::vector< double > >& one,
	std::vector< std::vector< double > >& two,
	ExPolicy && policy, IteratorTag)
{
	static_assert(
		hpx::parallel::is_execution_policy<ExPolicy>::value,
		"hpx::parallel::is_execution_policy<ExPolicy>::value");

	typedef std::vector<int>::iterator base_iterator;
	typedef test::test_iterator<base_iterator, IteratorTag> iterator;
	bool twenty_five = false, fifty = false, seventy_five = false;
	hpx::naming::id_type here = hpx::find_here();
	std::vector< std::vector< double > > data;
	data.reserve(one.capacity());
	std::vector< std::vector< double > > futuresParent;
	std::vector<int> futuresIndex(one.size());
	std::iota(boost::begin(futuresIndex), boost::end(futuresIndex), 0);
	for (int i = 0; i < one.size(); i++) {
		std::vector<double> temp;
		temp.reserve(two.at(0).size());
		futuresParent.push_back(temp);
	}
	futuresParent.reserve(one.size());
	std::cout << "Matrix Foreman Loading:" << std::endl;
	

	hpx::parallel::for_loop(
		std::forward<ExPolicy>(policy),
		iterator(boost::begin(futuresIndex)), iterator(boost::end(futuresIndex)),
		[&futuresParent,&one, &two,policy](iterator it)
	{
		std::vector<int> row_index(two.at(0).size());
		std::iota(boost::begin(row_index), boost::end(row_index), 0);
		hpx::parallel::for_loop(
			std::forward<ExPolicy>(policy),
			iterator(boost::begin(row_index)), iterator(boost::end(row_index)),
			[&futuresParent,it,&one,&two](iterator second)
		{
			futuresParent.at(*it).push_back(dot_product(one.at(*it), get_col(two, *second)));		    
		});

		//////////////////////// Useless Code
		for (int i = 0; i < pow(one.size()/2, 2); i++) {
			int k = rand();
		}
		////////////////////////
		

		/*
		for (int f = 0; f < pow(one.get_dim_one()/2, 2); f++) {
			int useless = rand();
		}
		*/

	});


	/*
	//So try making async call then get right after, 11s
	try never calling get, 7s
	then try doing parallel for, with async then get right after
	if (debug) {
	std::cout << "Inside loading, i = " << i << " And f.size() = " << futuresParent.at(i).size() << std::endl;
	if(i > 0)
	std::cout << "Inside loading, i = " << i-1 << " And f.size() = " << futuresParent.at(i-1).size() << std::endl;

	}
	if(debug)
	std::cout << "f.size() = " << futuresParent.at(0).size() << std::endl;
	if (futuresParent.size() > 0) {
		std::cout << "futuresParent.size() = " << futuresParent.size() << std::endl;
		if (futuresParent.at(0).size() > 0) {
	        std::cout << "futuresParent.at(0).size() = " << futuresParent.at(0).size() << std::endl;

		}
	}

	for (int i = 0; i < one.capacity(); i++) {
		//hpx::wait_all(futuresParent.at(i));
		if (debug) {
			std::cout << "Got to " << i << " in data assignment!" << std::endl;
			std::cout << "Accessed top level of futuresParent successfully! f.size() = " << std::endl;

		}

		for (int j = 0; j < two.at(0).capacity(); j++) {
			if (debug) {
				std::cout << "Accessed future level of futuresParent successfully!" << std::endl;
				std::cout << "Accessed lowest level of futuresParent successfully!" << std::endl;
			}

		}
	}
	*/
	std::cout << "Finshed loading the async calls!" << std::endl;
	return futuresParent;
}
///////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {
	
	if (argc != 5) {
		usage(argv[0]);
		std::string input;
		std::getline(std::cin, input);
		return 0;
	}
	
	
	srand(time(NULL));
	char* end;

	const int first_matrix_dim_one = strtol(argv[1], &end, 10);
	const int first_matrix_dim_two = strtol(argv[2], &end, 10);

	const int second_matrix_dim_one = strtol(argv[3], &end, 10);
	const int second_matrix_dim_two = strtol(argv[4], &end, 10);

	if (first_matrix_dim_two != second_matrix_dim_one) {
		rules();
		std::string input;
		std::getline(std::cin, input);
		return 0;
	}

	if (debug)
		std::cout << first_matrix_dim_one << " " << first_matrix_dim_two <<
		std::endl;
	std::vector< std::vector< double > > first_matrix =
		rand_filler(first_matrix_dim_one, first_matrix_dim_two);

	//if (debug)
		std::cout << "After first rand_filler" << std::endl;


	std::vector< std::vector< double > > second_matrix =
		rand_filler(second_matrix_dim_one, second_matrix_dim_two);

	//if (debug)
		std::cout << "After second rand_filler" << std::endl;

	
	std::vector< std::vector< double > > new_matrix = matrix_foreman_serial(
		first_matrix, second_matrix, hpx::parallel::seq, std::forward_iterator_tag());
	std::cout << "Outside new_matrix creation" << std::endl;
	
	/*
	for (int i = 0; i < new_matrix.size(); i++) {
		//std::cout << "new_matrix.size() = " << new_matrix.size() << std::endl;
		//std::cout << "new_matrix.at(0).size() = " << new_matrix.at(0).size() << std::endl;
		std::cout << "[ ";
		
		for (int j = 0; j < new_matrix.at(0).size(); j++) {
			std::cout << new_matrix.at(i).at(j) << " " ;
		}
		std::cout << "]" << std::endl;
	}
	
	
	*/
	

	

	std::cout << "Finished!" << std::endl;

	std::cout << "Seconds Running: " << clock() << std::endl;
	std::string input;
	std::getline(std::cin, input);
	return 0;


}




