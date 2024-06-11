// Tspline_PreImageDialog.cpp : implementation file
//

#include "stdafx.h"
#include "SDIViewSwitch.h"
#include "Tspline_PreImageDialog.h"
#include "Tspline.h"
#include "math.h"
#define  SHOWVERTEXUV 0
#define  SHOWVERTEXUVINDEX 1
#define  SHOWFACEINDEX 1
#define  SHOWUVBASIS 1
// Tspline_PreImageDialog dialog

IMPLEMENT_DYNAMIC(Tspline_PreImageDialog, CDialog)

Tspline_PreImageDialog::Tspline_PreImageDialog(CWnd* pParent /*=NULL*/)
	: CDialog(Tspline_PreImageDialog::IDD, pParent)
{
	pTspline = NULL;
	xmax = 100;
	ymax = 100;
}

Tspline_PreImageDialog::~Tspline_PreImageDialog()
{
}

void Tspline_PreImageDialog::DoDataExchange(CDataExchange* pDX)
{
	CDialog::DoDataExchange(pDX);
}


BEGIN_MESSAGE_MAP(Tspline_PreImageDialog, CDialog)
	ON_WM_PAINT()
	ON_WM_LBUTTONDOWN()
	ON_WM_SIZE()
END_MESSAGE_MAP()


// Tspline_PreImageDialog message handlers

void Tspline_PreImageDialog::OnPaint()
{
	CPaintDC dc(this); // device context for painting
	if(!pTspline)
		return;

	int startx = 50;
	int starty = 50;
	double scale = 150.0;
	VPROP_VERTEX_UV_PLOTCOORDINATE vprop_vertex_plotuv;
	VPROP_VERTEX_IMG_FLAG vprop_img_flag;
	VPROP_VERTEX_UV_COORDINATE vprop_vertex_uv;
	VPROP_VERTEX_OFFENDING_FLAG vprop_vertex_offending;
	FPROP_FACE_OFFENDING_FLAG fprop_face_offending;
	VPROP_VERTEX_UV_COORDINATE_INDEX vprop_vertex_uv_index;
	VPROP_VERTEX_UKNOTS vprop_vertex_uknots;
	VPROP_VERTEX_VKNOTS vprop_vertex_vknots;
	CFont font;
	CFont *pOldFont=NULL;
	font.CreatePointFont(100,"ËÎÌו");
	pOldFont=(CFont *)dc.SelectObject(&font);

	//Vertex UV PlotCoordinate
	if (!pTspline->pmesh->get_property_handle(vprop_vertex_plotuv, PROP_VERTEX_UV_PLOTCOORDINATE))
		pTspline->pmesh->add_property(vprop_vertex_plotuv, PROP_VERTEX_UV_PLOTCOORDINATE);

	//Vertex UV Coordinate
	if (!pTspline->pmesh->get_property_handle(vprop_vertex_uv, PROP_VERTEX_UV_COORDINATE))
		pTspline->pmesh->add_property(vprop_vertex_uv, PROP_VERTEX_UV_COORDINATE);

	//Vertex UV Coordinate Index
	if (!pTspline->pmesh->get_property_handle(vprop_vertex_uv_index, PROP_VERTEX_UV_COORDINATE_INDEX))
		pTspline->pmesh->add_property(vprop_vertex_uv_index, PROP_VERTEX_UV_COORDINATE_INDEX);

	//Imagination flag
	if (!pTspline->pmesh->get_property_handle(vprop_img_flag, PROP_VERTEX_IMG_FLAG))
		pTspline->pmesh->add_property(vprop_img_flag, PROP_VERTEX_IMG_FLAG);

	//Vertex offending
	if (!pTspline->pmesh->get_property_handle(vprop_vertex_offending, PROP_VERTEX_OFFENDING_FLAG))
		pTspline->pmesh->add_property(vprop_vertex_offending, PROP_VERTEX_OFFENDING_FLAG);

	//face offending
	if (!pTspline->pmesh->get_property_handle(fprop_face_offending, PROP_FACE_OFFENDING_FLAG))
		pTspline->pmesh->add_property(fprop_face_offending, PROP_FACE_OFFENDING_FLAG);

	//Vertex Uknots
	if (!pTspline->pmesh->get_property_handle(vprop_vertex_uknots, PROP_VERTEX_UKNOTS))
		pTspline->pmesh->add_property(vprop_vertex_uknots, PROP_VERTEX_UKNOTS);
	
	//Vertex Vknots
	if (!pTspline->pmesh->get_property_handle(vprop_vertex_vknots, PROP_VERTEX_VKNOTS))
		pTspline->pmesh->add_property(vprop_vertex_vknots, PROP_VERTEX_VKNOTS);

	CPen redpen(PS_SOLID,1,RGB(255,0,0));
	CPen blackpen(PS_SOLID,1,RGB(0,0,0));
	CPen greenpen(PS_SOLID,1,RGB(0,255,0));
	CBrush redbrush( RGB(255,0,0) ); 
	CBrush blackbrush( RGB(0,0,0) ); 
	CBrush greenbrush( RGB(0,255,0) ); 
	// brush.CreateSolidBrush( LIGHT_GREY ); 

	int flag;
	char message[80];
	CString s;
	for(MyMesh::VertexIter v_it = pTspline->pmesh->vertices_begin(); v_it!=pTspline->pmesh->vertices_end(); ++v_it)
	{
		Vector2D plotuv = pTspline->pmesh->property(vprop_vertex_plotuv,v_it.handle());
		std::vector<int> uv_index = pTspline->pmesh->property(vprop_vertex_uv_index,v_it.handle());
		Vector2D uv = pTspline->pmesh->property(vprop_vertex_uv,v_it.handle());
		flag = pTspline->pmesh->property(vprop_img_flag,v_it.handle());

		double x = plotuv[0];
		double y = plotuv[1];
		if(flag)
		{
			dc.SelectObject(&greenpen);
			dc.SelectObject(&greenbrush);
		}
		else
		{
			int offendingflag = pTspline->pmesh->property(vprop_vertex_offending,v_it.handle());
			if(offendingflag)
			{
				dc.SelectObject(&redpen);
				dc.SelectObject(&redbrush);
			}
			else
			{
				dc.SelectObject(&blackpen);
				dc.SelectObject(&blackbrush);
			}
		}

		if(!flag && SHOWVERTEXUV)
		{
			sprintf(message,"(%1.2f,%1.2f)",uv.x,uv.y);
			s = message;
			dc.TextOut(startx + (x) * scale - 4, starty + y * scale - 4,s);
			if(startx + (x) * scale > xmax)
				xmax = startx + (x) * scale;
			if(starty + y * scale > ymax)
				ymax = starty + y * scale;
		}
		if(!flag && SHOWVERTEXUVINDEX)
		{
			sprintf(message,"(%d,%d)",uv_index[0],uv_index[1]);
			s = message;
			dc.TextOut(startx + (x) * scale - 4, starty + y * scale - 4,s);
			if(startx + (x) * scale > xmax)
				xmax = startx + (x) * scale;
			if(starty + y * scale > ymax)
				ymax = starty + y * scale;
		}

		if(!flag && SHOWUVBASIS)
		{
			
			sprintf(message,"Vertex ID: %d \n", v_it.handle().idx());
			TRACE(message);
			sprintf(message,"Vertex UV: %3.2f, %3.2f \n", uv[0],uv[1]);
			TRACE(message);
			sprintf(message,"Vertex UV Index: %d, %d \n", uv_index[0],uv_index[1]);
			TRACE(message);
			std::vector<double> uknots = pTspline->pmesh->property(vprop_vertex_uknots,v_it.handle());
			std::vector<double> vknots = pTspline->pmesh->property(vprop_vertex_vknots,v_it.handle());
			sprintf(message,"Uknots: (%3.2f,%3.2f,%3.2f,%3.2f,%3.2f)\n",uknots[0],uknots[1],uknots[2],uknots[3],uknots[4]);
			TRACE(message);
			sprintf(message,"Vknots: (%3.2f,%3.2f,%3.2f,%3.2f,%3.2f)\n",vknots[0],vknots[1],vknots[2],vknots[3],vknots[4]);
			TRACE(message);
			//dc.TextOut(startx + (x) * scale - 4, starty + y * scale - 4,s);
			//if(startx + (x) * scale > xmax)
			//	xmax = startx + (x) * scale;
			//if(starty + y * scale > ymax)
			//	ymax = starty + y * scale;
		}
		dc.Ellipse(startx + (x) * scale - 4, starty + y * scale - 4, startx + x * scale + 4, starty + y * scale + 4);
		
	}
	dc.SelectObject(&blackpen);
	for(MyMesh::EdgeIter e_it = pTspline->pmesh->edges_begin(); e_it!=pTspline->pmesh->edges_end(); ++e_it)
	{
		MyMesh::HalfedgeHandle hh = pTspline->pmesh->halfedge_handle(e_it.handle(), 0);
		MyMesh::VertexHandle th = pTspline->pmesh->to_vertex_handle(hh);
		MyMesh::VertexHandle fh = pTspline->pmesh->from_vertex_handle(hh);

		Vector2D uv1 = pTspline->pmesh->property(vprop_vertex_plotuv,th);
		Vector2D uv2 = pTspline->pmesh->property(vprop_vertex_plotuv,fh);

		double x1 = uv1[0];
		double y1 = uv1[1];

		double x2 = uv2[0];
		double y2 = uv2[1];

		dc.MoveTo(startx + (x1) * scale, starty + y1 * scale);
		dc.LineTo(startx + (x2) * scale, starty + y2 * scale);
	}

	for(MyMesh::FaceIter f_it = pTspline->pmesh->faces_begin(); f_it!=pTspline->pmesh->faces_end(); ++f_it)
	{
		int foffendingflag = pTspline->pmesh->property(fprop_face_offending,f_it.handle());
		double minx = 1e10,miny = 1e10,maxx = -1e10,maxy = -1e10;
		for(TsplineMesh::FaceVertexIter fv_it = pTspline->pmesh->fv_iter(f_it); fv_it; ++fv_it )
		{
			Vector2D plotuv = pTspline->pmesh->property(vprop_vertex_plotuv,fv_it.handle());
			if(minx > plotuv.x)
				minx = plotuv.x;
			if(miny > plotuv.y)
				miny = plotuv.y;
			if(maxx < plotuv.x)
				maxx = plotuv.x;
			if(maxy < plotuv.y)
				maxy = plotuv.y;
		}
		if(foffendingflag)
		{
			dc.SelectObject(&redpen);
			dc.SelectObject(&redbrush);
			dc.Rectangle(startx + (minx) * scale, starty + miny * scale, startx + maxx * scale, starty + maxy * scale);
		}
		if(SHOWFACEINDEX)
		{
			sprintf(message,"%d",f_it.handle().idx());
			s = message;
			dc.TextOut((startx + (minx) * scale + startx + maxx * scale) / 2, (starty + miny * scale + starty + maxy * scale) / 2,s);
		}
	}
	/*CPaintDC dc(this); // device context for painting
	// TODO: Add your message handler code here
	// Do not call CDialog::OnPaint() for painting messages
	if(!pTspline)
		return;

	int startx = 100;
	int starty = 100;
	double scale = 100.0;

	for(MyMesh::VertexIter v_it=pTspline->preimage->vertices_begin(); v_it!=pTspline->preimage->vertices_end(); ++v_it)
	{
		OpenMesh::Vec3d pt;
		pt = pTspline->preimage->point (v_it);

		double x = pt[0];
		double y = pt[1];

		dc.Ellipse(startx + (x) * scale - 4, starty + y * scale - 4, startx + x * scale + 4, starty + y * scale + 4);
	}

	for(MyMesh::EdgeIter e_it = pTspline->preimage->edges_begin(); e_it!=pTspline->preimage->edges_end(); ++e_it)
	{
		MyMesh::HalfedgeHandle hh = pTspline->preimage->halfedge_handle(e_it.handle(), 0);
		MyMesh::VertexHandle th = pTspline->preimage->to_vertex_handle(hh);
		MyMesh::VertexHandle fh = pTspline->preimage->from_vertex_handle(hh);

		MyMesh::Point p = pTspline->preimage->point(th);
		MyMesh::Point q = pTspline->preimage->point(fh);
		double x1 = p[0];
		double y1 = p[1];

		double x2 = q[0];
		double y2 = q[1];

		dc.MoveTo(startx + (x1) * scale, starty + y1 * scale);
		dc.LineTo(startx + (x2) * scale, starty + y2 * scale);
	}*/
}

void Tspline_PreImageDialog::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO: Add your message handler code here and/or call default
	//
	/*int startx = 100;
	int starty = 100;
	double scale = 100.0;
	int edgeindex = -1;
	for(int i = 0; i < pTspline->preimage->m_knot2DEdgeArray.size(); i++)
	{
		TsplinePreImageEdge* pEdge = pTspline->preimage->m_knot2DEdgeArray[i];
		TsplinePreImageVertex* pVertex1 = pEdge->m_AdjacentVertexArray[0];
		TsplinePreImageVertex* pVertex2 = pEdge->m_AdjacentVertexArray[1];

		double x1 = pVertex1->plotcoordinate.x;
		double y1 = pVertex1->plotcoordinate.y;

		double x2 = pVertex2->plotcoordinate.x;
		double y2 = pVertex2->plotcoordinate.y;

		int px1 = startx + (x1) * scale;
		int py1 = starty + y1 * scale;
		int px2 = startx + (x2) * scale;
		int py2 = starty + y2 * scale;
		
		int x = point.x;
		int y = point.y;

		if(py1 == py2)
		{
			//horizontal edge
			if((fabs(y - py1) <= 1) && ((x >= px1) && (x <= px2) || (x <= px1) && (x >= px2) ) )
			{
				edgeindex = i;
				y = py1;
				break;
			}
		}else
		if(px1 == px2)
		{
			//vertical edge
			if((fabs(x - px1) <= 1) && ( (y >= py1) && (y <= py2) || (y <= py1) && (y >= py2)))
			{
				edgeindex = i;
				x = px1;
				break;
			}
		}
	}*/

	//if(edgeindex >= 0)
	//{
		//insert a vertex.
		/*TsplinePreImageEdge* pEdge = pTspline->preimage->m_knot2DEdgeArray[edgeindex];
		TsplinePreImageVertex* pVertex1 = pEdge->m_AdjacentVertexArray[0];
		TsplinePreImageVertex* pVertex2 = pEdge->m_AdjacentVertexArray[1];

		//
		Point2D point(x,y);
		double ratio = Distance(point,pVertex1->plotcoordinate) / Distance(pVertex2->plotcoordinate,pVertex1->plotcoordinate);

		TsplinePreImageVertex* pNewVertex = new TsplinePreImageVertex();
		pNewVertex->plotcoordinate = (1 - ratio) * pVertex1->plotcoordinate + ratio * pVertex2->plotcoordinate;
		pNewVertex->coordinate = (1 - ratio) * pVertex1->coordinate + ratio * pVertex2->coordinate;
		
		TsplineVertex3D* pNewVertex3D = new TsplineVertex3D();*/

	//}

	CDialog::OnLButtonDown(nFlags, point);
}

void Tspline_PreImageDialog::OnSize(UINT nType, int cx, int cy)
{
	CDialog::OnSize(nType, cx, cy);
	if(!pTspline)
		return;

	cx = xmax;
	cy = ymax;
	// TODO: Add your message handler code here
}
