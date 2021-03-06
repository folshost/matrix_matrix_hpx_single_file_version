// matrix_mult_no_pntrs.cpp : Defines the entry point for the console application.
//

//#include "stdafx.h"

#include <hpx/hpx_main.hpp>
#include <hpx/include/lcos.hpp>
#include <hpx/include/actions.hpp>
#include <hpx/include/components.hpp>
#include <hpx/include/iostreams.hpp>

#include <boost/format.hpp>

#include <cstddef>
#include <list>
#include <set>
#include <mutex>
#include <iostream>
#include <string>
#include <istream>
#include <stdlib.h>
#include <time.h>
#include <stdio.h>
#include <vector>


std::vector<double> get_col(const std::vector< std::vector<double> >& data,
	int col);

double dot_product(const std::vector<double>& row,
	const std::vector<double>& columns);



void usage(char* func_name) {
	std::cout << "usage: " << func_name << " double1 double2 double3 double4"
		<< std::endl;
}

void rules() {
	std::cout << "For matrix multiplication to be possible, with matrices" <<
		" X(x_1,x_2) and Y(y_1,y_2), for X*Y x_2 MUST equal y_1, and" <<
		" vice versa" << std::endl;
}


double dot_product(const std::vector<double>& row,
	const std::vector<double>& columns) {
	double sum = 0;
	for (int i = 0; i < row.size(); i++) {
		sum += row.at(i) * columns.at(i);
	}
	return sum;
}
HPX_PLAIN_ACTION(dot_product, dot_product_action);

std::vector< std::vector<double> > matrix_gen(double dim_on, double dim_tw) {
	//std::cout << dim_on << " " << dim_tw << std::endl;
	std::vector< std::vector<double> > data;
	data.reserve(dim_on);
	//std::cout << data.capacity() << std::endl;
	for (int i = 0; i < dim_on; i++) {
		std::vector<double> k;
		data.push_back(k);
		data.at(i).reserve(dim_tw);
	}
	return data;
}

std::vector<hpx::lcos::future<double>> matrix_worker(
	std::vector< std::vector<double> > one, std::vector< std::vector<double> >
	two, int i) 
{	
	hpx::naming::id_type const here = hpx::find_here();	
	std::vector<hpx::lcos::future<double> > futures;
	for (int j = 0; j < two.at(0).capacity(); j++)
	{
		futures.push_back(hpx::async<dot_product_action>(here,
			one.at(i), get_col(two, j)));		
	}
	
	return futures;
}
HPX_PLAIN_ACTION(matrix_worker, matrix_worker_action);

std::vector< std::vector <double> > matrix_foreman(
	std::vector< std::vector<double> > one, std::vector< std::vector<double> > 
	two) 
{
	hpx::naming::id_type here = hpx::find_here();
	std::vector< std::vector<double> > data;
	data.reserve(one.capacity());
	std::vector< hpx::lcos::future< std::vector< hpx::lcos::future<double> 
	  > > > futures;
	for (int i = 0; i < one.capacity(); i++) 
	{
		std::vector<double> k;
		k.reserve(two.at(0).capacity());
		data.push_back(k);
		matrix_worker_action mat;
		futures.push_back(hpx::async<matrix_worker_action>
			( here, one, two, i ));
	}
	//std::cout << "Got past calling matrix_worker_action!" << std::endl;
	for (int i = 0; i < one.capacity(); i++) {
		//std::cout << "Got to " << i << " in data assignment!" << std::endl;
		std::vector<hpx::lcos::future<double> > vick = futures.at(i).get(); 
		for (int j = 0; j < two.at(0).capacity(); j++) {			
			//std::cout << "\tGot to " << j << " in internal for-loop for data assignment!" << std::endl;
			
			double f = vick.at(j).get();                     //I should try doing f = vick.at(j), see what happens
			//std::cout << "\tHere's f: " << f << std::endl;			
			data.at(i).push_back( f );
			//std::cout << "\tHere's data[i][j]: " << data.at(i).at(j) << std::endl;
		}
	}
	return data;
}


std::vector< std::vector <double> >
  rand_filler(double dim_one, double dim_two) {
	std::vector<std::vector<double>> data =
		matrix_gen(dim_one, dim_two);
	for(int i = 0; i < data.capacity(); i++) {
		data.at(0);
		data.at(i).clear();
		//std::cout << "[";
		for (int j = 0; j < data.at(0).capacity(); j++) {
			data.at(i).push_back((double)(rand() % 100));
			//std::cout << data.at(i).at(j) << " ";
		}
		//std::cout << "]" << std::endl;

	}
	return data;
}
HPX_PLAIN_ACTION(rand_filler, rand_filler_action);






std::vector<double> get_col( const std::vector< std::vector<double> >& data,
	int col) 
{
	std::vector<double> column;
	column.reserve(data.capacity());
	for (int i = 0; i < data.capacity(); i++) 
	{
		column.push_back(data.at(i).at(col));
	}
	return column;
}



int main(int argc, char* argv[])
{

	if (argc != 5) {
		usage(argv[0]);
		std::string input;
		std::getline(std::cin, input);
		return 0;
	}
	srand(time(NULL));
	char* end;

	const int first_matrix_dim_one  = strtol(argv[1], &end, 10);
	const int first_matrix_dim_two  = strtol(argv[2], &end, 10);

	const int second_matrix_dim_one = strtol(argv[3], &end, 10);
	const int second_matrix_dim_two = strtol(argv[4], &end, 10);

	if (first_matrix_dim_two != second_matrix_dim_one) {
		rules();
		std::string input;
		std::getline(std::cin, input);
		return 0;
	}
	
	//std::cout << first_matrix_dim_one << " " << first_matrix_dim_two << std::endl;
	std::vector< std::vector<double> > first_matrix = 
		rand_filler(first_matrix_dim_one, first_matrix_dim_two);
	
	//std::cout << std::endl;


	std::vector< std::vector<double> > second_matrix = 
		rand_filler(second_matrix_dim_one, second_matrix_dim_two);

	//std::cout << std::endl;
	
	
	std::vector< std::vector<double> > new_matrix = matrix_foreman(
		first_matrix, second_matrix);
	/*
	
	for (int i = 0; i < new_matrix.capacity(); i++) {
		std::cout << "[ ";
		for (int j = 0; j < new_matrix.at(0).capacity(); j++) {
			std::cout << new_matrix.at(i).at(j) << " " ;
		}
		std::cout << "]" << std::endl;
	}
	
	*/
	
	std::cout << "Finished!" << std::endl;
	std::string input;
	std::getline(std::cin, input);
	return 0;
}