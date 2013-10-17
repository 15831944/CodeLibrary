#include <iostream>// for std::cout, std::endl
#include <string>  // for std::string
#include <vector>  // for std::vector
#include <numeric> // for std::accumulate
int main()
{
	using namespace std;
    string arr[] = {"hello", " ", "world"};
	vector<string> vec(arr, arr + 3);
	string s = accumulate(vec.begin(), vec.end(), s);
	cout << s << endl; // prints 'hello world' to standard output. 
    
    const char* init[]=
    {
        "A","B","C"
    };
 
    vector<string> strArray(init,init+3);
    
	return 0;
}


