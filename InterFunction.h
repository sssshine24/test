#ifndef _InterFunc_3D_
#define _InterFunc_3D_
// MAF���������е������������쳤��
#define  Extend_Length   0.15
struct CS_DisLine {
    int       np;       // number of intersection point
    Point3D   *p;       // 3D point coordinate of intersection
};
struct CS_FreeCurve {
    int       cn;       // number of control point
    int       order;    // order of the curve
    Point3D   *ctrl;    // control point array
    double    *weight;  // weight value array
    double    *knot;    // knot vector array
};
struct CS_IntCurve {
    int          type;  // INTER_POINT1P, ... , INTER_LINE, ...
    Point3D      pt[8], cp;
    Vector3D     vx, vy, vn;
    double       r, a, b, p;
    double       st, ed, z_min, z_max;
    CS_DisLine   disline;
    CS_FreeCurve curve;
};
// //////////////////////////////
// ����Bezier����
// //////////////////////////////
struct CS_RBezierCurve2d {
    int n;
    Point2D *ctrl;
    double  *weight;
    CS_RBezierCurve2d()
    {
		ctrl = NULL;
		weight = NULL;
    }
//    ~CS_RBezierCurve2d()
//    {
//		if (ctrl != NULL)
//		{
//			delete []ctrl;
//			ctrl = NULL;
//		}
//		if (weight != NULL)
//		{
//			delete []weight;
//			weight = NULL;
//		}	
//    }	
} ;
// ������������İ�Χ��
struct CS_Box{
    Point3D  m_min,m_max;
};
// //////////////////////////
// ��άƽ��
// //////////////////////////
struct CS_Plane{
    Point3D   pa;           // pa Ϊƽ����һ����
    Vector3D  vn;           // vn Ϊƽ��ķ���
}; 
// //////////////////////////////
// ����Bezier����
// //////////////////////////////
struct  CS_RBezierCurve {
    int n;
    Point3D *ctrl;
    double  *weight;
    CS_RBezierCurve(){;}
} ;
// ��ɢ�����ʼ���㼰MAF�������õ����⽻��ṹ
// ������
struct CS_CurvCurvIntPoint{
    Point3D p;           //  ���ƽ���
    double  t1,t2;       //  ���������������ϵĲ���
};
// //////////////////////////////
// NURBS����
// //////////////////////////////
enum CS_NurbsType {
		CSNT_UNKNOWN = 0, 
		CSNT_IsBSpline, CSNT_IsNURBS, 	// NURBS��������ΪB������NURBS
		CSNT_Close, CSNT_Open, 			// NURBS��������Ϊ���š����
		CSNT_Revolut, CSNT_Extrude,		// ��ת�桢������
};
struct CS_NurbsCType {
	CS_NurbsType isBSpline;
	CS_NurbsType close;
};
struct CS_SubRBezierCurve {   // �������㷨������Bezier���߶νṹ
    CS_RBezierCurve curve;    // ����Bezier���߶�
    double ts, te;            // ���߶������յ������ԭ���ߵĲ���
    CS_Box box;               // ���߶εİ�Χ��
    double linear;			//����ָ��
    int level;				//���ڵķָ����
    struct CS_SubRBezierCurve *subCurve[2]; // ϸ�ֳɵ����������߶�
} ;
struct CS_NurbsCurve;
// �������������󽻿��ڲ�������ʾ
struct CS_NurbsCurveAuxRep {    // ��֯Nurbs���߷ָ�����ͷ���
    CS_NurbsCurve* curve;      // ����Nurbs����
    CS_Box box;               // Nurbs���ߵİ�Χ��
    int subNum;               // �ֳɵ������߶εĶ���
    CS_SubRBezierCurve **subCurve;  // ÿ�������߶ν��
} ;
struct CS_NurbsCurve {
    int n;
    int order;
    double *knot;
    Point3D *ctrl;
    double *weight;
		//--------------Revised by YangLei 060330----��¼�����󽻿��ڲ�������ʾ-----
	CS_NurbsCurveAuxRep* auxRep;
	CS_NurbsCType type;
	//-----------------------------

	//---------Modified by Ruan Xiao Yu 05-09-30---��¼��������ɢ�ĵ���Ϣ-------
	int numb;
	double *t;
	Point3D *poin;
	//-----------------------------------
} ;
// Yang Lei 060330

// ///////////////////////////
//  ��άֱ��
// ///////////////////////////
struct CS_Line{
    Point3D   pa;           // pa Ϊֱ���ϵ�һ����
    Vector3D  vn;           // vn Ϊֱ�ߵķ���
};

struct CS_BSplineCurve {
    int n;				   // nΪ�������
    int order;			   // orderΪ����
    double *knot;
    Point3D *ctrl;
} ;

class Line2D
{
private:
  Point2D m_start,m_end;
public:
  Line2D(void);
  Line2D(Point2D start, Point2D end);

  double distanceTo(Point2D pt);
};
struct  CS_GRBezierCurve {
    double start,end;
    CS_RBezierCurve rbezier;
} ;
struct CS_GRBezierCurve2d {
    double start,end;
    CS_RBezierCurve2d rbezier;
} ;
// ������������İ�Χ��
struct CS_Box2d{
    Point2D  m_min,m_max;
};
class Line3D
{
private:
  Point3D m_start,m_end;
public:
  Line3D(void);
  //Line3D(double u1,double v1,double u2,double v2);
  Line3D(Point3D start, Point3D end);

  double distanceTo(Point3D pt);
};

#define  INTER_POINT8P        8
#define  INTER_POINT1P        1
extern int LineNurbscInters(CS_Line line, CS_NurbsCurve curve,  CS_IntCurve *inp, int &numb);
extern int LineBezier4Inters(CS_Line line, CS_NurbsCurve curve,  CS_IntCurve *inp, int &numb);
extern double NurbscGetPointPara(CS_NurbsCurve curve, Point3D p);
extern int NurbscIntervalIndex(CS_NurbsCurve curve, double value);
extern void NurbscInsertMultiKnot(CS_NurbsCurve &curve, int multiplicity, double value);
extern int BscurvIntervalIndex(CS_BSplineCurve curve, double value);
extern double SPL_RbezcHeightEstimate(CS_RBezierCurve curve);
extern double SPL_RbezcHeightEstimate(CS_RBezierCurve2d curve);
extern int NurbscToRBezier(CS_NurbsCurve curve, CS_RBezierCurve bezier[], double *&t);
extern void SPL_GRbezcSubdivision(CS_GRBezierCurve2d curve, double Epsilon, Point2D *&vp, double *&t, int &number);
extern void SPL_GRbezcSubdivision(CS_GRBezierCurve curve, double Epsilon, Point3D *&vp, double *&t, int &number)	;
extern CS_Box SPL_GetCurvSurfBox3d(int number,Point3D points[]);
extern void SPL_GRBezcRecursive(CS_GRBezierCurve curve, double length, Point3D *&vp, double *&t, int &number) ;
extern void SPL_GRBezcSplit(CS_GRBezierCurve curve, double value, CS_GRBezierCurve &pc1, CS_GRBezierCurve &pc2);
extern void SPL_RbezcSplit(CS_RBezierCurve curve, double value, CS_RBezierCurve &pc1, CS_RBezierCurve &pc2);
extern bool CS_NurbsCIsBspl(CS_NurbsCurve& curv);
extern void PutNurbsCToBspl(const CS_NurbsCurve& ncurv, CS_BSplineCurve& bcurv);
extern void BscurvComputAll(CS_BSplineCurve curve, double value, Point3D &iterp, Vector3D &axis);
extern Point3D BscurvPosition(CS_BSplineCurve curve, double value);
extern Vector3D BscurvDerivative1(CS_BSplineCurve curve, double value);
extern Point3D NurbscPosition(CS_NurbsCurve curve, double value);
extern Point3D PointProjectToLine(Point3D point, Point3D origin, Vector3D xaxis) ;
#endif
