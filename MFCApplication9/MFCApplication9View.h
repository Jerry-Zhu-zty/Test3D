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

// MFCApplication9View.h: CMFCApplication9View 类的接口
//

#pragma once
#include <vector>
#include "Common/AppLog.h"
#include <compare>
#define PI 3.1415926
#define OVERVIEW_DEGREE_X 90
#define OVERVIEW_DEGREE_Y 90
#define OFFSET3D_X 800
#define OFFSET3D_Y 500
#define OFFSET3D_Z -500
using namespace std;

class CPoint3D
{
public:
	double m_x = 0;
	double m_y = 0;
	double m_z = 0;
	float firstDist = 0;
	void SetPt(int x, int y, int z);
	CPoint Change3DTo2D();
	CPoint3D Transform3D(float fRotateX, float fRotateY, float fRotateZ, double MatrixTransform[3][3]);
	CPoint3D CameraPerspective(CPoint3D cameraPt);
private:
	void SetScaleMatrix(float fScaleX, float fScaleY, float fScaleZ, double(&Matrix)[3][3]);
	void MatrixMultiply(double Matrix1[1][3], double Matrix2[3][3], double(&MatrixResult)[3][3]);
	CPoint3D Scale3D(float fScaleX, float fScaleY, float fScaleZ);
public:
	CPoint3D(double x = 0, double y = 0, double z = 0)
	{
		m_x = x;
		m_y = y;
		m_z = z;
	}
#if !_HAS_CXX20
	void operator+=(_In_ CPoint3D a)
	{
		m_x += a.m_x;
		m_y += a.m_y;
		m_z += a.m_z;
	};
	void operator-=(_In_ CPoint3D a)
	{
		m_x -= a.m_x;
		m_y -= a.m_y;
		m_z -= a.m_z;
	};
	CPoint3D operator+(_In_ CPoint3D a)
	{
		return CPoint3D(m_x + a.m_x, m_y + a.m_y, m_z + a.m_z);
	};
	CPoint3D operator=(_In_ CPoint a)
	{
		return CPoint3D(m_x = a.x, m_y = a.y);
	};
	CPoint3D operator-(_In_ CPoint3D a)
	{
		return CPoint3D(m_x - a.m_x, m_y - a.m_y, m_z - a.m_z);
	}; 
#else
	auto operator<=>(const CPoint3D&)  const = default;
#endif
};
class CFace3D
{
public:
	vector<CPoint3D> m_facePt;
	vector<CPoint3D> m_showPt;
	void SetCameraPt(CPoint3D cameraPt);
	void AddFacePt(CPoint3D);
	void Transform3DPt(float fRotateX, float fRotateY, float fRotateZ, double Matrix[3][3]);
	void CameraPerspective3DPt();
	void DrawFace(CDC* pDC);
private:
	CPoint3D m_cameraPt;
	void ClearShowPt();
	float IncludedAngle();
	CPoint3D VectorCross();
};

class CShape3D
{
private:
	float m_fRotateXAccumlate = 0;
	float m_fRotateYAccumlate = 0;
	float m_fRotateZAccumlate = 0;
	
public:
	CShape3D()
	{
		m_CameraPt.SetPt(0, 0, 0);
	}
	CPoint3D m_CameraPt;
	vector <CPoint3D> m_vecPt;
	vector <CFace3D> m_vecFace;
	
	void SetCameraPt(int x, int y, int z);
	CPoint3D GetCameraPt();

	//file
	BOOL Read3DBbj(CString fileName);
private:
	//file treatment
	void AddVertex(CString str);
	void AddFace(CString str);
	//math
	void SetRotateMatrix(float fRotateX, float fRotateY, float fRotateZ, double(&MatrixTransform)[3][3]);
	void PreMatrixMultiply(double Matrix1[3][3], double Matrix2[3][3], double(&MatrixResult)[3][3]);
public:
	/*face treatment*/
	void TransformFace(float fRotateX, float fRotateY, float fRotateZ);
	void CameraPerspectiveFace();
	void DrawObj(CDC* pDC);
};

class CMFCApplication9View : public CView
{
protected: // 仅从序列化创建
	CMFCApplication9View() noexcept;
	DECLARE_DYNCREATE(CMFCApplication9View)

// 特性
public:
	CMFCApplication9Doc* GetDocument() const;

// 操作
public:
	CShape3D m_xShape3D;
	CShape3D m_xCoordinateAxis;
	CPoint m_lastMousePt;
	CString m_sMessageTmp;


	CDC* m_pMemoryDC;
	DWORD64 m_dwLastScroll;
// 重写
public:
	virtual void OnDraw(CDC* pDC);  // 重写以绘制该视图
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// 实现
public:
	virtual ~CMFCApplication9View();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// 生成的消息映射函数
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg void OnKeyDown(UINT nChar, UINT nRepCnt, UINT nFlags);
};

#ifndef _DEBUG  // MFCApplication9View.cpp 中的调试版本
inline CMFCApplication9Doc* CMFCApplication9View::GetDocument() const
   { return reinterpret_cast<CMFCApplication9Doc*>(m_pDocument); }
#endif

