#pragma once

#include "ofMain.h"
#include "ofxWinMenu.h"

//------ Fundamental C Structures for Implementaion ------//
struct Line {			// 직선을 표현하기 위한 구조체
	float x1, y1;		// 직선의 양 끝 점 (x1, y1)과 (x2, y2)
	float x2, y2;
};

struct Point {			// 점을 표현하기 위한 구조체
	int x, y;			// (x, y)
};

struct Node {			// 연결리스트를 구축하기 위한 노드 구조체
	char date[15];		// 게임 플레이가 종료된 날짜 및 시간을 기억
	int score;			// 1회의 게임에서의 최종 점수를 기억
	struct Node *link;	// link포인터
};

class ofApp : public ofBaseApp {
public:
	//------ Fundamental Data Structures Declaration ------//
	struct Line *lines;	// 미로를 화면 상에 그리기 위한 직선 배열
	struct Point *shortRoute;	// 최단 경로를 점 별로 기억할 스택
	struct Point *allRoute;		// 전체 경로를 점 별로 기억할 배열
	char **Maze;		// maze.maz로부터 미로를 읽어들일 2차원 배열
	int **Visit;		// 미로의 특정 칸 방문 여부를 기억할 2차원 배열
	int linesSize = 0;	// lines 배열의 크기를 기억할 변수
	int sIdx = 0;		// 최단 경로 스택의 top을 지칭하는 변수
	int aIdx = 0;		// 전체 경로 배열의 크기를 기억할 변수
	struct Point move[4] = {	// DFS 최단거리 탐색에 사용될 방향 구조체
		{ 1,0 }, { 0,1 }, { -1,0 }, { 0,-1 }
	};
	int *rows, *vert, *horiz;	// 임의의 미로를 생성하는데 사용되는 배열들
	ofPolyline shortroute;	// 최단경로를 그리는 과정에서 사용될 ofPolyLine
	ofPolyline allroute;	// 전체경로를 그리는 과정에서 사용될 ofPolyLine

	//------ Fundamental Coordinates Representation ------//
	int HEIGHT;			// 미로의 높이를 기억
	int WIDTH;			// 미로의 폭을 기억
	int coordX;			// 쥐의 현재 위치를 기억할 변수
	int coordY;
	int targetX;		// 치즈(목적지)의 위치를 기억할 변수
	int targetY;

	//------ Essential Variables for Implementation ------//
	int selectIdx = 0;	// 메인화면에서 슬롯 선택 위치를 판단하는 인덱스변수
	int currentframe;	// 현재 프레임수를 입력받을 변수
	int playtime;		// 게임 플레이 소요 시간을 기억할 변수
	int loadframe;		// 로딩 화면 시간 delay를 위한 프레임 기억 변수
	struct Node * head = NULL;	// 랭킹 연결리스트의 헤드
	int numOfRanks = 0;	// 연결리스트의 노드 개수 기억
	char printScore[10];// 게임 플레이 간 스코어 표시를 위한 문자열
	char date[10];		// 게임 플레이 종료 후 날짜 및 시간을 기억할 문자열
	int currentTopRanker = 999;	// 현재 랭킹 시스템에서 최고점을 기억할 변수

	//------ Flag Variables for Efficient Branching ------//
	int isOpen;			// maze.maz 파일 정상 오픈 여부 플래그
	int first_heap_alloc_flag = 1;	// 게임 최초 실행 시의 메모리 할당 플래그
	int first_line_flag = 1;	// 미로 생성 시 첫 번째 라인을 구성할 플래그
	int last_line_flag = 0;		// 미로 생성 시 마지막 라인을 구성할 플래그
	int initscrflag = 1;// 게임 초기화면 표시 플래그
	int isload = 0;		// 로딩화면 표시 플래그
	bool isgame;		// 게임 시작 플래그
	int upflag = 0;		// '상'키가 눌렸을 때의 플래그
	int downflag = 1;	// '하'키가 눌렸을 때의 플래그
	int rightflag = 0;	// '우'키가 눌렸을 때의 플래그
	int leftflag = 0;	// '좌'키가 눌렸을 때의 플래그
	int resetinitflag = 1;	// 10초 간격 리셋을 위한 10초 소요 여부 플래그
	int reachtargetflag = 0;// 목적지 도달 여부 플래그
	int endsoundflag = 0;	// 게임 종료 플래그
	bool isdfs;			// DFS 결과 표시 플래그
	int gameexit = 0;	// 게임 종료 플래그
	int rankscrflag = 0;// 랭킹 표현 플래그

	//------ Fundamental Functions Declaration ------//
	void freeMemory();	// 매 회 게임 간 재설정 및 게임 최종 종료 시 메모리 해제
	bool readFile();	// maze.maz 파일을 읽어들여 미로 구축 및 화면 표시 준비
	Line pathFind(int i, int j, int flag);	// 미로 구축 시 보조 함수
	void randomize(int n, int *set);	// 임의 미로 생성 시 랜덤하게 초기 설정
	void randomizeH(int n, int *set);	// 미로 생성 시 수평선 랜덤하게 설정
	void randomizeV(int n);	// 미로 생성 시 수직선 랜덤하게 설정
	void makemazefile(void);// 임의의 미로를 담은 maze.maz 파일을 생성
	void drawMaze(FILE* fp, int n);	// 미로를 maze.maz에 그리는 함수
	void createRankList(void);	// rank.txt 파일을 읽어 랭킹 구축
	void writeRankFile(void);	// rank.txt 파일에 현재의 랭킹을 업데이트
	void newRank(int score);	// 매 게임 종료 시 새로운 점수를 랭킹에 삽입
	void initScreen(void);	// 게임의 초기화면을 표시하는 함수
	void loadingScreen(int loadframe);	// 로딩화면을 표시하는 함수
	void showRankBoard(void);	// 랭킹을 화면에 표시하는 함수
	bool DFS();			// 최단 경로 계산을 수행하는 DFS함수
	void dfsdraw();		// DFS 결과 최단 경로를 화면에 표시하는 함수
	void drawclock(double current);	// 게임 시 시계 동작을 표시하는 함수
	void PlayGame(void);// 1회의 게임을 전체적으로 통제하는 함수
	Point maximumDist(void);	// 10초 간격 목적지 리셋을 위한 함수

	//------ Base OFW & GUI Interface Declaration ------//
	ofTrueTypeFont myFont;
	float windowWidth, windowHeight;
	HWND hWnd;
	HWND hWndForeground;
	ofxWinMenu * menu;
	bool bShowInfo;
	bool bFullscreen;
	bool bTopmost;

	void setup();
	void update();
	void draw();
	void keyPressed(int key);
	void keyReleased(int key);
	void mouseMoved(int x, int y);
	void mouseDragged(int x, int y, int button);
	void mousePressed(int x, int y, int button);
	void mouseReleased(int x, int y, int button);
	void appMenuFunction(string title, bool bChecked);
	void windowResized(int w, int h);
	void dragEvent(ofDragInfo dragInfo);
	void gotMessage(ofMessage msg);
	void doFullScreen(bool bFull);
	void doTopmost(bool bTop);

	//------ External Data Declaration for Importing ------//
	ofTrueTypeFont rankplace;	// GUI 및 사운드 등 게임 구현을 위해 필요한
	ofTrueTypeFont rankplace2;	// 외부 참조 파일을 프로그램에 로드하기 위한
	ofTrueTypeFont gamescore;	// 변수 선언부

	clock_t startgame;
	clock_t endgame;
	clock_t resetcriteria;
	clock_t resetfull;
	
	ofSoundPlayer menubgm;
	ofSoundPlayer menusound;
	ofSoundPlayer gamebgm;
	ofSoundPlayer endsound;
	ofSoundPlayer whoosh;
	ofSoundPlayer hooray;

	ofImage gamelogo;
	ofImage gameover;
	ofImage loadone;
	ofImage loadtwo;
	ofImage loadthree;
	ofImage loadfour;
	ofImage ratup;
	ofImage ratdown;
	ofImage ratright;
	ofImage ratleft;
	ofImage cheese;
	ofImage playbutton;
	ofImage rankbutton;
	ofImage exitbutton;
	ofImage returnbutton;
	ofImage quitbutton;
	ofImage brickwall;
	ofImage whitewall;
	ofImage decoration;
	ofImage numberone;
	ofImage scoreboard;
	ofImage rankboard;
	ofImage clockboard;
	ofImage clocktimer1;
	ofImage clocktimer2;
	ofImage clocktimer3;
	ofImage clocktimer4;
	ofImage clocktimer5;
	ofImage clocktimer6;
	ofImage clocktimer7;
	ofImage clocktimer8;
	ofImage clocktimer9;
	ofImage clocktimer10;
	ofImage clocktimer11;
	ofImage clocktimer12;
};