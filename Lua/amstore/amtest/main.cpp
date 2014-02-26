#include <list>
#include <iostream>
#include <algorithm>
           

typedef struct BusiFunctorEqual
{
    BusiFunctorEqual(int n) : business_id(n) { }; 
    bool operator()(int p)
    { 
        std::cout << "[BusiFunctorEqual]p=" << p << "," << business_id << std::endl;
        return (business_id == p); 
    };
    int business_id;
}BusiFunctorEqual;

int main()
{
    std::list<int> aList;
    aList.push_back(2);
    aList.push_back(3);
    aList.push_back(4);
    aList.push_back(5);

    std::list<int>::reverse_iterator it = std::find_if(aList.rbegin(), aList.rend(), BusiFunctorEqual(4));

    if(it != aList.rend()){
        
        int i = *(--it.base());
        aList.erase(--(it.base()));
        
        for(std::list<int>::iterator it = aList.begin(); it != aList.end(); ++it){
            std::cout << *it << std::endl;
        }

        std::cout << "i=" << i << std::endl;
    }else{
        std::cout << "end" << std::endl;
    }
    int i;
    std::cin >> i;
    return 0;
      
}