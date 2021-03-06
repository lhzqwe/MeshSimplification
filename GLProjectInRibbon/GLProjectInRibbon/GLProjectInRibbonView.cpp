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
#include "HoleFiller.h"

// ////////////////////////////////////////////////////////////////
// New codes begin:

// #include “GL/GL.H” // 这不是必要的，
#include "GL/GLU.H" // 已经包含GL.h
#include "Resource.h"

#define _CRTDBG_MAP_ALLOC

using std::cout;
using std::cin;
using std::endl;

#include <time.h>

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
	ON_COMMAND(ID_DELETE_REGION, &CGLProjectInRibbonView::OnDeleteRegion)
	ON_COMMAND(ID_DELETEDMESH, &CGLProjectInRibbonView::OnDeletedMesh)
	ON_COMMAND(ID_HOLEFILLING, &CGLProjectInRibbonView::OnHoleFilling)
	ON_COMMAND(ID_LOD_NORMAL, &CGLProjectInRibbonView::OnLodNormal)
	ON_COMMAND(ID_LOD_LOD, &CGLProjectInRibbonView::OnLodLod)
	ON_WM_TIMER()
	ON_COMMAND(ID_FEATURE, &CGLProjectInRibbonView::OnFeature)
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
	case DrawType::DELETE_REGION :
		DrawRegions(regionPs_gm_);
		break;
	case DrawType::AFTER_DELETE_MESH:
		DrawModel();
		break;
	case DrawType::AFTER_REFINE_MESH:
		DrawModel();
		break;
	case DrawType::LOD_DISPLAY:
		DrawLODMode();
		break;
	case DrawType::LOD_NEW_DISPLAY:
		DrawNewLODMode();
		break;
	default:
		break;
	}

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


void CGLProjectInRibbonView::OnOpenButton()
{
	// TODO:  在此添加控件通知处理程序代码
	BOOL isOpen = TRUE;
	CString defaultDir = L"D:\\MyProject\\MyOpenGL\\OBJ";   //默认打开的文件路径  
	CString fileName = L"";         //默认打开的文件名  
	CString filter = L"文件 (*.obj)|*.obj||";   //文件过虑的类型  
	CFileDialog openFileDlg(isOpen, defaultDir, fileName, OFN_HIDEREADONLY | OFN_READONLY, filter, NULL);
	openFileDlg.GetOFN().lpstrInitialDir = L"D:\\MyProject\\MyOpenGL\\OBJ";
	//openFileDlg.DoModal();
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
	if (pixelFormat == 0)
	{
		cout << "OpenGL context inited failed ..." << endl;
		return -1;
	}

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
	HGLRC tempContext = wglCreateContext(dc.m_hDC);

	wglMakeCurrent(dc.m_hDC, tempContext);

	const GLubyte* name = glGetString(GL_VENDOR);
	const GLubyte* renderer = glGetString(GL_RENDERER);
	const GLubyte* gl_version = glGetString(GL_VERSION);
	const GLubyte* glu_version = gluGetString(GLU_VERSION);

	cout << "OpenGL manufacturer: " << name << endl;
	cout << "Renderer name: " << renderer << endl;
	cout << "OpenGL version: " << gl_version << endl;
	cout << "GLU version: " << glu_version << endl;

	int major, minor;
	glGetIntegerv(GL_MAJOR_VERSION, &major);
	glGetIntegerv(GL_MINOR_VERSION, &minor);

	int attribs[] =
	{
		WGL_CONTEXT_MAJOR_VERSION_ARB, major,
		WGL_CONTEXT_MINOR_VERSION_ARB, minor,
		WGL_CONTEXT_FLAGS_ARB, WGL_CONTEXT_FORWARD_COMPATIBLE_BIT_ARB |
		WGL_CONTEXT_DEBUG_BIT_ARB,
		WGL_CONTEXT_PROFILE_MASK_ARB, WGL_CONTEXT_CORE_PROFILE_BIT_ARB,
		0
	};
	PFNWGLCREATECONTEXTATTRIBSARBPROC wglCreateContextAttribsARB = NULL;
	wglCreateContextAttribsARB = (PFNWGLCREATECONTEXTATTRIBSARBPROC)
		wglGetProcAddress("wglCreateContextAttribsARB");
	if (wglCreateContextAttribsARB != NULL)
		m_hRC = wglCreateContextAttribsARB(dc.m_hDC, 0, attribs);
	if (!m_hRC)
		m_hRC = tempContext;
	else
	{
		wglMakeCurrent(dc.m_hDC, m_hRC);
		wglDeleteContext(tempContext);
	}

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
	shader_ = new Shader("Shader/mesh_line_mode.vert", "Shader/mesh_line_mode.frag");
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

	mesh_line_mode_shader_ = new Shader("Shader/mesh_line_mode.vert", "Shader/mesh_line_mode.frag");
	if (mesh_line_mode_shader_ == nullptr) cout << "mesh_line_mode_shader_ " << "init failed..." << endl;
	else cout << "mesh_line_mode_shader_ " << "init success ..." << endl;

	dynamic_lod_shader_ = new Shader("Shader/dynamic_lod.vert", "Shader/dynamic_lod.geom", "Shader/dynamic_lod.frag");
	if (dynamic_lod_shader_ == nullptr) cout << "dynamic_lod_shader_ " << "init failed ..." << endl;
	else cout << "dynamic_lod_shader inited success ..." << endl;

	instanced_draw_shader_ = new Shader("Shader/instanced_draw.vert", "Shader/instanced_draw.frag");
	if (instanced_draw_shader_ == nullptr) cout << "instanced_draw_shader_ init failed ..." << endl;
	else cout << "instanced_draw_shader_ init success ..." << endl;

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

	//OpenGl extensions
	/*int NumberOfExtensions;
	glGetIntegerv(GL_NUM_EXTENSIONS, &NumberOfExtensions);
	for (int i = 0; i < NumberOfExtensions; ++i)
	{
	const GLubyte *ccc = glGetStringi(GL_EXTENSIONS, i);
	if (strcmp((const char*)ccc, "GL_ARB_debug_output") == 0)
	{
	glDebugMessageCallbackARB = (PFNGLDEBUGMESSAGECALLBACKARBPROC)wglGetProcAddress("glDebugMessageCallbackARB");
	}
	}*/

	glDebugMessageCallbackARB((GLDEBUGPROCARB)DebugCallback, NULL);

	glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS_ARB);

	draw_timer_id_event = 0;

	delta_time_ = 0;
	frames_ = 0;
	frame_rate_ = 30;
	average_frame_time_milliseconds_ = 33.333;

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

	if (mesh_line_mode_shader_ != nullptr)
	{
		delete mesh_line_mode_shader_;
		mesh_line_mode_shader_ = nullptr;
	}

	if (dynamic_lod_shader_ != nullptr)
	{
		delete dynamic_lod_shader_;
		dynamic_lod_shader_ = nullptr;
	}

	if (instanced_draw_shader_ != nullptr)
	{
		delete instanced_draw_shader_;
		instanced_draw_shader_ = nullptr;
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

	if (draw_timer_id_event != 0)
	{
		KillTimer(draw_timer_id_event);
	}

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
	/*static double total_frames = 0.0f;
	static double total_time = 0.0f;
	++total_frames;
	total_time += deltaTime;
	if (total_time > DBL_MAX)
	{
	total_frames = 0.0f;
	total_time = 0.0f;
	}
	num_fps_ = total_frames / total_time;*/
	num_fps_ = 1.0 / deltaTime;
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
			v1.Barycentric = glm::vec3(1.0f, 0.0f, 0.0f);
			v2.Normal = n;
			v2.Barycentric = glm::vec3(0.0f, 1.0f, 0.0f);
			v3.Normal = n;
			v3.Barycentric = glm::vec3(0.0f, 0.0f, 1.0f);

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

	unsigned int limitsN = sizeof(slValues) / sizeof(slValues[0]);
	unsigned int colorN = sizeof(baseColors) / sizeof(baseColors[0]);

	for (i = 0; i < limitsN; ++i)
	{
		for (j = 0; j < limitsN; ++j)
		{
			for (k = 0; k < colorN; ++k)
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
	/*QueryPerformanceFrequency(&frequency_);
	QueryPerformanceCounter(&count1_);*/
	begin_frame_ = clock();
}

double CGLProjectInRibbonView::ClockToMilliseconds(clock_t ticks)
{
	return (ticks / (double)CLOCKS_PER_SEC) * 1000.0;
}

void CGLProjectInRibbonView::TimeEnd()
{
	/*QueryPerformanceCounter(&count2_);
	double frametime = (count2_.QuadPart - count1_.QuadPart) * 1000000.0 / frequency_.QuadPart;
	frametime /= 1000000.0;
	cout << frametime << endl;
	GetFPS(frametime);*/
	end_frame_ = clock();
	
	delta_time_ += end_frame_ - begin_frame_;
	++frames_;
	//cout << ClockToMilliseconds(delta_time_) << endl;

	if (frames_ == 30)
	{
		double time_seconds = ClockToMilliseconds(delta_time_) / 1000.0;
		num_fps_ = frames_ / time_seconds;
		if (num_fps_ > 150.0)
			num_fps_ = 150.0;
		frames_ = 0;
		delta_time_ = 0;
	}

	/*cout << ClockToMilliseconds(delta_time_) << endl;

	if (ClockToMilliseconds(delta_time_) > 100.0)
	{
	frame_rate_ = (double)frames_ * 0.5 + frame_rate_ * 0.5;
	frames_ = 0;
	delta_time_ = 0;
	average_frame_time_milliseconds_ = 100.0 / (frame_rate_ == 0 ? 0.01 : frame_rate_);
	num_fps_ = 1000.0f / average_frame_time_milliseconds_;
	}*/
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
	else if (drawMode == DRAW_AFTER_DELETE)
	{
		DrawMesh(after_delete_mesh_);
	}
	else if (drawMode == DRAW_AFTER_REFINE)
	{
		DrawMesh(after_refine_mesh_);
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

	while (fhe_it.is_valid() && fhe_it)
	{
		MyMesh::HalfedgeHandle heh = *fhe_it;
		MyMesh::EdgeHandle eh = mesh.edge_handle(heh);
		if (!mesh.is_valid_handle(eh))
		{
			cout << "Edge is not a valid handle ..." << endl;
			assert(false);
		}
		if (mesh.property(edgeStatus, eh) == DELETING)
		{
			mesh.property(edgeStatus, eh) = DELETED;
			if (mesh.is_boundary(eh)) continue;
			MyMesh::FaceHandle fhNew = mesh.opposite_face_handle(heh);
			if (!mesh.is_valid_handle(fhNew))
			{
				cout << "Opposite new face handle is not valid ..." << endl;
				assert(false);
			}
			RegionSpread(mesh, edgeStatus, fhNew, mp);
		}
		++fhe_it;
	}
}

void CGLProjectInRibbonView::RefineRegionSpread(MyMesh & mesh,
	OpenMesh::EPropHandleT<int>& edgeStatus,
	MyMesh::FaceHandle &fh,
	std::map<MyMesh::FaceHandle, bool> &mp,
	set<MyMesh::HalfedgeHandle> &he_handles)
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
			RefineRegionSpread(mesh, edgeStatus, fhNew, mp, he_handles);
		}
		else if (mesh.property(edgeStatus, eh) == UNDETERMINED)
		{
			he_handles.insert(heh);
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
		float max_length = 0.0f;
		int n = 0;
		for (auto iter = r.blss.begin(); iter != r.blss.end(); ++iter, ++n)
		{
			max_length = std::max(borderLineSegments[*iter].length, max_length);
		}
		r.meanLength = max_length;
	}

	/*for (auto & r : cr)
	{
	float max_length = 0.0f;
	for (auto iter = r.blss.begin(); iter != r.blss.end(); ++iter)
	{
	max_length += borderLineSegments[*iter].length;
	}
	r.meanLength = max_length;
	}*/

	sort(cr.begin(), cr.end(), [](const ConnectRegion& lhs, const ConnectRegion& rhs){
		return lhs.meanLength < rhs.meanLength;
	});

	int delete_sum = 0;

	/*float dQ = 0.995;

	for (int i = 0; i < static_cast<int>(cr.size() * dQ); ++i)
	{
		cr[i].isDeleted = true;
		cout << "Mean length : " << cr[i].meanLength << endl;
		++delete_sum;
	}*/

	cout << "Connect regions num is : " << cr.size() << endl;

	for (auto & region : cr)
	{
		if (region.meanLength < 10.0f)
		{
			region.isDeleted = true;
			++delete_sum;
		}
	}

	cout << "Deleting " << delete_sum << " regions..." << endl;
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

void CGLProjectInRibbonView::RefineIsolatedTriangles(MyMesh & mesh)
{
	assert(mesh.n_vertices() != 0);
	
	for (MyMesh::FaceIter f_it = mesh.faces_begin(); f_it != mesh.faces_end(); ++f_it)
	{
		auto fe_it = mesh.fe_iter(*f_it);

		int boundaryNum = 0;
		while (fe_it)
		{
			if (mesh.is_boundary(*fe_it))
			{
				++boundaryNum;
			}
			++fe_it;
		}
		if (boundaryNum == 2 || boundaryNum == 3)
		{
			mesh.delete_face(*f_it, true);
		}
	}
	
	mesh.garbage_collection();
}

void CGLProjectInRibbonView::DFS(int nNum, int n)
{
	if (to_temp_[nNum - 1] == from_temp_[0])
	{
		if (nNum < vNum_)
		{
			vNum_ = nNum;

			for (int i = 0; i < vNum_; ++i)
			{
				fromV_[i] = from_temp_[i];
				toV_[i] = to_temp_[i];
			}
		}
		return;
	}

	for (int i = 1; i < n; ++i)
	{
		if (!vis_[i] && to_temp_[nNum - 1] == from_vertex_[i])
		{
			from_temp_[nNum] = from_vertex_[i];
			to_temp_[nNum] = to_vertex_[i];
			if (from_temp_[nNum] == to_temp_[nNum])
			{
				cout << "Error" << endl;
			}
			vis_[i] = true;
			DFS(nNum + 1, n);
		}
	}

}

void CGLProjectInRibbonView::RepairOpenMeshHole(MyMesh & mesh)
{
	if (mesh.n_vertices() == 0)
	{
		cout << "Mesh is empty, cannot finish hole filling..." << endl;
		return;
	}

	vector<MyMesh::VertexHandle> AddedFace;
	int i, edge_num = 0;

	//获取孔洞半边集合
	for (MyMesh::HalfedgeIter it = mesh.halfedges_begin(); it != mesh.halfedges_end(); ++it)
	{
		MyMesh::HalfedgeHandle he = *it;
		if (mesh.is_boundary(he))
		{
			from_vertex_[edge_num] = mesh.from_vertex_handle(he);
			to_vertex_[edge_num] = mesh.to_vertex_handle(he);
			++edge_num;
		}
	}

	for (int i = 0; i < edge_num; ++i)
	{
		if (from_vertex_[i] == to_vertex_[i])
		{
			cout << "error" << endl;
		}
	}


	if (edge_num == 0) return;
	vis_.resize(edge_num);
	fill(vis_.begin(), vis_.end(), false);

	int num_hole = 0;

	typedef unsigned int uint;
	for (uint k = 0; k < vis_.size(); ++k)
	{
		if (vis_[k] == true) continue;

		++num_hole;

		from_temp_.clear();
		to_temp_.clear();
		fromV_.clear();
		toV_.clear();

		from_temp_[0] = from_vertex_[k];
		to_temp_[0] = to_vertex_[k];

		vis_[k] = true;
		vNum_ = 10001;
		DFS(1, edge_num);
		cout << "The total number of edges : " << edge_num << endl;
		cout << "The number in this hole is : " << vNum_ << endl;

		double sum = 0.0f;
		for (int j = 0; j < vNum_; ++j)
		{
			auto s = mesh.point(fromV_[j]);
			auto e = mesh.point(toV_[j]);
			sum += glm::length(glm::vec3(s[0] - e[0], s[1] - e[1], s[2] - e[2]));
		}
		double avg_len = sum / vNum_;

		while (vNum_ > 3)
		{
			double min_ang = 360;
			int pos = 0, nxt;
			MyMesh::VertexHandle vVertex0, vVertex1, vVertex2;
			for (int i = 0; i < vNum_; ++i)
			{
				nxt = (i + 1) % vNum_;

				auto s1 = mesh.point(fromV_[i]);
				auto e1 = mesh.point(toV_[i]);

				auto s2 = mesh.point(fromV_[nxt]);
				auto e2 = mesh.point(toV_[nxt]);

				MyMesh::Normal v1(s1.data()[0] - e1.data()[0], s1.data()[1] - e1.data()[1], s1.data()[2] - e1.data()[2]);
				MyMesh::Normal v2(e2.data()[0] - s2.data()[0], e2.data()[1] - s2.data()[1], e2.data()[2] - s2.data()[2]);

				MyMesh::HalfedgeHandle minPointHaleAge;
				for (MyMesh::HalfedgeIter itx = mesh.halfedges_begin(); itx != mesh.halfedges_end(); ++itx)
				{
					MyMesh::HalfedgeHandle tmp = *itx;
					if (mesh.from_vertex_handle(tmp) == fromV_[i] && mesh.to_vertex_handle(tmp) == toV_[i])
					{
						minPointHaleAge = tmp;
						break;
					}
				}

				double angle = mesh.calc_sector_angle(minPointHaleAge);
				if (angle < min_ang)
				{
					min_ang = angle;
					pos = i;
					vVertex0 = fromV_[i];
					vVertex1 = toV_[i];
					vVertex2 = toV_[nxt];
				}
			}

			MyMesh::Point p0 = mesh.point(vVertex0);
			MyMesh::Point p1 = mesh.point(vVertex1);
			MyMesh::Point p2 = mesh.point(vVertex2);
			double dis = glm::length(glm::vec3(p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]));

			if (dis > 2 * avg_len)
			{
				MyMesh::Point newPoint((p0[0] + p2[0]) / 2, (p0[1] + p2[1]) / 2, (p0[2] + p2[2]) / 2);
				MyMesh::VertexHandle newVertexHandle = mesh.add_vertex(newPoint);
				toV_[pos] = newVertexHandle;
				fromV_[(pos + 1) % vNum_] = newVertexHandle;

				AddedFace.clear();
				AddedFace.push_back(vVertex0);
				AddedFace.push_back(vVertex1);
				AddedFace.push_back(newVertexHandle);
				mesh.add_face(AddedFace);

				AddedFace.clear();
				AddedFace.push_back(vVertex1);
				AddedFace.push_back(vVertex2);
				AddedFace.push_back(newVertexHandle);
				mesh.add_face(AddedFace);

			}
			else
			{
				AddedFace.clear();
				AddedFace.push_back(vVertex0);
				AddedFace.push_back(vVertex1);
				AddedFace.push_back(vVertex2);
				mesh.add_face(AddedFace);

				toV_[pos] = toV_[(pos + 1) % vNum_];
				if (pos + 1 == vNum_)
				{
					pos = -1;
				}

				for (i = pos + 1; i < vNum_ - 1; ++i)
				{
					fromV_[i] = fromV_[i + 1];
					toV_[i] = toV_[i + 1];
				}

				--vNum_;
				cout << "vNum : " << vNum_ << endl;

			}
		}

		if (vNum_ <= 3)
		{
			if (vNum_ != 0)
			{
				MyMesh::VertexHandle vVertex0 = fromV_[0];
				MyMesh::VertexHandle vVertex1 = fromV_[1];
				MyMesh::VertexHandle vVertex2 = fromV_[2];

				AddedFace.clear();
				AddedFace.push_back(vVertex0);
				AddedFace.push_back(vVertex1);
				AddedFace.push_back(vVertex2);
				mesh.add_face(AddedFace);
			}
		}
	}

	cout << "Repair " << num_hole <<  " hole finished ..." << endl;
}

void CGLProjectInRibbonView::RepairOpenMeshHoleTest(MyMesh & mesh)
{
	HoleFiller<MyMesh> hf(mesh);

	hf.fill_all_holes(1);
}

void CGLProjectInRibbonView::RefineMesh(MyMesh & mesh)
{
	if (mesh.n_vertices() == 0 || mesh.n_faces() == 0)
	{
		cout << "Refine mesh is not valid ..." << endl;
		return;
	}

	OpenMesh::IO::Options opt;
	//add property
	//normal property
	if (!opt.check(OpenMesh::IO::Options::FaceNormal))
	{
		mesh.request_face_normals();
		mesh.update_normals();
	}

	//dihedral angle
	OpenMesh::EPropHandleT<float> dihedralAngle;
	mesh.add_property(dihedralAngle);

	//Edge status 
	OpenMesh::EPropHandleT<int> status;
	mesh.add_property(status);

	//平面表面区域边界提取及分割
	//计算每一条边相邻面的二面角
	DeletingEdgeArray deletingEdges;
	UnDeterminedEdgeArray undeterminedEdges;

	float fi = 10.0f;
	for (MyMesh::EdgeIter e_it = mesh.edges_begin();
		e_it != mesh.edges_end(); ++e_it)
	{
		float f = mesh.calc_dihedral_angle_fast(*e_it);
		float degree = glm::degrees(f);
		mesh.property(dihedralAngle, *e_it) = degree;
		if (std::abs(degree) < fi)
		{
			mesh.property(status, *e_it) = DELETING;
			deletingEdges.push_back(std::make_pair(*e_it, true));
		}
		else
		{
			mesh.property(status, *e_it) = UNDETERMINED;
			undeterminedEdges.push_back(std::make_pair(*e_it, true));
		}
	}

	std::map<MyMesh::FaceHandle, bool> mp;
	set<MyMesh::HalfedgeHandle> he_handles;
	//生成平面区域
	for (unsigned int i = 0; i < deletingEdges.size(); ++i)
	{	
		if (mesh.property(status, deletingEdges[i].first) == DELETING) //边状态为待删除
		{
			mp.clear();
			he_handles.clear();
			RefineRegion newRegion;
			auto adjacentfh = mesh.face_handle(mesh.halfedge_handle(deletingEdges[i].first, 0));
			RefineRegionSpread(mesh, status, adjacentfh, mp, he_handles);
			for (const auto & i : mp)
			{
				newRegion.faces.insert(i.first);
			}
			for (const auto & i : he_handles)
			{
				newRegion.boundarys.insert(i);
			}
			refine_regions_.push_back(newRegion);
		}
	}
	int num_refine_region = 0;

	sort(refine_regions_.begin(), refine_regions_.end(), [](const RefineRegion &lhs, const RefineRegion & rhs) {
		int l = (int)(lhs.faces.size()) - (int)(lhs.boundarys.size());
		int r = (int)(rhs.faces.size()) - (int)(rhs.boundarys.size());
		return l > r;
	});

	int tmp_i = -1;

	for (auto & region : refine_regions_)
	{
		++tmp_i;

		//if (tmp_i == 0 || tmp_i == 1)
		//{
		//	cout << "Delete region " << num_refine_region << "..." << endl;
		//	//RefineTheRegion(result_mesh_, region);
		//	cout << "Boundary size is : " << region.boundarys.size() << endl;
		//	DeleteTheRegion(mesh, region);
		//	++num_refine_region;
		//}
		//tmp_i == 0 || 1 || 2
		int b = static_cast<int>(region.boundarys.size());
		int f = static_cast<int>(region.faces.size());
		cout << "tmp_i : " << tmp_i << " f - b : " << f - b << endl;
		if (f - b >= 18)
		{
			cout << "Delete region " << num_refine_region << "..." << endl;
			//RefineTheRegion(result_mesh_, region);
			cout << "Boundary size is : " << region.boundarys.size() << endl;
			DeleteTheRegion(mesh, region);
			GetRefineLoopRegion(region, mesh);
			++num_refine_region;
		}
	}

	mesh.garbage_collection();
	//RepairOpenMeshHoleTest(mesh);

	cout << "Refine " << num_refine_region << " " << "regions..." << endl;
}

void CGLProjectInRibbonView::RefineTheRegion(MyMesh & mesh, RefineRegion& region)
{
	result_mesh_.request_face_status();
	result_mesh_.request_edge_status();
	result_mesh_.request_vertex_status();

	for (auto & fh : region.faces)
	{
		mesh.delete_face(fh, true);
	}

	//假定只有一条边界，且边界首尾相连
	from_vertex_.clear();
	to_vertex_.clear();
	from_temp_.clear();
	to_temp_.clear();
	fromV_.clear();
	toV_.clear();

	vector<MyMesh::VertexHandle> AddedFace;
	int i, edge_num = 0;

	for (auto & he : region.boundarys)
	{
		from_vertex_[edge_num] = mesh.from_vertex_handle(he);
		to_vertex_[edge_num] = mesh.to_vertex_handle(he);
		++edge_num;
	}

	if (edge_num == 0) return;
	vis_.resize(edge_num);
	fill(vis_.begin(), vis_.end(), false);
	
	int num_hole = 0;

	typedef unsigned int uint;
	for (uint k = 0; k < vis_.size(); ++k)
	{
		if (vis_[k] == true) continue;

		++num_hole;

		from_temp_.clear();
		to_temp_.clear();
		fromV_.clear();
		toV_.clear();

		from_temp_[0] = from_vertex_[k];
		to_temp_[0] = to_vertex_[k];

		vis_[k] = true;
		vNum_ = 10001;
		DFS(1, edge_num);

		double sum = 0.0f;
		for (int j = 0; j < vNum_; ++j)
		{
			auto s = mesh.point(fromV_[j]);
			auto e = mesh.point(toV_[j]);
			sum += glm::length(glm::vec3(s[0] - e[0], s[1] - e[1], s[2] - e[2]));
		}
		double avg_len = sum / vNum_;

		while (vNum_ > 3)
		{
			double min_ang = 360;
			int pos = 0, nxt;
			MyMesh::VertexHandle vVertex0, vVertex1, vVertex2;
			for (i = 0; i < vNum_; ++i)
			{
				nxt = (i + 1) % vNum_;

				auto s1 = mesh.point(fromV_[i]);
				auto e1 = mesh.point(toV_[i]);

				auto s2 = mesh.point(fromV_[nxt]);
				auto e2 = mesh.point(toV_[nxt]);

				MyMesh::Normal v1(s1.data()[0] - e1.data()[0], s1.data()[1] - e1.data()[1], s1.data()[2] - e1.data()[2]);
				MyMesh::Normal v2(e2.data()[0] - s2.data()[0], e2.data()[1] - s2.data()[1], e2.data()[2] - s2.data()[2]);

				MyMesh::HalfedgeHandle minPointHaleAge;
				for (MyMesh::HalfedgeIter itx = mesh.halfedges_begin(); itx != mesh.halfedges_end(); ++itx)
				{
					MyMesh::HalfedgeHandle tmp = *itx;
					if (mesh.from_vertex_handle(tmp) == fromV_[i] && mesh.to_vertex_handle(tmp) == toV_[i])
					{
						minPointHaleAge = tmp;
						break;
					}
				}

				double angle = mesh.calc_sector_angle(minPointHaleAge);
				if (angle < min_ang)
				{
					min_ang = angle;
					pos = i;
					vVertex0 = fromV_[i];
					vVertex1 = toV_[i];
					vVertex2 = toV_[nxt];
				}
			}

			MyMesh::Point p0 = mesh.point(vVertex0);
			MyMesh::Point p1 = mesh.point(vVertex1);
			MyMesh::Point p2 = mesh.point(vVertex2);
			double dis = glm::length(glm::vec3(p1[0] - p2[0], p1[1] - p2[1], p1[2] - p2[2]));

			if (dis > 2 * avg_len)
			{
				MyMesh::Point newPoint((p0[0] + p2[0]) / 2, (p0[1] + p2[1]) / 2, (p0[2] + p2[2]) / 2);
				MyMesh::VertexHandle newVertexHandle = mesh.add_vertex(newPoint);
				toV_[pos] = newVertexHandle;
				fromV_[(pos + 1) % vNum_] = newVertexHandle;

				AddedFace.clear();
				AddedFace.push_back(vVertex0);
				AddedFace.push_back(vVertex1);
				AddedFace.push_back(newVertexHandle);
				mesh.add_face(AddedFace);

				AddedFace.clear();
				AddedFace.push_back(vVertex1);
				AddedFace.push_back(vVertex2);
				AddedFace.push_back(newVertexHandle);
				mesh.add_face(AddedFace);

			}
			else
			{
				AddedFace.clear();
				AddedFace.push_back(vVertex0);
				AddedFace.push_back(vVertex1);
				AddedFace.push_back(vVertex2);
				mesh.add_face(AddedFace);

				toV_[pos] = toV_[(pos + 1) % vNum_];
				if (pos + 1 == vNum_)
				{
					pos = -1;
				}

				for (i = pos + 1; i < vNum_ - 1; ++i)
				{
					fromV_[i] = fromV_[i + 1];
					toV_[i] = toV_[i + 1];
				}

				--vNum_;

			}
		}

		if (vNum_ <= 3)
		{
			if (vNum_ != 0)
			{
				MyMesh::VertexHandle vVertex0 = fromV_[0];
				MyMesh::VertexHandle vVertex1 = fromV_[1];
				MyMesh::VertexHandle vVertex2 = fromV_[2];

				AddedFace.clear();
				AddedFace.push_back(vVertex0);
				AddedFace.push_back(vVertex1);
				AddedFace.push_back(vVertex2);
				mesh.add_face(AddedFace);
			}
		}
	}

	mesh.garbage_collection();
}

void CGLProjectInRibbonView::DeleteTheRegion(MyMesh & mesh, RefineRegion& region)
{
	for (auto & m : region.faces)
	{
		mesh.delete_face(m);
	}
}

void CGLProjectInRibbonView::GetAfterDeleteMesh(MyMesh & mesh)
{
	mesh = mesh_;

	mesh.request_face_status();
	mesh.request_edge_status();
	mesh.request_vertex_status();

	for (unsigned int i = 0; i < regionPs_.size(); ++i)
	{
		if (regionPs_[i].isDeleted)
		{
			for (auto & fh : regionPs_[i].faces)
				mesh.delete_face(fh, true);
		}
	}
	mesh.garbage_collection();
}

void CGLProjectInRibbonView::GetRefineLoopRegion(RefineRegion & region, MyMesh & mesh)
{
	refine_loop_region_.vertices.clear();
	refine_loop_region_.border_loops.clear();

	set<MyMesh::VertexHandle> vh;

	map<MyMesh::VertexHandle, MyMesh::HalfedgeHandle> v_he;

	for (const auto & heh : region.boundarys)
	{
		vh.insert(mesh.from_vertex_handle(heh));
		vh.insert(mesh.to_vertex_handle(heh));

		v_he.insert({ mesh.from_vertex_handle(heh), heh });
	}

	assert(v_he.size() == region.boundarys.size());

	typedef int idx;
	map<MyMesh::VertexHandle, idx> v_idx;

	int i = 0;
	for (const auto & v : vh)
	{
		refine_loop_region_.vertices.push_back(v);
		v_idx.insert({ v, i });
		++i;
	}


	map<MyMesh::HalfedgeHandle, bool> used_map;
	for (const auto & heh : region.boundarys)
	{
		if (used_map.count(heh) > 0) continue;

		MyMesh::VertexHandle v_from = mesh.from_vertex_handle(heh);
		MyMesh::VertexHandle v_to = mesh.to_vertex_handle(heh);

		MyMesh::VertexHandle f = v_from, t = v_to;
		MyMesh::HalfedgeHandle heh;
		
		BorderLoop newBL;
		while (t != v_from)
		{
			heh = v_he[f];
			used_map[heh] = true;
			newBL.vertices_idx.push_back(v_idx[f]);
			t = mesh.to_vertex_handle(heh);
			f = t;
		}

		refine_loop_region_.border_loops.push_back(newBL);
	}

	cout << "Refine loop region consists " << refine_loop_region_.border_loops.size() << " loops..." << endl;
	
	//Points same line check ...
	auto & bls = refine_loop_region_.border_loops;
	auto & verts = refine_loop_region_.vertices;

	bls.erase(
		remove_if(
		bls.begin(), 
		bls.end(),
		[&](const BorderLoop & bl){
		int check = false;
		auto & point1 = mesh.point(verts[bl.vertices_idx[0]]);
		auto & point2 = mesh.point(verts[bl.vertices_idx[1]]);
		float k = (point2[1] - point1[1]) / (point2[0] - point1[0]);
		for (int i = 2; i < bl.vertices_idx.size(); ++i)
		{
			auto & point = mesh.point(verts[bl.vertices_idx[i]]);
			float k_tmp = (point[1] - point1[1]) / (point[0] - point1[0]);
			if (abs(k_tmp - k) <= 0.0000001)
			{
				check = true;
				break;
			}
		}

		if (check == true)
		{
			for (auto & v_idx : bl.vertices_idx)
			{
				mesh.delete_vertex(verts[v_idx]);
			}
		}

		return check;
	}),
		bls.end());
	
	for (int i = 0; i < refine_loop_region_.border_loops.size(); ++i)
	{
		cout << "Loop " << i + 1 << " : " << endl;
		for (auto & j : refine_loop_region_.border_loops[i].vertices_idx)
		{
			cout << j << " ";
		}
		cout << endl;
	}

	//points are in CCW order

	vector<Polygon_2> polygons;
	map<pair<float, float>, idx> mp;
	auto & vhs = refine_loop_region_.vertices;
	MyMesh::VertexHandle vhtmp;
	int loop_index = 0;
	for (const auto & bl : refine_loop_region_.border_loops)
	{
		cout << "loop " << loop_index << " points : " << endl;
		Polygon_2 pg2;
		for (const auto & vindex : bl.vertices_idx)
		{
			auto & point = mesh.point(vhs[vindex]);	
			pg2.push_back(Point_CDT(point[0], point[1]));
			mp[{(float)(point[0]), (float)(point[1])}] = vindex;
			cout << point[0] << "  " << point[1] << endl;
		}
		polygons.push_back(pg2);
	}

	CDT cdt;
	for (auto & pg : polygons)
	{
		cdt.insert_constraint(pg.vertices_begin(), pg.vertices_end(), true);
	}

	mark_domains(cdt);

	typedef MyMesh::VertexHandle MeshVertexHandle;
	typedef CDT::Vertex CGALVertex;
	typedef CDT::Vertex::Point CGALPoint;
	typedef MyMesh::Point MeshPoint;
	typedef CDT::Geom_traits::FT CGAL_FT;
	typedef std::array<CGAL_FT, 2> PointArray;

	PointArray key;
	std::map<PointArray, MeshVertexHandle> points_map;
	std::vector<MeshVertexHandle> face_vhandles;
	CGALVertex v;
	MeshPoint p;
	MeshVertexHandle p_h;

	cout << "After retriangulated ..." << endl;
	int count = 0;
	int invalid_num = 0;
	for (CDT::Finite_faces_iterator fit = cdt.finite_faces_begin();
		fit != cdt.finite_faces_end(); ++fit)
	{
		if (fit->info().in_domain())
		{
			++count;

			cout << "Facets " << count << " : " << endl;
			for (int i = 0; i < 3; ++i)
			{
				v = *(fit->vertex(i));
				auto & point = v.point();
				cout << point.x() << " " << point.y() << endl;
				cout << "vertice idx : " << mp[{(float)(point.x()), (float)(point.y())}] << endl;
				auto & vh = refine_loop_region_.vertices[mp[{(float)(point.x()), (float)(point.y())}]];
				cout << "vertex handle : " << vh << endl;
				face_vhandles.push_back(vh);
			}
			auto fh = mesh.add_face(face_vhandles[0], face_vhandles[2], face_vhandles[1]);
			if (!fh.is_valid())
			{
				cout << "Face is not valid ..." << endl;
				invalid_num++;
			}
			face_vhandles.clear();
		}
	}
	cout << invalid_num << "Faces are invalid..." << endl;
	cout << count << " facets are in domain..." << endl;
}

void CGLProjectInRibbonView::mark_domains(CDT& ct, CDT::Face_handle start, int index, std::list<CDT::Edge>& border)
{
	if (start->info().nesting_level != -1){
		return;
	}
	std::list<CDT::Face_handle> queue;
	queue.push_back(start);
	while (!queue.empty()){
		CDT::Face_handle fh = queue.front();
		queue.pop_front();
		if (fh->info().nesting_level == -1){
			fh->info().nesting_level = index;
			for (int i = 0; i < 3; i++){
				CDT::Edge e(fh, i);
				CDT::Face_handle n = fh->neighbor(i);
				if (n->info().nesting_level == -1){
					if (ct.is_constrained(e)) border.push_back(e);
					else queue.push_back(n);
				}
			}
		}
	}
}

void CGLProjectInRibbonView::mark_domains(CDT& cdt)
{
	for (CDT::All_faces_iterator it = cdt.all_faces_begin(); it != cdt.all_faces_end(); ++it){
		it->info().nesting_level = -1;
	}
	std::list<CDT::Edge> border;
	mark_domains(cdt, cdt.infinite_face(), 0, border);
	while (!border.empty()){
		CDT::Edge e = border.front();
		border.pop_front();
		CDT::Face_handle n = e.first->neighbor(e.second);
		if (n->info().nesting_level == -1){
			mark_domains(cdt, n, e.first->info().nesting_level + 1, border);
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

	float fi = 10.0f;
	map<float, int> mp;
	for (MyMesh::EdgeIter e_it = mesh_.edges_begin();
		e_it != mesh_.edges_end(); ++e_it)
	{
		if (mesh_.is_boundary(*e_it)) 
			cout << "Detect a boundary ..." << endl;

		//boundary's D angle is 0, tagged with DELETING
		float f = mesh_.calc_dihedral_angle_fast(*e_it);
		float degree = glm::degrees(f);
		mesh_.property(dihedralAngle, *e_it) = degree;
		
		if (mp.count(std::abs(degree)) > 0)
		{
			mp[std::abs(degree)] ++;
		}
		else
		{
			mp[std::abs(degree)] = 0;
		}

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

	/*for (const auto & j : mp)
	{
	cout << j.first << "    " << j.second << endl;
	}*/

	//生成平面区域
	std::map<MyMesh::FaceHandle, bool> mp_fb;
	for (unsigned int i = 0; i < deletingEdges.size(); ++i)
	{
		if (mesh_.property(status, deletingEdges[i].first) == DELETING) //边状态为待删除
		{
			mp_fb.clear();
			Region newRegion;
			auto adjacentfh = mesh_.face_handle(mesh_.halfedge_handle(deletingEdges[i].first, 0));
			if (!mesh_.is_valid_handle(adjacentfh))
			{
				cout << "Adjacent face handle is not valid!" << endl;
				assert(false);
			}
			RegionSpread(mesh_, status, adjacentfh, mp_fb);
			for (const auto & i : mp_fb)
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
			while (fe_iter.is_valid() && fe_iter)
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
			while (fe_iter.is_valid() && fe_iter)
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

	/*cout << "Border line segments : " << endl;
	for (unsigned int i = 0; i < border_line_segments_.size(); ++i)
	{
		const auto & bls = border_line_segments_[i];
		cout << "(" << bls.A.x << ", " << bls.A.y << ", " << bls.A.z << ")"
			<< " --> "
			<< "(" << bls.B.x << ", " << bls.B.y << ", " << bls.B.z << ")" << endl;
	}*/

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

	DeletedRegionAnalysis(connectBorderLines_, borderPointAdjGraph, border_line_segments_);

	DeleteMyMesh(mesh_, connectBorderLines_, border_line_segments_, regionPs_);

	RefineIsolatedTriangles(result_mesh_);

	if (!OpenMesh::IO::write_mesh(result_mesh_, "result_mesh_.off"))
	{
		cout << "Writing to result_mesh_.off failed..." << endl;
		return;
	}

	cout << "Writing to result_mesh_.off success ..." << endl;

	//RepairOpenMeshHole(result_mesh_);
	RepairOpenMeshHoleTest(result_mesh_);

	cout << "Mesh information after simplified : "
		<< "vertices : " << result_mesh_.n_vertices() << " "
		<< "faces : " << result_mesh_.n_faces() << endl;

	if (!OpenMesh::IO::write_mesh(result_mesh_, "result_repair_hole.off"))
	{
		cout << "Writing to result_repair_hole.off failed..." << endl;
	}

	cout << "Writing to result_repair_hole.off success ..." << endl;

	////Decimation Refine ...
	//float decimation_ratio = 0.5f;
	//int num_target_vertices = mesh_.n_vertices() * decimation_ratio;
	//int num_target_faces = mesh_.n_faces() * decimation_ratio;

	//Decimater decimater(result_mesh_);
	//HModQuadric hModQuadric;
	//HModEdgeLength hModEdgeLength;

	//decimater.add(hModQuadric);
	//decimater.add(hModEdgeLength);

	//decimater.module(hModQuadric).unset_max_err();
	//decimater.module(hModEdgeLength).set_edge_length(10.0f);
	//decimater.initialize();

	//decimater.decimate_to_faces(num_target_vertices, num_target_faces);

	//result_mesh_.garbage_collection();

	//if (!OpenMesh::IO::write_mesh(result_mesh_, "result.off"))
	//{
	//	cout << "Write error ..." << endl;
	//	return;
	//}

	cout << "Mesh refine starting ..." << endl;
	RefineMesh(result_mesh_);
	cout << "Mesh after refine : " << "v : " << result_mesh_.n_vertices()
		<< " f : " << result_mesh_.n_faces() << endl;


	if (!OpenMesh::IO::write_mesh(result_mesh_, "result.obj"))
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

	unsigned int colorSize = colorList.size();

	for (unsigned int i = 0; i < regionPs_.size(); ++i)
	{
		gpu::Region new_region;
		auto & region = regionPs_[i];
		Color & color = colorList[i % colorSize];

		regionPs_[i].set_color(color.R, color.G, color.B);

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

	unsigned int colorSize = colorList.size();

	for (unsigned int i = 0; i < connectBorderLines_.size(); ++i)
	{
		for (auto & blsIdx : connectBorderLines_[i].blss)
		{
			auto & bls = border_line_segments_[blsIdx];
			gpu::Edge new_edge(
				gpu::Point(bls.A.x, bls.A.y, bls.A.z),
				gpu::Point(bls.B.x, bls.B.y, bls.B.z));
			auto & color = colorList[i % colorSize];
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

void CGLProjectInRibbonView::OnDeleteRegion()
{
	// TODO:  在此添加命令处理程序代码
	if (regionPs_.empty())
	{
		cout << "Connect regionPs is not generated..." << endl;
		MessageBox(_T("连通区域为空,请重新运行算法!"));
		return;
	}

	typedef unsigned int uint;

	for (uint i = 0; i < regionPs_.size(); ++i)
	{
		if (regionPs_[i].isDeleted)
		{
			regionPs_[i].set_color(1.0f, 0.0f, 0.0f);
		}
		else
		{
			regionPs_[i].set_color(1.0f, 1.0, 1.0f);
		}
	}

	if (!regionPs_gpu_.empty())
	{
		regionPs_gpu_.clear();
	}

	MyMesh::FaceVertexIter fv_it;

	MyMesh::Point p1, p2, p3;
	gpu::Point gp1, gp2, gp3;
	glm::vec3 glm_p1, glm_p2, glm_p3;
	glm::vec3 n;
	for (uint i = 0; i < regionPs_.size(); ++i)
	{
		gpu::Region new_region;
		auto & region = regionPs_[i];
		Color & color = region.color;

		for (uint j = 0; j < region.faces.size(); ++j)
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

	cout << "Delete regions generate completed ..." << endl;
	regionPs_gm_.reset();
	InitRegionsData(regionPs_gpu_, RegionType(RegionType::CONNECT_REGION));
	cout << "Transport delete regions data to gpu ..." << endl;

	SetDrawingMode(DrawType(DrawType::DELETE_REGION));
	Invalidate();
}

void CGLProjectInRibbonView::DebugOutputToFile(
	unsigned int source, unsigned int type, unsigned int id,
	unsigned int severity, const char* message)
{
	FILE* f;
	f = fopen("ShaderDebug.txt", "w");
	if (f)
	{
		char debSource[16], debType[20], debSev[10];
		if (source == GL_DEBUG_SOURCE_API_ARB)
			strcpy(debSource, "OpenGL");
		else if (source == GL_DEBUG_SOURCE_WINDOW_SYSTEM_ARB)
			strcpy(debSource, "Windows");
		else if (source == GL_DEBUG_SOURCE_SHADER_COMPILER_ARB)
			strcpy(debSource, "Shader Compiler");
		else if (source == GL_DEBUG_SOURCE_THIRD_PARTY_ARB)
			strcpy(debSource, "Third Party");
		else if (source == GL_DEBUG_SOURCE_APPLICATION_ARB)
			strcpy(debSource, "Application");
		else if (source == GL_DEBUG_SOURCE_OTHER_ARB)
			strcpy(debSource, "Other");
		if (type == GL_DEBUG_TYPE_ERROR_ARB)
			strcpy(debType, "Error");
		else if (type == GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR_ARB)
			strcpy(debType, "Deprecated behavior");
		else if (type == GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR_ARB)
			strcpy(debType, "Undefined behavior");
		else if (type == GL_DEBUG_TYPE_PORTABILITY_ARB)
			strcpy(debType, "Portability");
		else if (type == GL_DEBUG_TYPE_PERFORMANCE_ARB)
			strcpy(debType, "Performance");
		else if (type == GL_DEBUG_TYPE_OTHER_ARB)
			strcpy(debType, "Other");
		if (severity == GL_DEBUG_SEVERITY_HIGH_ARB)
			strcpy(debSev, "High");
		else if (severity == GL_DEBUG_SEVERITY_MEDIUM_ARB)
			strcpy(debSev, "Medium");
		else if (severity == GL_DEBUG_SEVERITY_LOW_ARB)
			strcpy(debSev, "Low");
		else 
			strcpy(debSev, "Ignored");
		fprintf(f, "Source:%s\tType:%s\tID:%d\tSeverity:%s\tMessage:%s\n",
			debSource, debType, id, debSev, message);
		fclose(f);
	}
}

void CGLProjectInRibbonView::DebugCallback(unsigned int source, unsigned int type, unsigned int id,
	unsigned int severity, int length,
	const char* message, void* userParam)
{
	DebugOutputToFile(source, type, id, severity, message);
}


void CGLProjectInRibbonView::OnDeletedMesh()
{
	// TODO:  在此添加命令处理程序代码
	if (mesh_.n_vertices() == 0 || mesh_.n_faces() == 0)
	{
		MessageBox(_T("请重新执行算法！"));
		return;
	}
	if (after_delete_mesh_.is_empty())
	{
		MyMesh result_mesh;
		GetAfterDeleteMesh(result_mesh);
		after_delete_mesh_ = GenerateFlatMesh(result_mesh);
	}

	// TODO:  在此添加命令处理程序代码
	SetupMesh(after_delete_mesh_);
	AdjustCameraView(after_delete_mesh_);
	SetDrawingMode(DRAW_AFTER_DELETE);
	SetDrawingMode(DrawType(DrawType::AFTER_DELETE_MESH));

	Invalidate();
}


void CGLProjectInRibbonView::OnHoleFilling()
{
	// TODO:  在此添加命令处理程序代码
	cout << "Hole filling ..." << endl;
	if (result_mesh_.n_vertices() == 0 || result_mesh_.n_faces() == 0)
	{
		MessageBox(_T("请重新执行算法！"));
		return;
	}
	if (after_refine_mesh_.is_empty())
	{
		after_refine_mesh_ = GenerateFlatMesh(result_mesh_);
	}

	// TODO:  在此添加命令处理程序代码
	SetupMesh(after_refine_mesh_);
	AdjustCameraView(after_refine_mesh_);
	SetDrawingMode(DRAW_AFTER_REFINE);
	SetDrawingMode(DrawType(DrawType::AFTER_REFINE_MESH));

	Invalidate();
}


void CGLProjectInRibbonView::OnLodNormal()
{
	// TODO:  在此添加命令处理程序代码
	cout << "Lod normal ..." << endl;

	LoadLodMeshes();
	GenerateLodScene(50);
	AdjustLodCamera();
	SetDrawingMode(DrawType(DrawType::LOD_DISPLAY));
	
	//Set draw timer ...
	if (draw_timer_id_event == 0)
		draw_timer_id_event = SetTimer(1, 33, NULL);

	cout << "Draw set timer id event : " << draw_timer_id_event << endl;

	Invalidate();
}


void CGLProjectInRibbonView::OnLodLod()
{
	// TODO:  在此添加命令处理程序代码
	cout << "Test new mothod for lod ... " << endl;
	LoadLodMeshes();
	GenerateLodScene(50);
	AdjustLodCamera();
	SetDrawingMode(DrawType(DrawType::LOD_NEW_DISPLAY));
	InitDataForNewLodMethod();

	if (draw_timer_id_event == 0)
		draw_timer_id_event = SetTimer(1, 33, NULL);

	cout << "Draw set timer id event : " << draw_timer_id_event << endl;

	Invalidate();
}

void CGLProjectInRibbonView::InitDataForNewLodMethod()
{
	//Input ...
	assert(!lod_scene_.meshes_map.empty());
	
	GLuint points_size = 0;

	vector<glm::vec3> instance_world_pos_v;
	for (const auto & mesh : lod_scene_.meshes_map)
	{
		instance_world_pos_v.push_back(mesh.first);
		++points_size;
	}

	judge_points_size = points_size;

	glGenVertexArrays(1, &instance_world_pos_vao);
	glGenBuffers(1, &instance_world_pos_vbo);

	glBindBuffer(GL_ARRAY_BUFFER, instance_world_pos_vbo);
	glBufferData(
		GL_ARRAY_BUFFER,
		instance_world_pos_v.size() * sizeof(glm::vec3),
		&instance_world_pos_v[0],
		GL_STATIC_DRAW);

	glBindVertexArray(instance_world_pos_vao);
	glBindBuffer(GL_ARRAY_BUFFER, instance_world_pos_vbo);
	glEnableVertexAttribArray(0);
	glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (GLvoid*)0);
	glBindVertexArray(0);

	//Transform feedback ...		
	glGenTransformFeedbacks(1, &lod_tfb);
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, lod_tfb);

	glGenBuffers(3, lod_vbo);

	for (int i = 0; i < 3; ++i)
	{
		glBindBuffer(GL_ARRAY_BUFFER, lod_vbo[i]);
		glBufferData(GL_ARRAY_BUFFER,
			1024 * 1024 * sizeof(GLfloat),
			NULL, GL_DYNAMIC_COPY);
		glBindVertexArray(lod_scene_.meshes[i].VAO);
		glBindBuffer(GL_ARRAY_BUFFER, lod_vbo[i]);
		glEnableVertexAttribArray(4);
		glVertexAttribPointer(4, 3, GL_FLOAT, GL_FALSE, sizeof(glm::vec3), (void*)0);
		glVertexAttribDivisor(4, 1);
		glBindVertexArray(0);
		glBindBufferBase(GL_TRANSFORM_FEEDBACK_BUFFER, i, lod_vbo[i]);
	}

	glGenQueries(NUM_LOD, cull_query);
}

void CGLProjectInRibbonView::LoadLodMeshes()
{
	if (!lod_scene_.meshes.empty())
	{
		cout << "LOD meshes already exists ..." << endl;
		return;
	}

	MyMesh helper_mesh;
	string mesh_file = "D:\\MeshSimplification\\GLProjectInRibbon\\GLProjectInRibbon\\LOD\\2\\SWD4-8FRF-lod";
	if (!OpenMesh::IO::read_mesh(helper_mesh, string(mesh_file + "1.obj").c_str()))
	{
		cout << "Read lod 1 mesh file failed..." << endl;
		return;
	}
	cout << "Read lod 1 mesh completed ..." << endl;
	cout << "Lod 1 : " << "vertices." << helper_mesh.n_vertices() << " faces." << helper_mesh.n_faces() << endl;

	Mesh mesh_lod1 = GenerateFlatMesh(helper_mesh);
	lod_scene_.meshes.push_back(mesh_lod1);

	helper_mesh.clean();

	if (!OpenMesh::IO::read_mesh(helper_mesh, string(mesh_file + "2.obj").c_str()))
	{
		cout << "Read lod 2 mesh file failed..." << endl;
		return;
	}
	cout << "Read lod 2 mesh completed ..." << endl;
	cout << "Lod 2 : " << "vertices." << helper_mesh.n_vertices() << " faces." << helper_mesh.n_faces() << endl;

	Mesh mesh_lod2 = GenerateFlatMesh(helper_mesh);
	lod_scene_.meshes.push_back(mesh_lod2);

	helper_mesh.clean();

	if (!OpenMesh::IO::read_mesh(helper_mesh, string(mesh_file + "3.obj").c_str()))
	{
		cout << "Read lod 3 mesh file failed..." << endl;
		return;
	}
	cout << "Read lod 3 mesh completed ..." << endl;
	cout << "Lod 3 : " << "vertices." << helper_mesh.n_vertices() << " faces." << helper_mesh.n_faces() << endl;

	Mesh mesh_lod3 = GenerateFlatMesh(helper_mesh);
	lod_scene_.meshes.push_back(mesh_lod3);

	//Transport meshes to GPU ...
	for (auto & mesh : lod_scene_.meshes)
	{
		SetupMesh(mesh);
	}

	cout << "Transport meshes to gpu completed ..." << endl;
}

void CGLProjectInRibbonView::GenerateLodScene(int num_mesh)
{
	if (lod_scene_.meshes.empty())
	{
		cout << "Lod meshes is empty ..." << endl;
		return;
	}

	float xmin, ymin, zmin;
	xmin = ymin = zmin = 10000000000.0f;
	float xmax, ymax, zmax;
	xmax = ymax = zmax = -100000000000.0f;

	Mesh & mesh = lod_scene_.meshes[0];

	for (const auto & point : mesh.vertices)
	{
		xmin = std::min(xmin, point.Position.x);
		ymin = std::min(ymin, point.Position.y);
		zmin = std::min(zmin, point.Position.z);

		xmax = std::max(xmax, point.Position.x);
		ymax = std::max(ymax, point.Position.y);
		zmax = std::max(zmax, point.Position.z);
	}

	lod_scene_.center_point.x = xmin + 0.5f * (xmax - xmin);
	lod_scene_.center_point.y = ymin + 0.5f * (ymax - ymin);
	lod_scene_.center_point.z = zmin + 0.5f * (zmax - zmin);

	lod_scene_.num_mesh = num_mesh;
	lod_scene_.mesh_length = glm::vec3(xmax - xmin, ymax - ymin, zmax - zmin);

	float delta_x = (xmax - xmin) * 1.5f;
	float delta_y = (ymax - ymin) * 1.5f;

	glm::vec3 init_vec(0.0f, 0.0f, 0.0f);
	for (int i = 0; i < num_mesh; ++i)
	{
		for (int j = 0; j < num_mesh; ++j)
		{
			lod_scene_.meshes_map.push_back(
			{
				glm::vec3(
				init_vec.x + i * delta_x,
				init_vec.y + j * delta_y,
				0.0f), 
				0 });
		}
	}
}

void CGLProjectInRibbonView::MeshesLodDetermination(glm::vec3 camera_pos)
{
	float lod1_distance = 100.0f;
	float lod2_distance = 500.0f;

	for (auto & mesh : lod_scene_.meshes_map)
	{
		auto & pos = mesh.first;
		auto & lod = mesh.second;

		float distance = glm::distance(pos, camera_pos);

		if (distance < lod1_distance)
		{
			lod = 0;
		}
		else if (distance < lod2_distance)
		{
			lod = 1;
		}
		else
		{
			lod = 2;
		}
	}
}

void CGLProjectInRibbonView::AdjustLodCamera()
{
	glm::vec3 camera_target_pos;

	float delta_x = lod_scene_.mesh_length.x * 1.5f;
	float delta_y = lod_scene_.mesh_length.y * 1.5f;

	camera_target_pos.x = delta_x * lod_scene_.num_mesh * 0.5f;
	camera_target_pos.y = 0.0f;
	camera_target_pos.z = 200.0f;

	camera_->SetPosition(camera_target_pos.x, camera_target_pos.y, camera_target_pos.z);
	
	glm::vec3 scene_center_point;
	scene_center_point.x = lod_scene_.num_mesh * 0.5f * delta_x;
	scene_center_point.y = lod_scene_.num_mesh * 0.5f * delta_y;
	scene_center_point.z = 0.0f;

	glm::vec3 up_direction = glm::vec3(0.0f, 0.0f, 1.0f);
	glm::vec3 front_direction = glm::normalize(scene_center_point - camera_->Position);
	glm::vec3 right_direction = glm::cross(front_direction, up_direction);
	up_direction = glm::normalize(glm::cross(right_direction, front_direction));

	camera_->SetFront(front_direction.x, front_direction.y, front_direction.z);
	camera_->SetUp(up_direction.x, up_direction.y, up_direction.z);

	matrix_projection_ = glm::perspective(45.0f, (float)num_screen_width_ / (float)num_screen_height_, 10.0f, 2000.0f);
}

void CGLProjectInRibbonView::DrawLODMode()
{
	shader_->Use();
	// Transformation matrices
	matrix_view_ = camera_->GetViewMatrix();
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "projection"), 1, GL_FALSE, glm::value_ptr(matrix_projection_));
	glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "view"), 1, GL_FALSE, glm::value_ptr(matrix_view_));
	// Draw the loaded model
	
	clock_t start = clock();
	MeshesLodDetermination(camera_->Position);
	clock_t end = clock();
	cout << ClockToMilliseconds(end - start) << "milliseconds ... "<< endl;

	vector<Color> normal_colors(3, Color(1.0f, 1.0f, 1.0f));
	normal_colors[0].SetColor(1.0f, 0.0f, 0.0f);
	normal_colors[1].SetColor(0.0f, 1.0f, 0.0f);
	normal_colors[2].SetColor(0.0f, 0.0f, 1.0f);

	for (auto & mesh : lod_scene_.meshes_map)
	{
		auto & pos = mesh.first;
		auto & lod = mesh.second;

		matrix_model_ = glm::mat4(1.0);
		//no rotation
		//matrix_model_ = matrix_rotation_;
		matrix_model_ = glm::translate(matrix_model_, pos);
		matrix_model_ = glm::scale(matrix_model_, glm::vec3(num_model_scale_));
		matrix_model_ = glm::translate(matrix_model_, glm::vec3(-lod_scene_.center_point.x, -lod_scene_.center_point.y, -lod_scene_.center_point.z));
		glUniformMatrix4fv(glGetUniformLocation(shader_->Program, "model"), 1, GL_FALSE, glm::value_ptr(matrix_model_));

		//Color normalColor(1.0f, 1.0f, 1.0f);
		//InitMaterial(normalColor, *shader_);
		InitMaterial(normal_colors[lod], *shader_);
		lod_scene_.meshes[lod].Draw(*shader_);
	}
}

void CGLProjectInRibbonView::DrawNewLODMode()
{
	//lod determination pass ...
	dynamic_lod_shader_->Use();

	glm::mat4 view_matrix = camera_->GetViewMatrix();
	glm::vec2 lod_distance(100.0f, 500.0f);

	glUniformMatrix4fv(glGetUniformLocation(dynamic_lod_shader_->Program, "view_matrix"), 1, GL_FALSE, glm::value_ptr(view_matrix));
	glUniform2f(glGetUniformLocation(dynamic_lod_shader_->Program, "lod_distance"), lod_distance.x, lod_distance.y);

	glEnable(GL_RASTERIZER_DISCARD);

	for (int i = 0; i < NUM_LOD; ++i)
	{
		glBeginQueryIndexed(GL_PRIMITIVES_GENERATED, i, cull_query[i]);
	}

	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, lod_tfb);
	glBeginTransformFeedback(GL_POINTS);
	glBindVertexArray(instance_world_pos_vao);
	glDrawArrays(GL_POINTS, 0, judge_points_size);
	glBindVertexArray(0);
	glEndTransformFeedback();
	glBindTransformFeedback(GL_TRANSFORM_FEEDBACK, 0);

	for (int i = 0; i < NUM_LOD; ++i)
	{
		glEndQueryIndexed(GL_PRIMITIVES_GENERATED, i);
	}

	glDisable(GL_RASTERIZER_DISCARD);

	//draw instanced pass ...
	instanced_draw_shader_->Use();

	vector<Color> lod_colors;
	lod_colors.push_back(Color(1.0f, 0.0f, 0.0f));
	lod_colors.push_back(Color(0.0f, 1.0f, 0.0f));
	lod_colors.push_back(Color(0.0f, 0.0f, 1.0f));

	for (int i = 0; i < NUM_LOD; ++i)
	{
		auto & color = lod_colors[i];
		matrix_model_ = glm::mat4(1.0);
		matrix_model_ = glm::scale(matrix_model_, glm::vec3(num_model_scale_));
		matrix_model_ = glm::translate(matrix_model_, glm::vec3(-lod_scene_.center_point.x, -lod_scene_.center_point.y, -lod_scene_.center_point.z));
		glUniformMatrix4fv(glGetUniformLocation(instanced_draw_shader_->Program, "model"), 1, GL_FALSE, glm::value_ptr(matrix_model_));
		glUniformMatrix4fv(glGetUniformLocation(instanced_draw_shader_->Program, "view"), 1, GL_FALSE, glm::value_ptr(view_matrix));
		glUniformMatrix4fv(glGetUniformLocation(instanced_draw_shader_->Program, "projection"), 1, GL_FALSE, glm::value_ptr(matrix_projection_));

		InitDirectionLighting(*instanced_draw_shader_);
		InitMaterial(color, *instanced_draw_shader_);

		auto & mesh = lod_scene_.meshes[i];

		glBindVertexArray(mesh.VAO);
		glGetQueryObjectiv(cull_query[i], GL_QUERY_RESULT, &lod_tree_nums[i]);
		//cout << "lod " << i << " nums : " << lod_tree_nums[i] << endl;
		if (lod_tree_nums[i] > 0)
		{
			glDrawElementsInstanced(GL_TRIANGLES, mesh.indices.size(), GL_UNSIGNED_INT, 0, lod_tree_nums[i]);
		}
		glBindVertexArray(0);
	}

}

void CGLProjectInRibbonView::OnTimer(UINT_PTR nIDEvent)
{
	// TODO:  在此添加消息处理程序代码和/或调用默认值

	if (nIDEvent == draw_timer_id_event)
	{
		camera_->MoveForward(3.0f, 0.033f);
		Invalidate(TRUE);
	}

	CView::OnTimer(nIDEvent);
}


void CGLProjectInRibbonView::OnFeature()
{
	// TODO:  在此添加命令处理程序代码
	cout << "Feature ..." << endl;
}
