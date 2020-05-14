#include "ECC.h"
#include <ctime>
#include <stdlib.h>
#include <string.h>

/*
	����gcd(a,b)
	a, b����Ϊ0
*/

LL ECC::gcd(LL a, LL b)
{
	return b > 0 ? gcd(b, a % b) : a;
}


/*
	��չ��ŷ������㷨��(x,y)ʹ��ax+by = gcd(a,b)
*/
Point ECC::extend_gcd(LL a, LL b)
{
	Point ans;
	if (a % b == 0)
	{
		ans.x = 0; ans.y = 1; return ans;
	}
	ans = extend_gcd(b, a % b);
	LL x = ans.y;
	LL y = ans.x - (a / b) * ans.y;
	ans.x = x;
	ans.y = y;
	return ans;
}

/*
	���� a mod n����Ҫ�Ǵ���aΪ���������
*/
LL ECC::mod(LL a, LL n)
{
	LL ans = a % n;
	return ans >= 0 ? ans : ans + n;
}

/*
	����a����ģn����,��������棬�򷵻�0
*/
LL ECC::inv(LL a, LL n)
{
	LL flag = gcd(a, n);
	if (flag != 1)return 0;
	int m = n >= 0 ? n : -n;
	Point sol = extend_gcd(a, n);
	return mod(sol.x,m);
}


/*
	���ص�p1+p2�Ľ����p1��p2�����������ϵĵ�
*/
Point ECC::add(Point p1, Point p2)
{
	if (p1.O()) return p2; // ���p1��O����ֻ�践��p2
	if (p2.O()) return p1; // ���p2��O����ֻ�践��p1
	Point ans(0, 0);

	LL lambda = 0;
	if (p1 == p2)
	{
		LL Inv = inv(mod(2 * p1.y, p), p);
		if (Inv == 0)
		{
			ans.x = ans.y = INF;
			return ans;
		}
		lambda = mod(mod(mod(mod(p1.x * p1.x, p) * 3 , p) + a , p) * Inv, p);
	}
	else
	{
		LL Inv = inv(mod(p2.x - p1.x, p), p);
		if (Inv == 0)
		{
			ans.x = ans.y = INF;
			return ans;
		}
		lambda = mod(mod(p2.y - p1.y, p) * Inv, p);
	}
	ans.x = mod((mod(lambda * lambda , p) - p1.x - p2.x) , p);
	ans.y = mod((mod(lambda * (p1.x - ans.x) , p) - p1.y) , p);
	return ans;
}

/*
	���� p1 - p2 �Ľ��
*/
Point ECC::minus(Point p1, Point p2)
{
	if (p2.O()) return p1;
	p2.y = -p2.y;
	return add(p1, p2);
}

/*
	����ps��flags������£�����k*p��ֵ
	���У�ps[i]��ʾk*p�Ľ����flags[i]��ʾ�Ƿ��Ѿ����ps[i]
*/
Point ECC::subMul(LL k, Point p, Point ps[], bool flags[])
{
	if (flags[k]) return ps[k];
	else if (k % 2)//����
	{
		Point p1 = subMul(k / 2, p, ps, flags), p2 = subMul(k / 2 + 1, p, ps, flags);
		flags[k] = true;
		return ps[k] = add(p1, p2);
	}
	else//ż��
	{
		Point phalf = subMul(k / 2, p, ps, flags);
		flags[k] = true;
		return ps[k] = add(phalf, phalf);
	}
}

/*
	��k*p�Ľ��
*/
Point ECC::mul(LL k, Point p)
{
	bool* flags = new bool[k + 1];
	for (int i = 0; i < k + 1; ++i)
	{
		flags[i] = false;
	}
	//memset(flags, false, sizeof(bool) * (k + 1));
	Point* ps = new Point[k + 1];
	ps[0] = Point(INF, INF);
	ps[1] = p;
	flags[0] = flags[1] = true;
	Point ans = subMul(k, p, ps, flags);
	delete[] flags;
	delete[] ps;
	return ans;
}

/*
	����ϢPm���ܳ�{kG,Pm+kP}������ֵ�ĵ�һ������kG���ڶ�����Pm+kP
*/
PointPair ECC::encode(Point Pm)
{
	PointPair ans;
	bool loop = true;
	while (loop)
	{
		LL k = getRandom();
		Point p1 = mul(k, G);
		Point p2 = mul(k, P);
		if (!p1.O() && !p2.O())
		{
			ans.first = p1;
			ans.second = add(p2, Pm);
			loop = false;
		}
	}
	return ans;
}

/*
	������Cm = {kG,Pm+kP}���ܳ���ϢM��Ӧ�ĵ�Pm
*/
Point ECC::decode(PointPair Cm)
{
	Point pMinus = mul(r, Cm.first);
	return minus(Cm.second, pMinus);
}

/*
	���캯��
*/

ECC::ECC()
{
	//����ѡ�õĲ���


}

ECC::ECC(LL r, LL a, LL b, LL p, Point G)
{
	this->r = r;
	this->a = a;
	this->b = b;
	this->p = p;
	this->G = G;
	this->P = mul(r, G);
	//����n
	Point tG = G;
	this->n = 1;
	while (!tG.O())
	{
		this->n++;
		tG = add(tG, G);
	}
}

ECC::ECC(LL r, LL a, LL b, LL p, Point G, LL n)
{
	this->r = r;
	this->a = a;
	this->b = b;
	this->p = p;
	this->G = G;
	this->P = mul(r, G);
	this->n = n;
}

/*
	��ȡ�����k , 1 <= k < p
*/
LL ECC::getRandom()
{
	LL low = 1, up = p;
	return (rand() % (up - low)) + low;
}