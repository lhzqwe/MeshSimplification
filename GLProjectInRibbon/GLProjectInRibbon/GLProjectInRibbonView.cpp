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

// GLProjectInRibbonView.cpp : implementation of the CGLProjectInRibbonView class
//

#include "stdafx.h"
// SHARED_HANDLERS can be defined in an ATL project implementing preview, thumbnail
// and search filter handlers and allows sharing of document code with that project.
#ifndef SHARED_HANDLERS
#include "GLProjectInRibbon.h"
#endif

#include "GLProjectInRibbonDoc.h"
#include "GLProjectInRibbonView.h"

// ////////////////////////////////////////////////////////////////
// New codes begin:

// #include “GL/GL.H” // 这不是必要的，
#include "GL/GLU.H" // 已经包含GL.h
#include "Resource.h"

#include <map>

// New codes end.
// ////////////////////////////////////////////////////////////////

#ifdef _DEBUG
#define new DEBUG_NEW
#endif


// CGLProjectInRibbonView

IMPLEMENT_DYNCREATE(CGLProjectInRibbonView, CView)

BEGIN_MESSAGE_MAP(CGLProjectInRibbonView, CView)
	// Standard printing commands
	ON_COMMAND(ID_FILE_PRINT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_DIRECT, &CView::OnFilePrint)
	ON_COMMAND(ID_FILE_PRINT_PREVIEW, &CGLProjectInRibbonView::OnFilePrintPreview)
	ON_WM_CONTEXTMENU()
	ON_WM_RBUTTONUP()
	ON_COMMAND(ID_OPENBUTTON, &CGLProjectInRibbonView::OnOpenbutton)
	ON_COMMAND(ID_SAVEBUTTON, &CGLProjectInRibbonView::OnSavebutton)
	ON_WM_CREATE()
	ON_WM_DESTROY()
	ON_WM_SIZE()
	ON_WM_ERASEBKGND()
	ON_WM_LBUTTONDOWN()
	ON_WM_LBUTTONUP()
	ON_WM_MOUSEMOVE()
	ON_WM_MOUSEWHEEL()
	ON_COMMAND(ID_SHOWLINE, &CGLProjectInRibbonView::OnShowline)
	ON_UPDATE_COMMAND_UI(ID_SHOWLINE, &CGLProjectInRibbonView::OnUpdateShowline)
	ON_COMMAND(ID_QEMOPERATION, &CGLProjectInRibbonView::OnQemoperation)
	ON_COMMAND(ID_LODDISPLAY, &CGLProjectInRibbonView::OnLoddisplay)
	ON_COMMAND(ID_BUTTON2, &CGLProjectInRibbonView::OnMeshSegmentation)
END_MESSAGE_MAP()

// CGLProjectInRibbonView construction/destruction

CGLProjectInRibbonView::CGLProjectInRibbonView()
{
	// TODO: add construction code here

}

CGLProjectInRibbonView::~CGLProjectInRibbonView()
{
}

BOOL CGLProjectInRibbonView::PreCreateWindow(CREATESTRUCT& cs)
{
	// TODO: Modify the Window class or styles here by modifying
	//  the CREATESTRUCT cs

	return CView::PreCreateWindow(cs);
}

// CGLProjectInRibbonView drawing

void CGLProjectInRibbonView::OnDraw(CDC* pDC)
{
	CGLProjectInRibbonDoc* pDoc = GetDocument();
	ASSERT_VALID(pDoc);
	if (!pDoc)
		return;

	// Draw Model ......................................................
	wglMakeCurrent(pDC->m_hDC, m_hRC);
	GLSetRenderState();

	TimeStart();

	InitDirectionlLighting();

	DrawModel();
	//DrawModelInNormalMode();

	TimeEnd();

	DrawTextInfo();

	SwapBuffers(pDC->m_hDC);
}


// CGLProjectInRibbonView printing


void CGLProjectInRibbonView::OnFilePrintPreview()
{
#ifndef SHARED_HANDLERS
	AFXPrintPreview(this);
#endif
}

BOOL CGLProjectInRibbonView::OnPreparePrinting(CPrintInfo* pInfo)
{
	// default preparation
	return DoPreparePrinting(pInfo);
}

void CGLProjectInRibbonView::OnBeginPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add extra initialization before printing
}

void CGLProjectInRibbonView::OnEndPrinting(CDC* /*pDC*/, CPrintInfo* /*pInfo*/)
{
	// TODO: add cleanup after printing
}

void CGLProjectInRibbonView::OnRButtonUp(UINT /* nFlags */, CPoint point)
{
	ClientToScreen(&point);
	OnContextMenu(this, point);
}

void CGLProjectInRibbonView::OnContextMenu(CWnd* /* pWnd */, CPoint point)
{
#ifndef SHARED_HANDLERS
	theApp.GetContextMenuManager()->ShowPopupMenu(IDR_POPUP_EDIT, point.x, point.y, this, TRUE);
#endif
}


// CGLProjectInRibbonView diagnostics

#ifdef _DEBUG
void CGLProjectInRibbonView::AssertValid() const
{
	CView::AssertValid();
}

void CGLProjectInRibbonView::Dump(CDumpContext& dc) const
{
	CView::Dump(dc);
}

CGLProjectInRibbonDoc* CGLProjectInRibbonView::GetDocument() const // non-debug version is inline
{
	ASSERT(m_pDocument->IsKindOf(RUNTIME_CLASS(CGLProjectInRibbonDoc)));
	return (CGLProjectInRibbonDoc*)m_pDocument;
}
#endif //_DEBUG


// CGLProjectInRibbonView message handlers


void CGLProjectInRibbonView::OnOpenbutton()
{
	// TODO:  在此添加控件通知处理程序代码
	BOOL isOpen = TRUE;
	CString defaultDir = L"D:\\MyProject\\MyOpenGL\\OBJ";   //默认打开的文件路径  
	CString fileName = L"";         //默认打开的文件名  
	CString filter = L"文件 (*.obj)|*.obj||";   //文件过虑的类型  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = L"D:\\MyProject\\MyOpenGL\\OBJ";
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\bunny.obj";
	if (result == IDOK) {
		filePath = openFileDlg.GetPathName();
	}
	
	m_FilePath = filePath;

	CT2CA pszConvertedAnsiString(m_FilePath);
	string tmp(pszConvertedAnsiString);
	m_Model = new Model((GLchar *)tmp.c_str());
	SMeshLib::IO::importOBJ(tmp, &m_Polyhedron);
	assert(!m_Polyhedron.empty());
	
	AdjustCameraView(m_Model);
	Invalidate(TRUE);
}


void CGLProjectInRibbonView::OnSavebutton()
{
	// TODO:  在此添加命令处理程序代码
	m_Simplify.write_obj("../test_out.obj");
}


int CGLProjectInRibbonView::OnCreate(LPCREATESTRUCT lpCreateStruct)
{
	if (CView::OnCreate(lpCreateStruct) == -1)
		return -1;
	//Detect memory leaks
	/*_CrtSetDbgFlag(_CRTDBG_ALLOC_MEM_DF | _CRTDBG_LEAK_CHECK_DF);
	_CrtSetBreakAlloc(459);*/

	// TODO:  在此添加您专用的创建代码
	// ////////////////////////////////////////////////////////////////
	// New codes begin:
	// The PIXELFORMATDESCRIPTOR structure describes
	//		the pixel format of a drawing surface.
	PIXELFORMATDESCRIPTOR pfd =
	{
		sizeof(PIXELFORMATDESCRIPTOR),  //  size of this pfd 
		1,                     			// version number 
		PFD_DRAW_TO_WINDOW |   	// support window 
		PFD_SUPPORT_OPENGL |   	// support OpenGL 
		PFD_DOUBLEBUFFER,	// double buffered
		PFD_TYPE_RGBA,
		24,                    	// 24-bit color depth 
		0, 0, 0, 0, 0, 0,      // color bits ignored 
		0,                     	// no alpha buffer 
		0,                     	// shift bit ignored 
		0,                     	// no accumulation buffer 
		0, 0, 0, 0,            	// accum bits ignored 
		32,                    	// 32-bit z-buffer (depth)
		0,                     	// no stencil buffer 
		0,                     	// no auxiliary buffer 
		PFD_MAIN_PLANE,        // main layer 
		0,                     	// reserved 
		0, 0, 0                	// layer masks ignored 
	};
	CClientDC dc(this);
	// Get the best available match of pixel format for the device context
	// In other words, if this computer doesn't support features that I
	// asked for, try to get the next best thing.  i.e. 16-bit color mode
	// instead of 24-bit color mode.
	int pixelFormat = ChoosePixelFormat(dc.m_hDC, &pfd);

	// Set the pixel format to the best pixel format I can get (see above)
	// and if that operation fails, bring up a message box that tells the
	// user the error.
	if (!SetPixelFormat(dc.m_hDC, pixelFormat, &pfd))
	{
		MessageBox(_T("Error: Unable to Set Pixel Format in CGLTemplate1View::OnCreate( )"),
			_T("Application Error"), MB_ICONERROR);
	}
	// Creates an OpenGL rendering context so that OpenGL knows how to draw
	// to this view. You can't use OpenGL in MFC without using the handle
	// that this function returns
	m_hRC = wglCreateContext(dc.m_hDC);

	wglMakeCurrent(dc.m_hDC, m_hRC);
	// Initialize GLEW to setup the OpenGL Function pointers
	glewExperimental = GL_TRUE;
	GLenum err = glewInit();
	// Define the viewport dimensions
	// Setup some OpenGL options
	glEnable(GL_DEPTH_TEST);

	if (err != GLEW_OK)
	{
		AfxMessageBox(CString(glewGetErrorString(err)));
	}

	//Init Shader
	m_Shader = new Shader("Shader/normal.vert", "Shader/normal.frag");
	m_TextShader = new Shader("text.vs", "text.frag");
	m_Camera = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	m_Model = nullptr;
	//m_Model_LOD = nullptr;
	m_text.InitFreeType();
	distance = glm::distance(m_Camera->Position, glm::vec3(0.0f, 0.0f, 0.0f));
	fps = 0.0;

	xMin = 0.0f;
	xMax = 0.0f;
	yMin = 0.0f;
	yMax = 0.0f;
	zMin = 0.0f;
	zMax = 0.0f;

	modelScale = 1.0f;

	m_ScreenWidth = 1024;
	m_ScreenHeight = 1024;

	m_DrawingMode = 1;

	isCameraAdjusted = false;

	// New codes end.
	// ////////////////////////////////////////////////////////////////
	return 0;
}


void CGLProjectInRibbonView::OnDestroy()
{
	CView::OnDestroy();

	// TODO:  在此处添加消息处理程序代码
	// ////////////////////////////////////////////////////////////////
	// New codes begin:

	// Set : a specified OpenGL rendering context ==> NULL
	// Set : current rendering context ==> NULL
	wglMakeCurrent(NULL, NULL);

	// Delete the handle to an OpenGL rendering context 
	wglDeleteContext(m_hRC);
	m_hRC = NULL;

	if (m_Shader != nullptr)
	{
		delete m_Shader;
		m_Shader = nullptr;
	}
	
	if (m_TextShader != nullptr)
	{
		delete m_TextShader;
		m_TextShader = nullptr;
	}

	if (m_Model != nullptr)
	{
		delete m_Model;
		m_Model = nullptr;
	}


	if (m_Camera != nullptr)
	{
		delete m_Camera;
		m_Camera = nullptr;
	}

	/*if (m_Model_LOD != nullptr)
	{
	delete m_Model_LOD;
	m_Model_LOD = nullptr;
	}*/

	DeleteTree();
}


void CGLProjectInRibbonView::OnSize(UINT nType, int cx, int cy)
{
	if (cx == 0 && cy == 0) return;

	//CView::OnSize(nType, cx, cy);

	// TODO:  在此处添加消息处理程序代码
	// ////////////////////////////////////////////////////////////////
	// New codes begin:

	CClientDC dc(this);
	wglMakeCurrent(dc.m_hDC, m_hRC);
	
	CRect windowRect;
	GetClientRect(&windowRect);
	
	m_ScreenWidth = windowRect.Width();
	m_ScreenHeight = windowRect.Height();

	int a = cx;
	int b = cy;

	m_ProjectionMatrix = glm::perspective(m_Camera->Zoom, (float)m_ScreenWidth / (float)m_ScreenHeight, 1.0f, bodyDiagnalLength * 10);
	glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);

	wglMakeCurrent(NULL, NULL);

	// New codes end.
	// ////////////////////////////////////////////////////////////////
}


BOOL CGLProjectInRibbonView::OnEraseBkgnd(CDC* pDC)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	// ////////////////////////////////////////////////////////////////
	// return CView::OnEraseBkgnd(pDC);
	// The above statement is commented
	//	and changed into the following statement
	//	such that the background is not repainted
	//	by the system
	// ////////////////////////////////////////////////////////////////
	return(TRUE);
}


void CGLProjectInRibbonView::OnLButtonDown(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	isDragged = true;
	lastX = point.x;
	lastY = point.y;

	CView::OnLButtonDown(nFlags, point);
}


void CGLProjectInRibbonView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	isDragged = false;

	CView::OnLButtonUp(nFlags, point);

}


void CGLProjectInRibbonView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (isDragged)
	{
		DragBall(lastX, lastY, point.x, point.y, 1.0f);
		lastX = point.x;
		lastY = point.y;
		Invalidate();
	}

	CView::OnMouseMove(nFlags, point);

}

void CGLProjectInRibbonView::DragBall(GLfloat ax, GLfloat ay, GLfloat bx, GLfloat by, GLfloat r)
{
	if (ax == bx && ay == by)
		return;

	GLint viewport[4];
	glGetIntegerv(GL_VIEWPORT, viewport);

	ax = ax * 2.0f / viewport[2] - 1.0f;
	ay = 1.0f - ay * 2.0f / viewport[3];
	bx = bx * 2.0f / viewport[2] - 1.0f;
	by = 1.0f - by * 2.0f / viewport[3];

	GLfloat az, bz = 0.0f;

	if ((pow(ax, 2) + pow(ay, 2)) <= pow(r, 2) / 2)
		az = sqrtf(pow(r, 2) - (pow(ax, 2) + pow(ay, 2)));
	else
		az = pow(r, 2) / 2 / sqrtf(pow(ax, 2) + pow(ay, 2));

	if ((pow(bx, 2) + pow(by, 2)) <= pow(r, 2) / 2)
		bz = sqrtf(pow(r, 2) - (pow(bx, 2) + pow(by, 2)));
	else
		bz = pow(r, 2) / 2 / sqrtf(pow(bx, 2) + pow(by, 2));

	glm::vec3 V1 = glm::normalize(glm::vec3(ax, ay, az));
	glm::vec3 V2 = glm::normalize(glm::vec3(bx, by, bz));

	glm::vec3 N = glm::cross(V1, V2);
	GLfloat theta = std::acos(glm::dot(V1, V2));

	m_RotationMatrix = glm::rotate(m_RotationMatrix, theta, glm::vec3(glm::inverse(m_ModelMatrix * m_ViewMatrix) * glm::vec4(N, 0.0f)));
}

void CGLProjectInRibbonView::CalculateFocusPoint(const vector<Mesh>& pMeshList)
{
	double xMin = DOUBLE_MAX, xMax = DOUBLE_MIN;
	double yMin = DOUBLE_MAX, yMax = DOUBLE_MIN;
	double zMin = DOUBLE_MAX, zMax = DOUBLE_MIN;

	for (const auto & i : pMeshList)
		for (const auto & j : i.vertices)
		{
			if (j.Position.x < xMin) xMin = j.Position.x;
			if (j.Position.x > xMax) xMax = j.Position.x;
			if (j.Position.y < yMin) yMin = j.Position.y;
			if (j.Position.y > yMax) yMax = j.Position.y;
			if (j.Position.z < zMin) zMin = j.Position.z;
			if (j.Position.z > zMax) zMax = j.Position.z;
		}
	m_FocusPoint = glm::vec3((xMin + xMax) / 2.0, (yMin + yMax) / 2.0, (zMin + zMax) / 2.0);

	this->xMin = xMin;
	this->xMax = xMax;
	this->yMin = yMin;
	this->yMax = yMax;
	this->zMin = zMin;
	this->zMax = zMax;
	float bodyDiagonal = sqrt(pow(xMax - xMin, 2) + pow(yMax - yMin, 2) + pow(zMax - zMin, 2));
	this->bodyDiagnalLength = bodyDiagonal;
}

void CGLProjectInRibbonView::CalculateFocusPoint(Model * pModel)
{
	double xMin = DOUBLE_MAX, xMax = DOUBLE_MIN;
	double yMin = DOUBLE_MAX, yMax = DOUBLE_MIN;
	double zMin = DOUBLE_MAX, zMax = DOUBLE_MIN;

	vector<Mesh> meshes = pModel->getModelMeshes();

	for (int i = 0; i < meshes.size(); ++i)
		for (int j = 0; j < meshes[i].vertices.size(); ++j)
		{
			if (meshes[i].vertices[j].Position.x < xMin) xMin = meshes[i].vertices[j].Position.x;
			if (meshes[i].vertices[j].Position.x > xMax) xMax = meshes[i].vertices[j].Position.x;
			if (meshes[i].vertices[j].Position.y < yMin) yMin = meshes[i].vertices[j].Position.y;
			if (meshes[i].vertices[j].Position.y > yMax) yMax = meshes[i].vertices[j].Position.y;
			if (meshes[i].vertices[j].Position.z < zMin) zMin = meshes[i].vertices[j].Position.z;
			if (meshes[i].vertices[j].Position.z > zMax) zMax = meshes[i].vertices[j].Position.z;
		}

	m_FocusPoint = glm::vec3((xMin + xMax) / 2.0, (yMin + yMax) / 2.0, (zMin + zMax) / 2.0);

	this->xMin = xMin;
	this->xMax = xMax;
	this->yMin = yMin;
	this->yMax = yMax;
	this->zMin = zMin;
	this->zMax = zMax;
	float bodyDiagonal = sqrt(pow(xMax - xMin, 2) + pow(yMax - yMin, 2) + pow(zMax - zMin, 2));
	this->bodyDiagnalLength = bodyDiagonal;
}

void CGLProjectInRibbonView::AdjustCameraView(const vector<Mesh>& pMeshList)
{
	CalculateFocusPoint(pMeshList);
	float bodyDiagonal = sqrt(pow(xMax - xMin, 2) + pow(yMax - yMin, 2) + pow(zMax - zMin, 2));
	this->bodyDiagnalLength = bodyDiagonal;
	m_Camera->SetPosition(0.0f, 0.0f, 2.0f * bodyDiagonal);
	m_Camera->SetFront(0.0f, 0.0f, -bodyDiagonal * 0.2);
	m_Camera->SetUp(0.0f, 1.0f, 0.0f);
	m_ProjectionMatrix = glm::perspective(45.0f, (float)m_ScreenWidth / (float)m_ScreenHeight, 10.0f, bodyDiagonal * 10);
}

void CGLProjectInRibbonView::AdjustCameraView(Model * pModel)
{
	assert(pModel != nullptr);

	CalculateFocusPoint(pModel);
	float bodyDiagonal = sqrt(pow(xMax - xMin, 2) + pow(yMax - yMin, 2) + pow(zMax - zMin, 2));
	this->bodyDiagnalLength = bodyDiagonal;
	m_Camera->SetPosition(0.0f, 0.0f, 2.0f * bodyDiagonal);
	m_Camera->SetFront(0.0f, 0.0f, -bodyDiagonal * 0.2);
	m_Camera->SetUp(0.0f, 1.0f, 0.0f);
	m_ProjectionMatrix = glm::perspective(45.0f, (float)m_ScreenWidth / (float)m_ScreenHeight, 10.0f, bodyDiagonal * 10);
}

BOOL CGLProjectInRibbonView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	/*m_Camera->ProcessMouseScrollInCameraPosition(zDelta);
	distance = glm::distance(m_Camera->Position, glm::vec3(0.0f, 0.0f, 0.0f));
	if (m_ifShowLod)
	ChooseLodModel(distance);*/
	int direction = (zDelta > 0) ? 1 : -1;
	modelScale *= pow(1.2f, -direction);
	Invalidate();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CGLProjectInRibbonView::OnShowline()
{
	// TODO:  在此添加命令处理程序代码
	m_ifShowLine = !m_ifShowLine;
	Invalidate();
}


void CGLProjectInRibbonView::OnUpdateShowline(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(m_ifShowLine);
}


void CGLProjectInRibbonView::OnQemoperation()
{
	// TODO:  在此添加命令处理程序代码
	//Simplify initialization
	//SimplifyInitialization(*m_Model, m_Simplify);
	//// Simplification start
	//m_Simplify.simplify_mesh(200);

	////Change m_model
	//m_Model->ChangeToMesh(m_Simplify);
	////delete m_Model;
	////m_Model = new Model("D:\\MyProject\\MyOpenGL\\实验\\GLProjectInRib\\test_out.obj");
	//Invalidate();
}



void CGLProjectInRibbonView::OnLoddisplay()
{
	// TODO:  在此添加命令处理程序代码
	/*if (m_Model == NULL)
	{
	MessageBox(_T("Please check the model!"));
	return;
	}*/

	////generate Model Lod List
	//if (m_Model != NULL)
	//{
	//	m_ModelList.push_back(*m_Model);
	//	int simplify_face_num = m_Model->face_num;
	//	for (int i = 0; i < 2; i++)
	//	{
	//		simplify_face_num /= 10;
	//		Model model_in_lod;
	//		SimplifyInitialization(*m_Model, m_Simplify);
	//		m_Simplify.simplify_mesh(simplify_face_num / 3);
	//		model_in_lod.ChangeToMesh(m_Simplify);
	//		m_ModelList.push_back(model_in_lod);
	//	}
	//	m_ifShowLod = true;
	//	ChooseLodModel(distance);
	//}
}

void CGLProjectInRibbonView::SimplifyInitialization(Model& model, Simplify& simplify)
{
	simplify.vertices.clear();
	simplify.triangles.clear();

	vector<Mesh> m_mesh = model.getModelMeshes();
	for (int i = 0; i < m_mesh.size(); ++i)
	{
		for (int j = 0; j < m_mesh[i].vertices.size(); ++j)
		{
			Simplify::Vertex v;
			v.p = vec3f(m_mesh[i].vertices[j].Position.x, m_mesh[i].vertices[j].Position.y, m_mesh[i].vertices[j].Position.z);
			simplify.vertices.push_back(v);
		}

	}

	for (int i = 0; i < m_mesh.size(); ++i)
	{
		for (int j = 0; j < m_mesh[i].indices.size(); j += 3)
		{
			Simplify::Triangle t;
			for (int k = 0; k < 3; ++k)
			{
				t.v[k] = m_mesh[i].indices[j + k];
			}
			simplify.triangles.push_back(t);
		}
	}

}

void CGLProjectInRibbonView::ChooseLodModel(float distance)
{
	/*if (0.0 <= distance && distance < 1.5)
		m_Model_LOD = &m_ModelList[0];
		else
		if (1.5 <= distance && distance < 3.0)
		m_Model_LOD = &m_ModelList[1];
		else
		if (3.0 <= distance)
		m_Model_LOD = &m_ModelList[2];*/

}

void CGLProjectInRibbonView::GetFPS(double deltaTime)
{
	static double total_frames = 0.0f;
	static double total_time = 0.0f;
	++total_frames;
	total_time += deltaTime;
	if (total_time > DBL_MAX)
	{
		total_frames = 0.0f;
		total_time = 0.0f;
	}
	fps = total_frames / total_time;
}


void CGLProjectInRibbonView::OnMeshSegmentation()
{
	/*m_TestMesh.vertices.push_back(Vertex(glm::vec3(-1.5, 1.5, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0)));
	m_TestMesh.vertices.push_back(Vertex(glm::vec3(-1.5, -1.5, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0)));
	m_TestMesh.vertices.push_back(Vertex(glm::vec3(1.5, -1.5, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0)));
	m_TestMesh.vertices.push_back(Vertex(glm::vec3(1.5, 1.5, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0)));
	m_TestMesh.vertices.push_back(Vertex(glm::vec3(-0.5, 0.5, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0)));
	m_TestMesh.vertices.push_back(Vertex(glm::vec3(-0.5, -0.5, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0)));
	m_TestMesh.vertices.push_back(Vertex(glm::vec3(0.5, -0.5, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0)));
	m_TestMesh.vertices.push_back(Vertex(glm::vec3(0.5, 0.5, 0.0), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0)));

	m_TestMesh.indices.push_back(0);
	m_TestMesh.indices.push_back(1);
	m_TestMesh.indices.push_back(4);

	m_TestMesh.indices.push_back(4);
	m_TestMesh.indices.push_back(1);
	m_TestMesh.indices.push_back(5);

	m_TestMesh.indices.push_back(5);
	m_TestMesh.indices.push_back(1);
	m_TestMesh.indices.push_back(6);

	m_TestMesh.indices.push_back(6);
	m_TestMesh.indices.push_back(1);
	m_TestMesh.indices.push_back(2);

	m_TestMesh.indices.push_back(7);
	m_TestMesh.indices.push_back(6);
	m_TestMesh.indices.push_back(2);

	m_TestMesh.indices.push_back(7);
	m_TestMesh.indices.push_back(2);
	m_TestMesh.indices.push_back(3);

	m_TestMesh.indices.push_back(3);
	m_TestMesh.indices.push_back(4);
	m_TestMesh.indices.push_back(7);

	m_TestMesh.indices.push_back(0);
	m_TestMesh.indices.push_back(4);
	m_TestMesh.indices.push_back(3);

	CgalPolyhedron tmpPolyhedron;
	ConvertFromMeshToCgalPolyhedron(m_TestMesh, tmpPolyhedron);

	unsigned int nb_holes = 0;
	BOOST_FOREACH(Halfedge_handle h, halfedges(tmpPolyhedron))
	{
	if (h->is_border())
	{
	vector<Facet_handle> patch_facets;
	vector<Vertex_handle> patch_vertices;
	bool success = CGAL::cpp11::get<0>(
	CGAL::Polygon_mesh_processing::triangulate_refine_and_fair_hole(
	tmpPolyhedron,
	h,
	back_inserter(patch_facets),
	back_inserter(patch_vertices),
	CGAL::Polygon_mesh_processing::parameters::vertex_point_map(get(CGAL::vertex_point, tmpPolyhedron)).geom_traits(Kernel())));
	cout << "Number of facets in constructed patch: " << patch_facets.size() << endl;
	cout << "Number of vertices in constructed patch: " << patch_vertices.size() << endl;
	cout << "Fairing: " << (success ? "succeeded" : "Failed") << endl;
	++nb_holes;
	}
	}

	cout << endl;
	cout << nb_holes << "holes have been filled" << endl;
	std::ofstream out("filledTmp.off");
	out.precision(17);
	out << tmpPolyhedron << endl;
	*/
	//..........................
	//...........................
	//...............................



	// TODO:  在此添加命令处理程序代码
	if (m_Polyhedron.empty())
	{
		MessageBox(_T("m_Polyhedron is empty."));
		return;
	}
	// create a property-map for SDF values
	boost::associative_property_map<Facet_double_map> sdf_property_map(internal_sdf_map);
	// compute SDF values using default parameters for number of rays, and cone angle
	CGAL::sdf_values(m_Polyhedron, sdf_property_map);
	// create a property-map for segment-ids
	boost::associative_property_map<Facet_int_map> segment_property_map(internal_segment_map);
	// segment the mesh using default parameters for number of levels, and smoothing lambda
	// Any other scalar values can be used instead of using SDF values computed using the CGAL function
	const std::size_t number_of_clusters = 4; // use 4 clusters in soft clustering
	const double smoothing_lambda = 0.1; // importance of surface features, suggested to be in-between [0,1]
	// Note that we can use the same SDF values (sdf_property_map) over and over again for segmentation.
	// This feature is relevant for segmenting the mesh several times with different parameters.
	std::size_t number_of_segments = CGAL::segmentation_from_sdf_values(m_Polyhedron, sdf_property_map, segment_property_map, number_of_clusters, smoothing_lambda);
	//std::cout << "Number of segments: " << number_of_segments << std::endl;
	
	vector<Color> colorList;
	//GenerateColorList(colorList);
	vector<Color> excludedColor = { Color(0.0, 0.0, 0.0) };
	GenerateUniqueColorList(number_of_segments, colorList, excludedColor);
	m_MeshList.resize(number_of_segments);

	double maxSDF = DOUBLE_MIN;
	int indexMaxSDF = -1;
	for (CgalPolyhedron::Facet_iterator facet_it = m_Polyhedron.facets_begin();
		facet_it != m_Polyhedron.facets_end(); ++facet_it)
	{
		if (internal_sdf_map[facet_it] > maxSDF)
		{
			maxSDF = internal_sdf_map[facet_it];
			indexMaxSDF = internal_segment_map[facet_it];
		}
	}

	//m_MeshList[indexMaxSDF].SetDelete();

	for (CgalPolyhedron::Facet_iterator facet_it = m_Polyhedron.facets_begin();
		facet_it != m_Polyhedron.facets_end(); ++facet_it)
	{
		int meshIndex = segment_property_map[facet_it];
		int verticeIndex = m_MeshList[meshIndex].vertices.size();
		int indiceIndex = m_MeshList[meshIndex].indices.size();
		Halfedge_around_facet_circulator hec = facet_it->facet_begin();// get half edge handle
		Halfedge_handle he_begin = hec;
		Vertex_handle v;
		vector<glm::vec3> points;//points for calculate normals
		do
		{
			v = hec->vertex();
			Point p = v->point();
			m_MeshList[meshIndex].vertices.push_back(Vertex(glm::vec3(p.x(), p.y(), p.z()), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0)));
			m_MeshList[meshIndex].indices.push_back(indiceIndex);
			points.push_back(glm::vec3(p.x(), p.y(), p.z()));
			++indiceIndex;
			++hec;
		} while (he_begin != hec);
		//calculate normals
		assert(points.size() == 3);
		glm::vec3 v1 = points[1] - points[0];
		glm::vec3 v2 = points[2] - points[1];
		glm::vec3 vNormal = glm::cross(v1, v2);
		int lastIndex = m_MeshList[meshIndex].vertices.size() - 1;
		m_MeshList[meshIndex].vertices[lastIndex].Normal = vNormal;
		m_MeshList[meshIndex].vertices[lastIndex - 1].Normal = vNormal;
		m_MeshList[meshIndex].vertices[lastIndex - 2].Normal = vNormal;
	}
	
	//TO DO : mesh simplification
	//adjacent graph
	m_MeshGraph = vector<vector<int>>(number_of_segments, vector<int>(number_of_segments, 0));

	for (CgalPolyhedron::Halfedge_iterator halfedge_it = m_Polyhedron.halfedges_begin();
		halfedge_it != m_Polyhedron.halfedges_end(); ++halfedge_it)
	{
		int indexMesh = segment_property_map[halfedge_it->facet()];
		int indexOppositeMesh = segment_property_map[halfedge_it->opposite()->facet()];
		if (indexMesh == indexOppositeMesh) continue;
		m_MeshGraph[indexMesh][indexOppositeMesh] = 1;
	}

	//construct tree
	ConstructTree(indexMaxSDF);
	SimplifyWithDelete();

	//m_MeshList[0].SetDelete();

	CombineMesh();

#pragma region Test Hole

	CgalPolyhedron tmpPolyhedron;
	ConvertFromMeshToCgalPolyhedron(m_CombinedMesh, tmpPolyhedron);

	unsigned int nb_holes = 0;
	BOOST_FOREACH(Halfedge_handle h, halfedges(tmpPolyhedron))
	{
		if (h->is_border())
		{
			vector<Facet_handle> patch_facets;
			vector<Vertex_handle> patch_vertices;
			bool success = CGAL::cpp11::get<0>(
				CGAL::Polygon_mesh_processing::triangulate_refine_and_fair_hole(
				tmpPolyhedron,
				h,
				back_inserter(patch_facets),
				back_inserter(patch_vertices),
				CGAL::Polygon_mesh_processing::parameters::vertex_point_map(get(CGAL::vertex_point, tmpPolyhedron)).geom_traits(Kernel())));
			cout << "Number of facets in constructed patch: " << patch_facets.size() << endl;
			cout << "Number of vertices in constructed patch: " << patch_vertices.size() << endl;
			cout << "Fairing: " << (success ? "succeeded" : "Failed") << endl;
			++nb_holes;
		}
	}

	cout << endl;
	cout << nb_holes << "holes have been filled" << endl;
	std::ofstream out("filledTmp.off");
	out.precision(17);
	out << tmpPolyhedron << endl;

#pragma endregion Test Hole

	RepairMeshHole();

	//Set Color
	assert(m_MeshList.size() == colorList.size());
	for (int i = 0; i < m_MeshList.size(); ++i)
	{
		m_MeshList[i].color = colorList[i];
	}

	CRect rect;
	GetClientRect(&rect);

	m_ScreenWidth = rect.Width();
	m_ScreenHeight = rect.Height();

	SetupMeshList();
	AdjustCameraView(m_MeshList);
	SetDrawingMode(DRAW_MESHSEGMENTATION);

	Invalidate();
}

void CGLProjectInRibbonView::GenerateColorList(vector<Color>& pColorList)
{
	int nColor = pColorList.size();
	int i = 0;
	while (i != nColor)
	{
		int color = i % 3;
		switch (color)
		{
		case 0: pColorList[i] = Color(1.0, 0.0, 0.0); break;
		case 1: pColorList[i] = Color(0.0, 1.0, 0.0); break;
		case 2: pColorList[i] = Color(0.0, 0.0, 1.0); break;
		}
		++i;
	}
}

int CGLProjectInRibbonView::GenerateUniqueColorList(int count, vector<Color>& pColorList, vector<Color>& pExcludedColor)
{
	int i, j, k, l;
	int numUnique = 0;
	double slValues[] = { 0.0, 1.0, 0.5, 0.8, 0.3, 0.6, 0.9, 0.2, 0.7, 0.4, 0.1 };
	Color baseColors[] = {
		Color(0, 0, 255),
		Color(0, 255, 0),
		Color(255, 0, 0),
		Color(0, 255, 255),
		Color(255, 255, 0),
		Color(255, 0, 255),
		Color(255, 255, 255)
	};

	for (i = 0; i < sizeof(slValues) / sizeof(slValues[0]); ++i)
	{
		for (j = 0; j < sizeof(slValues) / sizeof(slValues[0]); ++j)
		{
			for (k = 0; k < sizeof(baseColors) / sizeof(baseColors[0]); ++k)
			{
				int newColor[3];
				int maxValue;

				newColor[0] = (int)(baseColors[k].R * slValues[j] + 0.5);
				newColor[1] = (int)(baseColors[k].G * slValues[j] + 0.5);
				newColor[2] = (int)(baseColors[k].B * slValues[j] + 0.5);

				maxValue = 0;
				for (l = 0; l < 3; ++l)
				{
					if (newColor[l] > maxValue)
					{
						maxValue = newColor[l];
					}
				}

				maxValue = (int)(maxValue * slValues[i] + 0.5);
				for (l = 0; l < 3; ++l)
				{
					if (newColor[l] < maxValue)
					{
						newColor[l] = maxValue;
					}
				}

				Color colorToInsert;
				colorToInsert.R = newColor[0];
				colorToInsert.G = newColor[1];
				colorToInsert.B = newColor[2];

				for (l = 0; l < pExcludedColor.size(); ++l)
				{
					if ((int)pExcludedColor[l].R == (int)colorToInsert.R
						&& (int)pExcludedColor[l].G == (int)colorToInsert.G
						&& (int)pExcludedColor[l].B == (int)colorToInsert.B)
						break;
				}

				if (l == pExcludedColor.size())
				{
					for (l = 0; l < pColorList.size(); ++l)
					{
						if ((int)pColorList[l].R == (int)colorToInsert.R
							&& (int)pColorList[l].G == (int)colorToInsert.G
							&& (int)pColorList[l].B == (int)colorToInsert.B)
							break;
					}
					if (l == pColorList.size())
					{
						colorToInsert.R /= 255.0;
						colorToInsert.G /= 255.0;
						colorToInsert.B /= 255.0;
						pColorList.push_back(colorToInsert);
						++numUnique;
						if (pColorList.size() == count)
						{
							return numUnique;
						}
					}
				}

			}
		}
	}
	return numUnique;
}

void CGLProjectInRibbonView::SetupMeshList()
{
	assert(!m_MeshList.empty());

	for (int i = 0; i < m_MeshList.size(); ++i)
	{
		m_MeshList[i].setupMesh();
	}
}

void CGLProjectInRibbonView::GLSetRenderState()
{
	glViewport(0, 0, m_ScreenWidth, m_ScreenHeight);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (m_ifShowLine)
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
	}
	else
	{
		glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	}
}

void CGLProjectInRibbonView::TimeStart()
{
	QueryPerformanceFrequency(&frequency);
	QueryPerformanceCounter(&count1);
}

void CGLProjectInRibbonView::TimeEnd()
{
	QueryPerformanceCounter(&count2);
	double frametime = (count2.QuadPart - count1.QuadPart) * 1000.0 / frequency.QuadPart;
	GetFPS(frametime);
}

void CGLProjectInRibbonView::DrawModel()
{
	if (m_DrawingMode == DRAW_NORMAL)
	{
		DrawModelInNormalMode();
	}
	else if (m_DrawingMode == DRAW_MESHSEGMENTATION)
	{
		DrawModelSegmentation();
	}

	//m_Shader->Use();
	//// Transformation matrices
	//m_ProjectionMatrix = glm::perspective(m_Camera->Zoom, (float)m_ScreenWidth / (float)m_ScreenHeight, 1.0f, bodyDiagnalLength * 10);
	//m_ViewMatrix = m_Camera->GetViewMatrix();
	//glUniformMatrix4fv(glGetUniformLocation(m_Shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(m_ProjectionMatrix));
	//glUniformMatrix4fv(glGetUniformLocation(m_Shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(m_ViewMatrix));
	//// Draw the loaded model
	//m_ModelMatrix = glm::mat4(1.0);
	//m_ModelMatrix = m_RotationMatrix;
	//m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(modelScale));
	//m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(-m_FocusPoint.x, -m_FocusPoint.y, -m_FocusPoint.z)); // Translate it down a bit so it's at the center of the scene
	//glUniformMatrix4fv(glGetUniformLocation(m_Shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(m_ModelMatrix));

	////Draw Objects....................................
	//for (int i = 0; i < m_MeshList.size(); ++i)
	//{
	//	InitMaterial(m_MeshList[i].color);
	//	m_MeshList[i].Draw(*m_Shader);
	//}
}

void CGLProjectInRibbonView::DrawModelInNormalMode()
{
	m_Shader->Use();
	// Transformation matrices
	m_ProjectionMatrix = glm::perspective(m_Camera->Zoom, (float)m_ScreenWidth / (float)m_ScreenHeight, 1.0f, bodyDiagnalLength * 10);
	m_ViewMatrix = m_Camera->GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(m_Shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(m_ProjectionMatrix));
	glUniformMatrix4fv(glGetUniformLocation(m_Shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(m_ViewMatrix));
	// Draw the loaded model
	m_ModelMatrix = glm::mat4(1.0);
	m_ModelMatrix = m_RotationMatrix;
	m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(modelScale));
	m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(-m_FocusPoint.x, -m_FocusPoint.y, -m_FocusPoint.z)); // Translate it down a bit so it's at the center of the scene
	glUniformMatrix4fv(glGetUniformLocation(m_Shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(m_ModelMatrix));

	Color normalColor(1.0f, 1.0f, 1.0f);
	InitMaterial(normalColor);
	m_Model->Draw(*m_Shader);
}

void CGLProjectInRibbonView::DrawModelSegmentation()
{
	m_Shader->Use();
	// Transformation matrices
	m_ProjectionMatrix = glm::perspective(m_Camera->Zoom, (float)m_ScreenWidth / (float)m_ScreenHeight, 1.0f, bodyDiagnalLength * 10);
	m_ViewMatrix = m_Camera->GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(m_Shader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(m_ProjectionMatrix));
	glUniformMatrix4fv(glGetUniformLocation(m_Shader->Program, "view"), 1, GL_FALSE, glm::value_ptr(m_ViewMatrix));
	// Draw the loaded model
	m_ModelMatrix = glm::mat4(1.0);
	m_ModelMatrix = m_RotationMatrix;
	m_ModelMatrix = glm::scale(m_ModelMatrix, glm::vec3(modelScale));
	m_ModelMatrix = glm::translate(m_ModelMatrix, glm::vec3(-m_FocusPoint.x, -m_FocusPoint.y, -m_FocusPoint.z)); // Translate it down a bit so it's at the center of the scene
	glUniformMatrix4fv(glGetUniformLocation(m_Shader->Program, "model"), 1, GL_FALSE, glm::value_ptr(m_ModelMatrix));

	//Draw Objects....................................
	for (int i = 0; i < m_MeshList.size(); ++i)
	{
		if (m_MeshList[i].isDeleted()) continue;
		InitMaterial(m_MeshList[i].color);
		m_MeshList[i].Draw(*m_Shader);
	}
}

void CGLProjectInRibbonView::DrawTextInfo()
{
	//Show Message................
	//Set OpenGL options
	glEnable(GL_CULL_FACE);
	glEnable(GL_BLEND);
	glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);
	glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
	//Compile and setup the shader
	glm::mat4 projection_message = glm::ortho(0.0f, (GLfloat)(m_ScreenWidth), 0.0f, (GLfloat)(m_ScreenHeight));
	m_TextShader->Use();
	glUniformMatrix4fv(glGetUniformLocation(m_TextShader->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection_message));
	char fps_msg[20] = { '\0' };
	sprintf(fps_msg, "FPS    :%.2lf", fps);
	m_text.RenderText(*m_TextShader, fps_msg, 25.0f, 105.0f, 0.5f, glm::vec3(0.5f, 0.5f, 0.5f));
}

void CGLProjectInRibbonView::InitMaterial(Color& pColor)
{
	m_Shader->Use();
	glUniform3f(glGetUniformLocation(m_Shader->Program, "material.ambient"), 0.2 * pColor.R, 0.2 * pColor.G, 0.2 * pColor.B);
	glUniform3f(glGetUniformLocation(m_Shader->Program, "material.diffuse"), pColor.R, pColor.G, pColor.B);
	glUniform3f(glGetUniformLocation(m_Shader->Program, "material.specular"), 0.508273f, 0.508273f, 0.508273f);
	glUniform1f(glGetUniformLocation(m_Shader->Program, "material.shininess"), 32.0f);
}

void CGLProjectInRibbonView::InitDirectionlLighting()
{
	m_Shader->Use();
	glUniform3fv(glGetUniformLocation(m_Shader->Program, "viewPos"), 1, glm::value_ptr(m_Camera->Position));
	glUniform3f(glGetUniformLocation(m_Shader->Program, "dirLight.direction"), 0.0f, 0.0f, -1.0f);
	glUniform3f(glGetUniformLocation(m_Shader->Program, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(m_Shader->Program, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(m_Shader->Program, "dirLight.specular"), 0.2f, 0.2f, 0.2f);
}

void CGLProjectInRibbonView::SetDrawingMode(int drawingMode)
{
	m_DrawingMode = drawingMode;
}

void CGLProjectInRibbonView::ConstructTree(int indexMeshWithMaxSDF)
{
	vector<bool> usedMesh(m_MeshGraph.size(), false);
	m_MeshTree = new TreeNode(indexMeshWithMaxSDF);
	HelperConstructTree(m_MeshTree, usedMesh);
}

void CGLProjectInRibbonView::HelperConstructTree(TreeNode* pTreeNode, vector<bool>& usedMesh)
{
	if (pTreeNode == nullptr) return;

	usedMesh[pTreeNode->indexMesh] = true;

	auto rowGraph = m_MeshGraph[pTreeNode->indexMesh];

	for (int i = 0; i < rowGraph.size(); ++i)
	{
		if (rowGraph[i] == 1)
		{
			if (usedMesh[i]) continue;
			else pTreeNode->childNodes.push_back(new TreeNode(i));
		}
	}

	if (pTreeNode->childNodes.empty()) return;
	else {
		for (auto & node : pTreeNode->childNodes)
		{
			HelperConstructTree(node, usedMesh);
		}
	}
}

void CGLProjectInRibbonView::DeleteTree()
{
	if (m_MeshTree == nullptr) return;
	if (m_MeshTree->childNodes.empty()) return;
	HelperDeleteTree(m_MeshTree);
	m_MeshTree = nullptr;
}

void CGLProjectInRibbonView::HelperDeleteTree(TreeNode* pTreeNode)
{
	if (pTreeNode == nullptr) return;
	if (!pTreeNode->childNodes.empty())
	{
		for (auto & node : pTreeNode->childNodes)
		{
			HelperDeleteTree(node);
		}
	}
	delete pTreeNode;
}

void CGLProjectInRibbonView::SimplifyWithDelete()
{
	if (m_MeshTree == nullptr) return;
	HelperSimplifyWithDelete(m_MeshTree, 1);
}

void CGLProjectInRibbonView::HelperSimplifyWithDelete(TreeNode * pTreeNode, int k)
{
	if (pTreeNode == nullptr) return;
	
	if (k > 2)
		m_MeshList[pTreeNode->indexMesh].SetDelete();

	if (!pTreeNode->childNodes.empty())
	{
		for (auto & node : pTreeNode->childNodes)
		{
			HelperSimplifyWithDelete(node, k + 1);
		}
	}
}

void CGLProjectInRibbonView::RepairMeshHole()
{
	int fileIndex = 0;
	for (int i = 0; i < m_MeshList.size(); ++i)
	{
		if (!m_MeshList[i].isDeleted())
		{
			//Mesh contain duplicate points
			CgalPolyhedron needRepairedPolyhedron;
			ConvertFromMeshToCgalPolyhedron(m_MeshList[i], needRepairedPolyhedron);

			unsigned int nb_holes = 0;
			BOOST_FOREACH(Halfedge_handle h, halfedges(needRepairedPolyhedron))
			{
				if (h->is_border())
				{
					vector<Facet_handle> patch_facets;
					vector<Vertex_handle> patch_vertices;
					bool success = CGAL::cpp11::get<0>(
						CGAL::Polygon_mesh_processing::triangulate_refine_and_fair_hole(
						needRepairedPolyhedron,
						h,
						back_inserter(patch_facets),
						back_inserter(patch_vertices),
						CGAL::Polygon_mesh_processing::parameters::vertex_point_map(get(CGAL::vertex_point, needRepairedPolyhedron)).geom_traits(Kernel())));
					cout << "Number of facets in constructed patch: " << patch_facets.size() << endl;
					cout << "Number of vertices in constructed patch: " << patch_vertices.size() << endl;
					cout << "Fairing: " << (success ? "succeeded" : "Failed") << endl;
					++nb_holes;
				}
			}

			cout << endl;
			cout << nb_holes << "holes have been filled" << endl;
			std::ofstream out("filled" + to_string(fileIndex) + ".off");
			out.precision(17);
			out << needRepairedPolyhedron << endl;

			fileIndex++;
		}
	}
	
}

void CGLProjectInRibbonView::ConvertFromMeshToCgalPolyhedron(const Mesh & pMesh, CgalPolyhedron & pPolyhedron)
{
	assert(pMesh.vertices.size() != 0);
	assert(pMesh.indices.size() != 0);
	
	SMeshLib::IO::importFromMesh(pMesh, &pPolyhedron);
}

void CGLProjectInRibbonView::CombineMesh()
{
	m_CombinedMesh.vertices.clear();
	m_CombinedMesh.indices.clear();
	m_CombinedMesh.textures.clear();

	map<tuple<float, float, float>, int> mp;

	for (int meshIndex = 0; meshIndex < m_MeshList.size(); ++meshIndex)
	{
		if (!m_MeshList[meshIndex].deleted)
		{
			for (const auto & indice : m_MeshList[meshIndex].indices)
			{
				auto v = m_MeshList[meshIndex].vertices[indice];
				auto tp = make_tuple(v.Position.x, v.Position.y, v.Position.z);
				if (mp.count(tp) > 0)
				{
					m_CombinedMesh.indices.push_back(mp[tp]);
				}
				else
				{
					m_CombinedMesh.vertices.push_back(Vertex(glm::vec3(get<0>(tp), get<1>(tp), get<2>(tp)), glm::vec3(0, 0, 0), glm::vec2(0, 0)));
					m_CombinedMesh.indices.push_back(m_CombinedMesh.vertices.size() - 1);

					mp.insert({ tp, m_CombinedMesh.vertices.size() - 1 });
				}
			}

		}
	}
}