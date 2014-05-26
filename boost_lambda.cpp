#include <boost/lambda/lambda.hpp>
#include <iostream>
#include <iterator>
#include <algorithm>

/*
	Demo for using the boost header only library.
	
	Build:
		c++ -I /usr/local/boost_1_55_0 boost_lambda.cpp -o a_example
	
	Usage:
		echo 1 2 3 | ./a_example
*/ 

int main()
{
    using namespace boost::lambda;
    typedef std::istream_iterator<int> in;

    std::for_each(
        in(std::cin), in(), std::cout << (_1 * 3) << " " );

		std::cout<<std::endl;
}
