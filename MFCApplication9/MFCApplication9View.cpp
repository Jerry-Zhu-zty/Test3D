// 这段 MFC 示例源代码演示如何使用 MFC Microsoft Office Fluent 用户界面
// (“Fluent UI”)。该示例仅供参考，
// 用以补充《Microsoft 基础类参考》和
// MFC C++ 库软件随附的相关电子文档。
// 复制、使用或分发 Fluent UI 的许可条款是单独提供的。
// 若要了解有关 Fluent UI 许可计划的详细信息，请访问
// https://go.microsoft.com/fwlink/?LinkId=238214.
//
// 版权所有(C) Microsoft Corporation
// 保留所有权利。

// MFCApplication9View.cpp: CMFCApplication9View 类的实现
//

#include "pch.h"
#include "framework.h"
// SHARED_HANDLERS 可以在实现预览、缩略图和搜索筛选器句柄的
// ATL 项目中进行定义，并允许与该项目共享文档代码。
#ifndef SHARED_HANDLERS
#include "MFCApplication9.h"
#endif

#include "MFCApplication9Doc.h"
#include "MFCApplication9View.h"

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CMFCApplication9View

IMPLEMENT_DYNCREATE(CMFCApplication9View, CView)

BEGIN_MESSAGE_MAP(CMFCApplication9View, CView)
	// 标准打印命令
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CMFCApplication9View::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEWHEEL()
	ON_WM_MOUSEMOVE()
	ON_WM_KEYDOWN()
END_MESSAGE_MAP()

// CMFCApplication9View 构造/析构
CLog _xLog;

CMFCApplication9View::CMFCApplication9View() noexcept
{

	int iXOffset = OFFSET3D_X;
	int iYOffset = OFFSET3D_Y/*-2000*/;
	int iZOffset = OFFSET3D_Z/*1500*/;

	m_pMemoryDC = NULL;

	m_dwLastScroll = GetTickCount64();
	// TODO: 在此处添加构造代码
	
	TCHAR szFilter[] = _T("3D Object (*.obj)||");
	// 构造打开文件对话框   
	CFileDialog fileDlg(TRUE, _T("obj"), NULL, 0, szFilter, this);
	CString strFilePath;

	// 显示打开文件对话框   
	if (IDOK == fileDlg.DoModal())
	{
		// 如果点击了文件对话框上的“打开”按钮，则将选择的文件路径显示到编辑框里   
		strFilePath = fileDlg.GetPathName();
		m_xShape3D.Read3DBbj(strFilePath);

	}
	//m_xShape3D.Read3DBbj(L"c:\\temp\\demo1.obj");
	
	//for (int iStep = 0; iStep < 100; iStep++)
	//{
	//	m_xShape3D.AddShapeSegment(- 100, iStep * 10, 0,  100, iStep * 10, 0);
	//}
	
	m_lastMousePt.SetPoint(-1, -1);
	if(m_xShape3D.m_vecPt.size())
		m_xShape3D.SetCameraPt(m_xShape3D.m_vecPt[0].m_x, 1000, m_xShape3D.m_vecPt[0].m_z);
	/*CLog log;
	log.SetFileName(L"C:\\temp\\123\\1.log");
	log.WriteLog("FILE:%s LINE:%d\n", __FILE__, __LINE__);
	log.WriteLog("test2 %d,%f\n", 111, 345.345);*/

}

CMFCApplication9View::~CMFCApplication9View()
{
	if (m_pMemoryDC != NULL)
		delete m_pMemoryDC;
}

BOOL CMFCApplication9View::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: 在此处通过修改
	//  CREATESTRUCT cs 来修改窗口类或样式

	return CView::PreCreateWindow(cs);
}

// CMFCApplication9View 绘图

void CMFCApplication9View::OnDraw(CDC* pDC)
{
	
	CRect rc;
	GetClientRect(&rc);

	if (m_pMemoryDC == NULL)
	{
		m_pMemoryDC = new CDC;
		m_pMemoryDC->CreateCompatibleDC(pDC);
		CBitmap MemBitmap;
		MemBitmap.CreateCompatibleBitmap(pDC, 3000, 3000);
		CBitmap* pOldBit = m_pMemoryDC->SelectObject(&MemBitmap);
		m_pMemoryDC->FillSolidRect(0, 0, 3000, 3000, RGB(255, 255, 255));
		
	}
	else
	{
		m_pMemoryDC->FillSolidRect(0, 0, 3000, 3000, RGB(255, 255, 255));
	}

	//m_xCoordinateAxis.Draw3DObject(m_pMemoryDC);
	CPoint point;
	double dAngle = 0;
	double dRate = 0;
	int nXPos = 1;
	int nYPos = 1;
	GetCursorPos(&point);
	ScreenToClient(&point);
	if (m_lastMousePt.x != -1)
	{
		if (m_lastMousePt.y == point.y)
		{
			dAngle = 90;
		}
		else if (m_lastMousePt.x == point.x)
		{
			dAngle = 0;
		}
		else
		{

			dAngle = atan((m_lastMousePt.x - point.x) / (m_lastMousePt.y - point.y) * PI / 180);
			//dRate = dAngle / (45 - dAngle);
			dRate = fmod(dAngle, 45) + 1 / 45;

		}
		if (m_lastMousePt.x < point.x)
		{
			nXPos = -1;
		}
		if (m_lastMousePt.y > point.y)
		{
			nYPos = -1;
		}

		m_sMessageTmp.Format(L"dAngle:%f dRate:%f  nXPos:%d  nYPos:%d", dAngle, dRate, nXPos, nYPos);
		//Invalidate();

		if (dAngle > 45)
		{
			//m_xShape3D.Transform3DObject(nYPos * 1 * dRate, 0, nXPos * 1);
				///m_xShape3D.TreatAllPt(nYPos * 1 * dRate, 0, nXPos * 1,GetDC());
				//m_xShape3D.CameraPerspectiveOldPos();
			m_xShape3D.TransformFace(nYPos * 5 * dRate, 0, nXPos * 5);
			m_xShape3D.CameraPerspectiveFace();
		}
		else
		{
			//m_xShape3D.Transform3DObject(nYPos * 1, 0, nXPos * 1 * dRate);
				///m_xShape3D.TreatAllPt(nYPos * 1, 0, nXPos * 1 * dRate,GetDC());
				//m_xShape3D.CameraPerspectiveOldPos();
			m_xShape3D.TransformFace(nYPos * 5, 0, nXPos * 5 * dRate);
			m_xShape3D.CameraPerspectiveFace();
		}
		m_xShape3D.DrawObj(m_pMemoryDC);

	}

	m_lastMousePt = point;
	
	//m_pMemoryDC->TextOutW(10,10, m_sMessageTmp);

	pDC->BitBlt(0, 0, rc.Width(), rc.Height(), m_pMemoryDC, 0, 0, SRCCOPY);

	CMFCApplication9Doc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// TODO: 在此处为本机数据添加绘制代码
}


// CMFCApplication9View 打印


void CMFCApplication9View::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CMFCApplication9View::OnPreparePrinting(CPrintInfo* pInfo)
{
	// 默认准备
	return DoPreparePrinting(pInfo);
}

void CMFCApplication9View::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加额外的打印前进行的初始化过程
}

void CMFCApplication9View::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: 添加打印后进行的清理过程
}



void CMFCApplication9View::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CMFCApplication9View 诊断

#ifdef _DEBUG
void CMFCApplication9View::AssertValid() const
{
	CView::AssertValid();
}

void CMFCApplication9View::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CMFCApplication9Doc* CMFCApplication9View::GetDocument() const // 非调试版本是内联的
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CMFCApplication9Doc)));
	return (CMFCApplication9Doc*)m_pDocument;
}
#endif //_DEBUG

// CMFCApplication9View 消息处理程序

////////////////////////////////////////////CPoint3D///////////////////////////////////////////////

void CPoint3D::SetPt(int x, int y, int z)
{
	m_x = x;
	m_y = y;
	m_z = z;
}
CPoint CPoint3D::Change3DTo2D()
{
	CPoint new2DPt;
	new2DPt.x = m_x + OFFSET3D_X + (m_x + OFFSET3D_X) * cos(OVERVIEW_DEGREE_Y * PI / 180.0) - (m_y + OFFSET3D_Y) * sin(OVERVIEW_DEGREE_X * PI / 180.0);
	new2DPt.y = m_z + OFFSET3D_Z + (m_x + OFFSET3D_X) * sin(OVERVIEW_DEGREE_Y * PI / 180.0) + (m_y + OFFSET3D_Y) * cos(OVERVIEW_DEGREE_X * PI / 180.0);
	return new2DPt;
}

CPoint3D CPoint3D::Transform3D(float fRotateX, float fRotateY, float fRotateZ,double MatrixTransform[3][3])
{

	//double Matrix1[3][3], Matrix2[3][3], Matrix3[3][3], 
	double MatrixResult[3][3];//, MatrixTransform[3][3];
	double MatrixPos[1][3];
	CPoint3D new3DPt;
	//SetRotateMatrix(fRotateX, fRotateY, fRotateZ, Matrix1, Matrix2, Matrix3);
	//PreMatrixMultiply(Matrix2, Matrix3, MatrixResult);
	//PreMatrixMultiply(Matrix1, MatrixResult, MatrixTransform);
	MatrixPos[0][0] = m_x;
	MatrixPos[0][1] = m_y;
	MatrixPos[0][2] = m_z;

	MatrixMultiply(MatrixPos, MatrixTransform, MatrixResult);

	new3DPt.m_x = MatrixResult[0][0];
	new3DPt.m_y = MatrixResult[0][1];
	new3DPt.m_z = MatrixResult[0][2];

	return new3DPt;
}
CPoint3D CPoint3D::Scale3D(float fScaleX, float fScaleY, float fScaleZ)
{
	double MatrixScale[3][3], MatrixPos[1][3], MatrixResult[3][3];
	CPoint3D new3DPt;

	SetScaleMatrix(fScaleX, fScaleY, fScaleZ, MatrixScale);
	MatrixPos[0][0] = m_x;
	MatrixPos[0][1] = m_y;
	MatrixPos[0][2] = m_z;

	MatrixMultiply(MatrixPos, MatrixScale, MatrixResult);

	new3DPt.m_x = MatrixResult[0][0];
	new3DPt.m_y = MatrixResult[0][1];
	new3DPt.m_z = MatrixResult[0][2];

	return new3DPt;
}
CPoint3D CPoint3D::CameraPerspective(CPoint3D cameraPt)
{
	double dLen = 0, dRate = 0;
	dLen = cameraPt.m_y - m_y;
	dRate = (100 + dLen) / 6000.0;
#ifdef _DEBUG
	_xLog.WriteLog("rate:%f = (100 + dLen:%f) / 100.0\n", dRate, dLen);
#endif
	return Scale3D(dRate * 10, dRate * 10, dRate * 10);
}


void CPoint3D::SetScaleMatrix(float fScaleX, float fScaleY, float fScaleZ, double(&Matrix)[3][3])
{
	memset(&Matrix, 0, sizeof(Matrix));
	Matrix[0][0] = fScaleX;
	Matrix[1][1] = fScaleY;
	Matrix[2][2] = fScaleZ;
}
void CPoint3D::MatrixMultiply(double Matrix1[1][3], double Matrix2[3][3], double(&MatrixResult)[3][3])
{
	double sum = 0;
	for (int times = 0; times < 3; times++)
	{
		sum = 0;
		for (int row = 0; row < 3; row++)
		{
			sum += Matrix1[0][row] * Matrix2[row][times];
		}
		MatrixResult[0][times] = sum;
	}
}
///////////////////////////////////////////////CFace3D//////////////////////////////////////////////////
int iFaceNum = 0;

void CFace3D::AddFacePt(CPoint3D pt)
{
	m_facePt.push_back(pt);
}
void CFace3D::Transform3DPt(float fRotateX, float fRotateY, float fRotateZ,double Matrix[3][3])
{
	vector<CPoint3D>::iterator it;
	ClearShowPt();
	for (it = m_facePt.begin(); it != m_facePt.end(); it++)
	{
		m_showPt.push_back(it->Transform3D(fRotateX, fRotateY, fRotateZ, Matrix));
	}
}
void CFace3D::CameraPerspective3DPt()
{
	vector<CPoint3D>::iterator it;
	if (m_showPt.empty())
	{
		for (it = m_facePt.begin(); it != m_facePt.end(); it++)
		{
			m_showPt.push_back(it->CameraPerspective(m_cameraPt));
		}
	}
	else
	{
		for (it = m_showPt.begin(); it != m_showPt.end(); it++)
		{
			it->SetPt(it->CameraPerspective(m_cameraPt).m_x, it->CameraPerspective(m_cameraPt).m_y, it->CameraPerspective(m_cameraPt).m_z);
		}
	}
}
void CFace3D::DrawFace(CDC* pDC)
{
	vector<CPoint3D>::iterator it;
	CPoint Pt2D;
	CString sTmp(L"");
	CPen myPen,oldPen;
	//CRgn rgn;
	int i = 0;
	//myPen.CreatePen(PS_SOLID, 2, RGB(255, 0, 0));
	oldPen.CreatePen(PS_SOLID, 2, RGB(0, 0, 0));
	//POINT ptList[1024];
	//myPen.CreatePen(PS_SOLID, 2, RGB(iFaceNum * 10, iFaceNum * 20, iFaceNum * 40));
	if (!m_showPt.empty())
	{
		pDC->MoveTo(m_showPt.begin()->Change3DTo2D());

		//CPoint3D vec;
		//vec = VectorCross();
		//pDC->SelectObject(&myPen);
		//pDC->LineTo(vec.Change3DTo2D());

		//pDC->MoveTo(m_showPt.begin()->Change3DTo2D());
		//pDC->LineTo(m_cameraPt.Change3DTo2D());

		//pDC->MoveTo(m_showPt.begin()->Change3DTo2D());
		//pDC->SelectObject(&oldPen);
		for (it = m_showPt.begin(); it != m_showPt.end(); it++,i++)
		{
			Pt2D = it->Change3DTo2D();
			//ptList[i].x = Pt2D.x; ptList[i].y = Pt2D.y;

			//pDC->SelectObject(&myPen);
			pDC->LineTo(Pt2D);
			//sTmp.Format(L"%d:x:%f,y:%f,z:%f", iFaceNum,it->m_x, it->m_y, it->m_z);
			//sTmp.Format(L"%d:x:%f,y:%f,z:%f", iFaceNum, m_facePt[i].m_x, m_facePt[i].m_y, m_facePt[i].m_z);
			//pDC->SetTextColor(RGB(iFaceNum*10, iFaceNum * 20, iFaceNum * 40));
			//pDC->TextOutW(Pt2D.x, Pt2D.y, sTmp);
		}
		pDC->LineTo(m_showPt.begin()->Change3DTo2D());
		/*rgn.CreatePolyPolygonRgn(ptList, &i, 1, WINDING);
		CBrush bsh;
		bsh.CreateSolidBrush(RGB(iFaceNum * 10, iFaceNum * 20, iFaceNum * 40));
		pDC->FillRgn(&rgn, &bsh);*/
	}
}
void CFace3D::ClearShowPt()
{
	m_showPt.clear();
}
CPoint3D CFace3D::VectorCross()
{
	const double x1 = m_showPt[1].m_x;
	const double x2 = m_showPt[2].m_x;
	const double y1 = m_showPt[1].m_y;
	const double y2 = m_showPt[2].m_y;
	const double z1 = m_showPt[1].m_y;
	const double z2 = m_showPt[2].m_y;
	CPoint3D resultPt(y1*z2-y2*z1,-(x1*z2-x2*z1),x1*y2-x2*y1);
	return resultPt;
}
float CFace3D::IncludedAngle()
{

	return 0.0f;
}
void CFace3D::SetCameraPt(CPoint3D cameraPt)
{
	m_cameraPt = cameraPt;
}
/////////////////////////////////////////////CShape3D/////////////////////////////////////////////////
void CShape3D::SetRotateMatrix(float fRotateX, float fRotateY, float fRotateZ, double(&MatrixTransform)[3][3])
{
	double Matrix1[3][3], Matrix2[3][3], Matrix3[3][3];
	Matrix1[0][0] = cos(fRotateZ * PI / 180);
	Matrix1[0][1] = sin(fRotateZ * PI / 180);
	Matrix1[0][2] = 0;
	Matrix1[1][0] = -sin(fRotateZ * PI / 180);
	Matrix1[1][1] = cos(fRotateZ * PI / 180);
	Matrix1[1][2] = 0;
	Matrix1[2][0] = 0;
	Matrix1[2][1] = 0;
	Matrix1[2][2] = 1;

	Matrix2[0][0] = cos(fRotateY * PI / 180);
	Matrix2[0][1] = 0;
	Matrix2[0][2] = sin(fRotateY * PI / 180);
	Matrix2[1][0] = 0;
	Matrix2[1][1] = 1;
	Matrix2[1][2] = 0;
	Matrix2[2][0] = -sin(fRotateY * PI / 180);
	Matrix2[2][1] = 0;
	Matrix2[2][2] = cos(fRotateY * PI / 180);

	Matrix3[0][0] = 1;
	Matrix3[0][1] = 0;
	Matrix3[0][2] = 0;
	Matrix3[1][0] = 0;
	Matrix3[1][1] = cos(fRotateX * PI / 180);
	Matrix3[1][2] = sin(fRotateX * PI / 180);
	Matrix3[2][0] = 0;
	Matrix3[2][1] = -sin(fRotateX * PI / 180);
	Matrix3[2][2] = cos(fRotateX * PI / 180);

	double MatrixResult[3][3];
	PreMatrixMultiply(Matrix2, Matrix3, MatrixResult);
	PreMatrixMultiply(Matrix1, MatrixResult, MatrixTransform);
}
void CShape3D::PreMatrixMultiply(double Matrix1[3][3], double Matrix2[3][3], double(&MatrixResult)[3][3])
{
	double sum = 0;
	for (int col = 0; col < 3; col++)
	{
		for (int times = 0; times < 3; times++)
		{
			sum = 0;
			for (int row = 0; row < 3; row++)
			{
				sum += Matrix1[col][row] * Matrix2[row][times];
			}
			MatrixResult[col][times] = sum;
		}
	}
}
void CShape3D::TransformFace(float fRotateX, float fRotateY, float fRotateZ)
{
	vector<CFace3D>::iterator it;
	double Matrix[3][3];
	m_fRotateXAccumlate += fRotateX;
	m_fRotateYAccumlate += fRotateY;
	m_fRotateZAccumlate += fRotateZ;
	SetRotateMatrix(m_fRotateXAccumlate, m_fRotateYAccumlate, m_fRotateZAccumlate, Matrix);
	for (it = m_vecFace.begin(); it != m_vecFace.end(); it++)
	{
		it->Transform3DPt(m_fRotateXAccumlate, m_fRotateYAccumlate, m_fRotateZAccumlate, Matrix);
	}
}
void CShape3D::CameraPerspectiveFace()
{
	vector<CFace3D>::iterator it;
	for (it = m_vecFace.begin(); it != m_vecFace.end(); it++)
	{
		it->SetCameraPt(m_CameraPt);
		it->CameraPerspective3DPt();
	}
}

void CShape3D::DrawObj(CDC* pDC)
{
	vector<CFace3D>::iterator it;
	iFaceNum = 0;
	for (it = m_vecFace.begin(); it != m_vecFace.end(); it++)
	{
		iFaceNum++;
		it->DrawFace(pDC);

	}
}

void CShape3D::SetCameraPt(int x, int y, int z)
{
	m_CameraPt.SetPt(x, y, z);
}
CPoint3D CShape3D::GetCameraPt()
{
	return m_CameraPt;
}
BOOL CShape3D::Read3DBbj(CString fileName)
{
	CStdioFile file3D;
	CString sLine, strText;
	CFileException* pFileException = NULL;
	int iPos = 0;
	file3D.Open(fileName, CStdioFile::modeRead | CStdioFile::modeNoTruncate | CStdioFile::shareDenyNone , pFileException);
	//file3D.SeekToBegin();
	sLine = L"";
	
	int iBreak = 0;

	while (file3D.ReadString(sLine))
	{
		iPos = 0;
		if ((iPos = sLine.Find(L" ", iPos)) != -1)
		{
			strText = sLine.Left(iPos);

			if (strText == "v")
			{
				AddVertex(/*m_vecPt, */sLine.Right(sLine.GetLength() - iPos - 1));
			}
			else if (strText == "f")
			{
			
				//AddSegment(/*m_vecPt,*/ sLine.Right(sLine.GetLength() - iPos - 1));
				AddFace(sLine.Right(sLine.GetLength() - iPos - 1));
				
			}
		}

	}
	file3D.Close();
	return true;
}
void CShape3D::AddVertex(/*vector <CPoint3D>& m_vecPt,*/ CString str)
{
	int iPos = -1,iOldPos = 0;
	CPoint3D pt;
	CString sTmp;
	for (int i = 0; i < 2; i++)
	{
		iPos = str.Find(L" ", iOldPos );
		sTmp = str.Mid(iOldPos , iPos - iOldPos );
		switch (i)
		{
		case 0:
			pt.m_x = _tstof(sTmp );
			break;
		case 1:
			pt.m_y = _tstof(sTmp);
			sTmp = str.Right(str.GetLength() - iPos - 1);
			pt.m_z = _tstof(sTmp);
			break;
		default:
			break;
		}
		iOldPos = iPos+1;
	}
	m_vecPt.push_back(pt);
}

void CShape3D::AddFace(CString str)
{
	CString sTmp; 
	int ptItm = 0;
	CFace3D pushFace;
	int iPos = -1, iOldPos = 0;
	int nSlashPos = 0;
	while ((iPos = str.Find(L" ", iOldPos)) != -1)
	{
		sTmp = str.Mid(iOldPos, iPos - iOldPos);

		if (sTmp.Find(L"/") < 0)
			ptItm = _tstoi(sTmp)-1;
		else
			ptItm = _tstoi(sTmp.Left(sTmp.Find(L"/")))-1;

		pushFace.m_facePt.push_back(m_vecPt[ptItm]);
		iOldPos = iPos + 1;
	}
	nSlashPos = str.Find(L"/", iOldPos);
	sTmp = str.Right(str.GetLength() - iOldPos);
	if (nSlashPos < 0)
		ptItm = _tstoi(sTmp)-1;
	else
		ptItm = _tstoi(sTmp.Mid(0, nSlashPos - iOldPos))-1;
	pushFace.AddFacePt(m_vecPt[ptItm]);
	m_vecFace.push_back(pushFace);
}


													/*消息处理*/


void CMFCApplication9View::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	/*ClientToScreen(&point);
	OnContextMenu(this, point);*/
	//Invalidate();
	/*CPoint3D a,b,c;
	CString str;
	a.SetPt(1, 2, 3);
	b.SetPt(3, 2, 1);
	a += b;
	c = a + b;
	str.Format(L"%f,%f,%f",a.m_x,a.m_y,a.m_z);
	GetDC()->TextOutW(10, 50, str);*/
	//m_xShape3D.DrawShape(GetDC());
}
void CMFCApplication9View::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
//	m_xShape3D.Transform3DObject(1, 0, 0);
	//Invalidate();
	//m_xShape3D.Transform3D(60, 30, 30);
	
	CView::OnLButtonUp(nFlags, point);
}


BOOL CMFCApplication9View::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	//m_xShape3D.Transform3DObject(0, (zDelta/120)*1, 0);

	double dwDirAdd = (1000 - (double)(GetTickCount64() - m_dwLastScroll));
	m_dwLastScroll = GetTickCount64();

	if (zDelta > 0)
	{
		//m_xShape3D.Scale3DAll((zDelta / 120) * 1.1, (zDelta / 120) * 1.1, (zDelta / 120) * 1.1);
		m_xShape3D.SetCameraPt(m_xShape3D.m_vecPt[0].m_x, m_xShape3D.GetCameraPt().m_y + dwDirAdd, m_xShape3D.m_vecPt[0].m_z);
#ifdef _DEBUG
		_xLog.WriteLog("Camera(%f,%f,%f)\n", m_xShape3D.GetCameraPt().m_x, m_xShape3D.GetCameraPt().m_y, m_xShape3D.GetCameraPt().m_z);
#endif
		///m_xShape3D.CameraPerspectiveOldPos();
		//m_xShape3D.TreatAllPt(-1, 0, 0, GetDC());
		//m_xShape3D.Transform3DObjOldPos(-1, 0, 0);
		m_xShape3D.CameraPerspectiveFace();

	}
	else
	{
		//m_xShape3D.Scale3DAll((zDelta / 120) * 0.9, (zDelta / 120) * 0.9, (zDelta / 120) * 0.9);
		m_xShape3D.SetCameraPt(m_xShape3D.m_vecPt[0].m_x, m_xShape3D.GetCameraPt().m_y - dwDirAdd, m_xShape3D.m_vecPt[0].m_z);
#ifdef _DEBUG
		_xLog.WriteLog("Camera(%f,%f,%f)\n", m_xShape3D.GetCameraPt().m_x, m_xShape3D.GetCameraPt().m_y, m_xShape3D.GetCameraPt().m_z);
#endif
		///m_xShape3D.CameraPerspectiveOldPos();
		//m_xShape3D.TreatAllPt(1, 0, 0,GetDC());
		m_xShape3D.CameraPerspectiveFace();
	}
	Invalidate(0);
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CMFCApplication9View::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	
	if (GetAsyncKeyState(VK_LBUTTON))
	{
		Invalidate(0);
	}


	CView::OnMouseMove(nFlags, point);
}


void CMFCApplication9View::OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags)
{
	// TODO: 在此添加消息处理程序代码和/或调用默认值
	switch (nChar)
	{
	case VK_RIGHT:
		m_xShape3D.m_CameraPt.m_x += 50;
		Invalidate();
		break;
	case VK_LEFT:
		m_xShape3D.m_CameraPt.m_x -= 50;
		Invalidate();
		break;
	case VK_UP:
		m_xShape3D.m_CameraPt.m_z += 50;
		Invalidate();
		break;
	case VK_DOWN:
		m_xShape3D.m_CameraPt.m_z -= 50;
		Invalidate();
		break;
	}
	CView::OnKeyDown(nChar, nRepCnt, nFlags);
}
