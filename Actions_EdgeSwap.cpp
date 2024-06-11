// Actions_EdgeSwap.cpp: 实现文件
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "Actions_EdgeSwap.h"
#include "afxdialogex.h"
#include "MyTreeView.h"

// Actions_EdgeSwap 对话框
extern CMyTreeView* pTreeView;
IMPLEMENT_DYNAMIC(Actions_EdgeSwap, CDialogEx)

Actions_EdgeSwap::Actions_EdgeSwap(CWnd* pParent /*=nullptr*/)
	: CDialogEx(IDD_DIALOG_EDGESWAP, pParent)
{

}

Actions_EdgeSwap::~Actions_EdgeSwap()
{
}

void Actions_EdgeSwap::DoDataExchange(CDataExchange* pDX)
{
	CDialogEx::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Actions_EdgeSwap, CDialogEx)
	ON_WM_PAINT()
END_MESSAGE_MAP()


// Actions_EdgeSwap 消息处理程序


void Actions_EdgeSwap::OnPaint()
{
	CPaintDC dc(this); // device context for painting
					   // TODO: 在此处添加消息处理程序代码
					   // 不为绘图消息调用 CDialogEx::OnPaint()
	//
	PlotInitialMesh(dc);
	//
	PlotInitialMeshCircular(dc);
	//
	PlotInitialMeshCircularEdgeFlip(dc);
	//
	PlotInitialMeshCircularEdgeFlipPointMove(dc);
}
void Actions_EdgeSwap::PlotInitialMesh(CPaintDC &dc)
{
	Point2D p1(0, 58);
	Point2D p2(50, 10);
	Point2D p3(100, 60);
	Point2D p4(48, 120);
	Point2D p5;
	dc.SelectStockObject(NULL_BRUSH);

	//Line Segments
	dc.MoveTo(p1.x, p1.y);
	dc.LineTo(p2.x, p2.y);
	dc.LineTo(p3.x, p3.y);
	dc.LineTo(p4.x, p4.y);
	dc.LineTo(p1.x, p1.y);
	dc.LineTo(p3.x, p3.y);

	//Points
	dc.Ellipse(p1.x - 2, p1.y - 2, p1.x + 2, p1.y + 2);
	dc.Ellipse(p2.x - 2, p2.y - 2, p2.x + 2, p2.y + 2);
	dc.Ellipse(p3.x - 2, p3.y - 2, p3.x + 2, p3.y + 2);
	dc.Ellipse(p4.x - 2, p4.y - 2, p4.x + 2, p4.y + 2);

	//Plot Circle.
	double A1 = 2 * (p2.x - p1.x);
	double B1 = 2 * (p2.y - p1.y);
	double C1 = SQR(p2.x) + SQR(p2.y) - SQR(p1.x) - SQR(p1.y);
	double A2 = 2 * (p3.x - p2.x);
	double B2 = 2 * (p3.y - p2.y);
	double C2 = SQR(p3.x) + SQR(p3.y) - SQR(p2.x) - SQR(p2.y);

	double centerx = ((C1*B2) - (C2*B1)) / ((A1*B2) - (A2*B1));
	double centery = ((A1*C2) - (A2*C1)) / ((A1*B2) - (A2*B1));

	double r = sqrt(SQR(centerx - p1.x) + SQR(centery - p1.y));
	
	dc.Ellipse(centerx - r, centery - r, centerx + r, centery + r);

	//
	dc.SelectStockObject(BLACK_BRUSH);
	double alpha, belta, gama;
	alpha = 0.45;
	belta = 0.45;
	gama = 0.1;

	p5.x = p1.x * alpha + p2.x * belta + p3.x * gama;
	p5.y = p1.y * alpha + p2.y * belta + p3.y * gama;

	dc.Ellipse(p5.x - 2, p5.y - 2, p5.x + 2, p5.y + 2);

	double t_inter_anotherdirection = 0.23584905660377334;
	Point2D newinter;
	newinter.x = p1.x * (t_inter_anotherdirection) + p3.x * (1 - t_inter_anotherdirection);
	newinter.y = p1.y * (t_inter_anotherdirection) + p3.y * (1 - t_inter_anotherdirection);

	double newalpha, newbelta, newgama;
	double tarea1 = fabs(pTreeView->TriangleArea(p1.x, p1.y, p2.x, p2.y, newinter.x, newinter.y));
	double tarea2 = fabs(pTreeView->TriangleArea(p1.x, p1.y, p2.x, p2.y, p5.x, p5.y));
	double tarea3 = fabs(pTreeView->TriangleArea(p1.x, p1.y, newinter.x, newinter.y, p5.x, p5.y));
	double tarea4 = fabs(pTreeView->TriangleArea(newinter.x, newinter.y, p2.x, p2.y, p5.x, p5.y));

	double r1 = tarea2 / tarea1;
	double r2 = tarea3 / tarea1;
	double r3 = tarea4 / tarea1;
}
void Actions_EdgeSwap::PlotInitialMeshCircular(CPaintDC &dc)
{
	int xdelta = 200;
	Point2D p1(0 + xdelta, 58);
	Point2D p2(50 + xdelta, 10);
	Point2D p3(100 + xdelta, 60);
	Point2D p4(90 + xdelta, 85);
	Point2D p5;

	dc.SelectStockObject(NULL_BRUSH);
	CPen pen1(PS_SOLID, 1, RGB(0, 0, 0));
	dc.SelectObject(&pen1);

	//Line Segments
	dc.MoveTo(p1.x, p1.y);
	dc.LineTo(p2.x, p2.y);
	dc.LineTo(p3.x, p3.y);
	dc.LineTo(p4.x, p4.y);
	dc.LineTo(p1.x, p1.y);
	dc.LineTo(p3.x, p3.y);

	//Points
	dc.Ellipse(p1.x - 1, p1.y - 1, p1.x + 1, p1.y + 1);
	dc.Ellipse(p2.x - 1, p2.y - 1, p2.x + 1, p2.y + 1);
	dc.Ellipse(p3.x - 1, p3.y - 1, p3.x + 1, p3.y + 1);
	dc.Ellipse(p4.x - 1, p4.y - 1, p4.x + 1, p4.y + 1);

	//Plot Circle.
	double A1 = 2 * (p2.x - p1.x);
	double B1 = 2 * (p2.y - p1.y);
	double C1 = SQR(p2.x) + SQR(p2.y) - SQR(p1.x) - SQR(p1.y);
	double A2 = 2 * (p3.x - p2.x);
	double B2 = 2 * (p3.y - p2.y);
	double C2 = SQR(p3.x) + SQR(p3.y) - SQR(p2.x) - SQR(p2.y);

	double centerx = ((C1*B2) - (C2*B1)) / ((A1*B2) - (A2*B1));
	double centery = ((A1*C2) - (A2*C1)) / ((A1*B2) - (A2*B1));

	double r = sqrt(SQR(centerx - p1.x) + SQR(centery - p1.y));

	dc.Ellipse(centerx - r, centery - r, centerx + r, centery + r);
	double alpha, belta, gama;
	alpha = 0.45;
	belta = 0.45;
	gama = 0.1;
	p5.x = p1.x * alpha + p2.x * belta + p3.x * gama;
	p5.y = p1.y * alpha + p2.y * belta + p3.y * gama;

	dc.Ellipse(p5.x - 2, p5.y - 2, p5.x + 2, p5.y + 2);

	CPen pen2(PS_DOT, 1, RGB(0, 0, 0));
	dc.SelectObject(&pen2);
	dc.MoveTo(p2.x, p2.y);
	dc.LineTo(p4.x, p4.y);

	double t_inter;
	Vector2D V(p3.y - p1.y, p1.x - p3.x);
	t_inter = ((p1 - p2) * V) / ((p4 - p2) * V);
	Point2D interpoint;
	interpoint.x = p2.x * (1 - t_inter) + p4.x * t_inter;
	interpoint.y = p2.y * (1 - t_inter) + p4.y * t_inter;

	dc.SelectStockObject(BLACK_BRUSH);
	dc.Ellipse(interpoint.x - 3, interpoint.y - 3, interpoint.x + 3, interpoint.y + 3);

	double t_inter_anotherdirection = Length(interpoint - p3) / Length(p3 - p1);

	Point2D p1_(0, 58);
	Point2D p2_(50, 10);
	Point2D p3_(100, 60);
	Point2D p4_(48, 120);

	Point2D newinter;
	newinter.x = p1_.x * (t_inter_anotherdirection) + p3_.x * (1 - t_inter_anotherdirection);
	newinter.y = p1_.y * (t_inter_anotherdirection) + p3_.y * (1 - t_inter_anotherdirection);

	dc.Ellipse(newinter.x - 3, newinter.y - 3, newinter.x + 3, newinter.y + 3);
	dc.MoveTo(p2_.x, p2_.y);
	dc.LineTo(newinter.x, newinter.y);
	dc.LineTo(p4_.x, p4_.y);
}
void Actions_EdgeSwap::PlotInitialMeshCircularEdgeFlip(CPaintDC &dc)
{
	int xdelta = 200;
	int ydelta = 200;
	Point2D p1(0 + xdelta, 58 + ydelta);
	Point2D p2(50 + xdelta, 10 + ydelta);
	Point2D p3(100 + xdelta, 60 + ydelta);
	Point2D p4(90 + xdelta, 85 + ydelta);
	Point2D p5,p6;
	dc.SelectStockObject(NULL_BRUSH);
	CPen pen1(PS_SOLID, 1, RGB(0, 0, 0));
	CPen pen2(PS_DOT, 1, RGB(0, 0, 0));
	dc.SelectObject(&pen1);

	//Line Segments
	dc.MoveTo(p1.x, p1.y);
	dc.LineTo(p2.x, p2.y);
	dc.LineTo(p3.x, p3.y);
	dc.LineTo(p4.x, p4.y);
	dc.LineTo(p1.x, p1.y);
	dc.MoveTo(p1.x, p1.y);
	dc.LineTo(p3.x, p3.y);
	dc.SelectObject(&pen2);
	dc.MoveTo(p2.x, p2.y);
	dc.LineTo(p4.x, p4.y);

	//Points
	dc.Ellipse(p1.x - 1, p1.y - 1, p1.x + 1, p1.y + 1);
	dc.Ellipse(p2.x - 1, p2.y - 1, p2.x + 1, p2.y + 1);
	dc.Ellipse(p3.x - 1, p3.y - 1, p3.x + 1, p3.y + 1);
	dc.Ellipse(p4.x - 1, p4.y - 1, p4.x + 1, p4.y + 1);

	//Plot Circle.
	double A1 = 2 * (p2.x - p1.x);
	double B1 = 2 * (p2.y - p1.y);
	double C1 = SQR(p2.x) + SQR(p2.y) - SQR(p1.x) - SQR(p1.y);
	double A2 = 2 * (p3.x - p2.x);
	double B2 = 2 * (p3.y - p2.y);
	double C2 = SQR(p3.x) + SQR(p3.y) - SQR(p2.x) - SQR(p2.y);

	double centerx = ((C1*B2) - (C2*B1)) / ((A1*B2) - (A2*B1));
	double centery = ((A1*C2) - (A2*C1)) / ((A1*B2) - (A2*B1));

	double r = sqrt(SQR(centerx - p1.x) + SQR(centery - p1.y));

	dc.Ellipse(centerx - r, centery - r, centerx + r, centery + r);

	double alpha, belta, gama;
	alpha = 0.45;
	belta = 0.45;
	gama = 0.1;
	p5.x = p1.x * alpha + p2.x * belta + p3.x * gama;
	p5.y = p1.y * alpha + p2.y * belta + p3.y * gama;
	double Area_p1p2p4 = fabs(pTreeView->TriangleArea(p1.x, p1.y, p2.x, p2.y, p4.x, p4.y));
	double Area_p1p2p5 = fabs(pTreeView->TriangleArea(p1.x, p1.y, p2.x, p2.y, p5.x, p5.y));
	double Area_p1p4p5 = fabs(pTreeView->TriangleArea(p1.x, p1.y, p4.x, p4.y, p5.x, p5.y));
	double Area_p4p2p5 = fabs(pTreeView->TriangleArea(p4.x, p4.y, p2.x, p2.y, p5.x, p5.y));


	alpha_ = Area_p1p2p5 / Area_p1p2p4;
	belta_ = Area_p1p4p5 / Area_p1p2p4;
	gama_ = Area_p4p2p5 / Area_p1p2p4;
	p6.x = p4.x * alpha_ + p2.x * belta_ + p1.x * gama_;
	p6.y = p4.y * alpha_ + p2.y * belta_ + p1.y * gama_;

	//dc.Ellipse(p5.x - 2, p5.y - 2, p5.x + 2, p5.y + 2);
	dc.Ellipse(p6.x - 2, p6.y - 2, p6.x + 2, p6.y + 2);
}
void Actions_EdgeSwap::PlotInitialMeshCircularEdgeFlipPointMove(CPaintDC &dc)
{
	int xdelta = 0;
	int ydelta = 200;
	Point2D p1(0 + xdelta, 58 + ydelta);
	Point2D p2(50 + xdelta, 10 + ydelta);
	Point2D p3(100 + xdelta, 60 + ydelta);
	Point2D p4(48 + xdelta, 109 + ydelta + 20);
	Point2D p5, p6;

	dc.SelectStockObject(NULL_BRUSH);
	CPen pen1(PS_SOLID, 1, RGB(0, 0, 0));
	dc.SelectObject(&pen1);

	//Line Segments
	dc.MoveTo(p1.x, p1.y);
	dc.LineTo(p2.x, p2.y);
	dc.LineTo(p3.x, p3.y);
	dc.LineTo(p4.x, p4.y);
	dc.LineTo(p1.x, p1.y);
	dc.MoveTo(p2.x, p2.y);
	dc.LineTo(p4.x, p4.y);

	//Points
	dc.Ellipse(p1.x - 1, p1.y - 1, p1.x + 1, p1.y + 1);
	dc.Ellipse(p2.x - 1, p2.y - 1, p2.x + 1, p2.y + 1);
	dc.Ellipse(p3.x - 1, p3.y - 1, p3.x + 1, p3.y + 1);
	dc.Ellipse(p4.x - 1, p4.y - 1, p4.x + 1, p4.y + 1);

	//Plot Circle.
	double A1 = 2 * (p2.x - p1.x);
	double B1 = 2 * (p2.y - p1.y);
	double C1 = SQR(p2.x) + SQR(p2.y) - SQR(p1.x) - SQR(p1.y);
	double A2 = 2 * (p3.x - p2.x);
	double B2 = 2 * (p3.y - p2.y);
	double C2 = SQR(p3.x) + SQR(p3.y) - SQR(p2.x) - SQR(p2.y);

	double centerx = ((C1*B2) - (C2*B1)) / ((A1*B2) - (A2*B1));
	double centery = ((A1*C2) - (A2*C1)) / ((A1*B2) - (A2*B1));

	double r = sqrt(SQR(centerx - p1.x) + SQR(centery - p1.y));

	dc.Ellipse(centerx - r, centery - r, centerx + r, centery + r);

	p6.x = p4.x * alpha_ + p2.x * belta_ + p1.x * gama_;
	p6.y = p4.y * alpha_ + p2.y * belta_ + p1.y * gama_;

	//dc.Ellipse(p5.x - 2, p5.y - 2, p5.x + 2, p5.y + 2);
	dc.Ellipse(p6.x - 2, p6.y - 2, p6.x + 2, p6.y + 2);

	double t_inter = 0.66037735849056600;
	Point2D interpoint;
	interpoint.x = p2.x * (1 - t_inter) + p4.x * t_inter;
	interpoint.y = p2.y * (1 - t_inter) + p4.y * t_inter;
	dc.MoveTo(p1.x, p1.y);
	dc.LineTo(interpoint.x, interpoint.y);
	dc.LineTo(p3.x, p3.y);

	double newalpha, newbelta, newgama;
	double tarea1 = fabs(pTreeView->TriangleArea(p1.x, p1.y, p2.x, p2.y, interpoint.x, interpoint.y));
	double tarea2 = fabs(pTreeView->TriangleArea(p1.x, p1.y, p2.x, p2.y, p6.x, p6.y));
	double tarea3 = fabs(pTreeView->TriangleArea(p1.x, p1.y, interpoint.x, interpoint.y, p6.x, p6.y));
	double tarea4 = fabs(pTreeView->TriangleArea(interpoint.x, interpoint.y, p2.x, p2.y, p6.x, p6.y));

	double r1 = tarea2 / tarea1;
	double r2 = tarea3 / tarea1;
	double r3 = tarea4 / tarea1;
}