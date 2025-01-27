// #include <iostream>
// using namespace std;
// #include <map>
// #include <string>
// #include <stack>

// int main() {
//     std::map<std::string, std::string> dictionary;
//     // Adding elements
//     // dictionary["Start #include"] = "St N M";
//     // dictionary["St"] = "5";
//     // dictionary["cherry"] = "15";
//     // // Accessing values by key
//     // if(dictionary.count("cherry")){
//     //     cout << dictionary["cherry"];
//     // }
//     // if(!dictionary.count("apple")){
//     //     cout << "No apples";
//     // }
//     string s="A B C D E";
//     cout << strrev(s);
//     return 0;
// }
#include <algorithm>
#include <iostream>
#include <string>

int main()
{
    std::string foo("A B C D E");
    std::string copy(foo);
    std::reverse(copy.begin(), copy.end());
    std::cout << foo << '\n' << copy << '\n';
    copy=copy+" ";
    while(copy.length()){
        int j = copy.find(" ");
        std::cout<< copy.substr(0,j)<< "\t"<<copy.substr(0,j).length()<<"\n";
        copy.erase(0, j+1);
    }
}