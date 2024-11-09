#include <iostream>
#include "Quick_sort.h"

using namespace std;

int main() 
{
    int a[100];
    
    for (int& x : a) {
        x = rand() % 100;
    }

    fast_sort(a, a + 100, [](int a, int b) { return a < b; });

    for (int x : a) {
        std::cout << x << "\n";
    }

    return 0;
}
