#include <iostream>
#include <algorithm>
#include <functional>
#include <vector>


template <class T> class less_than : public std::unary_function <T, bool>{
	T arg;
public:
	explicit less_than(const T & x) : arg(x) {  }
	bool operator()(const T& x) const { return x < arg; }
};


int main(){
	std::cout << "Begining algorithm test" << std::endl;

	std::vector<double> a, b;

	std::vector<double>::iterator i, j;
	a.push_back(12.5);
	a.push_back(32.7);
	a.push_back(85.3);
	a.push_back(25.8);
	a.push_back(63.8);
	a.push_back(92.7);

	std::cout << "Starting element list:\n";

	i = a.begin();
	while(i != a.end()){
		std::cout << *i << std::endl;
		++i;
	}

	std::sort<std::vector<double>::iterator>(a.begin(), a.end());

	std::cout << "Sorted elements:\n";

	i = a.begin();
	while(i != a.end()){
		std::cout << *i << std::endl;
		++i;
	}

	std::cout << "Checking set operations\n";

	b.clear();
	b.push_back(12.5);
	b.push_back(32.7);
	b.push_back(92.7);

	std::cout << "The following should be 1 : ";
	std::cout << std::includes(a.begin(), a.end(), b.begin(), b.end());
	std::cout << std::endl;

	b.clear();
	b.push_back(12.5);
	b.push_back(92.7);
	b.push_back(92.8);

	std::cout << "The following should be 0 : ";
	std::cout << std::includes(a.begin(), a.end(), b.begin(), b.end());
	std::cout << std::endl;

	b.clear();
	std::cout << "The following should be 1 : ";
	std::cout << std::includes(a.begin(), a.end(), b.begin(), b.end());
	std::cout << std::endl;

	b.push_back(12.5);
	std::cout << "The following should be 1 : ";
	std::cout << std::includes(a.begin(), a.end(), b.begin(), b.end());
	std::cout << std::endl;

	b.clear();
	b.push_back(1.0);
	std::cout << "The following should be 0 : ";
	std::cout << std::includes(a.begin(), a.end(), b.begin(), b.end());
	std::cout << std::endl;


	std::cout << "Searching\n";
	std::cout << "The following should be 1 : ";
	std::cout << std::binary_search(a.begin(), a.end(), 32.7);
	std::cout << std::endl;

	std::cout << "The following should be 0 : ";
	std::cout << std::binary_search(a.begin(), a.end(), 99.9);
	std::cout << std::endl;

	std::cout << "The following should be 0 : ";
	std::cout << std::binary_search(a.begin(), a.end(), 1.0);
	std::cout << std::endl;

	std::cout << "The following should be 0 : ";
	std::cout << std::binary_search(a.begin(), a.end(), 27.7);
	std::cout << std::endl;

	std::cout << "The following should be 0 : ";
	std::cout << std::binary_search(a.begin(), a.end(), -26);
	std::cout << std::endl;

	std::cout << "The following should be 1 : ";
	std::cout << std::binary_search(a.begin(), a.end(), 12.5);
	std::cout << std::endl;

	std::cout << "Bounds checks\n";
	std::cout << "The following should read 32.7 : ";
	std::cout << *(std::lower_bound(a.begin(), a.end(), 32.7));
	std::cout << std::endl;

	std::cout << "The following should read 32.7 : ";
	std::cout << *(std::lower_bound(a.begin(), a.end(), 27.5));
	std::cout << std::endl;

	b.clear();
	b.push_back(2);
	b.push_back(3);
	b.push_back(3);
	b.push_back(3);
	b.push_back(4);
	b.push_back(7);
	b.push_back(8);
	b.push_back(9);

	std::cout << "The following should read 3 : ";
	std::cout << *(std::lower_bound(b.begin(), b.end(), 3));
	std::cout << std::endl;

	std::cout << "The following two lines should be identical:\n";
	std::cout << "3 3 3 4 7 8 9 \n";
	i = std::lower_bound(b.begin(), b.end(), 3);
	while( i != b.end()){
		std::cout << *i << " ";
		++i;
	}
	std::cout << std::endl;


	std::cout << "The following should read 32.7 : ";
	std::cout << *(std::upper_bound(a.begin(), a.end(), 27.5));
	std::cout << std::endl;

	std::cout << "The following should read 2 : ";
	std::cout << *(std::lower_bound(b.begin(), b.end(), 1));
	std::cout << std::endl;

	std::cout << "The following two lines should be identical:\n";
	std::cout << "4 7 8 9 \n";
	i = std::upper_bound(b.begin(), b.end(), 3);
	while( i != b.end()){
		std::cout << *i << " ";
		++i;
	}
	std::cout << std::endl;


	std::cout << "Partition Tests\n";
	b.clear();
	b.push_back(12.5);
	b.push_back(32.7);
	b.push_back(10.8);
	b.push_back(92.7);
	b.push_back(12.5);
	b.push_back(22.7);
	b.push_back(38.4);
	b.push_back(52.9);
	b.push_back(72.3);
	b.push_back(19.6);


	std::cout << "The following two lines should be identical:\n";
	std::cout << "32.7 92.7 22.7 38.4 52.9 72.3" << std::endl;
	i = std::stable_partition(b.begin(), b.end(), less_than<double>(20) );
	j = i;
	while(j != b.end()){
		std::cout << *j << " ";
		++j;
	}
	std::cout << std::endl;

	std::cout << "The following two lines should be identical:\n";
	std::cout << "12.5 10.8 12.5 19.6" << std::endl;
	j = b.begin();
	while(j != i){
		std::cout << *j << " ";
		++j;
	}
	std::cout << std::endl;



	std::cout << "Push heap\n";

	a.clear();
	a.push_back(12.5);

	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	std::cout << "The following two lines should be identical:\n";
	std::cout << "12.5 " << std::endl;
	i = a.begin();
	while(i != a.end()){
		std::cout << *i << " ";
		++i;
	}
	std::cout << std::endl;

	a.clear();
	a.push_back(12.5);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(7.2);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(27.4);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(21.8);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(93.4);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(36.3);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(55.5);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(5.2);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(67.9);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());

	std::cout << "The following two lines should be identical:\n";
	std::cout << "93.4 67.9 55.5 36.6 27.4 21.8 12.5 7.2 5.2 " << std::endl;
	i = a.begin();
	while(i != a.end()){
		std::cout << *i << " ";
		++i;
	}
	std::cout << std::endl;


	std::cout << "Push heap\n";
	a.clear();
	a.push_back(12.5);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(7.2);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(27.4);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(21.8);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(93.4);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(36.3);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(55.5);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(5.2);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(67.9);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());

	std::pop_heap<std::vector<double>::iterator>(a.begin(), a.end());

	std::cout << "The following two lines should be identical:\n";
	std::cout << "67.9 55.5 36.6 27.4 21.8 12.5 7.2 5.2 93.4 " << std::endl;
	i = a.begin();
	while(i != a.end()){
		std::cout << *i << " ";
		++i;
	}
	std::cout << std::endl;


	std::cout << "Sort Heap\n";
	a.clear();
	a.push_back(12.5);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(7.2);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(27.4);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(21.8);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(93.4);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(36.3);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(55.5);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(5.2);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());
	a.push_back(67.9);
	std::push_heap<std::vector<double>::iterator>(a.begin(), a.end());

	std::sort_heap<std::vector<double>::iterator, std::greater<double> >(a.begin(), a.end(), std::greater<double>() );

	std::cout << "The following two lines should be identical:\n";
	std::cout << "5.2 7.2 12.5 21.8 27.4 36.6 55.5 67.9 93.4" << std::endl;
	i = a.begin();
	while(i != a.end()){
		std::cout << *i << " ";
		++i;
	}
	std::cout << std::endl;


	std::cout << "Partial sort test\n";
	a.clear();
	a.push_back(12.5);
	a.push_back(32.7);
	a.push_back(10.8);
	a.push_back(92.7);
	a.push_back(12.5);
	a.push_back(22.7);
	a.push_back(38.4);
	a.push_back(52.9);
	a.push_back(72.3);
	a.push_back(19.6);

	i = a.begin();
	i+=4;

	std::partial_sort<std::vector<double>::iterator>(a.begin(), i, a.end());

	std::cout << "The following two lines should be identical:\n";
	std::cout << "10.8 12.5 12.5 19.6 22.7 " << std::endl;

	for(int k = 0; k < 5; ++k){
		std::cout << a[k] << " ";
	}
	std::cout << std::endl;




	std::cout << "Merge test\n";
	a.clear();
	a.push_back(10.8);
	a.push_back(12.5);
	a.push_back(32.7);
	a.push_back(72.3);

	i = a.end();

	a.push_back(12.5);
	a.push_back(19.6);
	a.push_back(22.7);
	a.push_back(38.4);
	a.push_back(52.9);
	a.push_back(92.7);

	std::inplace_merge<std::vector<double>::iterator>(a.begin(), i, a.end());

	std::cout << "The following two lines should be identical:\n";
	std::cout << "10.8 12.5 12.5 19.6 22.7 32.7 38.4 52.9 72.3 92.7 " << std::endl;

	j = a.begin();
	while(j != a.end()){
		std::cout << *j << " ";
		++j;
	}
	std::cout << std::endl;


	return 0;
}