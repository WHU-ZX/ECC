#pragma once
#include <string>
#define LL long long
#define INF 0x7FFFFFFFFFFFFFFF
#define BYTE_PER_POINT 1 //�����ٸ��ֽ�ѹ����һ����

struct Point
{
	LL x, y;
	Point()
	{
		this->x = 0;
		this->y = 0;
	}
	Point(LL x, LL y)
	{
		this->x = x;
		this->y = y;
	}
	bool operator == (const Point& p) const
	{
		return this->x == p.x && this->y == p.y;
	}
	bool O()
	{
		return this->x == INF && this->y == INF;
	}
};

struct SignedMessage
{
	std::string message;
	std::string hash;
	LL numbers[16 / BYTE_PER_POINT];
	LL Sn[16 / BYTE_PER_POINT];//MD5��HASH���Ϊ16�ֽ�
};

struct VerifyResult
{
	bool success;//�Ƿ�ǩ���ɹ�
	std::string message;
};

struct PointPair
{
	Point first, second;
};

class ECC
{
private:// ��Ա����
	
	LL r;//��Կ

	//��Բ���� y^2 = x^3 + ax + b  mod p
	LL a;
	LL b;
	LL p;
	LL n;// G �Ľ�

	LL randomK;//����� -- ���ܲ����õ�

	Point G;//����
	Point P;//��Կ P = rG
public:// ��Ա����
	Point add(Point p1, Point p2);// ����p1 + p2 �Ľ��
	Point minus(Point p1, Point p2);// ���� p1 - p2 �Ľ��
	Point mul(LL k, Point p);// ���� k*p �Ľ��
	Point subMul(LL k, Point p, Point ps[], bool flags[]);// ��mul��������k*p
	
	PointPair encode(Point Pm); // ����ϢPm���ܳ�{kG,Pm+kP}������ֵ�ĵ�һ������kG���ڶ�����Pm+kP
	Point decode(PointPair Cm); // ������Cm = {kG,Pm+kP}���ܳ���ϢM��Ӧ�ĵ�Pm

	//��Ϣ�����Ϳ����޸ģ���һ����char*  ,stringʲô��Ҳ��
	Point encodeMessage(char* message);//����Ϣת��Ϊ�����ϵĵ�Pm
	char* decodeMessage(Point Pm);//�������ϵĵ�Pmת��Ϊ��Ϣmessage����encodeMessage��Ϊ�����

	LL getRandom(); // ��ȡ�����k , 1 <= k < p


private:// ��̬���ߺ�����
	static LL mod(LL a, LL n); // ���� a mod n����Ҫ�Ǵ���aΪ���������
	static LL inv(LL a, LL n); // ���� a����ģn����Ԫ������������򷵻�0
	static LL gcd(LL a, LL b); // ����gcd(a,b)
	static Point extend_gcd(LL a, LL b);//��չ��ŷ������㷨��(x,y)ʹ��ax+by = gcd(a,b)
	
public:// ��ǰ�˵��õĽӿ�
	ECC(); // Ĭ��ʹ��Ĭ�ϵ�ֵ���й���

	//���¹��캯��Ҫͨ����ʽ P=rG �����P��ֵ
	ECC(LL r,LL a,LL b,LL p,Point G); // �����Զ����ֵ���й��죬���Ǳ�������G�Ľף�ͨ��O=nG����
	ECC(LL r, LL a, LL b, LL p, Point G, LL n);

	
	//��ʱ���ܷ���ֵ�Ͳ�������  ---  ÿ���һ���ٽ��ע��һ��
	/*
	void encode(); // �����ļ���Ϊ����
	void decode(); // �����ļ���Ϊ����
	
	
	*/

	SignedMessage sign(std::string message); // ����Ϣ��������ǩ��
	VerifyResult verify(SignedMessage signedMessage); // ����ǩ����֤
};

