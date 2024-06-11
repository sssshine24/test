#if !defined(CURVE_H)
#define CURVE_H
#include "Point3D.h"
#include "InterFunction.h"
class Point3D;
class Vector3D;
class  C_Entity3D : public CObject
{

		
private:
	
public:
	// ���캯������������
	C_Entity3D(){};
	~C_Entity3D(){};
	// �������ݴ�ȡ����ʾ�ĳ�Ա����
	void GetBox(Point3D& min,Point3D& max){};
	void SetBox(Point3D min,Point3D max){};
	virtual int PrintInformation(){return 0;};

};

class  C_Curve3D : public C_Entity3D
{
	//DECLARE_SERIAL( C_Curve3D )
		
private:
	
public:
	// ���캯������������
	C_Curve3D();
	virtual ~C_Curve3D(); // nt add 1997/3/31
	// �������ݴ�ȡ����ʾ�ĳ�Ա����
	unsigned char GetStyle();
	void SetStyle(unsigned char style);
	int  PrintInformation();
	virtual void Draw(CDC* pDC);
	
	virtual Point3D GetStartPoint();
	virtual Point3D GetEndPoint();
	virtual Point3D GetMiddlePoint();
	virtual Point3D GetParaPoint(double t);
	virtual double GetPointPara(Point3D p);
	virtual double GetPointDistance(Point3D p);
	virtual BOOL PointOn(Point3D p, double toler);
	virtual C_Curve3D *Copy();
	virtual C_Curve3D *Reverse();
	
	virtual void SplitD(double t, C_Curve3D **pC1, C_Curve3D **pC2);
	virtual void SplitP(Point3D& p, C_Curve3D **pC1, C_Curve3D **pC2);
	virtual void SplitN(int n, double t[], Point3D p[], C_Curve3D *newcurve[]);
	virtual Vector3D GetParaTangent(double t);
	
	//yy, 97/04/28
	//virtual void Serialize(CArchive& ar);

};

class  C_ParaCurve3D : public C_Curve3D
{

		
protected:
	int     m_number;     // ���Ƶ���,���Ƶ��0��m_number
	Point3D *m_control;   // ���߿��Ƶ�
public:
	C_ParaCurve3D(void);
	C_ParaCurve3D(int number);
	C_ParaCurve3D(int number, Point3D *control);
	~C_ParaCurve3D(void); // �ͷ�m_control���ڴ�
	// �������ݴ�ȡ����ʾ�ĳ�Ա����
	int      GetCtrlNumber(void);
	Point3D  GetControlPoint(int index);
	void     SetCtrlNumber(int number);
	void     SetControlPoint(int index,Point3D point);

	
	// ���ڼ�������ĺ���
	virtual Point3D GetStartPoint(void);
	virtual Point3D GetEndPoint(void);
	virtual Point3D GetMiddlePoint(void);
	virtual Point3D GetParaPoint(double t);
	virtual double GetPointPara(Point3D p);
	virtual double GetPointDistance(Point3D p);
	virtual BOOL PointOn(Point3D p, double toler);
	virtual C_Curve3D *Copy(void);
	virtual C_Curve3D *Reverse(void);
	virtual void SplitD(double t, C_Curve3D **pC1, C_Curve3D **pC2);
	virtual void SplitP(Point3D &p, C_Curve3D **pC1, C_Curve3D **pC2);
	virtual void SplitN(int n, double t[], Point3D p[], C_Curve3D *newcurve[]);
	
	virtual Vector3D GetParaTangent(double t);

	virtual void Serialize(CArchive& ar);
};

class  C_BezierCurve3D : public C_ParaCurve3D
{

		
public:
	double weight[50][50];
	// ���캯������������
	C_BezierCurve3D(void);
	C_BezierCurve3D(int number);
	C_BezierCurve3D(int number, Point3D *control);
	
	// ����
	virtual C_Curve3D *Copy(void);
	
	// ����������һ���ֵ
	Point3D ComputPosition(double value);
	// �����߷ָ�Ϊ��������
	void SplitD(double value, C_BezierCurve3D &pc1, C_BezierCurve3D &pc2);
	// ��������ɢΪֱ�߶Σ�EpsilonΪ�߶ȹ��Ƶ���vp���У�numberΪ�����
	void Subdivision(double Epsilon, Point3D *&vp, int &number);	
	// Bezier������ɢΪ�㼰��Ӧ���� 
	void SubdivisionWithPara(double Epsilon, Point3D *&vp, double *&t, int &number);
	
	// �������ݴ�ȡ����ʾ�ĳ�Ա����
	int PrintInformation(void);
	void SplitP(Point3D point, C_BezierCurve3D &pc1, C_BezierCurve3D &pc2);

	virtual void Serialize(CArchive& ar);
};

class  C_RationalBezierCurve3D : public C_BezierCurve3D
{

		
private:
	double *m_weight;  // ���Ƶ�Ȩֵ
public:
	// ���캯������������
	C_RationalBezierCurve3D(void);
	C_RationalBezierCurve3D(int number);
	C_RationalBezierCurve3D(int number, Point3D *control, double *weight);
	~C_RationalBezierCurve3D(void);
	// ����������һ���ֵ
	Point3D ComputPosition(double value);
	// ����
	C_Curve3D *Copy(void);
	
	// �������ݴ�ȡ����ʾ�ĳ�Ա����
	double GetWeight(int index);
	void   SetWeight(int index,double value);
	int    PrintInformation(void);
	

	virtual void Serialize(CArchive& ar);
};


class  C_BSplineCurve3D : public C_ParaCurve3D
{

		
protected:
	int   m_order;     // ���߽���
public:
	// ���캯������������
	C_BSplineCurve3D(void);
	C_BSplineCurve3D(int number, int order);
	C_BSplineCurve3D(int number, int order, Point3D *control);
	~C_BSplineCurve3D(void);
	// �������ݴ�ȡ����ʾ�ĳ�Ա����
	int  GetDegree(void);
	int  GetOrder(void);
	void SetOrder(int order);
	int  CalBFunc(double u, double *Nu, double *Du, double *Hu);
	int  PrintInformation(void);
	
	// ���ڼ�������ĺ���
	virtual Point3D GetStartPoint(void);
	virtual Point3D GetEndPoint(void);
	virtual Point3D GetMiddlePoint(void);
	virtual Point3D GetParaPoint(double t);
	virtual double GetPointPara(Point3D p);
	virtual double GetPointDistance(Point3D p);
	virtual BOOL PointOn(Point3D p, double toler);
	virtual C_Curve3D *Copy(void);
	virtual C_Curve3D *Reverse(void);
	virtual void SplitD(double t, C_Curve3D **pC1, C_Curve3D **pC2);
	virtual void SplitP(Point3D &p, C_Curve3D **pC1, C_Curve3D **pC2);
	virtual void SplitN(int n, double t[], Point3D p[], C_Curve3D *newcurve[]);


};





class  C_NonuniformBCurve3D : public C_BSplineCurve3D
{

		
protected:
	double  *m_knot;      // ���߽ڵ�����
public:
	// ���캯������������
	C_NonuniformBCurve3D(void);
	C_NonuniformBCurve3D(int number, int order);
	C_NonuniformBCurve3D(int number, int order, Point3D *control,
		double *knot, int flag = 0);   // flag = 1 ���ڲ�ֵ

	// ��ֵ,����Blending, �����˵����򡢽ڵ�����
	C_NonuniformBCurve3D(int number, int order, Point3D *control, Vector3D tangent1, 
		Vector3D tangent2, double *knot);  

	// ���ڲ�ֵ��numberΪ��ֵ�����, �ڵ����ۼ��ҳ���ȷ�� 
	C_NonuniformBCurve3D(int number, int order, Point3D *point);  
	~C_NonuniformBCurve3D(void);
	// �������ݴ�ȡ����ʾ�ĳ�Ա����
	double  GetKnot(int index);
	void    SetKnot(int index,double value);
	double  *GetKnotVector();  
	int     GetKnotMulti(double value);
	int     PrintInformation(void);
	int     GetIntervalIndex(double  value);
	int     GetMonotonyKnot(double  *&t);
	virtual void     SetNumberOrder(int number, int order);
	virtual Point3D  ComputPosition(double value); 
	virtual Vector3D ComputDerivative1(double value); 
	virtual Vector3D ComputDerivative2(double value); 
	virtual double   ComputCurvature(double value);
	virtual void     ComputDerivative2All(double value, Point3D &position, Vector3D &p1p, Vector3D &p2p);
	virtual void     Draw(CDC *pDC);
	virtual void   InsertKnot(double value);
	virtual void   InsertMultiKnot(int multiplicity, double value);
	
	// ���ڼ�������ĺ���
	virtual Point3D GetStartPoint(void);
	virtual Point3D GetEndPoint(void);
	virtual Point3D GetMiddlePoint(void);
	virtual Point3D GetParaPoint(double t);
	virtual double GetPointPara(Point3D p);
	virtual double GetPointDistance(Point3D p);
	virtual double GetPointDistance(Point3D p, double * para); 
	virtual BOOL PointOn(Point3D p, double toler);
	virtual BOOL PointOn(Point3D p, Point3D &q, double toler);
	virtual C_Curve3D *Copy(void);
	// ��չ
	virtual void Expand(int flag, double scale);  //flag=0�������,flag=1�Ҷ�����,scaleΪ����
	virtual C_Curve3D *Reverse(void);
	virtual void SplitD(double t, C_Curve3D **pC1, C_Curve3D **pC2);
	virtual void SplitP(Point3D &p, C_Curve3D **pC1, C_Curve3D **pC2);
	virtual void SplitN(int n, double t[], Point3D p[], C_Curve3D *newcurve[]);
	
	virtual Vector3D GetParaTangent(double t);
	// ������ת��ΪBezier����
	int ConvertToBezier(C_BezierCurve3D *bezier[]);
	// ������ɢΪ�㼰��Ӧ���� 
	void SubdivisionWithPara(double Epsilon, Point3D *&vp, double *&t, int &number);
	// ���Ƿ��Խ�
	BOOL IfSelfInter(void); 
	


	virtual void Serialize(CArchive& ar);
};
class  C_NurbsCurve3D : public C_NonuniformBCurve3D
{

		
private:
	double *m_weight;  // ���Ƶ�Ȩֵ
public:
	// ���캯������������
	C_NurbsCurve3D(void);
	C_NurbsCurve3D(int number, int order);
	C_NurbsCurve3D(int number, int order, Point3D *control,
		double *knot, double *weight);
	C_NurbsCurve3D(int number, int order, Point3D *control, Vector3D tangent1, Vector3D tangent2,
		double *knot, double *weight);

	// ���ڲ�ֵ��numberΪ��ֵ�����, �ڵ����ۼ��ҳ���ȷ�� 
	C_NurbsCurve3D(int number, int order, Point3D *point); 

	//���ڲ�ֵ��numberΪ��ֵ����������Ƚڵ�
	C_NurbsCurve3D(int number, int order, Point3D *point,double* knot); 

	~C_NurbsCurve3D(void);

	//Compute the basis 
	double ComputeBasis(int i,double t);

	//Compute the basis with degree
	double ComputeBasis(int i,int p,double u);

	// �������ݴ�ȡ����ʾ�ĳ�Ա����
	double   GetWeight(int index);
	void     SetWeight(int index,double value);
	int      PrintInformation(void);
	void     SetNumberOrder(int number, int order);
	Point3D  ComputPosition(double value); 
	Vector3D ComputDerivative1(double value); 
	Vector3D ComputDerivative2(double value); 
	void     ComputAll(double value, Point3D &position, Vector3D &Derivative1, Vector3D &Derivative2);
	void	   ComputLocal(double value, Point3D &position, Vector3D &tangent, Vector3D &normal ,Vector3D &binormal, double &curvature);
	double   ComputCurvature(double value);
	void     Draw(CDC *pDC);
	void     InsertKnot(double value);
	void     InsertMultiKnot(int multiplicity, double value);
	int      InsertKnotSeq(int count, double *knots);
	
	// ���ڼ�������ĺ���
	// Point3D GetStartPoint(void);
	// Point3D GetEndPoint(void);
	// Point3D GetMiddlePoint(void);
	// Point3D GetParaPoint(double t);
	
	double  GetPointPara(Point3D p);
	double  GetPointDistance(Point3D p);
	double GetPointDistance(Point3D p, double * para); 
	BOOL PointOn(Point3D p, double toler);
	BOOL PointOn(Point3D p, Point3D &q, double toler);
	C_Curve3D *Copy(void);
	void Expand(int flag, double scale);
	C_Curve3D *Reverse(void);
	void SplitD(double t, C_Curve3D **pC1, C_Curve3D **pC2);
	void SplitP(Point3D &p, C_Curve3D **pC1, C_Curve3D **pC2);
	void SplitN(int n, double t[], Point3D p[], C_Curve3D *newcurve[]);
	Vector3D GetParaTangent(double t);

	void Serialize(CArchive& ar);
	void Inflection(int &num, double *&t, Point3D *&position, Vector3D *&vector);
	void glDraw();
};





Vector3D GetEndTangent(Point3D a, Point3D b, Point3D c, int type=0);
int BscurvInterpolateSweep(int number, Point3D *point, double *knot, CS_BSplineCurve &rcurv) ;
int LineLineInters(CS_Line,CS_Line,Point3D &);
int BscurvInterpolat(int number, Point3D *point, Vector3D tang1, Vector3D tang2, double *knot, CS_BSplineCurve &rcurv);
int BscurvInterpolatAuto(int number, Point3D point[], CS_BSplineCurve &rcurv);
void GetInterpolatKnot(int number, Point3D *point, double *&knot);

int NurbscInterpolat(int number, Point3D *point, double *knot, double *weight,
                           Vector3D tangent0, Vector3D tangent1, CS_NurbsCurve &rcurv);
int IllegalLine(CS_Line &line);
void BscurvBaseValu(CS_BSplineCurve curve, double u, double *coef);
int EquatSystemPoint(int degree, double **a, Point3D *y, Point3D *x);
void guassSelect(double **&a,int *&z,int k,int np);
void GetUniformInterpolatKnot(int number, Point3D *point, double *&knot);
#endif