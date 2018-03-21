//Author : LHZ
#pragma once
#pragma region include
//System include
#include <windows.h>

//Standard include
#include <map>
#include <vector>
#include <unordered_map>
#include <algorithm>
#include <cmath>

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

#include <CGAL/Constrained_Delaunay_triangulation_2.h>
#include <CGAL/Triangulation_face_base_with_info_2.h>
#include <CGAL/Polygon_2.h>

// GLM Mathemtics
#include <glm.hpp>
#include <gtc/matrix_transform.hpp>
#include <gtc/type_ptr.hpp>

// Other Libs
#include <SOIL.h>

//OpenMesh
#include <OpenMesh/Core/IO/MeshIO.hh>
#include <OpenMesh/Core/Mesh/TriMesh_ArrayKernelT.hh>
#include <OpenMesh/Tools/Decimater/DecimaterT.hh>
#include <OpenMesh/Tools/Decimater/ModQuadricT.hh>
#include <OpenMesh/Tools/Decimater/ModEdgeLengthT.hh>

// GL includes
#include "Shader.h"
#include "Camera.h"
#include "Model.h"
#include "GLText.h"
#include "ImportOBJ.h"
#include "Color.h"
#pragma endregion include
#pragma region define
//Define Marcos
#define DOUBLE_MAX  10000000000000.0f
#define DOUBLE_MIN -10000000000000.0f

//Drawing Mode
#define DRAW_NORMAL 1
#define DRAW_MESHSEGMENTATION 2
#define DRAW_SIMPLIFIEDMESH 3
#define DRAW_FLAT 4
#define DRAW_SMOOTH 5
#define DRAW_AFTER_DELETE 6
#define DRAW_AFTER_REFINE 7

#define FLOAT_MAX 1000000000000.0f

//Status
#define DELETING 0
#define DELETED 1
#define UNDETERMINED 2
#define BORDER 3
#define BORDER_CALCED 4

#pragma endregion define

namespace gpu{
	struct Point {
		Point(float x = 0.0f, float y = 0.0f, float z = 0.0f) {
			position.x = x;
			position.y = y;
			position.z = z;
			normal.x = 0.0f;
			normal.y = 0.0f;
			normal.z = 0.0f;
			color.x = 0.0f;
			color.y = 0.0f;
			color.z = 0.0f;
		}

		Point(const Point & p) {
			position = p.position;
			normal = p.normal;
			color = p.color;
		}

		Point& operator=(const Point & p)
		{
			if (this == &p) return *this;
			position = p.position;
			normal = p.normal;
			color = p.color;
		}

		void set_normal(float x, float y, float z) {
			normal.x = x;
			normal.y = y;
			normal.z = z;
		}

		void set_color(float r, float g, float b) {
			color.r = r;
			color.g = g;
			color.b = b;
		}

		glm::vec3 position;
		glm::vec3 normal;
		glm::vec3 color;
	};
	
	struct Edge {
		Edge(Point _A, Point _B) :
			A(_A),
			B(_B)
		{};

		Point A;
		Point B;

		void set_color(float r, float g, float b)
		{
			A.set_color(r, g, b);
			B.set_color(r, g, b);
		}
	};

	struct Region {
		vector<Point> points;
	};
};

namespace sms = CGAL::Surface_mesh_simplification;

struct MeshGpuManager
{
	MeshGpuManager() : isMeshTransportedToGPU(false), indiceSize(0) {};
	~MeshGpuManager(){};

	GLuint VAO, VBO, EBO;
	bool isMeshTransportedToGPU;
	GLsizei indiceSize;

	void reset()
	{
		glDeleteVertexArrays(1, &VAO);
		glDeleteBuffers(1, &VBO);
		glDeleteBuffers(1, &EBO);

		indiceSize = 0;
		isMeshTransportedToGPU = false;
	}
};

enum class RegionType{
	CONNECT_REGION,
	MAXIMUM_CONNECT_REGION
};

enum class DrawType{
	NORMAL_MESH,
	REGION,
	BORDER_LINE_SEGMENT,
	CONNECT_BORDER_LINE_SEGMENT,
	DELETE_REGION,
	AFTER_DELETE_MESH,
	AFTER_REFINE_MESH,
	LOD_DISPLAY,
	LOD_NEW_DISPLAY
};

class CGLProjectInRibbonView : public CView
{
#pragma region typedef&struct
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

	//New Add...
	struct FaceInfo2
	{
		FaceInfo2(){}
		int nesting_level;
		bool in_domain(){
			return nesting_level % 2 == 1;
		}
	};
	typedef CGAL::Exact_predicates_inexact_constructions_kernel       K;
	typedef CGAL::Triangulation_vertex_base_2<K>                      Vb;
	typedef CGAL::Triangulation_face_base_with_info_2<FaceInfo2, K>    Fbb;
	typedef CGAL::Constrained_triangulation_face_base_2<K, Fbb>        Fb;
	typedef CGAL::Triangulation_data_structure_2<Vb, Fb>               TDS;
	typedef CGAL::Exact_predicates_tag                                Itag;
	typedef CGAL::Constrained_Delaunay_triangulation_2<K, TDS, Itag>  CDT;
	typedef CDT::Point                                                Point_CDT;
	typedef CGAL::Polygon_2<K>                                        Polygon_2;

#pragma region New_Algorithm

	typedef OpenMesh::TriMesh_ArrayKernelT<> MyMesh;
	typedef OpenMesh::Decimater::DecimaterT<MyMesh> Decimater;
	typedef OpenMesh::Decimater::ModQuadricT<MyMesh>::Handle HModQuadric;
	typedef OpenMesh::Decimater::ModEdgeLengthT<MyMesh>::Handle HModEdgeLength;

	typedef std::vector<std::pair<MyMesh::EdgeHandle, bool>> DeletingEdgeArray;
	typedef std::vector<std::pair<MyMesh::EdgeHandle, bool>> UnDeterminedEdgeArray;
	typedef std::vector<std::pair<MyMesh::EdgeHandle, bool>> DeletedEdgeArray;

	typedef struct Point_
	{
		Point_(float _x = 0.0f, float _y = 0.0f, float _z = 0.0f) :
			x(_x),
			y(_y),
			z(_z)
		{
		};

		Point_(const Point_ & p)
		{
			x = p.x;
			y = p.y;
			z = p.z;
		}

		bool operator< (const Point_ & rhs)
		{
			return this->x < rhs.x
				|| (this->x == rhs.x) && (this->y < rhs.y)
				|| (this->x == rhs.x && this->y == rhs.y) && (this->z < rhs.z);
		}

		bool operator==(const Point_ & rhs)
		{
			return this->x == rhs.x && this->y == rhs.y && this->z == rhs.z;
		}

		bool operator() (const Point_ & lhs, const Point_ & rhs)
		{
			return lhs.x < rhs.x
				|| (lhs.x == rhs.x) && (lhs.y < rhs.y)
				|| (lhs.x == rhs.x && lhs.y == rhs.y) && (lhs.z < rhs.z);
		}

		float x;
		float y;
		float z;
	} PointNA;

	typedef PointNA BorderPoint;

	typedef struct Region_
	{
		Region_() : isDeleted(false) {};

		std::vector<MyMesh::FaceHandle> faces;
		typedef int blsIdx;
		std::set<blsIdx> blss;
		bool isDeleted;
		Color color;

		void set_color(float r, float g, float b)
		{
			color.R = r;
			color.G = g;
			color.B = b;
		}
	} Region;

	typedef struct BorderLineSegment_
	{
		BorderLineSegment_(PointNA& _A, const PointNA& _B)
		{
			if (!(_A < _B))
			{
				A = _B;
				B = _A;
			}
			else
			{
				A = _A;
				B = _B;
			}

			length = glm::length(glm::vec3(A.x - B.x, A.y - B.y, A.z - B.z));
			needsDeleted = false;
		}

		PointNA A;
		PointNA B;

		float length;
		std::set<int> RegionIDs;

		bool needsDeleted;
	} BorderLineSegment;

	typedef int BLS_id; // Border Line Segments idx
	typedef struct GraphNode_
	{
		GraphNode_() : isVisited(false) {};

		std::vector<std::pair<BorderPoint, BLS_id>> toPoints;
		bool isVisited;
	} GraphNode;

	//生成边界点构成的最大连通区域
	typedef struct ConnectRegion_
	{
		ConnectRegion_() : isDeleted(false), meanLength(FLOAT_MAX) {};

		std::vector<BorderPoint> points;
		typedef int blsIdx;
		typedef int regionPIdx;
		std::set<blsIdx> blss;
		std::set<regionPIdx> regionIDs;
		float meanLength;
		bool isDeleted;

	} ConnectRegion;
#pragma endregion New_Algorithm


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
	typedef sms::Constrained_placement<sms::Midpoint_placement<Surface_mesh>,
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
#pragma endregion typedef&struct
#pragma region algorithm_method
public:
	Mesh GenerateFlatMesh(Mesh & pMesh);
	Mesh GenerateFlatMesh(MyMesh & mesh);
	void GenerateColorList(vector<Color>& pColorList);
	int  GenerateUniqueColorList(int count, vector<Color>& pColorList, vector<Color>& pExcludedColor);
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
	int  HelperUpdateTreeNodeFaceNum(TreeNode* pTreeNode);
	void UpdateTreeNodeSDF(TreeNode* pTreeNode);
	double HelperUpdateTreeNodeSDF(TreeNode* pTreeNode);
	void DeleteTreeNode(TreeNode* pTreeNode);
	bool isLeaf(TreeNode* pTreeNode);
	bool isJoint(TreeNode* pTreeNode);
	bool SimplifyInFinalMethod(TreeNode* pTreeNode, int& restFaceNum, int& targetNum);
	int  TreeNodesSum(TreeNode* tn);
	void BuildFinalMeshFromMeshTree(TreeNode* root);
	void EstimateSimplifyTarget(int &targetFaceNum, float &targetSDF);
	void RepairHole();

	//New Algorithm
public:
	void RegionSpread(MyMesh & mesh,
		OpenMesh::EPropHandleT<int>& edgeStatus,
		MyMesh::FaceHandle &fh,
		std::map<MyMesh::FaceHandle,
		bool>
		&mp);
	void RefineRegionSpread(MyMesh & mesh,
		OpenMesh::EPropHandleT<int>& edgeStatus,
		MyMesh::FaceHandle &fh,
		std::map<MyMesh::FaceHandle,
		bool>
		&mp,
		set<MyMesh::HalfedgeHandle> &he_handles);
	void GraphBFS(std::map < BorderPoint, GraphNode, BorderPoint>& mp,
		BorderPoint & startPoint,
		ConnectRegion & newRegion);
	void DeletedRegionAnalysis(std::vector<ConnectRegion> & cr,
		std::map < BorderPoint,
		GraphNode, BorderPoint>& mp,
		std::vector<BorderLineSegment> &borderLineSegments);
	void DeleteMyMesh(MyMesh & mesh,
		std::vector<ConnectRegion> & cr,
		std::vector<BorderLineSegment> &borderLineSegments,
		std::vector<Region>& regionPs);
	void RefineIsolatedTriangles(MyMesh & mesh);
	void ContourLineBasedMethod();

	map<int, MyMesh::VertexHandle> from_vertex_;
	map<int, MyMesh::VertexHandle> to_vertex_;
	map<int, MyMesh::VertexHandle> from_temp_;
	map<int, MyMesh::VertexHandle> to_temp_;
	map<int, MyMesh::VertexHandle> fromV_;
	map<int, MyMesh::VertexHandle> toV_;
	vector<bool> vis_;
	int vNum_;
	void DFS(int nNum, int n);
	void RepairOpenMeshHole(MyMesh & mesh);
	void RepairOpenMeshHoleTest(MyMesh & mesh);
	struct RefineRegion
	{
		set<MyMesh::FaceHandle> faces;
		set<MyMesh::HalfedgeHandle> boundarys;
	};
	vector<RefineRegion> refine_regions_;

	void RefineMesh(MyMesh & mesh);
	void RefineTheRegion(MyMesh & mesh, RefineRegion& region);
	void DeleteTheRegion(MyMesh & mesh, RefineRegion& region);
	void GetAfterDeleteMesh(MyMesh & mesh);

	struct BorderLoop{
		typedef int idx;
		vector<idx> vertices_idx;
	};

	struct RefineLoopRegion{
		vector<MyMesh::VertexHandle> vertices;
		vector<BorderLoop> border_loops;
	};

	RefineLoopRegion refine_loop_region_;

	void GetRefineLoopRegion(RefineRegion & region, MyMesh & mesh);

	void mark_domains(CDT& ct,
		CDT::Face_handle start,
		int index,
		std::list<CDT::Edge>& border);

	void mark_domains(CDT& cdt);


#pragma endregion algorithm_method
#pragma region rendering_method
public:
	void CalculateFocusPoint(const vector<Mesh>& pMeshList);
	void CalculateFocusPoint(Model * pModel);
	void CalculateFocusPoint(const Mesh& pMesh);
	void AdjustCameraView(const vector<Mesh>& pMeshList);
	void AdjustCameraView(const Mesh& pMesh);
	void AdjustCameraView(Model * pModel);
	void DragBall(GLfloat ax, GLfloat ay, GLfloat bx, GLfloat by, GLfloat r);
	void ChooseLodModel(float distance);
	void GetFPS(double deltaTime);
	void GLSetRenderState();

public:
	void SetDrawingMode(int drawingMode);
	void SetDrawingMode(DrawType draw_type);

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
	double ClockToMilliseconds(clock_t ticks);
	void InitMaterial(Color& pColor, Shader & shader);
	void InitMaterial(Shader & shader);
	void InitDirectionLighting();
	void InitDirectionLighting(Shader& shader);

#pragma endregion rendering_method
#pragma region event_handler
protected:
	virtual BOOL PreCreateWindow(CREATESTRUCT& cs);
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
	afx_msg void OnOpenButton();
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
	afx_msg void OnStartCLBAlgorithm();
#pragma endregion event_handler
#pragma region data
private:
	//Data Structure
	CString file_path_;
	Shader* shader_;
	Shader* line_shader_;
	Shader* region_shader_;
	Shader* text_shader_;
	Shader* mesh_line_mode_shader_;
	Shader* dynamic_lod_shader_;
	Shader* instanced_draw_shader_;
	Model* model_;
	string model_name_;
	Camera* camera_;
	GLText text_;
	//Model* m_Model_LOD;

	//CGAL Model
	CgalPolyhedron polyhedron_;

	//GPU Data Structure
	float num_model_scale_;
	glm::mat4 matrix_model_;
	glm::mat4 matrix_view_;
	glm::mat4 matrix_projection_;
	glm::mat4 matrix_rotation_;

	bool isDragged_ = false;
	GLfloat num_lastX_;
	GLfloat num_lastY_;

	//Time FPS
	LARGE_INTEGER frequency_;
	LARGE_INTEGER count1_;
	LARGE_INTEGER count2_;

	clock_t begin_frame_;
	clock_t end_frame_;
	clock_t delta_time_;
	unsigned int frames_;
	double frame_rate_;
	double average_frame_time_milliseconds_;

	float num_distance_;
	double num_fps_;

	glm::vec3 focus_point_;
	double num_xMin_;
	double num_xMax_;
	double num_yMin_;
	double num_yMax_;
	double num_zMin_;
	double num_zMax_;
	float num_bodyDiagnalLength_;

	int num_screen_width_;
	int num_screen_height_;

	bool isShowLine_ = false;
	bool isShowLod_ = false;

	bool isCameraAdjusted_;

	vector<Model> model_list_;

	//Mesh Segmentation
	Facet_double_map internal_sdf_map_; //facet_handle -> sdf value
	Facet_int_map internal_segment_map_;//facet_handle -> mesh index
	std::size_t number_of_segments_;

	vector<vector<int>> mesh_graph_;
	int index_max_SDF_ = 0;
	TreeNode* mesh_tree_;

	//Mesh For Display
	Mesh* pFlatMesh_;
	Mesh* pSmoothMesh_;

	int drawing_mode_;

	//Mesh Cache
	Mesh combined_mesh_;

	Mesh normal_mesh_;
	Mesh after_delete_mesh_;
	Mesh after_refine_mesh_;
	vector<Mesh> mesh_list_; //For Segmentation
	Mesh simplified_mesh_;

	//Data for contour line based method
	MyMesh mesh_;
	MyMesh result_mesh_;
	std::vector<BorderLineSegment> border_line_segments_;
	std::vector<Region> regionPs_;
	std::vector<ConnectRegion> connectBorderLines_;

	//Data for contour line based rendering

	DrawType draw_type_;

	vector<GLuint> gpuindices_;
	vector<gpu::Point> gpupoints_;

	vector<gpu::Edge> border_line_segments_gpu_;
	MeshGpuManager border_line_gm_;
	bool ifDrawBorderLine;

	vector<gpu::Region> regionPs_gpu_;
	MeshGpuManager regionPs_gm_;
	bool ifDrawRegions;

	vector<gpu::Edge> connectBorderLines_gpu_;
	MeshGpuManager connectBorderLines_gm_;
	bool ifDrawConnectBorderLines;

	MeshGpuManager default_gm_;

	//Display FPS info
	UINT_PTR draw_timer_id_event;



public:
	static void CALLBACK DebugCallback(unsigned int source, unsigned int type,
		unsigned int id, unsigned int severity,
		int length, const char* message, void* userParam);

	static void DebugOutputToFile(
		unsigned int source, unsigned int type, unsigned int id,
		unsigned int severity, const char* message);

public :
	
	void DrawLine(float line_width, float r, float g, float b);
	void DrawConnectBorderLine(float line_width);
	void InitLineData();
	void InitConnectBorderLine();
	
	void DrawRegions(MeshGpuManager& region_gm);
	void InitRegionsData(vector<gpu::Region>& region, RegionType type);
	MeshGpuManager& WhichRegionType(RegionType type);


	////Log System
	//Log m_Log;
	string infile_name_;
	// Operations
#pragma endregion data

public:
	struct LodScene {
		vector<Mesh> meshes; //Lod meshes from file ...
		typedef int lod; // 0 : meshes[0] ...
		vector<pair<glm::vec3, lod>> meshes_map; // different meshes location generated from transport matrix
		glm::vec3 center_point;
		glm::vec3 mesh_length;
		int num_mesh; // single direction mesh num
	};

	LodScene lod_scene_;

//New Lod method based on Gpu
	static const int NUM_LOD = 3;

	GLuint instance_world_pos_vao;
	GLuint instance_world_pos_vbo;
	GLuint lod_tfb;
	GLuint lod_vbo[NUM_LOD];
	GLuint judge_points_size;
	GLuint cull_query[NUM_LOD];
	int lod_tree_nums[NUM_LOD];

	void InitDataForNewLodMethod();


	void LoadLodMeshes();
	void GenerateLodScene(int num_mesh);
	void MeshesLodDetermination(glm::vec3 camera_pos);
	void AdjustLodCamera();
	void DrawLODMode();
	void DrawNewLODMode();

protected:
		CGLProjectInRibbonView();
		DECLARE_DYNCREATE(CGLProjectInRibbonView)
public:
	CGLProjectInRibbonDoc* GetDocument() const;
	afx_msg void OnGenerateBorderLine();
	afx_msg void OnGenerateConnectFaces();
	afx_msg void OnGenerateMaximumConnectBorderLines();
	afx_msg void OnDeleteRegion();
	afx_msg void OnDeletedMesh();
	afx_msg void OnHoleFilling();
	afx_msg void OnLodNormal();
	afx_msg void OnLodLod();
	afx_msg void OnTimer(UINT_PTR nIDEvent);
	afx_msg void OnFeature();
};
#ifndef _DEBUG  // debug version in GLProjectInRibbonView.cpp
inline CGLProjectInRibbonDoc* CGLProjectInRibbonView::GetDocument() const
{
	return reinterpret_cast<CGLProjectInRibbonDoc*>(m_pDocument);
}
#endif

