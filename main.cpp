#include <cstddef>
#include <cstdlib>
#include <string>
#include <iostream>
#include <map>
#include <ctime>
#include <vector>

#include "Vector.h"
#include "LinkedList.h"

namespace 
{

    template<typename T>
    using test_col = aisdi::LinkedList<T>;
   //using test_col = aisdi::Vector<T>;

    std::clock_t test_pop_first(size_t size)
    {
        test_col<int> my_col;
        std::clock_t time;
        for (unsigned int i = 0; i < size; ++i) {
            my_col.append(i);
        }
        time = std::clock();
	my_col.popFirst();
        time = std::clock() - time;
        return time;
    }

    std::clock_t test_insert_middle(size_t size)
    {
        test_col<int> my_col;
        std::clock_t time;
        for (unsigned int i = 0; i < size; ++i) {
            my_col.append(i);
        }
        time = std::clock();
        auto it = my_col.begin() + my_col.getSize() / 2;
	my_col.insert(it, 24);
        time = std::clock() - time;
        return time;
    }



    void perfomTest() 
    {
        std::vector<unsigned int> sizes{100, 5000, 50000, 500000, 5000000, 10000000};

        for (unsigned int size : sizes)
	{
		std::cout<< "Operacja wstawiania w srodek, liczba elementów: "<<size<<" w czasie: "<< (float)test_insert_middle(size)<<std::endl;
		std::cout<< "Operacja usuwania z poczatku, liczba elementów: "<<size<<" w czasie: "<< (float)test_pop_first(size)<<std::endl;
			
        }
    }
}

int main(int argc, char** argv)
{
    const std::size_t repeatCount = argc > 1 ? std::atoll(argv[1]) : 1;
    std::cout << "Testing for " << repeatCount << " iterations..." << std::endl;
    std::cout << "Testy dla listy: "<<std::endl;
    for (std::size_t i = 0; i < repeatCount; ++i) {
        perfomTest();
    }
		
    
    return 0;
}
