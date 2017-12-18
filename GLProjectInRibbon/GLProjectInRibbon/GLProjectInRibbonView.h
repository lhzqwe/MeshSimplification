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
#include "Log.h"

//Standard include
#include <map>
#include <algorithm>

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
#include <CGAL/Polygon_mesh_processing/triangulate_hole.h>

//Simplification function
#include <CGAL/Surface_mesh_simplification/edge_collapse.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Midpoint_placement.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Constrained_placement.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/Count_stop_predicate.h>
#include <CGAL/Surface_mesh_simplification/Policies/Edge_collapse/LindstromTurk_cost.h>

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
#define DRAW_SIMPLIFIEDMESH 3
#define DRAW_FLAT 4
#define DRAW_SMOOTH 5

//CGAL Data Structure
typedef CGAL::Exact_predicates_inexact_constructions_kernel Kernel;
typedef CGAL::Polyhedron_3<Kernel,
	CGAL::Polyhedron_items_3,
	CGAL::HalfedgeDS_list> CgalPolyhedron;
typedef CgalPolyhedron::Halfedge_around_facet_circulator Halfedge_around_facet_circulator;
typedef CgalPolyhedron::Vertex_handle Vertex_handle;
typedef CgalPolyhedron::Facet_handle       Facet_handle;
typedef CgalPolyhedron::Point Point;
typedef CgalPolyhedron::Halfedge_handle Halfedge_handle;

typedef std::map<CgalPolyhedron::Facet_const_handle, double> Facet_double_map;
typedef std::map<CgalPolyhedron::Facet_const_handle, std::size_t> Facet_int_map;

//Simplification needed
typedef CGAL::Simple_cartesian<double> SimpKernel;
typedef SimpKernel::Point_3 Point_3;
typedef CGAL::Polyhedron_3<SimpKernel> Surface_mesh;

namespace SMS = CGAL::Surface_mesh_simplification;

//
// BGL property map which indicates whether an edge is marked as non-removable
//
struct Border_is_constrained_edge_map{
	const Surface_mesh* sm_ptr;
	typedef boost::graph_traits<Surface_mesh>::edge_descriptor key_type;
	typedef bool value_type;
	typedef value_type reference;
	typedef boost::readable_property_map_tag category;
	Border_is_constrained_edge_map(const Surface_mesh& sm)
		: sm_ptr(&sm)
	{}
	friend bool get(Border_is_constrained_edge_map m, const key_type& edge) {
		return CGAL::is_border(edge, *m.sm_ptr);
	}
};
//
// Placement class
//
typedef SMS::Constrained_placement<SMS::Midpoint_placement<Surface_mesh>,
	Border_is_constrained_edge_map > Placement;

struct TreeNode {
	TreeNode() {
		indexMesh = -1;
		avgSDF = 0.0f;
		singleSDF = 0.0f;
		faceNum = 0;
		singleFaceNum = 0;
		isDeleted = false;
	};
	TreeNode(int pIndexMesh) : indexMesh(pIndexMesh) {
		avgSDF = 0.0f;
		faceNum = 0;
		singleFaceNum = 0;
		isDeleted = false;
	};

	int indexMesh;
	double avgSDF;
	double singleSDF;
	int faceNum;
	int singleFaceNum;

	int isDeleted;

	vector<TreeNode *> childNodes;
};

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
	string m_ModelName;
	Camera* m_Camera;
	GLText m_text;
	//Model* m_Model_LOD;


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


	Simplify m_Simplify;

	vector<Model> m_ModelList;

	//Mesh Segmentation
	Facet_double_map internal_sdf_map; //facet_handle -> sdf value
	Facet_int_map internal_segment_map;//facet_handle -> mesh index
	std::size_t number_of_segments;

	vector<vector<int>> m_MeshGraph;
	int m_IndexMaxSDF = 0;
	TreeNode* m_MeshTree;

	//Mesh For Display
	Mesh* m_pFlatMesh;
	Mesh* m_pSmoothMesh;

	int m_DrawingMode;

	//Mesh Cache
	Mesh m_CombinedMesh;

	Mesh m_NormalMesh;
	vector<Mesh> m_MeshList; //For Segmentation
	Mesh m_SimplifiedMesh;

	//Log System
	Log m_Log;
	string m_InFileName;
// Operations
public:
	Mesh GenerateFlatMesh(Mesh & pMesh);
	void GenerateColorList(vector<Color>& pColorList);
	int GenerateUniqueColorList(int count, vector<Color>& pColorList, vector<Color>& pExcludedColor);
	void SetupMeshList();
	void SetupMesh(Mesh & pMesh);
	void SetupSimplifiedMesh();
	void ConstructTree(int indexMeshWithMaxSDF);
	void ConstructTreeBFS(int indexMeshWithSDF);
	void HelperConstructTree(TreeNode* pTreeNode, vector<bool>& usedMesh);
	void DeleteTree();
	void HelperDeleteTree(TreeNode* pTreeNode);
	void SimplifyWithDelete();
	void HelperSimplifyWithDelete(TreeNode * pTreeNode, int k);
	void CombineMesh();
	void CombineTheMesh(int meshIndex);
	void RepairMeshHole();
	void ConvertFromMeshToCgalPolyhedron(const Mesh & pMesh, CgalPolyhedron & pPolyhedron);
	void ConvertFromMeshToSurfaceMeshPolyhedron(const Mesh & pMesh, Surface_mesh & pSurfaceMesh);
	void ClearMeshSegmentationDeleteState();
	void SimplifyFinalMethod(int targetFaceNum, double sdfThreshold);
	void DeleteTreeNodeWithLessSDF(double sdfThreshold);
	void HelperDeleteTreeNodeWithLessSDF(TreeNode* pTreeNode, double sdfThreshold);
	void UpdateTreeNode();
	void HelperUpdateTreeNode(TreeNode* pTreeNode);
	void UpdateTreeNodeFaceNum(TreeNode* pTreeNode);
	int HelperUpdateTreeNodeFaceNum(TreeNode* pTreeNode);
	void UpdateTreeNodeSDF(TreeNode* pTreeNode);
	double HelperUpdateTreeNodeSDF(TreeNode* pTreeNode);
	void DeleteTreeNode(TreeNode* pTreeNode);
	bool isLeaf(TreeNode* pTreeNode);
	bool isJoint(TreeNode* pTreeNode);
	bool SimplifyInFinalMethod(TreeNode* pTreeNode, int& restFaceNum, int& targetNum);
	int TreeNodesSum(TreeNode* tn);
	void BuildFinalMeshFromMeshTree(TreeNode* root);
	void EstimateSimplifyTarget(int &targetFaceNum, float &targetSDF);

public:
	void CalculateFocusPoint(const vector<Mesh>& pMeshList);
	void CalculateFocusPoint(Model * pModel);
	void CalculateFocusPoint(const Mesh& pMesh);
	void AdjustCameraView(const vector<Mesh>& pMeshList);
	void AdjustCameraView(const Mesh& pMesh);
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
	void DrawMesh(int drawMode);
	void DrawMesh(Mesh& pMesh);
	void DrawModelInNormalMode();
	void DrawModelSegmentation();
	void DrawSimplifiedModel();
	void DrawFlatMesh();
	void DrawSmoothMesh();
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
	afx_msg void OnNormalDisplay();
	afx_msg void OnSegmentationDisplay();
	afx_msg void OnSimplifiedDisplay();
	afx_msg void OnDeleteSimp();
	afx_msg void OnSerializeWrite();
	afx_msg void OnSerializeRead();
	afx_msg void OnQuadricSimp();
	afx_msg void OnFinalmethod();

	void RepairHole();

};

#ifndef _DEBUG  // debug version in GLProjectInRibbonView.cpp
inline CGLProjectInRibbonDoc* CGLProjectInRibbonView::GetDocument() const
   { return reinterpret_cast<CGLProjectInRibbonDoc*>(m_pDocument); }
#endif

