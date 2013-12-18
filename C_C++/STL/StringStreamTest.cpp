#include <iostream>
#include <sstream>


int main()
{
    std::stringstream ss;
    ss.str("2_3_\n4_5");

    char out;
    ss.get(out);
    std::cout << "1:" << out << " " << ss.gcount() << std::endl;
    ss.get(out);
    std::cout << "2:" << out << " " << ss.gcount() << std::endl;

    char str[100];
    ss.get(str, 100);
    std::cout << "3:" << str << " " << ss.gcount() << std::endl;
    ss.get(str, 100, '=');
    std::cout << "4:" << str << " " << ss.gcount() << std::endl;

    // ss.str("123\n456");
    // ss.clear();
    // std::streambuf sb;
    // ss.get(sb);
    // std::cout << "5:" << sb << " " << ss.gcount() << std::endl;
    // ss.get(sb);
    // std::cout << "6:" << sb << " " << ss.gcount() << std::endl;

    char c;
    std::cout << "end." << std::endl;
    std::cin >> c;
    return 0;
}