 // Point3D.h: interface for the Point3D class.
//
//////////////////////////////////////////////////////////////////////

#if !defined(AFX_POINT3D_H__C77F83A2_3EFB_4580_B188_5E8736F6F009__INCLUDED_)
#define AFX_POINT3D_H__C77F83A2_3EFB_4580_B188_5E8736F6F009__INCLUDED_

#if _MSC_VER > 1000
#pragma once
#endif // _MSC_VER > 1000
#include "OpenMesh.h"

class Vector2D;
class Vector3D;
class Vector4D;
class Point2D;
class Point3D;
class CVertex3d;
/*-------------------------------------------------------*/

class Vector2D
{
public:
	double	x, y;

public:
	//���캯��
	Vector2D ()
		{ x = y = 0; }
	Vector2D (double newx, double newy)
		{ x = newx; y = newy; }

	//��ֵ������
	void operator += (const Vector2D& v)
		{ x += v.x; y += v.y; }
	void operator -= (const Vector2D& v)
		{ x -= v.x; y -= v.y; }
	void operator *= (double num)
		{ x *= num; y *= num; }
	void operator /= (double num)
		{ //ASSERT (fabs (num) < TOLER);
		  x /= num; y /= num; 
		}

	//ʸ����
	Vector2D operator + (const Vector2D& v) const
		{ return Vector2D (x + v.x, y + v.y); }
	//ʸ����
	Vector2D operator - (const Vector2D& v) const
		{ return Vector2D (x - v.x, y - v.y); }
	//ʸ����(���)
	double  operator * (const Vector2D& v) const
		{ return x * v.x + y * v.y; }
	//����
	Vector2D operator * (double num) const
		{ return Vector2D (x * num, y * num); }
	//����
	Vector2D operator / (double num) const
		{ //ASSERT (fabs (num) < TOLER);
		  return Vector2D (x / num, y / num); 
		}

	//��Ŀ��
	Vector2D operator - () const
		{ return Vector2D (-x, -y); }

	//�е�(����)
	int operator == (const Vector2D& v) const;
	int operator != (const Vector2D& v) const;

	//����ת��
	operator Point2D ();

	//ȡԪ��
	double& operator [] (int i)
		{
			switch (i) 
			{
				case 0: return x;
				case 1: return y;
				default: return x;
			}
		}
	
	//��Ϊ��λʸ��
	void SetUnit ();       

	void SetValue(double xx,double yy)
	{
		x = xx;
		y = yy;
	}
};

/*-------------------------------------------------------*/

class Vector3D
{
public:
	double	x, y, z;

public:
	//���캯��
	Vector3D ()
		{ x = y = z = 0; }
	Vector3D (double newx, double newy, double newz)
		{ x = newx; y = newy; z = newz; }

	//��ֵ������
	void operator += (const Vector3D& v)
		{ x += v.x; y += v.y; z += v.z; }
	void operator -= (const Vector3D& v)
		{ x -= v.x; y -= v.y; z -= v.z; }
	void operator *= (double num)
		{ x *= num; y *= num; z *= num; }
	void operator /= (double num)
		{ //ASSERT (fabs (num) < TOLER);
		  x /= num; y /= num; z /= num; 
		}
	void operator ^= (const Vector3D& v)
		{ double a =   y * v.z - z * v.y;
		  double b = - x * v.z + z * v.x;
		  double c =   x * v.y - y * v.x;

		  x = a;
		  y = b;
		  z = c;
		}

	//ʸ����
	Vector3D operator + (const Vector3D& v) const
		{ return Vector3D (x + v.x, y + v.y, z + v.z); }
	//ʸ����
	Vector3D operator - (const Vector3D& v) const
		{ return Vector3D (x - v.x, y - v.y, z - v.z); }
	//ʸ����(���)
	double  operator * (const Vector3D& v) const
		{ return x * v.x + y * v.y + z * v.z; }
	//ʸ����(���)
	Vector3D operator ^ (const Vector3D& v) const
		{ return Vector3D ( y * v.z - z * v.y,
						  - x * v.z + z * v.x,
							x * v.y - y * v.x); 
		}
	//����
	Vector3D operator * (double num) const
		{ return Vector3D (x * num, y * num, z * num); }




	//����
	Vector3D operator / (double num) const
		{ //ASSERT (fabs (num) < TOLER);
		  return Vector3D (x / num, y / num, z / num); 
		}
	//��Ŀ��
	Vector3D operator - () const
		{ return Vector3D (-x, -y, -z); }

	//�е�(����)
	int operator == (const Vector3D& v) const;
	int operator != (const Vector3D& v) const;

	//����ת��
	operator Point3D ();

	//setvalue
	void SetValue(double x_,double y_,double z_)
	{
		x = x_;
		y = y_;
		z = z_;
	}

	//ȡԪ��
	double& operator [] (int i)
		{
			switch (i) 
			{
				case 0: return x;
				case 1: return y;
				case 2: return z;
				default: return x;
			}
		}

	//��Ϊ��λʸ��
	void SetUnit ();
};

/*-------------------------------------------------------*/

class Point2D
{
public:
	double	x, y;

public:
	//���캯��
	Point2D ()
		{ x = y = 0;}
	Point2D (double newx, double newy)
		{ x = newx; y = newy;}

	//��ֵ������
	void operator += (const Vector2D& v)
		{ x += v.x; y += v.y; }
	void operator -= (const Vector2D& v)
		{ x -= v.x; y -= v.y; }

	//�������
	Point2D operator + (const Vector2D& v) const;
	//�������
	Point2D operator - (const Vector2D& v) const;
	//�������
	Vector2D operator - (const Point2D& v) const;
	
	//��Ŀ��
	Point2D operator - () const;

	//�е�(����)
	int operator == (const Point2D& v) const;
	int operator != (const Point2D& v) const;
   
    //ȡԪ��
	double& operator [] (int i)
		{
			switch (i) 
			{
				case 0: return x;
				case 1: return y;
				default: return x;
			}
		}
};

class Point3D
{
public:
	double	x, y, z;

public:
	//���캯��
	Point3D ()
		{ x = y = z = 0;}
	Point3D (double newx, double newy, double newz)
		{ x = newx; y = newy; z = newz; }

	//��ֵ������
	void operator += (const Vector3D& v)
		{ x += v.x; y += v.y; z += v.z; }
	//��ֵ������
	void operator = (const Point3D& v)
	{
		x = v.x; 
		y = v.y; 
		z = v.z; 
	}
	
	void operator -= (const Vector3D& v)
		{ x -= v.x; y -= v.y; z -= v.z; }

	//����
	Point3D operator * (double num) const
		{ return Point3D (x * num, y * num, z * num); }
	//����
	Point3D operator / (double num) const
		{ return Point3D (x / num, y / num, z / num); }

	//�������
	Point3D operator + (const Point3D& v) const;

	//�������
	Point3D operator + (const Vector3D& v) const;
	//�������
	Point3D operator - (const Vector3D& v) const;
	//�������
	Vector3D operator - (const Point3D& v) const;

	//��Ŀ��
	Point3D operator - () const;

	//�е�(����)
	int operator == (const Point3D& v) const;
	int operator != (const Point3D& v) const;
	
	//ȡԪ��
	double& operator [] (int i)
		{
			switch (i) 
			{
				case 0: return x;
				case 1: return y;
				case 2: return z;
				default: return x;
			}
		}
	//��ֵ
	
	void setValue(double newx, double newy, double newz)
		{ x = newx; y = newy; z = newz; }
};
class Vector4D
{
public:
	double	x, y, z, w;

public:
	//���캯��
	Vector4D ()
		{ x = y = z = w = 0; }
	Vector4D (double newx, double newy, double newz, double neww)
		{ x = newx; y = newy; z = newz; w = neww; }
	Vector4D (Vector3D v, double neww)
		{ x = v.x ; y = v.y ; z = v.z ; w = neww; }
	//��ֵ������
	void operator += (const Vector4D& v)
		{ x += v.x; y += v.y; z += v.z; w+=v.w;}
	void operator -= (const Vector4D& v)
		{ x -= v.x; y -= v.y; z -= v.z; w -= v.w; }
	void operator *= (double num)
		{ x *= num; y *= num; z *= num; w *= num;}
	void operator /= (double num)
		{ //ASSERT (fabs (num) < TOLER);
		  x /= num; y /= num; z /= num; w /= num; 
		}
	

	//ʸ����
	Vector4D operator + (const Vector4D& v) const
		{ return Vector4D (x + v.x, y + v.y, z + v.z, w + v.w); }

	//ʸ����
	Vector4D operator - (const Vector4D& v) const
	{ 
		return Vector4D (x - v.x, y - v.y, z - v.z, w - v.w); 
	}
	//ʸ����(���)
	double  operator * (const Vector4D& v) const
		{ return x * v.x + y * v.y + z * v.z + w * v.w; }
	
	//����
	Vector4D operator * (double num) const
		{ return Vector4D (x * num, y * num, z * num , w * num); }
	//����
	Vector4D operator / (double num) const
		{ //ASSERT (fabs (num) < TOLER);
		  return Vector4D (x / num, y / num, z / num, w / num); 
		}
	//��Ŀ��
	Vector4D operator - () const
		{ return Vector4D (-x, -y, -z, -w); }


	void setValue(Vector3D v, double neww)
	{
		x = v.x*neww; y = v.y*neww; z = v.z*neww; w = neww;
	}
	void setValue(double newx,double newy,double newz, double neww)
	{
		x = newx; y = newy; z = newz; w = neww;
	}
	void SetUnit();
	//����ת��
	//ȡԪ��
	double& operator [] (int i)
		{
			switch (i) 
			{
				case 0: return x;
				case 1: return y;
				case 2: return z;
				case 3: return w;
				default: return x;
			}
		}


};
// ///////////////////////////
// ��ά�߶�
// ///////////////////////////
struct CS_LineSeg{
    Point3D pa,pb;          // pa,pb: �߶ε����˵�
};
struct CS_LineSeg2d{
    Point2D pa,pb;          // pa,pb: �߶ε����˵�
};
struct CS_Line2d{
    Point2D   pa;           // pa Ϊֱ���ϵ�һ����
    Vector2D  vn;           // vn Ϊֱ�ߵķ���
};

double Length (const Vector2D& v);
double Length (const Vector3D& v);
double Length (Vector4D v);
double Length(OpenMesh::Vec3d v);
Point2D MidPoint (const Point2D& p1, const Point2D& p2);
Point3D MidPoint (const Point3D& p1, const Point3D& p2);
double Distance (const Point2D& p1, const Point2D& p2);
double Distance (const Point3D& p1, const Point3D& p2);
double Distance(CVertex3d *pVertex1, Point3D p2);
Point2D Min (const Point2D& p1, const Point2D& p2);


Point3D Min (const Point3D& p1, const Point3D& p2);

Point2D Max (const Point2D& p1, const Point2D& p2);

Point3D Max (const Point3D& p1, const Point3D& p2);
inline int     Max(int v1, int v2){
	return v1>v2?v1:v2;
}

#endif // !defined(AFX_POINT3D_H__C77F83A2_3EFB_4580_B188_5E8736F6F009__INCLUDED_)
