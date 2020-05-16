#include <iostream>
#include "ECC.h"
#define P 9967
using namespace std;

void printBigPrime()
{
    long long int min;
    long long int max;
    cout << "������������Сֵ:";
    cin >> min;
    cout << "�������������ֵ:";
    cin >> max;
    cout << "�����б�";
    for (long long int a = min; a <= max; a++)
    {
        long long int key = 0;
        for (long long int i = 2; i <= a / 2; i++)
        {
            if (a % i == 0)
            {
                key = 1;
                break;
            }
        }
        if (!key && a % 4 == 3)
        {
            cout << a << " ";
        }
    }
    cout << endl;
}

void findECC()
{
    int aMax = P, bMax = P;
    //cout << "����Χ����Ϊ" << P << "������a�����ֵ:" << endl;
    //cin >> aMax;
    //cout << "����b�����ֵ:" << endl;
    //cin >> bMax;
    //cout << "�������ٵ�����������:" << endl;
    //cin >> nMin;
    //cout << "���е�����:" << endl;
    //ECC ecc;
    cout << "P = " << P << endl;
    int MAXN = 0;
    int mA = 0, mB = 0;
    /*for (int a = 1; a < aMax; a++)
        for (int b = 1; b < bMax; b++)
        {*/
            int n = 0,a= 188,b= 9220;
            //ecc.setEllipticParameter(0, a, b, P, Point(0,0));
            for (int x = 1; x < P; x++)
            {
                int m = x * x * x + a * x + b;
				int tmp = round(sqrt(m));
                if (tmp*tmp == m)
                {
                    n++;
                }
            }
			cout << n << endl;
            /*if (n > MAXN)
            {
                MAXN = n;
                mA = a;
                mB = b;
                cout << "�ҵ�a = " << a << " , b = " << b << " , n = " << n << endl;
            }*/
        //}
    cout << "����������ʱ��" << endl;
    cout << "a:" << mA << ",b:" << mB << ",����������:" << MAXN << endl;
}

void findG(LL a, LL b, LL p, LL minN) // minN��G����С����
{
    ECC ecc;
    ecc.setEllipticParameter(0, a, b, p, Point(0, 0));
    for (LL x = 1; x < p; ++x)
    {
        int m = x * x * x + a * x + b;
        if (sqrt(m) - int(sqrt(m)) <= 0)
        {
            Point p0 = Point(x,LL(sqrt(m)));
            int n = 1;
            Point tP = p0;
            while (!tP.O())
            {
                ++n;
                tP = ecc.add(tP, p0);
            }
            if (n >= minN)
            {
                cout << "x = " << p0.x << " , y = " << p0.y << " , n = " << n << endl;
            }
        }
    }
}
