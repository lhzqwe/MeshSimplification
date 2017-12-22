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

#define _CRTDBG_MAP_ALLOC

#include <map>

// New codes end.
// ////////////////////////////////////////////////////////////////

#define MIDDLE_PROCESS

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
	ON_COMMAND(ID_OPENBUTTON, &CGLProjectInRibbonView::OnOpenButton)
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
	ON_COMMAND(ID_BUTTON4, &CGLProjectInRibbonView::OnNormalDisplay)
	ON_COMMAND(ID_BUTTON5, &CGLProjectInRibbonView::OnSegmentationDisplay)
	ON_COMMAND(ID_BUTTON6, &CGLProjectInRibbonView::OnSimplifiedDisplay)
	ON_COMMAND(ID_DELETESIMP, &CGLProjectInRibbonView::OnDeleteSimp)
	ON_COMMAND(ID_SERIALIZE_WRITE, &CGLProjectInRibbonView::OnSerializeWrite)
	ON_COMMAND(ID_SERIALIZE_READ, &CGLProjectInRibbonView::OnSerializeRead)
	ON_COMMAND(ID_QUADRICSIMP, &CGLProjectInRibbonView::OnQuadricSimp)
	ON_COMMAND(ID_FINALMETHOD, &CGLProjectInRibbonView::OnFinalmethod)
	ON_COMMAND(ID_START, &CGLProjectInRibbonView::OnStartCLBAlgorithm)
	ON_COMMAND(ID_GENERATE_BORDR_LINE, &CGLProjectInRibbonView::OnGenerateBorderLine)
	ON_COMMAND(ID_GENERATE_CONNECT_FACES, &CGLProjectInRibbonView::OnGenerateConnectFaces)
	ON_COMMAND(ID_GENERATE_MAXIMUM_CONNECT_REGIONS, &CGLProjectInRibbonView::OnGenerateMaximumConnectBorderLines)
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

	InitDirectionLighting();

	//DrawModel();

	/*if (ifDrawBorderLine) DrawLine(5.0f, 1.0f, 0.0, 0.0f);
	if (ifDrawRegions) DrawRegions(regionPs_gm_);
	if (ifDrawConnectBorderLines) DrawConnectBorderLine(5.0f);*/

	switch (draw_type_)
	{
	case DrawType::NORMAL_MESH:
		DrawModel();
		break;
	case DrawType::REGION:
		DrawRegions(regionPs_gm_);
		break;
	case DrawType::BORDER_LINE_SEGMENT:
		DrawLine(5.0f, 1.0f, 0.0f, 0.0f);
		break;
	case DrawType::CONNECT_BORDER_LINE_SEGMENT:
		DrawConnectBorderLine(5.0f);
		break;
	default:
		break;
	}

	//DrawModelInNormalMode();

	TimeEnd();

	//DrawTextInfo();

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


void CGLProjectInRibbonView::OnOpenButton()
{
	// TODO:  在此添加控件通知处理程序代码
	BOOL isOpen = TRUE;
	CString defaultDir = L"D:\\MyProject\\MyOpenGL\\OBJ";   //默认打开的文件路径  
	CString fileName = L"";         //默认打开的文件名  
	CString filter = L"文件 (*.obj)|*.obj||";   //文件过虑的类型  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = L"D:\\MyProject\\MyOpenGL\\OBJ";
	INT_PTR result = openFileDlg.DoModal();
	CString filePath = defaultDir + "\\SWD4-8FRF-10.obj";
	if (result == IDOK) {
		filePath = openFileDlg.GetPathName();
		CT2CA pszConvertedAnsiString(filePath);
		string tmp(pszConvertedAnsiString);
		model_name_ = tmp.substr(tmp.find_last_of("\\") + 1, tmp.find_last_of(".") - tmp.find_last_of("\\") - 1);
	}
	else
	{
		MessageBox(_T("请重新选择Mesh!"));
		return;
	}
	
	file_path_ = filePath;

	if (model_ != nullptr)
	{
		polyhedron_.clear();
		delete model_;
		model_ = nullptr;
		normal_mesh_.indices.clear();
		normal_mesh_.vertices.clear();
		normal_mesh_.textures.clear();
		normal_mesh_.RefreshGpuData();
	}
	
	//OpenMesh read mesh ...
	//read mesh
	CT2CA pszConvertedAnsiString(file_path_);
	string file_path_tmp(pszConvertedAnsiString);
	cout << "File path : " << file_path_tmp << endl;
	cout << "Reading mesh file..." << endl;
	if (!OpenMesh::IO::read_mesh(mesh_, file_path_tmp.c_str()))
	{
		cout << "Read mesh file failed..." << endl;
		return;
	}
	cout << "Read mesh file completed..." << endl;
	cout << "Mesh vertices: " << mesh_.n_vertices() << " "
		<< "faces: " << mesh_.n_faces() << endl;

	normal_mesh_ = GenerateFlatMesh(mesh_);

	OnNormalDisplay();

	//CGAL
	/*CT2CA pszConvertedAnsiString(file_path_);
	string tmp(pszConvertedAnsiString);
	SMeshLib::IO::importOBJ(tmp, &polyhedron_);
	assert(!polyhedron_.empty());*/


	//m_Model = new Model((GLchar *)tmp.c_str());
	
	//AdjustCameraView(m_Model);

	//auto meshes = m_Model->getModelMeshes();
	//m_NormalMesh = GenerateFlatMesh(meshes[0]);

	//OnSerializeRead();

	//OnNormalDisplay();

	Invalidate(TRUE);
}


void CGLProjectInRibbonView::OnSavebutton()
{
	// TODO:  在此添加命令处理程序代码
	//m_Simplify.write_obj("../test_out.obj");
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

	//Init Console
	WCHAR szBuf[100];
	GetConsoleTitle(szBuf, 100);
	HWND hwnd = ::FindWindow(NULL, szBuf);
	HMENU hmenu = ::GetSystemMenu(hwnd, FALSE);
	::RemoveMenu(hmenu, SC_CLOSE, MF_BYCOMMAND);

	//Init Shader ...
	shader_ = nullptr;
	line_shader_ = nullptr;
	text_shader_ = nullptr;
	camera_ = nullptr;

	cout << "Init shader ..." << endl;
	shader_ = new Shader("Shader/normal.vert", "Shader/normal.frag");
	if (shader_ == nullptr) cout << "shader_ " << "init failed..." << endl;
	else cout << "shader_ " << "init success ..." << endl;

	//m_Shader = new Shader("Shader/normal_vShading.vert", "Shader/normal_vShading.frag");
	text_shader_ = new Shader("text.vs", "text.frag");
	if (text_shader_ == nullptr) cout << "text_shader_ " << "init failed..." << endl;
	else cout << "text_shader_ " << "init success ..." << endl;

	line_shader_ = new Shader("Shader/line.vert", "Shader/line.frag");
	if (line_shader_ == nullptr) cout << "line_shader_ " << "init failed..." << endl;
	else cout << "line_shader_ " << "init success ..." << endl;

	region_shader_ = new Shader("Shader/region.vert", "Shader/region.frag");
	if (region_shader_ == nullptr) cout << "region_shader_ " << "init failed..." << endl;
	else cout << "region_shader_ " << "init success..." << endl;

	camera_ = new Camera(glm::vec3(0.0f, 0.0f, 3.0f));
	if (camera_ == nullptr) cout << "Camera " << "init failed..." << endl;
	else cout << "Camera " << "init success with position : " << "(" 
		<< camera_->Position.x << ", " << camera_->Position.y << ", " << camera_->Position.z
		<< ")";
	model_ = nullptr;
	//m_Model_LOD = nullptr;
	text_.InitFreeType();
	num_distance_ = glm::distance(camera_->Position, glm::vec3(0.0f, 0.0f, 0.0f));
	num_fps_ = 0.0;

	num_xMin_ = 0.0f;
	num_xMax_ = 0.0f;
	num_yMin_ = 0.0f;
	num_yMax_ = 0.0f;
	num_zMin_ = 0.0f;
	num_zMax_ = 0.0f;

	num_model_scale_ = 1.0f;

	num_screen_width_ = 1024;
	num_screen_height_ = 1024;

	drawing_mode_ = DRAW_SMOOTH;

	isCameraAdjusted_ = false;

	pFlatMesh_ = nullptr;
	pSmoothMesh_ = nullptr;

	mesh_tree_ = nullptr;

	ifDrawBorderLine = false;
	ifDrawRegions = false;
	ifDrawConnectBorderLines = false;
	// New codes end.
	// //////////////////////////////////////////////////////////////

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

	if (shader_ != nullptr)
	{
		delete shader_;
		shader_ = nullptr;
	}

	if (line_shader_ != nullptr)
	{
		delete line_shader_;
		line_shader_ = nullptr;
	}

	if (region_shader_ != nullptr)
	{
		delete region_shader_;
		region_shader_ = nullptr;
	}
	
	if (text_shader_ != nullptr)
	{
		delete text_shader_;
		text_shader_ = nullptr;
	}

	if (model_ != nullptr)
	{
		delete model_;
		model_ = nullptr;
	}


	if (camera_ != nullptr)
	{
		delete camera_;
		camera_ = nullptr;
	}

	if (pFlatMesh_ != nullptr)
	{
		delete pFlatMesh_;
		pFlatMesh_ = nullptr;
	}

	if (pSmoothMesh_ != nullptr)
	{
		delete pSmoothMesh_;
		pSmoothMesh_ = nullptr;
	}

	/*if (m_Model_LOD != nullptr)
	{
	delete m_Model_LOD;
	m_Model_LOD = nullptr;
	}*/

	DeleteTree();
	//_CrtDumpMemoryLeaks();
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
	
	num_screen_width_ = windowRect.Width();
	num_screen_height_ = windowRect.Height();

	int a = cx;
	int b = cy;

	matrix_projection_ = glm::perspective(camera_->Zoom, (float)num_screen_width_ / (float)num_screen_height_, 1.0f, num_bodyDiagnalLength_ * 10);
	glViewport(0, 0, num_screen_width_, num_screen_height_);

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
	isDragged_ = true;
	num_lastX_ = static_cast<GLfloat>(point.x);
	num_lastY_ = static_cast<GLfloat>(point.y);

	CView::OnLButtonDown(nFlags, point);
}


void CGLProjectInRibbonView::OnLButtonUp(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	isDragged_ = false;

	CView::OnLButtonUp(nFlags, point);

}


void CGLProjectInRibbonView::OnMouseMove(UINT nFlags, CPoint point)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	if (isDragged_)
	{
		DragBall(num_lastX_, num_lastY_, static_cast<GLfloat>(point.x), static_cast<GLfloat>(point.y), 1.0f);
		num_lastX_ = static_cast<GLfloat>(point.x);
		num_lastY_ = static_cast<GLfloat>(point.y);
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

	matrix_rotation_ = glm::rotate(matrix_rotation_, theta, glm::vec3(glm::inverse(matrix_model_ * matrix_view_) * glm::vec4(N, 0.0f)));
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
	focus_point_ = glm::vec3((xMin + xMax) / 2.0, (yMin + yMax) / 2.0, (zMin + zMax) / 2.0);

	this->num_xMin_ = xMin;
	this->num_xMax_ = xMax;
	this->num_yMin_ = yMin;
	this->num_yMax_ = yMax;
	this->num_zMin_ = zMin;
	this->num_zMax_ = zMax;
	float bodyDiagonal = static_cast<float>((pow(xMax - xMin, 2) + pow(yMax - yMin, 2) + pow(zMax - zMin, 2)));
	this->num_bodyDiagnalLength_ = bodyDiagonal;
}

void CGLProjectInRibbonView::CalculateFocusPoint(Model * pModel)
{
	double xMin = DOUBLE_MAX, xMax = DOUBLE_MIN;
	double yMin = DOUBLE_MAX, yMax = DOUBLE_MIN;
	double zMin = DOUBLE_MAX, zMax = DOUBLE_MIN;

	vector<Mesh> meshes = pModel->getModelMeshes();

	for (unsigned int i = 0; i < meshes.size(); ++i)
		for (unsigned int j = 0; j < meshes[i].vertices.size(); ++j)
		{
			if (meshes[i].vertices[j].Position.x < xMin) xMin = meshes[i].vertices[j].Position.x;
			if (meshes[i].vertices[j].Position.x > xMax) xMax = meshes[i].vertices[j].Position.x;
			if (meshes[i].vertices[j].Position.y < yMin) yMin = meshes[i].vertices[j].Position.y;
			if (meshes[i].vertices[j].Position.y > yMax) yMax = meshes[i].vertices[j].Position.y;
			if (meshes[i].vertices[j].Position.z < zMin) zMin = meshes[i].vertices[j].Position.z;
			if (meshes[i].vertices[j].Position.z > zMax) zMax = meshes[i].vertices[j].Position.z;
		}

	focus_point_ = glm::vec3((xMin + xMax) / 2.0, (yMin + yMax) / 2.0, (zMin + zMax) / 2.0);

	this->num_xMin_ = xMin;
	this->num_xMax_ = xMax;
	this->num_yMin_ = yMin;
	this->num_yMax_ = yMax;
	this->num_zMin_ = zMin;
	this->num_zMax_ = zMax;
	float bodyDiagonal = static_cast<float>((pow(xMax - xMin, 2) + pow(yMax - yMin, 2) + pow(zMax - zMin, 2)));
	this->num_bodyDiagnalLength_ = bodyDiagonal;
}

void CGLProjectInRibbonView::AdjustCameraView(const vector<Mesh>& pMeshList)
{
	CalculateFocusPoint(pMeshList);
	float bodyDiagonal = static_cast<float>((pow(num_xMax_ - num_xMin_, 2) + pow(num_yMax_ - num_yMin_, 2) + pow(num_zMax_ - num_zMin_, 2)));
	this->num_bodyDiagnalLength_ = bodyDiagonal;
	camera_->SetPosition(0.0f, 0.0f, 2.0f * bodyDiagonal);
	camera_->SetFront(0.0f, 0.0f, -bodyDiagonal * 0.2f);
	camera_->SetUp(0.0f, 1.0f, 0.0f);
	matrix_projection_ = glm::perspective(45.0f, (float)num_screen_width_ / (float)num_screen_height_, 10.0f, bodyDiagonal * 10);
}

void CGLProjectInRibbonView::AdjustCameraView(Model * pModel)
{
	assert(pModel != nullptr);

	CalculateFocusPoint(pModel);
	float bodyDiagonal = static_cast<float>(sqrt(pow(num_xMax_ - num_xMin_, 2) + pow(num_yMax_ - num_yMin_, 2) + pow(num_zMax_ - num_zMin_, 2)));
	this->num_bodyDiagnalLength_ = bodyDiagonal;
	camera_->SetPosition(0.0f, 0.0f, 2.0f * bodyDiagonal);
	camera_->SetFront(0.0f, 0.0f, -bodyDiagonal * 0.2f);
	camera_->SetUp(0.0f, 1.0f, 0.0f);
	matrix_projection_ = glm::perspective(45.0f, (float)num_screen_width_ / (float)num_screen_height_, 10.0f, bodyDiagonal * 10);
}

BOOL CGLProjectInRibbonView::OnMouseWheel(UINT nFlags, short zDelta, CPoint pt)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值
	/*m_Camera->ProcessMouseScrollInCameraPosition(zDelta);
	distance = glm::distance(m_Camera->Position, glm::vec3(0.0f, 0.0f, 0.0f));
	if (m_ifShowLod)
	ChooseLodModel(distance);*/
	int direction = (zDelta > 0) ? 1 : -1;
	num_model_scale_ *= pow(1.2f, -direction);
	Invalidate();
	return CView::OnMouseWheel(nFlags, zDelta, pt);
}


void CGLProjectInRibbonView::OnShowline()
{
	// TODO:  在此添加命令处理程序代码
	isShowLine_ = !isShowLine_;
	Invalidate();
}


void CGLProjectInRibbonView::OnUpdateShowline(CCmdUI *pCmdUI)
{
	// TODO:  在此添加命令更新用户界面处理程序代码
	pCmdUI->SetCheck(isShowLine_);
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
	num_fps_ = total_frames / total_time;
}


void CGLProjectInRibbonView::OnMeshSegmentation()
{
	// TODO:  在此添加命令处理程序代码
	if (polyhedron_.empty())
	{
		MessageBox(_T("m_Polyhedron is empty."));
		return;
	}
	// create a property-map for SDF values
	boost::associative_property_map<Facet_double_map> sdf_property_map(internal_sdf_map_);
	// compute SDF values using default parameters for number of rays, and cone angle
	CGAL::sdf_values(polyhedron_, sdf_property_map, 1.0 / 2.0 * CGAL_PI, 30, true);
	// create a property-map for segment-ids
	boost::associative_property_map<Facet_int_map> segment_property_map(internal_segment_map_);
	// segment the mesh using default parameters for number of levels, and smoothing lambda
	// Any other scalar values can be used instead of using SDF values computed using the CGAL function
	const std::size_t number_of_clusters = 4; // use 4 clusters in soft clustering
	const double smoothing_lambda = 0.3; // importance of surface features, suggested to be in-between [0,1]
	// Note that we can use the same SDF values (sdf_property_map) over and over again for segmentation.
	// This feature is relevant for segmenting the mesh several times with different parameters.
	number_of_segments_ = CGAL::segmentation_from_sdf_values(polyhedron_, sdf_property_map, segment_property_map, number_of_clusters, smoothing_lambda);
	//std::cout << "Number of segments: " << number_of_segments << std::endl;
	
	vector<Color> colorList;
	//GenerateColorList(colorList);
	vector<Color> excludedColor = { Color(0.0, 0.0, 0.0) };
	GenerateUniqueColorList(number_of_segments_, colorList, excludedColor);
	mesh_list_.resize(number_of_segments_);

	double maxSDF = DOUBLE_MIN;
	int indexMaxSDF = -1;
	for (CgalPolyhedron::Facet_iterator facet_it = polyhedron_.facets_begin();
		facet_it != polyhedron_.facets_end(); ++facet_it)
	{
		if (internal_sdf_map_[facet_it] > maxSDF)
		{
			maxSDF = internal_sdf_map_[facet_it];
			indexMaxSDF = internal_segment_map_[facet_it];
		}
	}

	index_max_SDF_ = indexMaxSDF;

//	//m_MeshList[indexMaxSDF].SetDelete();
//
	for (CgalPolyhedron::Facet_iterator facet_it = polyhedron_.facets_begin();
		facet_it != polyhedron_.facets_end(); ++facet_it)
	{
		int meshIndex = segment_property_map[facet_it];
		int verticeIndex = mesh_list_[meshIndex].vertices.size();
		int indiceIndex = mesh_list_[meshIndex].indices.size();
		Halfedge_around_facet_circulator hec = facet_it->facet_begin();// get half edge handle
		Halfedge_handle he_begin = hec;
		Vertex_handle v;
		vector<glm::vec3> points;//points for calculate normals
		do
		{
			v = hec->vertex();
			Point p = v->point();
			mesh_list_[meshIndex].vertices.push_back(Vertex(glm::vec3(p.x(), p.y(), p.z()), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0)));
			mesh_list_[meshIndex].indices.push_back(indiceIndex);
			points.push_back(glm::vec3(p.x(), p.y(), p.z()));
			++indiceIndex;
			++hec;
		} while (he_begin != hec);

		//update mesh sdf & faceNum
		mesh_list_[meshIndex].sdf += internal_sdf_map_[facet_it];
		mesh_list_[meshIndex].faceNum++;

		//calculate normals
		assert(points.size() == 3);
		glm::vec3 v1 = points[1] - points[0];
		glm::vec3 v2 = points[2] - points[1];
		glm::vec3 vNormal = glm::cross(v1, v2);
		int lastIndex = mesh_list_[meshIndex].vertices.size() - 1;
		mesh_list_[meshIndex].vertices[lastIndex].Normal = vNormal;
		mesh_list_[meshIndex].vertices[lastIndex - 1].Normal = vNormal;
		mesh_list_[meshIndex].vertices[lastIndex - 2].Normal = vNormal;
	}

	//Caculate average sdf value
	for (unsigned int i = 0; i < mesh_list_.size(); ++i)
	{
		mesh_list_[i].sdf /= mesh_list_[i].faceNum;
	}

	//adjacent graph
	mesh_graph_ = vector<vector<int>>(number_of_segments_, vector<int>(number_of_segments_, 0));

	for (CgalPolyhedron::Halfedge_iterator halfedge_it = polyhedron_.halfedges_begin();
		halfedge_it != polyhedron_.halfedges_end(); ++halfedge_it)
	{
		int indexMesh = segment_property_map[halfedge_it->facet()];
		int indexOppositeMesh = segment_property_map[halfedge_it->opposite()->facet()];
		if (indexMesh == indexOppositeMesh) continue;
		mesh_graph_[indexMesh][indexOppositeMesh] = 1;
	}
//
//	//construct tree
//	ConstructTree(indexMaxSDF);
//	SimplifyWithDelete();
//
//	//m_MeshList[0].SetDelete();
//
//	CombineMesh();
//
//#pragma region Test Hole
//
//	CgalPolyhedron tmpPolyhedron;
//	ConvertFromMeshToCgalPolyhedron(m_CombinedMesh, tmpPolyhedron);
//
//	unsigned int nb_holes = 0;
//	BOOST_FOREACH(Halfedge_handle h, halfedges(tmpPolyhedron))
//	{
//		if (h->is_border())
//		{
//			vector<Facet_handle> patch_facets;
//			vector<Vertex_handle> patch_vertices;
//			bool success = CGAL::cpp11::get<0>(
//				CGAL::Polygon_mesh_processing::triangulate_refine_and_fair_hole(
//				tmpPolyhedron,
//				h,
//				back_inserter(patch_facets),
//				back_inserter(patch_vertices),
//				CGAL::Polygon_mesh_processing::parameters::vertex_point_map(get(CGAL::vertex_point, tmpPolyhedron)).geom_traits(Kernel())));
//			cout << "Number of facets in constructed patch: " << patch_facets.size() << endl;
//			cout << "Number of vertices in constructed patch: " << patch_vertices.size() << endl;
//			cout << "Fairing: " << (success ? "succeeded" : "Failed") << endl;
//			++nb_holes;
//		}
//	}
//
//	cout << endl;
//	cout << nb_holes << "holes have been filled" << endl;
//	std::ofstream out("filledTmp.off");
//	out.precision(17);
//	out << tmpPolyhedron << endl;
//
//	//Extract mesh from simplified polyhedron
//	m_SimplifiedMesh.vertices.clear();
//	m_SimplifiedMesh.indices.clear();
//	m_SimplifiedMesh.textures.clear();
//
//	//Simplified Flat Mesh
//	for (CgalPolyhedron::Facet_iterator facet_it = tmpPolyhedron.facets_begin();
//		facet_it != tmpPolyhedron.facets_end(); ++facet_it)
//	{
//		int verticeIndex = m_SimplifiedMesh.vertices.size();
//		int indiceIndex = m_SimplifiedMesh.indices.size();
//		Halfedge_around_facet_circulator hec = facet_it->facet_begin();// get half edge handle
//		Halfedge_handle he_begin = hec;
//		Vertex_handle v;
//		vector<glm::vec3> points;//points for calculate normals
//		do
//		{
//			v = hec->vertex();
//			Point p = v->point();
//			m_SimplifiedMesh.vertices.push_back(Vertex(glm::vec3(p.x(), p.y(), p.z()), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0)));
//			m_SimplifiedMesh.indices.push_back(indiceIndex);
//			points.push_back(glm::vec3(p.x(), p.y(), p.z()));
//			++indiceIndex;
//			++hec;
//		} while (he_begin != hec);
//		//calculate normals
//		assert(points.size() == 3);
//		glm::vec3 v1 = points[1] - points[0];
//		glm::vec3 v2 = points[2] - points[1];
//		glm::vec3 vNormal = glm::cross(v1, v2);
//		int lastIndex = m_SimplifiedMesh.vertices.size() - 1;
//		m_SimplifiedMesh.vertices[lastIndex].Normal = vNormal;
//		m_SimplifiedMesh.vertices[lastIndex - 1].Normal = vNormal;
//		m_SimplifiedMesh.vertices[lastIndex - 2].Normal = vNormal;
//	}
//
//
//#pragma endregion Test Hole

	//RepairMeshHole();

	//Set Color
	assert(mesh_list_.size() == colorList.size());
	for (unsigned int i = 0; i < mesh_list_.size(); ++i)
	{
		mesh_list_[i].color = colorList[i];
	}

	CRect rect;
	GetClientRect(&rect);

	num_screen_width_ = rect.Width();
	num_screen_height_ = rect.Height();

	OnSegmentationDisplay();
}

void CGLProjectInRibbonView::SetupSimplifiedMesh()
{
	assert(!simplified_mesh_.vertices.empty());
	assert(!simplified_mesh_.indices.empty());

	simplified_mesh_.setupMesh();
}

void CGLProjectInRibbonView::AdjustCameraView(const Mesh& pMesh)
{
	CalculateFocusPoint(pMesh);
	float bodyDiagonal = static_cast<float>(sqrt(pow(num_xMax_ - num_xMin_, 2) + pow(num_yMax_ - num_yMin_, 2) + pow(num_zMax_ - num_zMin_, 2)));
	this->num_bodyDiagnalLength_ = bodyDiagonal;
	camera_->SetPosition(0.0f, 0.0f, 2.0f * bodyDiagonal);
	camera_->SetFront(0.0f, 0.0f, -bodyDiagonal * 0.2f);
	camera_->SetUp(0.0f, 1.0f, 0.0f);
	matrix_projection_ = glm::perspective(45.0f, (float)num_screen_width_ / (float)num_screen_height_, 10.0f, bodyDiagonal * 10);
}

void CGLProjectInRibbonView::CalculateFocusPoint(const Mesh& pMesh)
{
	double xMin = DOUBLE_MAX, xMax = DOUBLE_MIN;
	double yMin = DOUBLE_MAX, yMax = DOUBLE_MIN;
	double zMin = DOUBLE_MAX, zMax = DOUBLE_MIN;

	for (const auto & i : pMesh.vertices)
	{
		if (i.Position.x < xMin) xMin = i.Position.x;
		if (i.Position.x > xMax) xMax = i.Position.x;
		if (i.Position.y < yMin) yMin = i.Position.y;
		if (i.Position.y > yMax) yMax = i.Position.y;
		if (i.Position.z < zMin) zMin = i.Position.z;
		if (i.Position.z > zMax) zMax = i.Position.z;
	}
	focus_point_ = glm::vec3((xMin + xMax) / 2.0, (yMin + yMax) / 2.0, (zMin + zMax) / 2.0);

	this->num_xMin_ = xMin;
	this->num_xMax_ = xMax;
	this->num_yMin_ = yMin;
	this->num_yMax_ = yMax;
	this->num_zMin_ = zMin;
	this->num_zMax_ = zMax;
	float bodyDiagonal = static_cast<float>
		(sqrt(pow(xMax - xMin, 2) + pow(yMax - yMin, 2) + pow(zMax - zMin, 2)));
	this->num_bodyDiagnalLength_ = bodyDiagonal;
}

Mesh CGLProjectInRibbonView::GenerateFlatMesh(Mesh & pMesh)
{
	assert(!pMesh.vertices.empty());
	assert(!pMesh.indices.empty());

	Mesh result;

	auto points = pMesh.vertices;
	auto indices = pMesh.indices;
	glm::vec3 n;
	for (unsigned int i = 0, indice = 0; i < indices.size(); i += 3, indice += 3)
	{
		result.vertices.push_back(points[indices[i]]);
		result.vertices.push_back(points[indices[i + 1]]);
		result.vertices.push_back(points[indices[i + 2]]);

		result.indices.push_back(indice);
		result.indices.push_back(indice + 1);
		result.indices.push_back(indice + 2);

		n = glm::cross(points[indices[i + 1]].Position - points[indices[i]].Position, points[indices[i + 2]].Position - points[indices[i + 1]].Position);
		
		result.vertices[indice].Normal = n;
		result.vertices[indice + 1].Normal = n;
		result.vertices[indice + 2].Normal = n;
	}

	return result;
}

Mesh CGLProjectInRibbonView::GenerateFlatMesh(MyMesh & mesh)
{
	assert(mesh.n_vertices() != 0);

	Mesh result;

	MyMesh::FaceIter f_it, f_end(mesh.faces_end());
	MyMesh::FaceVertexIter fv_it;

	glm::vec3 n;
	for (f_it = mesh.faces_begin(); f_it != f_end; ++f_it)
	{
		fv_it = mesh.fv_iter(*f_it);
		if (fv_it.is_valid())
		{
			auto &point1 = mesh.point(*fv_it);
			++fv_it;
			auto &point2 = mesh.point(*fv_it);
			++fv_it;
			auto &point3 = mesh.point(*fv_it);

			Vertex v1(glm::vec3(point1[0], point1[1], point1[2]));
			Vertex v2(glm::vec3(point2[0], point2[1], point2[2]));
			Vertex v3(glm::vec3(point3[0], point3[1], point3[2]));

			n = glm::cross(v2.Position - v1.Position, v3.Position - v2.Position);

			v1.Normal = n;
			v2.Normal = n;
			v3.Normal = n;

			result.vertices.push_back(v1);
			result.vertices.push_back(v2);
			result.vertices.push_back(v3);

			for (int i = 0; i < 3; ++i)
			{
				result.indices.push_back(result.indices.size());
			}			
		}
	}
	return result;
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
	unsigned int i, j, k, l;
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
				colorToInsert.R = newColor[0] * 1.0f;
				colorToInsert.G = newColor[1] * 1.0f;
				colorToInsert.B = newColor[2] * 1.0f;

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
	assert(!mesh_list_.empty());

	for (unsigned int i = 0; i < mesh_list_.size(); ++i)
	{
		mesh_list_[i].setupMesh();
	}
}

void CGLProjectInRibbonView::ClearMeshSegmentationDeleteState()
{
	for (unsigned int i = 0; i < mesh_list_.size(); ++i)
	{
		mesh_list_[i].SetExist();
	}
}

void CGLProjectInRibbonView::SetupMesh(Mesh & pMesh)
{
	assert(!pMesh.vertices.empty());
	assert(!pMesh.indices.empty());

	pMesh.setupMesh();
}

void CGLProjectInRibbonView::GLSetRenderState()
{
	glViewport(0, 0, num_screen_width_, num_screen_height_);

	glClearColor(0, 0, 0, 1);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	if (isShowLine_)
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
	QueryPerformanceFrequency(&frequency_);
	QueryPerformanceCounter(&count1_);
}

void CGLProjectInRibbonView::TimeEnd()
{
	QueryPerformanceCounter(&count2_);
	double frametime = (count2_.QuadPart - count1_.QuadPart) * 1000.0 / frequency_.QuadPart;
	GetFPS(frametime);
}

void CGLProjectInRibbonView::DrawMesh(int drawMode)
{
	if (drawMode == DRAW_NORMAL)
	{
		DrawMesh(normal_mesh_);
	}
	else if (drawMode == DRAW_MESHSEGMENTATION)
	{
		DrawModelSegmentation();
	}
	else if (drawMode == DRAW_SIMPLIFIEDMESH)
	{
		DrawMesh(simplified_mesh_);
	}
}

void CGLProjectInRibbonView::DrawMesh(Mesh& pMesh)
{
	shader_->Use();
	// Transformation matrices
	matrix_projection_ = glm::perspective(camera_->Zoom, (float)num_screen_width_ / (float)num_screen_height_, 1.0f, num_bodyDiagnalLength_ * 10);
	matrix_view_ = camera_->GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "projection"), 1, GL_FALSE, glm::value_ptr(matrix_projection_));
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "view"), 1, GL_FALSE, glm::value_ptr(matrix_view_));
	// Draw the loaded model
	matrix_model_ = glm::mat4(1.0);
	matrix_model_ = matrix_rotation_;
	matrix_model_ = glm::scale(matrix_model_, glm::vec3(num_model_scale_));
	matrix_model_ = glm::translate(matrix_model_, glm::vec3(-focus_point_.x, -focus_point_.y, -focus_point_.z)); // Translate it down a bit so it's at the center of the scene
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "model"), 1, GL_FALSE, glm::value_ptr(matrix_model_));

	Color normalColor(1.0f, 1.0f, 1.0f);
	InitMaterial(normalColor, *shader_);
	pMesh.Draw(*shader_);
}

void CGLProjectInRibbonView::DrawModel()
{
	DrawMesh(drawing_mode_);
}

void CGLProjectInRibbonView::DrawSimplifiedModel()
{
	shader_->Use();
	// Transformation matrices
	matrix_projection_ = glm::perspective(camera_->Zoom, (float)num_screen_width_ / (float)num_screen_height_, 1.0f, num_bodyDiagnalLength_ * 10);
	matrix_view_ = camera_->GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "projection"), 1, GL_FALSE, glm::value_ptr(matrix_projection_));
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "view"), 1, GL_FALSE, glm::value_ptr(matrix_view_));
	// Draw the loaded model
	matrix_model_ = glm::mat4(1.0);
	matrix_model_ = matrix_rotation_;
	matrix_model_ = glm::scale(matrix_model_, glm::vec3(num_model_scale_));
	matrix_model_ = glm::translate(matrix_model_, glm::vec3(-focus_point_.x, -focus_point_.y, -focus_point_.z)); // Translate it down a bit so it's at the center of the scene
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "model"), 1, GL_FALSE, glm::value_ptr(matrix_model_));

	Color normalColor(1.0f, 1.0f, 1.0f);
	InitMaterial(normalColor, *shader_); 
	simplified_mesh_.Draw(*shader_);
}

void CGLProjectInRibbonView::DrawModelInNormalMode()
{
	shader_->Use();
	// Transformation matrices
	matrix_projection_ = glm::perspective(camera_->Zoom, (float)num_screen_width_ / (float)num_screen_height_, 1.0f, num_bodyDiagnalLength_ * 10);
	matrix_view_ = camera_->GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "projection"), 1, GL_FALSE, glm::value_ptr(matrix_projection_));
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "view"), 1, GL_FALSE, glm::value_ptr(matrix_view_));
	// Draw the loaded model
	matrix_model_ = glm::mat4(1.0);
	matrix_model_ = matrix_rotation_;
	matrix_model_ = glm::scale(matrix_model_, glm::vec3(num_model_scale_));
	matrix_model_ = glm::translate(matrix_model_, glm::vec3(-focus_point_.x, -focus_point_.y, -focus_point_.z)); // Translate it down a bit so it's at the center of the scene
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "model"), 1, GL_FALSE, glm::value_ptr(matrix_model_));

	Color normalColor(1.0f, 1.0f, 1.0f);
	InitMaterial(normalColor, *shader_);
	model_->Draw(*shader_);
}

void CGLProjectInRibbonView::DrawModelSegmentation()
{
	shader_->Use();
	// Transformation matrices
	matrix_projection_ = glm::perspective(camera_->Zoom, (float)num_screen_width_ / (float)num_screen_height_, 1.0f, num_bodyDiagnalLength_ * 10);
	matrix_view_ = camera_->GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "projection"), 1, GL_FALSE, glm::value_ptr(matrix_projection_));
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "view"), 1, GL_FALSE, glm::value_ptr(matrix_view_));
	// Draw the loaded model
	matrix_model_ = glm::mat4(1.0);
	matrix_model_ = matrix_rotation_;
	matrix_model_ = glm::scale(matrix_model_, glm::vec3(num_model_scale_));
	matrix_model_ = glm::translate(matrix_model_, glm::vec3(-focus_point_.x, -focus_point_.y, -focus_point_.z)); // Translate it down a bit so it's at the center of the scene
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "model"), 1, GL_FALSE, glm::value_ptr(matrix_model_));

	//Draw Objects....................................
	for (unsigned int i = 0; i < mesh_list_.size(); ++i)
	{
		if (mesh_list_[i].isDeleted()) continue;
		InitMaterial(mesh_list_[i].color, *shader_);
		mesh_list_[i].Draw(*shader_);
	}
}

void CGLProjectInRibbonView::DrawFlatMesh()
{
	assert(pFlatMesh_ != nullptr);

	shader_->Use();
	// Transformation matrices
	matrix_projection_ = glm::perspective(camera_->Zoom, (float)num_screen_width_ / (float)num_screen_height_, 1.0f, num_bodyDiagnalLength_ * 10);
	matrix_view_ = camera_->GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "projection"), 1, GL_FALSE, glm::value_ptr(matrix_projection_));
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "view"), 1, GL_FALSE, glm::value_ptr(matrix_view_));
	// Draw the loaded model
	matrix_model_ = glm::mat4(1.0);
	matrix_model_ = matrix_rotation_;
	matrix_model_ = glm::scale(matrix_model_, glm::vec3(num_model_scale_));
	matrix_model_ = glm::translate(matrix_model_, glm::vec3(-focus_point_.x, -focus_point_.y, -focus_point_.z)); // Translate it down a bit so it's at the center of the scene
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "model"), 1, GL_FALSE, glm::value_ptr(matrix_model_));

	Color normalColor(1.0f, 1.0f, 1.0f);
	InitMaterial(normalColor, *shader_);
	pFlatMesh_->Draw(*shader_);
}

void CGLProjectInRibbonView::DrawSmoothMesh()
{
	assert(pSmoothMesh_ != nullptr);

	shader_->Use();
	// Transformation matrices
	matrix_projection_ = glm::perspective(camera_->Zoom, (float)num_screen_width_ / (float)num_screen_height_, 1.0f, num_bodyDiagnalLength_ * 10);
	matrix_view_ = camera_->GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "projection"), 1, GL_FALSE, glm::value_ptr(matrix_projection_));
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "view"), 1, GL_FALSE, glm::value_ptr(matrix_view_));
	// Draw the loaded model
	matrix_model_ = glm::mat4(1.0);
	matrix_model_ = matrix_rotation_;
	matrix_model_ = glm::scale(matrix_model_, glm::vec3(num_model_scale_));
	matrix_model_ = glm::translate(matrix_model_, glm::vec3(-focus_point_.x, -focus_point_.y, -focus_point_.z)); // Translate it down a bit so it's at the center of the scene
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "model"), 1, GL_FALSE, glm::value_ptr(matrix_model_));

	Color normalColor(1.0f, 1.0f, 1.0f);
	InitMaterial(normalColor, *shader_);
	pSmoothMesh_->Draw(*shader_);
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
	glm::mat4 projection_message = glm::ortho(0.0f, (GLfloat)(num_screen_width_), 0.0f, (GLfloat)(num_screen_height_));
	text_shader_->Use();
	glUniformMatrix4fv(glGetUniformLocation(text_shader_->Program, "projection"), 1, GL_FALSE, glm::value_ptr(projection_message));
	char fps_msg[20] = { '\0' };
	sprintf(fps_msg, "FPS    :%.2lf", num_fps_);
	text_.RenderText(*text_shader_, fps_msg, 25.0f, 105.0f, 0.5f, glm::vec3(0.5f, 0.5f, 0.5f));
}

void CGLProjectInRibbonView::InitMaterial(Color& pColor, Shader & shader)
{
	shader.Use();
	glUniform3f(glGetUniformLocation(shader.Program, "material.ambient"), 0.2f * pColor.R, 0.2f * pColor.G, 0.2f * pColor.B);
	glUniform3f(glGetUniformLocation(shader.Program, "material.diffuse"), pColor.R, pColor.G, pColor.B);
	glUniform3f(glGetUniformLocation(shader.Program, "material.specular"), 0.508273f, 0.508273f, 0.508273f);
	glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 32.0f);
}

void CGLProjectInRibbonView::InitMaterial(Shader & shader)
{
	shader.Use();
	glUniform3f(glGetUniformLocation(shader.Program, "material.ambient"), 0.2f , 0.2f, 0.2f);
	glUniform3f(glGetUniformLocation(shader.Program, "material.diffuse"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shader.Program, "material.specular"), 0.508273f, 0.508273f, 0.508273f);
	glUniform1f(glGetUniformLocation(shader.Program, "material.shininess"), 32.0f);
}

void CGLProjectInRibbonView::InitDirectionLighting()
{
	shader_->Use();
	glUniform3fv(glGetUniformLocation(shader_->Program, "viewPos"), 1, glm::value_ptr(camera_->Position));
	glUniform3f(glGetUniformLocation(shader_->Program, "dirLight.direction"), 0.0f, 0.0f, -1.0f);
	glUniform3f(glGetUniformLocation(shader_->Program, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shader_->Program, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(shader_->Program, "dirLight.specular"), 0.2f, 0.2f, 0.2f);
}

void CGLProjectInRibbonView::InitDirectionLighting(Shader& shader)
{
	shader.Use();
	glUniform3fv(glGetUniformLocation(shader.Program, "viewPos"), 1, glm::value_ptr(camera_->Position));
	glUniform3f(glGetUniformLocation(shader.Program, "dirLight.direction"), 0.0f, 0.0f, -1.0f);
	glUniform3f(glGetUniformLocation(shader.Program, "dirLight.ambient"), 1.0f, 1.0f, 1.0f);
	glUniform3f(glGetUniformLocation(shader.Program, "dirLight.diffuse"), 0.5f, 0.5f, 0.5f);
	glUniform3f(glGetUniformLocation(shader.Program, "dirLight.specular"), 0.2f, 0.2f, 0.2f);
}

void CGLProjectInRibbonView::SetDrawingMode(int drawingMode)
{
	drawing_mode_ = drawingMode;
}

void CGLProjectInRibbonView::SetDrawingMode(DrawType draw_type)
{
	draw_type_ = draw_type;
}

void CGLProjectInRibbonView::ConstructTree(int indexMeshWithMaxSDF)
{
	vector<bool> usedMesh(mesh_graph_.size(), false);
	mesh_tree_ = new TreeNode(indexMeshWithMaxSDF);
	HelperConstructTree(mesh_tree_, usedMesh);
}

void CGLProjectInRibbonView::HelperConstructTree(TreeNode* pTreeNode, vector<bool>& usedMesh)
{
	if (pTreeNode == nullptr) return;

	usedMesh[pTreeNode->indexMesh] = true;

	auto rowGraph = mesh_graph_[pTreeNode->indexMesh];

	for (unsigned int i = 0; i < rowGraph.size(); ++i)
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

void CGLProjectInRibbonView::ConstructTreeBFS(int indexMeshWithSDF)
{
	mesh_tree_ = new TreeNode(indexMeshWithSDF);
	mesh_tree_->avgSDF = mesh_list_[indexMeshWithSDF].sdf;
	mesh_tree_->singleSDF = mesh_list_[indexMeshWithSDF].sdf;
	mesh_tree_->faceNum = mesh_list_[indexMeshWithSDF].faceNum;
	mesh_tree_->singleFaceNum = mesh_list_[indexMeshWithSDF].faceNum;

	queue<TreeNode*> queueBFS;
	queueBFS.push(mesh_tree_);
	vector<bool> usedMesh(number_of_segments_, false);

	usedMesh[indexMeshWithSDF] = true;

	int meshIndex;
	TreeNode* treeNode = nullptr;
	while (!queueBFS.empty())
	{
		treeNode = queueBFS.front();
		queueBFS.pop();
		meshIndex = treeNode->indexMesh;

		auto adjacentMeshes = mesh_graph_[meshIndex];

		for (unsigned int i = 0; i < adjacentMeshes.size(); ++i)
		{
			if (adjacentMeshes[i] == 1 && !usedMesh[i])
			{
				TreeNode* newNode = new TreeNode(i);
				newNode->avgSDF = mesh_list_[i].sdf;
				newNode->singleSDF = mesh_list_[i].sdf;
				newNode->faceNum = mesh_list_[i].faceNum;
				newNode->singleFaceNum = mesh_list_[i].faceNum;
				treeNode->childNodes.push_back(newNode);
				queueBFS.push(newNode);
				usedMesh[i] = true;
			}
		}
	}
}

void CGLProjectInRibbonView::DeleteTree()
{
	if (mesh_tree_ == nullptr) return;
	if (mesh_tree_->childNodes.empty()) return;
	HelperDeleteTree(mesh_tree_);
	mesh_tree_ = nullptr;
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
	pTreeNode = nullptr;
}

void CGLProjectInRibbonView::SimplifyWithDelete()
{
	if (mesh_tree_ == nullptr) return;
	HelperSimplifyWithDelete(mesh_tree_, 1);
}

void CGLProjectInRibbonView::HelperSimplifyWithDelete(TreeNode * pTreeNode, int k)
{
	if (pTreeNode == nullptr) return;
	
	if (k == 3)
		mesh_list_[pTreeNode->indexMesh].SetDelete();

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
	for (unsigned int i = 0; i < mesh_list_.size(); ++i)
	{
		if (!mesh_list_[i].isDeleted())
		{
			//Mesh contain duplicate points
			CgalPolyhedron needRepairedPolyhedron;
			ConvertFromMeshToCgalPolyhedron(mesh_list_[i], needRepairedPolyhedron);

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
	//assert(pMesh.vertices.size() != 0);
	//assert(pMesh.indices.size() != 0);
	
	SMeshLib::IO::importFromMesh(pMesh, &pPolyhedron);
}

void CGLProjectInRibbonView::ConvertFromMeshToSurfaceMeshPolyhedron(const Mesh & pMesh, Surface_mesh & pSurfaceMesh)
{
	//assert(pMesh.vertices.size() != 0);
	//assert(pMesh.indices.size() != 0);

	SMeshLib::IO::importFromMesh(pMesh, &pSurfaceMesh);
}

void CGLProjectInRibbonView::CombineMesh()
{
	combined_mesh_.vertices.clear();
	combined_mesh_.indices.clear();
	combined_mesh_.textures.clear();

	map<tuple<float, float, float>, int> mp;

	for (unsigned int meshIndex = 0; meshIndex < mesh_list_.size(); ++meshIndex)
	{
		if (!mesh_list_[meshIndex].deleted)
		{
			for (const auto & indice : mesh_list_[meshIndex].indices)
			{
				auto v = mesh_list_[meshIndex].vertices[indice];
				auto tp = make_tuple(v.Position.x, v.Position.y, v.Position.z);
				if (mp.count(tp) > 0)
				{
					combined_mesh_.indices.push_back(mp[tp]);
				}
				else
				{
					combined_mesh_.vertices.push_back(Vertex(glm::vec3(get<0>(tp), get<1>(tp), get<2>(tp)), glm::vec3(0, 0, 0), glm::vec2(0, 0)));
					combined_mesh_.indices.push_back(combined_mesh_.vertices.size() - 1);

					mp.insert({ tp, combined_mesh_.vertices.size() - 1 });
				}
			}

		}
	}
}

void CGLProjectInRibbonView::CombineTheMesh(int meshIndex)
{
	combined_mesh_.vertices.clear();
	combined_mesh_.indices.clear();
	combined_mesh_.textures.clear();

	map<tuple<float, float, float>, int> mp;

	for (const auto & indice : mesh_list_[meshIndex].indices)
	{
		auto v = mesh_list_[meshIndex].vertices[indice];
		auto tp = make_tuple(v.Position.x, v.Position.y, v.Position.z);
		if (mp.count(tp) > 0)
		{
			combined_mesh_.indices.push_back(mp[tp]);
		}
		else
		{
			combined_mesh_.vertices.push_back(Vertex(glm::vec3(get<0>(tp), get<1>(tp), get<2>(tp)), glm::vec3(0, 0, 0), glm::vec2(0, 0)));
			combined_mesh_.indices.push_back(combined_mesh_.vertices.size() - 1);

			mp.insert({ tp, combined_mesh_.vertices.size() - 1 });
		}
	}
}


void CGLProjectInRibbonView::OnNormalDisplay()
{
	// TODO:  在此添加命令处理程序代码
	SetupMesh(normal_mesh_);
	AdjustCameraView(normal_mesh_);
	SetDrawingMode(DRAW_NORMAL);
	SetDrawingMode(DrawType(DrawType::NORMAL_MESH));

	Invalidate();
}


void CGLProjectInRibbonView::OnSegmentationDisplay()
{
	// TODO:  在此添加命令处理程序代码
	SetupMeshList();
	AdjustCameraView(mesh_list_);
	//ClearMeshSegmentationDeleteState();
	SetDrawingMode(DRAW_MESHSEGMENTATION);

	Invalidate();
}


void CGLProjectInRibbonView::OnSimplifiedDisplay()
{
	// TODO:  在此添加命令处理程序代码
	SetupMesh(simplified_mesh_);
	AdjustCameraView(simplified_mesh_);
	SetDrawingMode(DRAW_SIMPLIFIEDMESH);

	Invalidate();
}

void CGLProjectInRibbonView::OnDeleteSimp()
{
	// TODO:  在此添加命令处理程序代码

	// TODO:  在此添加命令处理程序代码
#ifndef MIDDLE_PROCESS

	double maxSDF = DOUBLE_MIN;
	int indexMaxSDF = -1;
	for (CgalPolyhedron::Facet_iterator facet_it = polyhedron_.facets_begin();
		facet_it != polyhedron_.facets_end(); ++facet_it)
	{
		if (internal_sdf_map_[facet_it] > maxSDF)
		{
			maxSDF = internal_sdf_map_[facet_it];
			indexMaxSDF = internal_segment_map_[facet_it];
		}
	}

	//TO DO : mesh simplification
	//adjacent graph
	mesh_graph_ = vector<vector<int>>(number_of_segments_, vector<int>(number_of_segments_, 0));
	index_max_SDF_ = indexMaxSDF;

	for (CgalPolyhedron::Halfedge_iterator halfedge_it = polyhedron_.halfedges_begin();
		halfedge_it != polyhedron_.halfedges_end(); ++halfedge_it)
	{
		int indexMesh = internal_segment_map_[halfedge_it->facet()];
		int indexOppositeMesh = internal_segment_map_[halfedge_it->opposite()->facet()];
		if (indexMesh == indexOppositeMesh) continue;
		mesh_graph_[indexMesh][indexOppositeMesh] = 1;
	}

#endif // MIDDLE_PROCESS
	//construct tree
	ConstructTree(index_max_SDF_);
	SimplifyWithDelete();

	//m_MeshList[0].SetDelete();

	CombineMesh();

#pragma region Hole Filling

	CgalPolyhedron tmpPolyhedron;
	ConvertFromMeshToCgalPolyhedron(combined_mesh_, tmpPolyhedron);

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

	//Extract mesh from simplified polyhedron
	simplified_mesh_.vertices.clear();
	simplified_mesh_.indices.clear();
	simplified_mesh_.textures.clear();

	//Simplified Flat Mesh
	for (CgalPolyhedron::Facet_iterator facet_it = tmpPolyhedron.facets_begin();
		facet_it != tmpPolyhedron.facets_end(); ++facet_it)
	{
		int verticeIndex = simplified_mesh_.vertices.size();
		int indiceIndex = simplified_mesh_.indices.size();
		Halfedge_around_facet_circulator hec = facet_it->facet_begin();// get half edge handle
		Halfedge_handle he_begin = hec;
		Vertex_handle v;
		vector<glm::vec3> points;//points for calculate normals
		do
		{
			v = hec->vertex();
			Point p = v->point();
			simplified_mesh_.vertices.push_back(Vertex(glm::vec3(p.x(), p.y(), p.z()), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0)));
			simplified_mesh_.indices.push_back(indiceIndex);
			points.push_back(glm::vec3(p.x(), p.y(), p.z()));
			++indiceIndex;
			++hec;
		} while (he_begin != hec);
		//calculate normals
		//assert(points.size() == 3);
		glm::vec3 v1 = points[1] - points[0];
		glm::vec3 v2 = points[2] - points[1];
		glm::vec3 vNormal = glm::cross(v1, v2);
		int lastIndex = simplified_mesh_.vertices.size() - 1;
		simplified_mesh_.vertices[lastIndex].Normal = vNormal;
		simplified_mesh_.vertices[lastIndex - 1].Normal = vNormal;
		simplified_mesh_.vertices[lastIndex - 2].Normal = vNormal;
	}
#pragma endregion Hole Filling

	OnSimplifiedDisplay();
}


void CGLProjectInRibbonView::OnSerializeWrite()
{
	// TODO:  在此添加命令处理程序代码

	//assert(!m_MeshList.empty());
	string outFileName = "model.mseg";
	ofstream ofs(outFileName, std::ios::out);

	for (unsigned int i = 0; i < mesh_list_.size(); ++i)
	{
		ofs << "m" << " " << mesh_list_[i].color.R << " " << mesh_list_[i].color.G << " " << mesh_list_[i].color.B << endl;
		ofs << mesh_list_[i].faceNum << endl;
		ofs << mesh_list_[i].sdf << endl;
		for (const auto & v : mesh_list_[i].vertices)
		{
			ofs << "v" << " "
				<< v.Position.x << " " << v.Position.y << " " << v.Position.z << " "
				<< v.Normal.x << " " << v.Normal.y << " " << v.Normal.z << endl;
		}
		for (unsigned int j = 0; j < mesh_list_[i].indices.size(); j += 3)
		{
			ofs << "f" << " " << mesh_list_[i].indices[j] << " " << mesh_list_[i].indices[j + 1] << " "<< mesh_list_[i].indices[j + 2] << endl;
		}
		ofs << "#" << endl;
	}

	outFileName = "model.adjg";
	ofstream ofsMeshGraph(outFileName, std::ios::out);

	ofsMeshGraph << number_of_segments_ << endl;
	for (unsigned int i = 0; i < mesh_graph_.size(); ++i)
	{
		for (unsigned int j = 0; j < mesh_graph_[i].size(); ++j)
		{
			ofsMeshGraph << mesh_graph_[i][j] << " ";
		}
		ofsMeshGraph << endl;
	}
	ofsMeshGraph << index_max_SDF_;
}

void CGLProjectInRibbonView::OnSerializeRead()
{
	// TODO:  在此添加命令处理程序代码
	mesh_list_.clear();

	infile_name_ = model_name_ + ".mseg";
	ifstream ifs(infile_name_, std::ios::in);
	//assert(ifs.is_open());

	mesh_list_.clear();

	string token;
	Mesh newMesh;
	Vertex newVertex;
	int indice1, indice2, indice3;
	while (!ifs.eof())
	{
		token.clear();
		ifs >> token;
		if (token == "m")
		{
			ifs >> newMesh.color.R >> newMesh.color.G >> newMesh.color.B;
			ifs >> newMesh.faceNum >> newMesh.sdf;
		}
		else if (token == "v")
		{			
			ifs >> newVertex.Position.x >> newVertex.Position.y >> newVertex.Position.z;
			ifs >> newVertex.Normal.x >> newVertex.Normal.y >> newVertex.Normal.z;
			newMesh.vertices.push_back(newVertex);
		}
		else if (token == "f")
		{
			ifs >> indice1 >> indice2 >> indice3;
			newMesh.indices.push_back(indice1);
			newMesh.indices.push_back(indice2);
			newMesh.indices.push_back(indice3);
		}
		else if (token == "#")
		{
			mesh_list_.push_back(newMesh);
			newMesh.vertices.clear();
			newMesh.indices.clear();
			newMesh.textures.clear();
		}
	}

	infile_name_ = model_name_ + ".adjg";
	ifstream ifsMeshGraph(infile_name_, std::ios::in);

	mesh_graph_.clear();
	ifsMeshGraph >> number_of_segments_;
	vector<int> line;
	int number;
	for (unsigned int i = 0; i < number_of_segments_; ++i)
	{
		line.clear();
		for (unsigned int j = 0; j < number_of_segments_; ++j)
		{
			ifsMeshGraph >> number;
			line.push_back(number);
		}
		mesh_graph_.push_back(line);
	}
	ifsMeshGraph >> index_max_SDF_;
}


void CGLProjectInRibbonView::OnQuadricSimp()
{
	// TODO:  在此添加命令处理程序代码
	//construct tree
	ConstructTree(index_max_SDF_);
	SimplifyWithDelete();

	vector<int> deletedMeshes;
	for (unsigned int i = 0; i < mesh_list_.size(); ++i)
	{
		if (mesh_list_[i].isDeleted())
		{
			deletedMeshes.push_back(i);
		}
	}

	mesh_list_[deletedMeshes[0]].SetDelete();

	int deletedMeshIndex = deletedMeshes[0];
	
	CombineTheMesh(deletedMeshIndex);

	Surface_mesh surface_mesh;
	ConvertFromMeshToSurfaceMeshPolyhedron(combined_mesh_, surface_mesh);

	// map used to check that constrained_edges and the points of its vertices
	// are preserved at the end of the simplification
	std::map<Surface_mesh::Halfedge_handle, std::pair<Point_3, Point_3> >constrained_edges;
	std::size_t nb_border_edges = 0;
	for (Surface_mesh::Halfedge_iterator hit = surface_mesh.halfedges_begin(),
		hit_end = surface_mesh.halfedges_end();
		hit != hit_end; ++hit)
	{
		if (hit->is_border()){
			constrained_edges[hit] = std::make_pair(hit->opposite()->vertex()->point(),
				hit->vertex()->point());
			++nb_border_edges;
		}
	}
	// Contract the surface mesh as much as possible
	sms::Count_stop_predicate<Surface_mesh> stop(surface_mesh.size_of_halfedges() / 4);
	Border_is_constrained_edge_map bem(surface_mesh);
	// This the actual call to the simplification algorithm.
	// The surface mesh and stop conditions are mandatory arguments.
	// The index maps are needed because the vertices and edges
	// of this surface mesh lack an "id()" field.
	int r = sms::edge_collapse
		(surface_mesh
		, stop
		, CGAL::parameters::vertex_index_map(get(CGAL::vertex_external_index, surface_mesh))
		.halfedge_index_map(get(CGAL::halfedge_external_index, surface_mesh))
		.edge_is_constrained_map(bem)
		.get_placement(Placement(bem))
		);
	std::cout << "\nFinished...\n" << r << " edges removed.\n"
		<< (surface_mesh.size_of_halfedges()) << " final edges.\n";
	std::ofstream os("SimpOut.off"); os << surface_mesh;
	// now check!
	for (Surface_mesh::Halfedge_iterator hit = surface_mesh.halfedges_begin(),
		hit_end = surface_mesh.halfedges_end();
		hit != hit_end; ++hit)
	{
		if (hit->is_border()){
			--nb_border_edges;
			/*assert(constrained_edges[hit] ==
				std::make_pair(hit->opposite()->vertex()->point(),
				hit->vertex()->point()));*/
		}
	}
	//assert(nb_border_edges == 0);

	//Update Mesh List

	mesh_list_[deletedMeshIndex].vertices.clear();
	mesh_list_[deletedMeshIndex].indices.clear();

	for (Surface_mesh::Facet_iterator facet_it = surface_mesh.facets_begin();
		facet_it != surface_mesh.facets_end(); ++facet_it)
	{
		int verticeIndex = mesh_list_[deletedMeshIndex].vertices.size();
		int indiceIndex = mesh_list_[deletedMeshIndex].indices.size();
		Surface_mesh::Halfedge_around_facet_circulator hec = facet_it->facet_begin();// get half edge handle
		Surface_mesh::Halfedge_handle he_begin = hec;
		Surface_mesh::Vertex_handle v;
		vector<glm::vec3> points;//points for calculate normals
		do
		{
			v = hec->vertex();
			Surface_mesh::Point p = v->point();
			mesh_list_[deletedMeshIndex].vertices.push_back(Vertex(glm::vec3(p.x(), p.y(), p.z()), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0)));
			mesh_list_[deletedMeshIndex].indices.push_back(indiceIndex);
			points.push_back(glm::vec3(p.x(), p.y(), p.z()));
			++indiceIndex;
			++hec;
		} while (he_begin != hec);
		//calculate normals
		//assert(points.size() == 3);
		glm::vec3 v1 = points[1] - points[0];
		glm::vec3 v2 = points[2] - points[1];
		glm::vec3 vNormal = glm::cross(v1, v2);
		int lastIndex = mesh_list_[deletedMeshIndex].vertices.size() - 1;
		mesh_list_[deletedMeshIndex].vertices[lastIndex].Normal = vNormal;
		mesh_list_[deletedMeshIndex].vertices[lastIndex - 1].Normal = vNormal;
		mesh_list_[deletedMeshIndex].vertices[lastIndex - 2].Normal = vNormal;
	}
	
	mesh_list_[deletedMeshIndex].SetExist();

	OnSegmentationDisplay();
}

void CGLProjectInRibbonView::SimplifyFinalMethod(int targetFaceNum, double sdfThreshold)
{
	//assert(m_MeshTree != nullptr);

	while (true)
	{
		//set tag isDeleted = true if sdf is less than the threshold
		DeleteTreeNodeWithLessSDF(sdfThreshold);

		//Update tree and each node's faceNum according to the tag
		UpdateTreeNode();

		if (mesh_tree_->faceNum < targetFaceNum) return;

		if (mesh_list_[mesh_tree_->indexMesh].faceNum > targetFaceNum)
		{
			//reserve main body
			/*for (int i = 0; i < m_MeshTree->childNodes.size(); ++i)
			{
			HelperDeleteTree(m_MeshTree->childNodes[i]);
			}
			m_MeshTree->childNodes.clear();
			return;		*/

			sort(mesh_tree_->childNodes.begin(), mesh_tree_->childNodes.end(),
				[](TreeNode* &lhs, TreeNode* &rhs) { return lhs->faceNum / lhs->avgSDF > rhs->faceNum / rhs->avgSDF; });

			for (unsigned int i = 1; i < mesh_tree_->childNodes.size(); ++i)
			{
				mesh_tree_->childNodes[i]->isDeleted = true;
			}

			UpdateTreeNode();

			mesh_tree_->singleFaceNum = 300;
			mesh_tree_->childNodes[0]->singleFaceNum --;

			return;
		}

		if (SimplifyInFinalMethod(mesh_tree_, mesh_tree_->faceNum, targetFaceNum)) {
			UpdateTreeNode();
			return;
		}
	}

}

bool CGLProjectInRibbonView::SimplifyInFinalMethod(TreeNode* pTreeNode, int& restFaceNum, int& targetNum)
{
	if (pTreeNode == nullptr) return false;

	sort(pTreeNode->childNodes.begin(), pTreeNode->childNodes.end(),
		[](TreeNode* &lhs, TreeNode* &rhs) { return lhs->faceNum / lhs->avgSDF > rhs->faceNum / rhs->avgSDF; });

	for (unsigned int i = 0; i < pTreeNode->childNodes.size(); ++i)
	{
		if (isLeaf(pTreeNode->childNodes[i]))
		{		
			pTreeNode->childNodes[i]->faceNum -= pTreeNode->childNodes[i]->singleFaceNum / 2;
			restFaceNum -= pTreeNode->childNodes[i]->singleFaceNum / 2;
			pTreeNode->childNodes[i]->singleFaceNum -= (pTreeNode->childNodes[i]->singleFaceNum / 2);
			pTreeNode->childNodes[i]->avgSDF /= 2;
			pTreeNode->childNodes[i]->singleSDF /= 2;
			if (restFaceNum < targetNum) return true;
		}
		else
		{
			if (isJoint(pTreeNode->childNodes[i]))
			{
				restFaceNum -= pTreeNode->childNodes[i]->singleFaceNum / 9;
				pTreeNode->childNodes[i]->faceNum -= pTreeNode->childNodes[i]->singleFaceNum / 9;
				pTreeNode->childNodes[i]->singleFaceNum -= (pTreeNode->childNodes[i]->singleFaceNum / 9);
				pTreeNode->childNodes[i]->avgSDF = pTreeNode->childNodes[i]->avgSDF * 8 / 9;
				pTreeNode->childNodes[i]->singleSDF = pTreeNode->childNodes[i]->singleSDF * 8 / 9;
				if (restFaceNum < targetNum) return true;
			}

			if (SimplifyInFinalMethod(pTreeNode->childNodes[i], restFaceNum, targetNum))
			{
				return true;
			}
		}
	}

	return false;
}

bool CGLProjectInRibbonView::isLeaf(TreeNode* pTreeNode)
{
	return pTreeNode != nullptr && pTreeNode->childNodes.empty();
}

bool CGLProjectInRibbonView::isJoint(TreeNode* pTreeNode)
{
	if (pTreeNode == nullptr || pTreeNode->childNodes.empty()) return false;

	double rootV = pTreeNode->faceNum / pTreeNode->avgSDF;

	for (unsigned int i = 0; i < pTreeNode->childNodes.size(); ++i)
	{
		if (pTreeNode->childNodes[i]->faceNum / pTreeNode->childNodes[i]->avgSDF * 2 < rootV)
		{
			return true;
		}
	}

	return false;
}

void CGLProjectInRibbonView::DeleteTreeNode(TreeNode* pTreeNode)
{
	if (pTreeNode == nullptr) return;

	mesh_list_[pTreeNode->indexMesh].SetDelete();
	if (!pTreeNode->childNodes.empty())
	{
		for (unsigned int i = 0; i < pTreeNode->childNodes.size(); ++i)
		{
			DeleteTreeNode(pTreeNode->childNodes[i]);
		}
	}
}

void CGLProjectInRibbonView::DeleteTreeNodeWithLessSDF(double sdfThreshold)
{
	HelperDeleteTreeNodeWithLessSDF(mesh_tree_, sdfThreshold);
}

void CGLProjectInRibbonView::HelperDeleteTreeNodeWithLessSDF(TreeNode* pTreeNode, double sdfThreshold)
{
	if (pTreeNode == nullptr) return;

	if (pTreeNode->childNodes.empty())
	{
		if (pTreeNode->singleSDF < sdfThreshold)
		{
			pTreeNode->isDeleted = true;
		}
	}
	else
	{
		for (unsigned int i = 0; i < pTreeNode->childNodes.size(); ++i)
		{
			HelperDeleteTreeNodeWithLessSDF(pTreeNode->childNodes[i], sdfThreshold);
		}
	}
}

void CGLProjectInRibbonView::UpdateTreeNode()
{
	HelperUpdateTreeNode(mesh_tree_);
	UpdateTreeNodeFaceNum(mesh_tree_);
	UpdateTreeNodeSDF(mesh_tree_);
}

void CGLProjectInRibbonView::UpdateTreeNodeFaceNum(TreeNode* pTreeNode)
{
	if (pTreeNode == nullptr || pTreeNode->childNodes.empty()) return;
	
	pTreeNode->faceNum = HelperUpdateTreeNodeFaceNum(pTreeNode);
}

int CGLProjectInRibbonView::HelperUpdateTreeNodeFaceNum(TreeNode* pTreeNode)
{
	if (pTreeNode == nullptr) return 0;
	if (pTreeNode->childNodes.empty()) return pTreeNode->faceNum;

	int faceSum = pTreeNode->singleFaceNum;
	for (unsigned int i = 0; i < pTreeNode->childNodes.size(); ++i)
	{
		faceSum += HelperUpdateTreeNodeFaceNum(pTreeNode->childNodes[i]);
	}
	pTreeNode->faceNum = faceSum;
	return pTreeNode->faceNum;
}

void CGLProjectInRibbonView::UpdateTreeNodeSDF(TreeNode* pTreeNode)
{
	if (pTreeNode == nullptr || pTreeNode->childNodes.empty()) return;

	pTreeNode->avgSDF = HelperUpdateTreeNodeSDF(pTreeNode);
}

double CGLProjectInRibbonView::HelperUpdateTreeNodeSDF(TreeNode* pTreeNode)
{
	if (pTreeNode == nullptr) return 0.0f;
	if (pTreeNode->childNodes.empty()) return pTreeNode->avgSDF;

	double sdf = pTreeNode->singleSDF;
	for (unsigned int i = 0; i < pTreeNode->childNodes.size(); ++i)
	{
		sdf += HelperUpdateTreeNodeSDF(pTreeNode->childNodes[i]);
	}
	pTreeNode->avgSDF = sdf;
	return pTreeNode->avgSDF;
}

void CGLProjectInRibbonView::HelperUpdateTreeNode(TreeNode* pTreeNode)
{
	if (pTreeNode == nullptr) return;

	if (!pTreeNode->childNodes.empty())
	{
		auto & nodes = pTreeNode->childNodes;
		int p = -1, q = -1;
		for (unsigned int i = 0; i < nodes.size(); ++i)
		{
			if (!nodes[i]->isDeleted)
			{
				++p;
				swap(nodes[p], nodes[i]);
				++q;
			}
			else
			{
				++q;
			}
		}

		auto remove_nodes = remove_if(pTreeNode->childNodes.begin(), pTreeNode->childNodes.end(), [](TreeNode* & tn) { return tn->isDeleted; });
		
		if (remove_nodes != pTreeNode->childNodes.end())
		{
			for (vector<TreeNode*>::iterator it = remove_nodes; it != pTreeNode->childNodes.end(); ++it)
			{
				HelperDeleteTree(*it);
			}
			pTreeNode->childNodes.erase(remove_nodes, pTreeNode->childNodes.end());
		}
	}

	if (!pTreeNode->childNodes.empty())
	{
		for (unsigned int i = 0; i < pTreeNode->childNodes.size(); ++i)
		{
			HelperUpdateTreeNode(pTreeNode->childNodes[i]);
		}
	}
}

void CGLProjectInRibbonView::OnFinalmethod()
{
	// TODO:  在此添加命令处理程序代码
	if (mesh_tree_ != nullptr) DeleteTree();

	ConstructTreeBFS(index_max_SDF_);

	int targetFaceNum = 0;
	float targetSDF = 0.0f;
	EstimateSimplifyTarget(targetFaceNum, targetSDF);

	//targetFaceNum = 1130;
	SimplifyFinalMethod(targetFaceNum, targetSDF);

	for (unsigned int i = 0; i < mesh_list_.size(); ++i)
	{
		mesh_list_[i].SetDelete();
	}

	BuildFinalMeshFromMeshTree(mesh_tree_);
	
	CombineMesh();

	RepairHole();

	//OnSegmentationDisplay();
	OnSimplifiedDisplay();
}

void CGLProjectInRibbonView::BuildFinalMeshFromMeshTree(TreeNode* root)
{
	if (root == nullptr) return;

	int meshIndex = root->indexMesh;

	if (root->singleFaceNum < mesh_list_[root->indexMesh].faceNum)
	{
		CombineTheMesh(meshIndex);

		Surface_mesh surface_mesh;
		ConvertFromMeshToSurfaceMeshPolyhedron(combined_mesh_, surface_mesh);

		// map used to check that constrained_edges and the points of its vertices
		// are preserved at the end of the simplification
		std::map<Surface_mesh::Halfedge_handle, std::pair<Point_3, Point_3> >constrained_edges;
		std::size_t nb_border_edges = 0;
		for (Surface_mesh::Halfedge_iterator hit = surface_mesh.halfedges_begin(),
			hit_end = surface_mesh.halfedges_end();
			hit != hit_end; ++hit)
		{
			if (hit->is_border()){
				constrained_edges[hit] = std::make_pair(hit->opposite()->vertex()->point(),
					hit->vertex()->point());
				++nb_border_edges;
			}
		}
		// Contract the surface mesh as much as possible
		sms::Count_stop_predicate<Surface_mesh> stop(root->singleFaceNum);
		Border_is_constrained_edge_map bem(surface_mesh);
		// This the actual call to the simplification algorithm.
		// The surface mesh and stop conditions are mandatory arguments.
		// The index maps are needed because the vertices and edges
		// of this surface mesh lack an "id()" field.
		int r = sms::edge_collapse
			(surface_mesh
			, stop
			, CGAL::parameters::vertex_index_map(get(CGAL::vertex_external_index, surface_mesh))
			.halfedge_index_map(get(CGAL::halfedge_external_index, surface_mesh))
			.edge_is_constrained_map(bem)
			.get_cost(sms::LindstromTurk_cost<Surface_mesh>())
			.get_placement(Placement(bem))
			);
		//std::cout << "\nFinished...\n" << r << " edges removed.\n"
		//<< (surface_mesh.size_of_halfedges()) << " final edges.\n";
		//std::ofstream os("SimpOut.off"); os << surface_mesh;
		// now check!
		for (Surface_mesh::Halfedge_iterator hit = surface_mesh.halfedges_begin(),
			hit_end = surface_mesh.halfedges_end();
			hit != hit_end; ++hit)
		{
			if (hit->is_border()){
				--nb_border_edges;
				/*assert(constrained_edges[hit] ==
					std::make_pair(hit->opposite()->vertex()->point(),
					hit->vertex()->point()));*/
			}
		}
		//assert(nb_border_edges == 0);

		//Update Mesh List

		mesh_list_[meshIndex].vertices.clear();
		mesh_list_[meshIndex].indices.clear();

		for (Surface_mesh::Facet_iterator facet_it = surface_mesh.facets_begin();
			facet_it != surface_mesh.facets_end(); ++facet_it)
		{
			int verticeIndex = mesh_list_[meshIndex].vertices.size();
			int indiceIndex = mesh_list_[meshIndex].indices.size();
			Surface_mesh::Halfedge_around_facet_circulator hec = facet_it->facet_begin();// get half edge handle
			Surface_mesh::Halfedge_handle he_begin = hec;
			Surface_mesh::Vertex_handle v;
			vector<glm::vec3> points;//points for calculate normals
			do
			{
				v = hec->vertex();
				Surface_mesh::Point p = v->point();
				mesh_list_[meshIndex].vertices.push_back(Vertex(glm::vec3(p.x(), p.y(), p.z()), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0)));
				mesh_list_[meshIndex].indices.push_back(indiceIndex);
				points.push_back(glm::vec3(p.x(), p.y(), p.z()));
				++indiceIndex;
				++hec;
			} while (he_begin != hec);
			//calculate normals
			//assert(points.size() == 3);
			glm::vec3 v1 = points[1] - points[0];
			glm::vec3 v2 = points[2] - points[1];
			glm::vec3 vNormal = glm::cross(v1, v2);
			int lastIndex = mesh_list_[meshIndex].vertices.size() - 1;
			mesh_list_[meshIndex].vertices[lastIndex].Normal = vNormal;
			mesh_list_[meshIndex].vertices[lastIndex - 1].Normal = vNormal;
			mesh_list_[meshIndex].vertices[lastIndex - 2].Normal = vNormal;
		}
	}

	mesh_list_[meshIndex].SetExist();

	if (!root->childNodes.empty())
	{
		for (unsigned int i = 0; i < root->childNodes.size(); ++i)
		{
			BuildFinalMeshFromMeshTree(root->childNodes[i]);
		}
	}
}

int CGLProjectInRibbonView::TreeNodesSum(TreeNode* tn)
{
	if (tn == nullptr) return -1;
	if (tn->childNodes.empty()) return 1;
	
	int sum = 1;
	for (unsigned int i = 0; i < tn->childNodes.size(); ++i)
	{
		sum += TreeNodesSum(tn->childNodes[i]);
	}
	return sum;
}

void CGLProjectInRibbonView::EstimateSimplifyTarget(int &targetFaceNum, float &targetSDF)
{
	int meshListFaceNum = 0;
	double minSdf = 10000.0f;
	for (unsigned int i = 0; i < mesh_list_.size(); ++i)
	{
		meshListFaceNum += mesh_list_[i].faceNum;
		minSdf = min(minSdf, mesh_list_[i].sdf);
	}
	targetFaceNum = meshListFaceNum / 4;
	targetSDF = static_cast<float>(minSdf) + 0.00001f;
}

void CGLProjectInRibbonView::RepairHole()
{
	CgalPolyhedron tmpPolyhedron;
	ConvertFromMeshToCgalPolyhedron(combined_mesh_, tmpPolyhedron);

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

	//Extract mesh from simplified polyhedron
	simplified_mesh_.vertices.clear();
	simplified_mesh_.indices.clear();
	simplified_mesh_.textures.clear();

	//Simplified Flat Mesh
	for (CgalPolyhedron::Facet_iterator facet_it = tmpPolyhedron.facets_begin();
		facet_it != tmpPolyhedron.facets_end(); ++facet_it)
	{
		int verticeIndex = simplified_mesh_.vertices.size();
		int indiceIndex = simplified_mesh_.indices.size();
		Halfedge_around_facet_circulator hec = facet_it->facet_begin();// get half edge handle
		Halfedge_handle he_begin = hec;
		Vertex_handle v;
		vector<glm::vec3> points;//points for calculate normals
		do
		{
			v = hec->vertex();
			Point p = v->point();
			simplified_mesh_.vertices.push_back(Vertex(glm::vec3(p.x(), p.y(), p.z()), glm::vec3(0.0, 0.0, 0.0), glm::vec2(0.0, 0.0)));
			simplified_mesh_.indices.push_back(indiceIndex);
			points.push_back(glm::vec3(p.x(), p.y(), p.z()));
			++indiceIndex;
			++hec;
		} while (he_begin != hec);
		//calculate normals
		//assert(points.size() == 3);
		glm::vec3 v1 = points[1] - points[0];
		glm::vec3 v2 = points[2] - points[1];
		glm::vec3 vNormal = glm::cross(v1, v2);
		int lastIndex = simplified_mesh_.vertices.size() - 1;
		simplified_mesh_.vertices[lastIndex].Normal = vNormal;
		simplified_mesh_.vertices[lastIndex - 1].Normal = vNormal;
		simplified_mesh_.vertices[lastIndex - 2].Normal = vNormal;
	}
}

void CGLProjectInRibbonView::RegionSpread(MyMesh & mesh, 
	OpenMesh::EPropHandleT<int>& edgeStatus, 
	MyMesh::FaceHandle &fh, 
	std::map<MyMesh::FaceHandle, bool> &mp)
{
	if (mp.count(fh) > 0) return;

	mp.insert(std::make_pair(fh, true));

	MyMesh::FaceHalfedgeIter fhe_it = mesh.fh_iter(fh);

	while (fhe_it.is_valid())
	{
		MyMesh::HalfedgeHandle heh = *fhe_it;
		MyMesh::EdgeHandle eh = mesh.edge_handle(heh);
		if (mesh.property(edgeStatus, eh) == DELETING)
		{
			mesh.property(edgeStatus, eh) = DELETED;
			MyMesh::FaceHandle fhNew = mesh.opposite_face_handle(heh);
			RegionSpread(mesh, edgeStatus, fhNew, mp);
		}
		++fhe_it;
	}
}

void CGLProjectInRibbonView::GraphBFS(std::map < BorderPoint, GraphNode, BorderPoint>& mp,
								   BorderPoint & startPoint, 
								   ConnectRegion & newRegion)
{
	if (mp[startPoint].isVisited) return;

	mp[startPoint].isVisited = true;
	newRegion.points.push_back(startPoint);

	for (const auto & i : mp[startPoint].toPoints)
	{
		newRegion.blss.insert(i.second);
	}

	for (auto & adjPoint : mp[startPoint].toPoints)
	{
		GraphBFS(mp, adjPoint.first, newRegion);
	}
}

void CGLProjectInRibbonView::DeletedRegionAnalysis(std::vector<ConnectRegion> & cr,
											  std::map < BorderPoint, GraphNode, BorderPoint>& mp, 
											  std::vector<BorderLineSegment> &borderLineSegments)
{
	for (auto & r : cr)
	{
		float meanLength = 0.0f;
		int n = 0;
		for (auto iter = r.blss.begin(); iter != r.blss.end(); ++iter, ++n)
		{
			meanLength += borderLineSegments[*iter].length;
		}
		meanLength /= n;
		r.meanLength = meanLength;
	}

	sort(cr.begin(), cr.end(), [](const ConnectRegion& lhs, const ConnectRegion& rhs){
		return lhs.meanLength < rhs.meanLength;
	});

	cr[0].isDeleted = true;
	cr[1].isDeleted = true;
}

void CGLProjectInRibbonView::DeleteMyMesh(MyMesh & mesh,
									  std::vector<ConnectRegion> & cr,
								      std::vector<BorderLineSegment> &borderLineSegments,
								      std::vector<Region>& regionPs)
{
	result_mesh_ = mesh;

	for (const auto & crAuto : cr)
	{
		if (crAuto.isDeleted)
		{
			for (const auto & blssAuto : crAuto.blss)
			{
				borderLineSegments[blssAuto].needsDeleted = true;
			}
		}
	}

	for (auto & rPsAuto : regionPs)
	{
		bool isDeleted = false;
		auto iter = rPsAuto.blss.begin();
		for (; iter != rPsAuto.blss.end(); ++iter)
		{
			if (!borderLineSegments[*iter].needsDeleted) break;
		}
		if (iter == rPsAuto.blss.end()) isDeleted = true;
		rPsAuto.isDeleted = isDeleted;
	}

	result_mesh_.request_face_status();
	result_mesh_.request_edge_status();
	result_mesh_.request_vertex_status();

	for (unsigned int i = 0; i < regionPs.size(); ++i)
	{
		if (regionPs[i].isDeleted)
		{
			for (auto & fh : regionPs[i].faces)
				result_mesh_.delete_face(fh, true);
		}
	}
	result_mesh_.garbage_collection();
}

void CGLProjectInRibbonView::CombineConnectRegions()
{
	assert(!connectBorderLines_.empty());

	unordered_map<int, int> count_map; // regionId -> count
	for (unsigned int i = 0; i < connectBorderLines_.size(); ++i)
	{
		count_map.clear();
		for (auto & blsId : connectBorderLines_[i].blss)
		{
			for (auto & regionID : border_line_segments_[blsId].RegionIDs)
			{
				if (count_map.count(regionID) > 0)
				{
					++count_map[regionID];
				}
				else
				{
					count_map[regionID] = 1;
				}
			}
		}
		for (const auto & count : count_map)
		{
			if (count.second > 1) connectBorderLines_[i].regionIDs.insert(count.first);
		}
	}
}

void CGLProjectInRibbonView::ContourLineBasedMethod()
{
	OpenMesh::IO::Options opt;
	//add property
	//normal property
	if (!opt.check(OpenMesh::IO::Options::FaceNormal))
	{
		mesh_.request_face_normals();
		mesh_.update_normals();
	}

	//dihedral angle
	OpenMesh::EPropHandleT<float> dihedralAngle;
	mesh_.add_property(dihedralAngle);

	//Edge status 
	OpenMesh::EPropHandleT<int> status;
	mesh_.add_property(status);

	OpenMesh::EPropHandleT<std::set<int>> regionIDs;
	mesh_.add_property(regionIDs);

	//平面表面区域边界提取及分割
	//计算每一条边相邻面的二面角
	DeletingEdgeArray deletingEdges;
	UnDeterminedEdgeArray undeterminedEdges;

	float fi = 0.1f;
	for (MyMesh::EdgeIter e_it = mesh_.edges_begin();
		e_it != mesh_.edges_end(); ++e_it)
	{
		float f = mesh_.calc_dihedral_angle_fast(*e_it);
		float degree = glm::degrees(f);
		mesh_.property(dihedralAngle, *e_it) = degree;
		if (std::abs(degree) < fi)
		{
			mesh_.property(status, *e_it) = DELETING;
			deletingEdges.push_back(std::make_pair(*e_it, true));
		}
		else
		{
			mesh_.property(status, *e_it) = UNDETERMINED;
			undeterminedEdges.push_back(std::make_pair(*e_it, true));
		}
	}

	//生成平面区域
	for (unsigned int i = 0; i < deletingEdges.size(); ++i)
	{
		std::map<MyMesh::FaceHandle, bool> mp;
		if (mesh_.property(status, deletingEdges[i].first) == DELETING) //边状态为待删除
		{
			mp.clear();
			Region newRegion;
			auto adjacentfh = mesh_.face_handle(mesh_.halfedge_handle(deletingEdges[i].first, 0));
			RegionSpread(mesh_, status, adjacentfh, mp);
			for (const auto & i : mp)
			{
				newRegion.faces.push_back(i.first);
			}
			regionPs_.push_back(newRegion);
		}
	}

	int borderSum = 0;
	for (unsigned int i = 0; i < regionPs_.size(); ++i)
	{
		for (unsigned int j = 0; j < regionPs_[i].faces.size(); ++j)
		{
			MyMesh::FaceEdgeIter fe_iter = mesh_.fe_iter(regionPs_[i].faces[j]);
			while (fe_iter.is_valid())
			{
				if (mesh_.property(status, *fe_iter) == UNDETERMINED)
				{
					mesh_.property(status, *fe_iter) = BORDER;
					mesh_.property(regionIDs, *fe_iter).insert(i);
					++borderSum;
				}
				else if (mesh_.property(status, *fe_iter) == BORDER)
				{
					mesh_.property(regionIDs, *fe_iter).insert(i);
				}

				++fe_iter;
			}
		}
	}

	cout << "Generated " << regionPs_.size() << " " << "regions..." << endl;
	cout << "Generated " << borderSum << " " << "borders..." << endl;

	for (unsigned int i = 0; i < regionPs_.size(); ++i)
	{
		for (unsigned int j = 0; j < regionPs_[i].faces.size(); ++j)
		{
			MyMesh::FaceEdgeIter fe_iter = mesh_.fe_iter(regionPs_[i].faces[j]);
			while (fe_iter.is_valid())
			{
				if (mesh_.property(status, *fe_iter) == BORDER)
				{
					MyMesh::Point pA =
						mesh_.point(mesh_.from_vertex_handle(mesh_.halfedge_handle(*fe_iter, 0)));
					MyMesh::Point pB =
						mesh_.point(mesh_.to_vertex_handle(mesh_.halfedge_handle(*fe_iter, 0)));

					BorderLineSegment bls(BorderLineSegment(
						PointNA(pA[0], pA[1], pA[2]),
						PointNA(pB[0], pB[1], pB[2])));

					for (const auto & i : mesh_.property(regionIDs, *fe_iter))
					{
						bls.RegionIDs.insert(i);
					}

					border_line_segments_.push_back(bls);
					mesh_.property(status, *fe_iter) = BORDER_CALCED;
				}

				++fe_iter;
			}
		}
	}

	cout << "Border line segments : " << endl;
	for (unsigned int i = 0; i < border_line_segments_.size(); ++i)
	{
		const auto & bls = border_line_segments_[i];
		cout << "(" << bls.A.x << ", " << bls.A.y << ", " << bls.A.z << ")"
			<< " --> "
			<< "(" << bls.B.x << ", " << bls.B.y << ", " << bls.B.z << ")" << endl;
	}

	cout << "Extracting " << border_line_segments_.size() << " border line segments..." << endl;

	std::map < BorderPoint, GraphNode, BorderPoint> borderPointAdjGraph;
	for (unsigned int i = 0; i < border_line_segments_.size(); ++i)
	{
		borderPointAdjGraph[border_line_segments_[i].A].toPoints.push_back({ border_line_segments_[i].B, i });
		borderPointAdjGraph[border_line_segments_[i].B].toPoints.push_back({ border_line_segments_[i].A, i });

		for (const auto & j : border_line_segments_[i].RegionIDs)
		{
			regionPs_[j].blss.insert(i);
		}
	}

	for (auto & bp : borderPointAdjGraph)
	{
		if (bp.second.isVisited) continue;
		ConnectRegion newRegion;
		BorderPoint startPoint(bp.first);
		GraphBFS(borderPointAdjGraph, startPoint, newRegion);
		connectBorderLines_.push_back(newRegion);
	}

	cout << "Generated " << connectBorderLines_.size() << " maximum connected regions..." << endl;
	CombineConnectRegions();
	cout << "Combine connect regions completed ..." << endl;

	DeletedRegionAnalysis(connectBorderLines_, borderPointAdjGraph, border_line_segments_);

	cout << "Deleted 2 connect regions..." << endl;

	DeleteMyMesh(mesh_, connectBorderLines_, border_line_segments_, regionPs_);

	cout << "Mesh information after simplified : " 
	<< "vertices : " << result_mesh_.n_vertices() << " "
	<< "faces : " << result_mesh_.n_faces() << endl;

	if (!OpenMesh::IO::write_mesh(result_mesh_, "result.off"))
	{
		cout << "Writing to result.off failed..." << endl;
	}

	cout << "Writing to result.off completed..." << endl;

	//delete faces edges vertices
	/*mesh.request_face_status();
	mesh.request_edge_status();
	mesh.request_vertex_status();

	mesh.delete_face(fh, true);
	mesh.delete_vertex(vh, true);
	mesh.delete_edge(eh, true);

	mesh.garbage_collection();*/



	/*std::cout << "Before: " << std::endl;
	for (MyMesh::EdgeIter e_it = mesh.edges_begin();
	e_it != mesh.edges_end(); ++e_it)
	{
	std::cout << "Edge #" << *e_it << " : " << mesh.property(dihedralAngle, *e_it) << std::endl;
	}

	std::cout << "After : " << std::endl;
	for (MyMesh::EdgeIter e_it = mesh.edges_begin();
	e_it != mesh.edges_end(); ++e_it)
	{
	std::cout << "Edge #" << *e_it << " : " << mesh.property(dihedralAngle, *e_it) << std::endl;

	auto fh1 = mesh.face_handle(mesh.halfedge_handle(*e_it, 0));
	auto fh2 = mesh.opposite_face_handle(mesh.halfedge_handle(*e_it, 0));
	}*/
}

void CGLProjectInRibbonView::OnStartCLBAlgorithm()
{
	// TODO:  在此添加命令处理程序代码
	ContourLineBasedMethod();
}


void CGLProjectInRibbonView::OnGenerateBorderLine()
{
	// TODO:  在此添加命令处理程序代码
	if (border_line_segments_.empty())
	{
		MessageBox(_T("请先执行算法，产生边界边！"));
		return;
	}
	cout << "Generate border line ..." << endl;
	ifDrawBorderLine = true;

	for (unsigned int i = 0; i < border_line_segments_.size(); ++i)
	{
		auto & bls = border_line_segments_[i];
		gpu::Edge new_edge(
			gpu::Point(bls.A.x, bls.A.y, bls.A.z),
			gpu::Point(bls.B.x, bls.B.y, bls.B.z));

		new_edge.set_color(1.0f, 0.0f, 0.0f);

		border_line_segments_gpu_.push_back(new_edge);
	}
	SetDrawingMode(DrawType(DrawType::BORDER_LINE_SEGMENT));
	Invalidate();
}


void CGLProjectInRibbonView::OnGenerateConnectFaces()
{
	// TODO:  在此添加命令处理程序代码
	if (regionPs_.empty())
	{
		MessageBox(_T("请先执行算法，产生连通区域！"));
		return;
	}

	cout << "Generating connect faces ..." << endl;
	ifDrawRegions = true;

	vector<Color> colorList;
	vector<Color> excludedColor = { Color(0.0, 0.0, 0.0) };
	GenerateUniqueColorList(regionPs_.size(), colorList, excludedColor);

	MyMesh::FaceVertexIter fv_it;

	MyMesh::Point p1, p2, p3;
	gpu::Point gp1, gp2, gp3;
	glm::vec3 glm_p1, glm_p2, glm_p3;
	glm::vec3 n;
	for (unsigned int i = 0; i < regionPs_.size(); ++i)
	{
		gpu::Region new_region;
		auto & region = regionPs_[i];
		Color & color = colorList[i];
		for (unsigned int j = 0; j < region.faces.size(); ++j)
		{
			auto & face = region.faces[j];
			fv_it = mesh_.fv_iter(face);
			if (!fv_it.is_valid())
			{
				cout << "Region " << i << "face " << j << "is not valid!" << endl;
				system("pause");
			}


			p1 = mesh_.point(*fv_it);
			glm_p1 = glm::vec3(p1[0], p1[1], p1[2]);
			++fv_it;
			p2 = mesh_.point(*fv_it);
			glm_p2 = glm::vec3(p2[0], p2[1], p2[2]);
			++fv_it;
			p3 = mesh_.point(*fv_it);
			glm_p3 = glm::vec3(p3[0], p3[1], p3[2]);

			n = glm::normalize(glm::cross(glm_p2 - glm_p1, glm_p3 - glm_p2));

			gp1.position = glm_p1;
			gp2.position = glm_p2;
			gp3.position = glm_p3;

			gp1.normal = n;
			gp2.normal = n;
			gp3.normal = n;

			gp1.set_color(color.R, color.G, color.B);
			gp2.set_color(color.R, color.G, color.B);
			gp3.set_color(color.R, color.G, color.B);

			new_region.points.push_back(gp1);
			new_region.points.push_back(gp2);
			new_region.points.push_back(gp3);
		}

		regionPs_gpu_.push_back(new_region);
	}

	cout << "Connect regions generate completed ..." << endl;
	InitRegionsData(regionPs_gpu_, RegionType(RegionType::CONNECT_REGION));
	cout << "Transport connect regions data to gpu ..." << endl;

	SetDrawingMode(DrawType(DrawType::REGION));
	Invalidate();
}


void CGLProjectInRibbonView::OnGenerateMaximumConnectBorderLines()
{
	// TODO:  在此添加命令处理程序代码
	cout << "Generating maximum connnect border lines ..." << endl;
	if (connectBorderLines_.empty())
	{
		MessageBox(_T("请先执行算法，产生最大连通边界！"));
		return;
	}

	ifDrawConnectBorderLines = true;

	vector<Color> colorList;
	vector<Color> excludedColor = { Color(0.0, 0.0, 0.0) };
	GenerateUniqueColorList(connectBorderLines_.size(), colorList, excludedColor);

	for (unsigned int i = 0; i < connectBorderLines_.size(); ++i)
	{
		for (auto & blsIdx : connectBorderLines_[i].blss)
		{
			auto & bls = border_line_segments_[blsIdx];
			gpu::Edge new_edge(
				gpu::Point(bls.A.x, bls.A.y, bls.A.z),
				gpu::Point(bls.B.x, bls.B.y, bls.B.z));
			auto & color = colorList[i];
			new_edge.set_color(color.R, color.G, color.B);
			connectBorderLines_gpu_.push_back(new_edge);
		}
	}
	
	SetDrawingMode(DrawType(DrawType::CONNECT_BORDER_LINE_SEGMENT));
	Invalidate();
}

void CGLProjectInRibbonView::DrawLine(float line_width, float r, float g, float b)
{
	InitLineData();

	line_shader_->Use();

	glLineWidth(line_width);
	//Setting Color

	matrix_view_ = camera_->GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(line_shader_->Program, "projection"), 1, GL_FALSE, glm::value_ptr(matrix_projection_));
	glUniformMatrix4fv(glGetUniformLocation(line_shader_->Program, "view"), 1, GL_FALSE, glm::value_ptr(matrix_view_));

	matrix_model_ = glm::mat4(1.0);
	matrix_model_ = matrix_rotation_;
	matrix_model_ = glm::scale(matrix_model_, glm::vec3(num_model_scale_));
	matrix_model_ = glm::translate(matrix_model_, glm::vec3(-focus_point_.x, -focus_point_.y, -focus_point_.z));
	glUniformMatrix4fv(glGetUniformLocation(line_shader_->Program, "model"), 1, GL_FALSE, glm::value_ptr(matrix_model_));

	glBindVertexArray(border_line_gm_.VAO);
	glDrawElements(GL_LINES, border_line_segments_gpu_.size() * 2, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void CGLProjectInRibbonView::DrawConnectBorderLine(float line_width)
{
	InitConnectBorderLine();

	line_shader_->Use();

	glLineWidth(line_width);
	//Setting Color

	matrix_view_ = camera_->GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(line_shader_->Program, "projection"), 1, GL_FALSE, glm::value_ptr(matrix_projection_));
	glUniformMatrix4fv(glGetUniformLocation(line_shader_->Program, "view"), 1, GL_FALSE, glm::value_ptr(matrix_view_));

	matrix_model_ = glm::mat4(1.0);
	matrix_model_ = matrix_rotation_;
	matrix_model_ = glm::scale(matrix_model_, glm::vec3(num_model_scale_));
	matrix_model_ = glm::translate(matrix_model_, glm::vec3(-focus_point_.x, -focus_point_.y, -focus_point_.z));
	glUniformMatrix4fv(glGetUniformLocation(line_shader_->Program, "model"), 1, GL_FALSE, glm::value_ptr(matrix_model_));

	glBindVertexArray(connectBorderLines_gm_.VAO);
	glDrawElements(GL_LINES, connectBorderLines_gpu_.size() * 2, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);

}

void CGLProjectInRibbonView::InitLineData()
{
	if (!border_line_gm_.isMeshTransportedToGPU)
	{
		auto &vao = border_line_gm_.VAO;
		auto &vbo = border_line_gm_.VBO;
		auto &ebo = border_line_gm_.EBO;

		if (!gpuindices_.empty())
		{
			gpuindices_.clear();
		}

		for (unsigned int i = 0; i < border_line_segments_gpu_.size(); ++i)
		{
			gpuindices_.push_back(2 * i);
			gpuindices_.push_back(2 * i + 1);
		}

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(vao);
		assert(!border_line_segments_gpu_.empty());
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, border_line_segments_gpu_.size() * sizeof(gpu::Edge), &border_line_segments_gpu_[0], GL_STATIC_DRAW);
		assert(!gpuindices_.empty());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, gpuindices_.size() * sizeof(GLuint), &gpuindices_[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gpu::Point), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(gpu::Point), (GLvoid*)offsetof(gpu::Point, color));
		glBindVertexArray(0);

		border_line_gm_.isMeshTransportedToGPU = true;
	}

}

void CGLProjectInRibbonView::InitConnectBorderLine()
{
	if (!connectBorderLines_gm_.isMeshTransportedToGPU)
	{
		auto &vao = connectBorderLines_gm_.VAO;
		auto &vbo = connectBorderLines_gm_.VBO;
		auto &ebo = connectBorderLines_gm_.EBO;

		if (!gpuindices_.empty())
		{
			gpuindices_.clear();
		}

		for (unsigned int i = 0; i < connectBorderLines_gpu_.size(); ++i)
		{
			gpuindices_.push_back(2 * i);
			gpuindices_.push_back(2 * i + 1);
		}

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(vao);
		assert(!connectBorderLines_gpu_.empty());
		glBindBuffer(GL_ARRAY_BUFFER, vbo);
		glBufferData(GL_ARRAY_BUFFER, connectBorderLines_gpu_.size() * sizeof(gpu::Edge), &connectBorderLines_gpu_[0], GL_STATIC_DRAW);
		assert(!gpuindices_.empty());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, gpuindices_.size() * sizeof(GLuint), &gpuindices_[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gpu::Point), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(gpu::Point), (GLvoid*)offsetof(gpu::Point, color));
		glBindVertexArray(0);

		connectBorderLines_gm_.isMeshTransportedToGPU = true;
	}

}

MeshGpuManager& CGLProjectInRibbonView::WhichRegionType(RegionType type)
{
	switch (type)
	{
	case RegionType::CONNECT_REGION: return regionPs_gm_; break;
	case RegionType::MAXIMUM_CONNECT_REGION: return connectBorderLines_gm_; break;
	default: return default_gm_;
	}
	return default_gm_;
}

void CGLProjectInRibbonView::InitRegionsData(vector<gpu::Region>& region, RegionType type)
{
	auto& gm = WhichRegionType(type);

	if (!gm.isMeshTransportedToGPU)
	{
		auto &vao = gm.VAO;
		auto &vbo = gm.VBO;
		auto &ebo = gm.EBO;

		if (!gpuindices_.empty())
		{
			gpuindices_.clear();
		}

		typedef unsigned int uint;
		uint indice = 0;
		for (uint i = 0; i < region.size(); ++i)
		{
			for (uint j = 0; j < region[i].points.size(); ++j)
			{
				gpuindices_.push_back(indice);
				++indice;
			}
		}

		gm.indiceSize = gpuindices_.size();

		if (!gpupoints_.empty())
		{
			gpupoints_.clear();
		}

		for (uint i = 0; i < region.size(); ++i)
		{
			for (uint j = 0; j < region[i].points.size(); ++j)
			{
				auto & point = region[i].points[j];
				gpupoints_.push_back(point);
			}
		}

		glGenVertexArrays(1, &vao);
		glGenBuffers(1, &vbo);
		glGenBuffers(1, &ebo);

		glBindVertexArray(vao);
		assert(!region.empty());
		glBindBuffer(GL_ARRAY_BUFFER, vbo);

		glBufferData(GL_ARRAY_BUFFER, gpuindices_.size() * sizeof(gpu::Point), &gpupoints_[0], GL_STATIC_DRAW);
		assert(!gpuindices_.empty());
		glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo);
		glBufferData(GL_ELEMENT_ARRAY_BUFFER, gpuindices_.size() * sizeof(GLuint), &gpuindices_[0], GL_STATIC_DRAW);
		glEnableVertexAttribArray(0);
		glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(gpu::Point), (GLvoid*)0);
		glEnableVertexAttribArray(1);
		glVertexAttribPointer(1, 3, GL_FLOAT, GL_FALSE, sizeof(gpu::Point), (GLvoid*)offsetof(gpu::Point, normal));
		glEnableVertexAttribArray(2);
		glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, sizeof(gpu::Point), (GLvoid*)offsetof(gpu::Point, color));
		glBindVertexArray(0);

		gm.isMeshTransportedToGPU = true;
	}
}

void CGLProjectInRibbonView::DrawRegions(MeshGpuManager& region_gm)
{
	region_shader_->Use();

	matrix_view_ = camera_->GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(region_shader_->Program, "projection"), 1, GL_FALSE, glm::value_ptr(matrix_projection_));
	glUniformMatrix4fv(glGetUniformLocation(region_shader_->Program, "view"), 1, GL_FALSE, glm::value_ptr(matrix_view_));

	matrix_model_ = glm::mat4(1.0);
	matrix_model_ = matrix_rotation_;
	matrix_model_ = glm::scale(matrix_model_, glm::vec3(num_model_scale_));
	matrix_model_ = glm::translate(matrix_model_, glm::vec3(-focus_point_.x, -focus_point_.y, -focus_point_.z));
	glUniformMatrix4fv(glGetUniformLocation(region_shader_->Program, "model"), 1, GL_FALSE, glm::value_ptr(matrix_model_));

	InitDirectionLighting(*region_shader_);
	InitMaterial(*region_shader_);
	glBindVertexArray(region_gm.VAO);
	glDrawElements(GL_TRIANGLES, region_gm.indiceSize, GL_UNSIGNED_INT, 0);
	glBindVertexArray(0);
}