#pragma once
#include "transformation.h"
class CMesh3d;
class CNurbsSuface;
class GeneralBilinearTransformation : public Transformation
{
public:

	CNurbsSuface* pSurface,*pNewSurface;

	//for Inverse Evaluation to select which branch.
	int label;

	//the coefficient of the transformation
	double e00,e01,e10,e11,f00,f01,f10,f11;
	double g00,g01,g10,g11,h00,h01,h10,h11;

	void AddCoefficient(double s,double t,std::vector<double> &coefficient,std::vector<std::vector<double>> &coefficients);

	//evaluation
	int Evaluation(double s,double t,double &u,double &v);

	//
	void solveControlPoints();

	//
	virtual void Save(std::ofstream &file);

	virtual void Read(std::ifstream &file);

	//inverse evaluation
	int InverseEvaluation(double u,double v,double &s,double &t);

	void SetInverseEvaluationLabel(CMesh3d* pMeshuv,CMesh3d* pMeshst);

	//Update Bounding Box
	void UpdateBoundingBox();

	//the seven points
	Point2D p1,p2,p3,p4,p5,p6,p7;

	//Set the bilinear transformation from seven points
	int ComputeGeneralBilinearTransformationFromSevenPoints();

	//check whether the transformation is valid or not
	int IsValid();

	//Set seven points
	int SetSevenPoints(Point2D p1,Point2D p2,Point2D p3,Point2D p4,Point2D p5,Point2D p6,Point2D p7);

	int SetReparameterizationCoefficients(std::vector<double> &c);

	double Comput_u_s(double s,double t);

	double Comput_u_t(double s,double t);

	double Comput_v_s(double s,double t);

	double Comput_v_t(double s,double t);

	double GetCoefficient(int index);

	void SetCoefficient(int index,double val);

	Transformation* copy();

	void Trace();

	void GetWeight_33();

	void AddWeightCoefficient(double s,double t,std::vector<std::vector<double>>& coefficients,std::vector<double> &b);

	GeneralBilinearTransformation(void);
	~GeneralBilinearTransformation(void);
};
