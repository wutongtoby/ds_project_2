#include <fstream>

using namespace std;

int main(void)
{
    fstream f;
    f.open("aaa.data", ios::out);
    f << 1000 << ' ' << 1000 << ' ' << 2100 << endl;

    for (int i = 0; i < 1000; i++)
        f << 1;
    f << endl;

    for (int i = 0; i < 997; i++) {
        f << 1;
        for (int j = 0; j < 998; j++)
            f << 0;
        f << 1;
        f << endl;
    }

    f << 1;
    for (int i = 1; i < 500; i++)
        f << 0;
    f << 'R';
    for (int i = 1; i < 499; i++)
        f << 0;
    f << 1;
    f << endl;

    for (int i = 0; i < 1000; i++)
        f << 1;
    f << endl;
    f.close();
    return 0;
}