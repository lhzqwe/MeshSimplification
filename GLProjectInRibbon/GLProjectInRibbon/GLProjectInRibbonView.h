// This MFC Samples source code demonstrates using MFC Microsoft Office Fluent User Interface 
// (the "Fluent UI") and is provided only as referential material to supplement the 
// Microsoft Foundation Classes Reference and related electronic documentation 
// included with the MFC C++ library software.  
// License terms to copy, use or distribute the Fluent UI are available separately.  
// To learn more about our Fluent UI licensing program, please visit 
// http://go.microsoft.com/fwlink/?LinkId=238214.
//
// Copyright (C) Microsoft Corporation
// All rights reserved.

// GLProjectInRibbonView.h : interface of the CGLProjectInRibbonView class
//

#pragma once

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "Simplify.h"
#include "GLText.h"
#include "ImportOBJ.h"
#include "Color.h"
//CGAL include
#include <CGAL/Simple_cartesian.h>
#include <CGAL/Polyhedron_items_3.h>
#include <CGAL/HalfedgeDS_list.h>
#include <CGAL/Polyhedron_3.h>
#include <CGAL/Exact_predicates_inexact_constructions_kernel.h>
#include <CGAL/boost/graph/graph_traits_Polyhedron_3.h>
#include <CGAL/IO/Polyhedron_iostream.h>
#include <CGAL/mesh_segmentation.h>
#include <CGAL/property_map.h>

//System include
#include <windows.h>

// GLM Mathemtics
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// Other Libs
#include <SOIL.h>

//Define Marcos
#define DOUBLE_MAX  10000000000000.0f
#define DOUBLE_MIN -10000000000000.0f

//Drawing Mode
#define DRAW_NORMAL 1
#define DRAW_MESHSEGMENTATION 2

//CGAL Data Structure
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel,
	CGAL::Polyhedron_items_3,
	CGAL::HalfedgeDS_list> CgalPolyhedron;
typedef CgalPolyhedron::Halfedge_around_facet_circulator Halfedge_around_facet_circulator;
typedef CgalPolyhedron::Vertex_handle Vertex_handle;
typedef CgalPolyhedron::Point Point;
typedef CgalPolyhedron::Halfedge_handle Halfedge_handle;


class CGLProjectInRibbonView : public CView
{
protected: // create from serialization only
	CGLProjectInRibbonView();
	DECLARE_DYNCREATE(CGLProjectInRibbonView)
// Attributes
public:
	CGLProjectInRibbonDoc* GetDocument() const;
public:
	//Data Structure
	CString m_FilePath;
	Shader* m_Shader;
	Shader* m_TextShader;
	Model* m_Model;
	Camera* m_Camera;
	GLText m_text;
	//Model* m_Model_LOD;
	vector<Mesh> m_MeshList;

	//CGAL Model
	CgalPolyhedron m_Polyhedron;

	//GPU Data Structure
	float modelScale;
	glm::mat4 m_ModelMatrix;
	glm::mat4 m_ViewMatrix;
	glm::mat4 m_ProjectionMatrix;
	glm::mat4 m_RotationMatrix;

	bool isDragged = false;
	GLfloat lastX;
	GLfloat lastY;

	//Time FPS
	LARGE_INTEGER frequency;
	LARGE_INTEGER count1;
	LARGE_INTEGER count2;

	float distance;
	double fps;

	glm::vec3 m_FocusPoint;
	double xMin;
	double xMax;
	double yMin;
	double yMax;
	double zMin;
	double zMax;
	float bodyDiagnalLength;

	int m_ScreenWidth;
	int m_ScreenHeight;

	bool m_ifShowLine = false;
	bool m_ifShowLod = false;

	bool isCameraAdjusted;

	int m_DrawingMode;

	Simplify m_Simplify;

	vector<Model> m_ModelList;
// Operations
public:
	void GenerateColorList(vector<Color>& pColorList);
	int GenerateUniqueColorList(int count, vector<Color>& pColorList, vector<Color>& pExcludedColor);
	void SetupMeshList();

public:
	void CalculateFocusPoint(const vector<Mesh>& pMeshList);
	void CalculateFocusPoint(Model * pModel);
	void AdjustCameraView(const vector<Mesh>& pMeshList);
	void AdjustCameraView(Model * pModel);
	void DragBall(GLfloat ax, GLfloat ay, GLfloat bx, GLfloat by, GLfloat r);
	void SimplifyInitialization(Model& model, Simplify& simplify);
	void ChooseLodModel(float distance);
	void GetFPS(double deltaTime);
	void GLSetRenderState();

public:
	void SetDrawingMode(int drawingMode);

// Overrides
public:
	virtual void OnDraw(CDC* pDC);  // overridden to draw this view

	void DrawTextInfo();
	void DrawModel();
	void DrawModelInNormalMode();
	void DrawModelSegmentation();
	void TimeEnd();
	void TimeStart();
	void InitMaterial(Color& pColor);
	void InitDirectionlLighting();


	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
protected:
	virtual BOOL OnPreparePrinting(CPrintInfo* pInfo);
	virtual void OnBeginPrinting(CDC* pDC, CPrintInfo* pInfo);
	virtual void OnEndPrinting(CDC* pDC, CPrintInfo* pInfo);

// Implementation
public:
	virtual ~CGLProjectInRibbonView();
#ifdef _DEBUG
	virtual void AssertValid() const;
	virtual void Dump(CDumpContext& dc) const;
#endif

protected:

// Generated message map functions
protected:
	afx_msg void OnFilePrintPreview();
	afx_msg void OnRButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnContextMenu(CWnd* pWnd, CPoint point);
	DECLARE_MESSAGE_MAP()
public:
	afx_msg void OnOpenbutton();
	afx_msg void OnSavebutton();
protected:
	// a handle to an OpenGL rendering context
	HGLRC m_hRC;
public:
	afx_msg int OnCreate(LPCREATESTRUCT lpCreateStruct);
	afx_msg void OnDestroy();
	afx_msg void OnSize(UINT nType, int cx, int cy);
	afx_msg BOOL OnEraseBkgnd(CDC* pDC);
	afx_msg void OnLButtonDown(UINT nFlags, CPoint point);
	afx_msg void OnLButtonUp(UINT nFlags, CPoint point);
	afx_msg void OnMouseMove(UINT nFlags, CPoint point);
	afx_msg BOOL OnMouseWheel(UINT nFlags, short zDelta, CPoint pt);
	afx_msg void OnShowline();
	afx_msg void OnUpdateShowline(CCmdUI *pCmdUI);
	afx_msg void OnQemoperation();
	afx_msg void OnLoddisplay();
	afx_msg void OnMeshSegmentation();
};

#ifndef _DEBUG  // debug version in GLProjectInRibbonView.cpp
inline CGLProjectInRibbonDoc* CGLProjectInRibbonView::GetDocument() const
   { return reinterpret_cast<CGLProjectInRibbonDoc*>(m_pDocument); }
#endif

