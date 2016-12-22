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





int debug = 0;

std::vector<double> get_col(const std::vector< std::vector<double> >& data,
	int col);

double dot_product(const std::vector<double> row,
	const std::vector<double> columns);

void usage(char* func_name) {
	hpx::cout << "usage: " << func_name << " double1 double2 double3 double4 [0..2]"
		<< hpx::endl;
}

void rules() {
	hpx::cout << "For matrix multiplication to be possible, with matrices" <<
		" X(x_1,x_2) and Y(y_1,y_2), for X*Y x_2 MUST equal y_1, and" <<
		" vice versa" << hpx::endl;
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
	//hpx::cout << dim_on << " " << dim_tw << hpx::endl;
	std::vector< std::vector<double > > data;
	data.reserve(dim_on);
	//hpx::cout << data.capacity() << hpx::endl;
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
		for (int j = 0; j < data.at(0).capacity(); j++) {
			data.at(i).push_back(j);
			//data.at(i).push_back((double)(rand() % 100));
		}

	}
	if (debug > 1) {
		for (int i = 0; i < data.capacity(); i++) {
			hpx::cout << "[";
			for (int j = 0; j < data.at(0).capacity(); j++) {
				hpx::cout << data.at(i).at(j) << " ";
			}
			hpx::cout << "]" << hpx::endl;
		}
	}
	return data;
}
HPX_PLAIN_ACTION(rand_filler, rand_filler_action);


std::vector< std::vector < double > >
first_filler(int dim_one, int dim_two) {
	std::vector< std::vector< double > > data =
		matrix_gen(dim_one, dim_two);
	for (int i = 0; i < data.capacity(); i++) {
		data.at(0);
		data.at(i).clear();
		for (int j = 0; j < data.at(0).capacity(); j++) {
			data.at(i).push_back(j);
			//data.at(i).push_back((double)(rand() % 100));
		}

	}
	if (debug > 1) {
		for (int i = 0; i < data.capacity(); i++) {
			hpx::cout << "[";
			for (int j = 0; j < data.at(0).capacity(); j++) {
				hpx::cout << data.at(i).at(j) << " ";
			}
			hpx::cout << "]" << hpx::endl;
		}
	}
	return data;
}


std::vector< std::vector < double > >
second_filler(int dim_one, int dim_two) {
	std::vector< std::vector< double > > data =
		matrix_gen(dim_one, dim_two);
	for (int i = 0; i < data.capacity(); i++) {
		data.at(0);
		data.at(i).clear();
		for (int j = 0; j < data.at(0).capacity(); j++) {
			data.at(i).push_back(i);
			//data.at(i).push_back((double)(rand() % 100));
		}

	}
	if (debug > 1) {
		for (int i = 0; i < data.capacity(); i++) {
			hpx::cout << "[";
			for (int j = 0; j < data.at(0).capacity(); j++) {
				hpx::cout << data.at(i).at(j) << " ";
			}
			hpx::cout << "]" << hpx::endl;
		}
	}
	return data;
}




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
	hpx::cout << "First round!" << hpx::endl;
	for (int i = 0; i < c.size(); i++) {
	hpx::cout << i << "\n\t" << c.at(i).size() << hpx::endl;

	}
	int f;
	hpx::parallel::for_loop(
	std::forward<ExPolicy>(policy),
	iterator(boost::begin(c)), iterator(boost::end(c)),
	[](iterator it)
	{
	//(*it).clear();


	});
	hpx::cout << "Second Round!" << hpx::endl;
	for (int i = 0; i < c.size(); i++) {
	hpx::cout << i << "\t" << c.at(i).size() << hpx::endl;
	}

	std::cin.ignore();
	std::cin.ignore();

	*/



	/*
	hpx::cout << "After par-for, c.size() = " << c.size() << hpx::endl;
	for (int i = 0; i < c.size(); i++) {
	hpx::cout << c.at(0).at(0) << hpx::endl;
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
	futuresParent.reserve(one.size());
	std::iota(boost::begin(futuresIndex), boost::end(futuresIndex), 0);
	hpx::cout << one.size() << hpx::endl;
	for (int i = 0; i < one.size(); i++) {
		std::vector<double> temp;
		temp.reserve(two.at(0).size());
		futuresParent.push_back(temp);
	}

	hpx::cout << "Matrix Foreman Loading:" << hpx::endl;


	hpx::parallel::for_loop(
		std::forward<ExPolicy>(policy),
		iterator(boost::begin(futuresIndex)), iterator(boost::end(futuresIndex)),
		[&futuresParent, &one, &two](iterator it)
	{
		if (debug > 1)
			hpx::cout << "Before for!" << hpx::endl;
		for (int i = 0; i < two.at(0).size(); i++) {
			futuresParent.at(*it).push_back(dot_product(one.at(*it), get_col(two, i)));
		}
		if(debug > 1)
		    hpx::cout << "Here's i: " << (*it) << "\nAnd the first elem: "
			    << futuresParent.at(*it).at(0) << hpx::endl;
		if(debug > 0)
		    hpx::evaluate_active_counters(true,
			    "Finished parallel-for segment");

	});


	/*
	//So try making async call then get right after, 11s
	try never calling get, 7s
	then try doing parallel for, with async then get right after
	if (debug) {
	hpx::cout << "Inside loading, i = " << i << " And f.size() = " << futuresParent.at(i).size() << hpx::endl;
	if(i > 0)
	hpx::cout << "Inside loading, i = " << i-1 << " And f.size() = " << futuresParent.at(i-1).size() << hpx::endl;

	}
	if(debug)
	hpx::cout << "f.size() = " << futuresParent.at(0).size() << hpx::endl;
	if (futuresParent.size() > 0) {
	hpx::cout << "futuresParent.size() = " << futuresParent.size() << hpx::endl;
	if (futuresParent.at(0).size() > 0) {
	hpx::cout << "futuresParent.at(0).size() = " << futuresParent.at(0).size() << hpx::endl;

	}
	}

	for (int i = 0; i < one.capacity(); i++) {
	//hpx::wait_all(futuresParent.at(i));
	if (debug) {
	hpx::cout << "Got to " << i << " in data assignment!" << hpx::endl;
	hpx::cout << "Accessed top level of futuresParent successfully! f.size() = " << hpx::endl;

	}

	for (int j = 0; j < two.at(0).capacity(); j++) {
	if (debug) {
	hpx::cout << "Accessed future level of futuresParent successfully!" << hpx::endl;
	hpx::cout << "Accessed lowest level of futuresParent successfully!" << hpx::endl;
	}

	}
	}
	*/
	hpx::cout << "Finshed loading the async calls!" << hpx::endl;
	return futuresParent;
}
///////////////////////////////////////////////////////////////////////////////

int main(int argc, char* argv[]) {

	if (argc != 5 && argc != 6) {
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

	if (argc == 6) {
		debug = strtol(argv[5], &end, 10);
	}

	if (first_matrix_dim_two != second_matrix_dim_one) {
		rules();
		std::string input;
		std::getline(std::cin, input);
		return 0;
	}

	if (debug > 0)
		hpx::cout << first_matrix_dim_one << " " << first_matrix_dim_two <<
		hpx::endl;
	std::vector< std::vector< double > > first_matrix =
		first_filler(first_matrix_dim_one, first_matrix_dim_two);


	if (debug > -1)
		hpx::cout << "After first rand_filler" << hpx::endl;


	std::vector< std::vector< double > > second_matrix =
		second_filler(second_matrix_dim_one, second_matrix_dim_two);

	if (debug > -1)
		hpx::cout << "After second rand_filler" << hpx::endl;


	std::vector< std::vector< double > > new_matrix = matrix_foreman_serial(
		first_matrix, second_matrix, hpx::parallel::par, std::forward_iterator_tag());
	hpx::cout << "Outside new_matrix creation" << hpx::endl;

	if (debug > 1) {
		for (int i = 0; i < new_matrix.size(); i++) {
			//hpx::cout << "new_matrix.size() = " << new_matrix.size() << hpx::endl;
			//hpx::cout << "new_matrix.at(0).size() = " << new_matrix.at(0).size() << hpx::endl;
			std::cout << "[ ";

			for (int j = 0; j < new_matrix.at(0).size(); j++) {
				std::cout << new_matrix.at(i).at(j) << " ";
			}
			std::cout << "]" << hpx::endl;
		}
	}




	hpx::cout << "Finished!" << hpx::endl;

	hpx::evaluate_active_counters(true, "Thing");
	std::string input;
	std::getline(std::cin, input);
	//std::getline(std::cin, input);
	return 0;


}





