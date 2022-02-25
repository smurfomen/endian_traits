#include <endian_traits>
#include <iostream>

using namespace std;

int main(int argc, char *argv[])
{
    le16 little(0xff00);
    be16 big(0xff00);


    cout << "current endian is: " << ENDIAN_TRAITS_DESC << endl;

    cout << "Test number: " << 0xff00 << endl;
    cout << "little to little: " << little.to_little_endian() << "\tlittle to big: " << little.to_big_endian() << endl;
    cout << "big to little: " << big.to_little_endian() << "\tbig to big: " << big.to_big_endian() << endl;
    cout << (uint16_t)little << ' ' << (uint16_t)big << endl;

    le16 marker = big;

    cout << "marker is: " << (uint16_t)marker << ' ' << "expected: " << 0xff << endl;

    return 0;
}
