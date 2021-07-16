#include "ofApp.h"
#include <string>
#include <iostream>
// Comsil Project : Where is my Cheese ?
// Mady by Junhyeok-Park 20171643 CSE / Sogang
// Period : 2021.06.18 ~ 2021.06.21
// Brief Description : Maze Game with some unique features
/*
남은 아이디어
4. 보고서 작성
5. 영상 촬영
*/
using namespace std;
clock_t inittime;

//------ Basic Setup Settings for Openframeworks Program ------//
void ofApp::setup(void) {
	ofSetWindowTitle("Where is my Cheese?");	// 프로그램 최상단 윈도우형 화면 제목
	ofSetFrameRate(15);		// OFS의 프레임 비율을 설정
	ofBackground(200, 200, 0);	// 배경화면 색상 설정 - 진한 노란색
	windowWidth = ofGetWidth()*1.5;	// 게임 화면 크기 설정
	windowHeight = ofGetHeight()*1.5;
	isdfs = false;	// isdfs 플래그 0 초기화
	isOpen = 0;		// isOpen 플래그 0 초기화

	//------ GUI Settings (Offered from TA when MAZE Practice Class) ------//
	ofSetWindowPosition((ofGetScreenWidth() - windowWidth) / 2, (ofGetScreenHeight() - windowHeight) / 2);
	ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU));
	ofShowCursor();
	myFont.loadFont("verdana.ttf", 12, true, true);
	hWnd = WindowFromDC(wglGetCurrentDC());
	ofSetEscapeQuitsApp(false);
	menu = new ofxWinMenu(this, hWnd);
	menu->CreateMenuFunction(&ofApp::appMenuFunction);
	HMENU hMenu = menu->CreateWindowMenu();
	HMENU hPopup;;
	hPopup = menu->AddPopupMenu(hMenu, "Info");
	menu->AddPopupItem(hPopup, "About", false, false);
	menu->SetWindowMenu();

	//------ External Data Loading Part ------//
	gamelogo.load("gamelogo.png");		// GUI 및 사운드 bgm 구현을 위한 각종
	playbutton.load("playbutton.png");	// 외부데이터를 로드하고, 크기, 색상, 볼륨,
	rankbutton.load("rankbutton.png");	// 속도 등을 제각각 설정하는 부분
	exitbutton.load("exitbutton.png");
	returnbutton.load("returnbutton.png");
	quitbutton.load("quitbutton.png");
	menubgm.load("menubgm.mp3");
	menubgm.setVolume(0.1);
	menusound.load("menusound.mp3");
	menusound.setVolume(0.6);
	menusound.setSpeed(2);
	if (initscrflag)
		menubgm.play();
	endsound.load("endsound.mp3");
	endsound.setVolume(0.4);
	gamebgm.load("gamebgm.mp3");
	gamebgm.setVolume(0.2);
	ratup.load("ratup.png");
	ratdown.load("ratdown.png");
	ratright.load("ratright.png");
	ratleft.load("ratleft.png");
	cheese.load("cheese.png");
	brickwall.load("brickwall.jpg");
	whitewall.load("whitewall.jpg");
	scoreboard.load("scoreboard.png");
	loadone.load("loadone.png");
	loadtwo.load("loadtwo.png");
	loadthree.load("loadthree.png");
	loadfour.load("loadfour.png");
	rankboard.load("rankboard.png");
	rankplace.load("verdana.ttf", 12);
	rankplace2.load("verdana.ttf", 24);
	gamescore.load("verdana.ttf", 50);
	clocktimer1.load("clocktimer1.png");
	clocktimer2.load("clocktimer2.png");
	clocktimer3.load("clocktimer3.png");
	clocktimer4.load("clocktimer4.png");
	clocktimer5.load("clocktimer5.png");
	clocktimer6.load("clocktimer6.png");
	clocktimer7.load("clocktimer7.png");
	clocktimer8.load("clocktimer8.png");
	clocktimer9.load("clocktimer9.png");
	clocktimer10.load("clocktimer10.png");
	clocktimer11.load("clocktimer11.png");
	clocktimer12.load("clocktimer12.png");
	clockboard.load("clockboard.png");
	gameover.load("gameover.png");
	whoosh.load("whoosh.mp3");
	whoosh.setVolume(0.2);
	numberone.load("numberone.png");
	hooray.load("hooray.mp3");
	hooray.setSpeed(1.4);
	hooray.setVolume(0.2);
	decoration.load("decoration.png");

	inittime = clock();	// 게임 시작 시간을 기억
	createRankList();	// rank.txt 파일을 읽어 미리 랭킹 구축
	bShowInfo = 1;
}

//------ Graphic User Interface Setting Part ------//
void ofApp::doFullScreen(bool bFull) {
	if (bFull) {
		menu->RemoveWindowMenu();
		ofHideCursor();
		ofSetFullscreen(true);
	}
	else {
		ofSetFullscreen(false);
		menu->SetWindowMenu();
		ofSetWindowShape(windowWidth, windowHeight + GetSystemMetrics(SM_CYMENU));
		ofSetWindowPosition((ofGetScreenWidth() - ofGetWidth()) / 2, (ofGetScreenHeight() - ofGetHeight()) / 2);
		ofShowCursor();
		if (bTopmost) doTopmost(true);
	}
}

void ofApp::doTopmost(bool bTop) {
	if (bTop) {
		hWndForeground = GetForegroundWindow();

		SetWindowPos(hWnd, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);
	}
	else {
		SetWindowPos(hWnd, HWND_NOTOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		ShowWindow(hWnd, SW_SHOW);

		if (GetWindowLong(hWndForeground, GWL_EXSTYLE) & WS_EX_TOPMOST)
			SetWindowPos(hWndForeground, HWND_TOPMOST, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
		else
			SetWindowPos(hWndForeground, HWND_TOP, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE);
	}
}

void ofApp::appMenuFunction(string title, bool bChecked) {
	ofFileDialogResult result;
	string filePath;
	size_t pos;

	if (title == "About")
		ofSystemAlertDialog("ID : 20171643\nName : Junhyeok Park\nComsil Project\n");
}

//------ Some Empty OFS Functions ------//
void ofApp::update() {

}

void ofApp::keyReleased(int key) {

}

void ofApp::mouseMoved(int x, int y) {

}

void ofApp::mouseDragged(int x, int y, int button) {

}

void ofApp::mousePressed(int x, int y, int button) {

}

void ofApp::mouseReleased(int x, int y, int button) {

}

void ofApp::windowResized(int w, int h) {

}

void ofApp::gotMessage(ofMessage msg) {

}

void ofApp::dragEvent(ofDragInfo dragInfo) {

}

//------ Core Functions Operating on frame-by-frame basis ------//
void ofApp::keyPressed(int key) {	// 키보드 입력에 대한 처리 부분
	if (key == VK_ESCAPE) {	// esc 누르면 종료 되도록 설정
		ofExit();
	}
	// 초기화면에서 play 슬롯을 선택한 상황에서 엔터키를 누르는 경우 : 게임 실행
	if (key == OF_KEY_RETURN && selectIdx == 0 && initscrflag == 1) {
		initscrflag = 0;	// 초기화면 해제
		reachtargetflag = 0;	// 목적지 도달 플래그 0 초기화
		menubgm.stop();	// 초기화면 bgm 해제
		freeMemory();	// 초기 설정으로 복구
		readFile();	// maze.maz 파일을 읽어 미로 구축

		if (isOpen) {	// maze.maz 파일 정상 오픈 시
			isload = 1;	// 로딩화면 표시 (순전히 게임적 특성을 위함.)
			coordX = coordY = 0;	// 쥐의 초기 위치 설정
			targetX = WIDTH - 3;	// 치즈의 초기 위치 설정
			targetY = HEIGHT - 3;
		}
	}
	// 초기화면에서 rank 슬롯을 선택한 상황에서 엔터키를 누르는 경우 : 랭킹 표시
	if (key == OF_KEY_RETURN && selectIdx == 1 && initscrflag == 1) {
		menusound.play();	// 슬롯 선택 사운드
		initscrflag = 0;	// 초기화면 해제
		rankscrflag = 1;	// 랭킹화면 표시
	}
	// 랭킹 표시 화면에서 다시 엔터키를 누르는 경우 : 초기화면으로 복귀
	else if (key == OF_KEY_RETURN && selectIdx == 1 && rankscrflag == 1) {
		menusound.play();	// 슬롯 선택 사운드
		initscrflag = 1;	// 초기화면 표시
		rankscrflag = 0;	// 랭킹화면 해제
	}
	// 초기화면에서 exit 슬롯을 선택한 상황에서 엔터키를 누르는 경우 : 프로그램 종료
	if (key == OF_KEY_RETURN && selectIdx == 2 && initscrflag == 1) {
		menusound.play();	// 슬롯 선택 사운드
		gameexit = 1;		// heap 메모리 해제를 위한 종료 플래그
		freeMemory();	// 모든 힙 할당 메모리 해제
		ofExit();	// 프로그램 종료
	}

	if (initscrflag && !isgame) {	// 초기화면에서
		if (key == OF_KEY_UP) {	// '상'키를 누르는 경우
			menusound.play();	// 슬롯 이동 사운드
			selectIdx--;	// 슬롯 표시 인덱스 변경
			if (selectIdx < 0)	// 제한점 설정
				selectIdx = 0;
		}

		if (key == OF_KEY_DOWN) {	// '하'키를 누르는 경우
			menusound.play();	// 위의 조건의 반대급부 수행
			selectIdx++;
			if (selectIdx == 3)
				selectIdx = 2;
		}
	}
	// 게임 실행 상태 및 최종 목적지에 도달하지 않은 상태에서의 키입력 처리
	if (isgame && !isload && !(coordX == targetX && coordY == targetY)) {
		if (key == 'q' || key == 'Q') {	// q키를 누르는 경우 1회 게임 탈출
			initscrflag = 1;	// 다시 초기화면 표시
			isgame = 0;		// 게임 화면 해제
			gamebgm.stop();	// 게임 bgm 해제
			menubgm.play();	// 초기화면 bgm 실행
		}

		if (key == OF_KEY_RIGHT) {	// '우'키를 누르는 경우
			if (coordX < WIDTH - 4) {	// 미로 최우측의 경우 이동 불가
				if (Maze[1 + coordY][1 + coordX + 1] == ' ') { // 미로 뚫려있을때
					coordX += 2;	// 이동
					rightflag = 1;	// 우측 플래그 설정 -> 쥐의 모양 변경을 위해
					leftflag = upflag = downflag = 0; // 다른 방향 플래그 해제
				}
			}
		}

		if (key == OF_KEY_LEFT) {	// '좌'키를 누르는 경우
			if (coordX) {	// 위의 조건부와 유사하게 작동
				if (Maze[1 + coordY][1 + coordX - 1] == ' ') {
					coordX -= 2;
					leftflag = 1;
					rightflag = upflag = downflag = 0;
				}
			}
		}

		if (key == OF_KEY_UP) {
			if (coordY) {
				if (Maze[1 + coordY - 1][1 + coordX] == ' ') {
					coordY -= 2;
					upflag = 1;
					leftflag = rightflag = downflag = 0;
				}
			}
		}

		if (key == OF_KEY_DOWN) {
			if (coordY < HEIGHT - 4) {
				if (Maze[1 + coordY + 1][1 + coordX] == ' ') {
					coordY += 2;
					downflag = 1;
					leftflag = upflag = rightflag = 0;
				}
			}
		}
	}
}

void ofApp::draw() {	// 매 프레임 별로 화면을 그리는 함수
	int i, sec;	// 인덱스 변수 i와, 초를 기억할 sec변수
	char str[256];	// 저작권 문자열 표시
	Point temp;	// 가장 먼 꼭지점 좌표를 리턴받을 변수
	ofSetColor(255, 255, 255);	// 모든 로드 이미지파일의 밝기 최대 설정
	ofSetLineWidth(5);	// 라인 굵기를 5로 설정
	brickwall.draw(0, 0, windowWidth, windowHeight);	// 게임의 바탕화면 :벽돌

	if (initscrflag) {	// 초기화면 플래그 설정 시
		initScreen();	// 초기화면 표시 함수
		loadframe = ofGetFrameNum();	// 로딩화면 표시 시 로딩 시작 프레임 기억
	}	// 을 위한 설정

	if (rankscrflag) {	// 랭킹화면 플래그 설정 시
		rankboard.draw(33, 0, 900, 600);	// 랭킹보드 그림
		returnbutton.draw(670, 580, 210, 110);	// 복귀버튼 그림
		showRankBoard();	// 랭킹 화면에 표시 함수
	}

	if (isload)	// 로딩화면 플래그 설정 시
		loadingScreen(loadframe);	// 로딩화면을 일정 프레임 동안 표시
	// 순전히 게임적 요소를 갖추기 위한 로딩화면일 뿐, 실질적 데이터 로딩과정 전무
	if (isgame) {	// 게임 플레이 플래그 설정 시
		whitewall.draw(10, 10, 600, 600);	// GUI구축 이미지 파일 표시
		quitbutton.draw(650, 580, 180, 80);	// 종료 버튼
		scoreboard.draw(637, 75, 300, 200);	// 스코어보드
		clockboard.draw(710, 350, 150, 150);// 시계
		decoration.draw(800, 420, 150, 250);// 우측 하단 쥐 그림

		if (!reachtargetflag) {	// 목적지에 도달하지 못했을 때
			if (resetinitflag) {	// 10초 간격으로 플래그 설정 및 해제
				resetcriteria = clock();	// 10초 간격 계산 기준점
				resetinitflag = 0;
			}
			resetfull = clock();	// 10초 간격 계산 끝점
			drawclock(resetfull);	// 매 초마다 시계그림 회전 기능
			sec = (int)((resetfull - resetcriteria) / (double)1000);
			// 기준점으로부터 흐른 시간 초를 기억하는 sec변수
			if (sec > 9) {	// 10초가 지난 경우
				temp = maximumDist();	// 현 위치에서 가장 먼 꼭짓점 확인
				targetX = temp.x;	// 해당 꼭지점으로 목적지 변경
				targetY = temp.y;
				whoosh.play();	// 치즈가 움직이는 사운드 효과
				resetinitflag = 1;	// 다시 10초를 세기 위한 플래그 설정
			}
		}
		else
			drawclock(resetfull);	// 목적지 도달한 경우 시계 멈추도록 설정

		ofSetColor(0, 0, 0);	// 미로를 그리는 선은 까맣게 표시
		for (i = 0; i < linesSize; i++)
			ofDrawLine(20 * lines[i].x1, 20 * lines[i].y1, 20 * lines[i].x2, 20 * lines[i].y2);
		// 미로를 그리는 반복문
		ofSetColor(255, 255, 255);	// 미로 제외하고는 모두 밝기 최대
		if (isOpen) {	// 에러 처리
			if (isdfs)	// DFS가 정상적으로 수행되었다면,
				dfsdraw();	// DFS 결과인 최단경로를 화면에 표시
			PlayGame();	// 게임 플레이를 진행하는 게임 플레이 메인 함수(추후 설명)
		}
	}

	if (bShowInfo) {
		ofSetColor(255);	// 카피 라이트 문자열 표시
		sprintf(str, "Copyright 2021. joon hyeok comsil project. all rights reserved.");
		myFont.drawString(str, 15, ofGetHeight() - 20);
	}
}

//------ Assistant Functions for draw() in 'Game Play Mode' ------//
// 치즈의 10초 간격 새로운 위치를 찾아주는 함수. 미로의 4개의 꼭짓점들이
// 후보이다. 아래의 temp배열은 이들을 가리킨다.
// 현위치를 기준으로 절대 거리를 각각 계산하여 판단한다.
Point ofApp::maximumDist(void) {
	int maxIdx;	// 가장 거리간 먼 꼭지점의 인덱스 기억
	int maximum;// 최댓값 비교를 위한 변수
	Point temp[4] = { {WIDTH - 3,HEIGHT - 3},{WIDTH - 3,0},{0,HEIGHT - 3},{0,0} };
	Point tmp = temp[0];	// 초기 설정

	maximum = (coordX - tmp.x)*(coordX - tmp.x) + (coordY - tmp.y)*(coordY - tmp.y);
	maxIdx = 0;
	// 가장 거리가 먼 지점 찾는 과정 : 점과 점 사이의 거리를 피타고라스정리를 이용해
	tmp = temp[1];	// '절대적인 거리'를 비교함. 경로의 개수가 아님.
	if (maximum < (coordX - tmp.x)*(coordX - tmp.x) + (coordY - tmp.y)*(coordY - tmp.y)) {
		maximum = (coordX - tmp.x)*(coordX - tmp.x) + (coordY - tmp.y)*(coordY - tmp.y);
		maxIdx = 1;
	}
	tmp = temp[2];
	if (maximum < (coordX - tmp.x)*(coordX - tmp.x) + (coordY - tmp.y)*(coordY - tmp.y)) {
		maximum = (coordX - tmp.x)*(coordX - tmp.x) + (coordY - tmp.y)*(coordY - tmp.y);
		maxIdx = 2;
	}
	tmp = temp[3];
	if (maximum < (coordX - tmp.x)*(coordX - tmp.x) + (coordY - tmp.y)*(coordY - tmp.y)) {
		maximum = (coordX - tmp.x)*(coordX - tmp.x) + (coordY - tmp.y)*(coordY - tmp.y);
		maxIdx = 3;
	}

	return temp[maxIdx];	// 절대거리가 가장 먼 꼭지점을 반환
}

void ofApp::drawclock(double current) {	// 게임플레이 동안 시계를 그리는 함수
	switch ((int)((resetfull - inittime) / (double)1000) % 12) { // 초별로
	case 0:
		clocktimer1.draw(710, 350, 150, 150);
		break;
	case 1:	// 매초 간격으로 시계가 동적으로 움직이는 것처럼 보이게 하는 함수이다.
		clocktimer2.draw(710, 350, 150, 150);
		break;	// 실제로는 이처럼 12개의 상황별 그림을 보여주는 과정이다.
	case 2:
		clocktimer3.draw(710, 350, 150, 150);
		break;
	case 3:
		clocktimer4.draw(710, 350, 150, 150);
		break;
	case 4:
		clocktimer5.draw(710, 350, 150, 150);
		break;
	case 5:
		clocktimer6.draw(710, 350, 150, 150);
		break;
	case 6:
		clocktimer7.draw(710, 350, 150, 150);
		break;
	case 7:
		clocktimer8.draw(710, 350, 150, 150);
		break;
	case 8:
		clocktimer9.draw(710, 350, 150, 150);
		break;
	case 9:
		clocktimer10.draw(710, 350, 150, 150);
		break;
	case 10:
		clocktimer11.draw(710, 350, 150, 150);
		break;
	case 11:
		clocktimer12.draw(710, 350, 150, 150);
		break;
	}
}

//------ Create a random basis maze of WIDTH 15, HEIGHT 15 ------//
void ofApp::makemazefile(void) {	// 매 게임 회차를 위한 임의의 미로를 생성하는 함수
	FILE*fp = fopen("maze.maz", "wt");	// 파일을 작성하도록 스트림 설정
	int width, height;	// 폭과 높이를 기억
	int set = 0, i;	// 수업 시간에 사용했던 Eller's Algorithm을 사용한다.
	width = 15;
	height = 15;

	if (first_heap_alloc_flag) {	// 최초 게임 실행 시에만 메모리를 할당한다.
		rows = (int*)malloc(sizeof(int)*width);
		vert = (int*)malloc(sizeof(int)*(width - 1));
		horiz = (int*)malloc(sizeof(int)*width);
	}	// 다 차례 게임 회차 실행 시 메모리가 터지지 않게 하기 위해 메모리를 한 번만
	// 할당하고 최초 이후 게임 실행부터는 해당 메모리를 재사용하는 것이다.

	for (i = 0; i < width; i++)
		fprintf(fp, "+-");	// 최상단 테두리 그림
	fprintf(fp, "+\n");

	for (i = 0; i < height; i++) {	// 높이별로 순회하며,
		if (first_line_flag)	// 첫 번째 줄을 그릴 때는
			randomize(width, &set);	// Randomize 함수 호출
		else {	// 그 외의 줄에 대해서는,
			randomizeH(width, &set);	// 수평선 별로 랜덤하게 설정
			if (i == (height - 1))	// 마지막 라인일 때만 따로 분기
				last_line_flag = 1;
			randomizeV(width);	// 수직선 별로 랜덤하게 설정
		}
		drawMaze(fp, width);	// 미로를 maze.maz 파일에 문자들로 그린다.
		first_line_flag = 0;	// 플래그 변수 해제
	}
	for (i = 0; i < width; i++)
		fprintf(fp, "+-");	// 최하단 테두리 그림
	fprintf(fp, "+\n");

	fclose(fp);
}

void ofApp::randomize(int n, int *set) {	// 최초 seed 배정 함수
	int prevIdx = -1;	// 초기 set 초기화
	int i, j;

	srand(time(NULL));	// seed 배정
	for (i = 0; i < n - 1; i++) {
		vert[i] = rand() % 2;	// 수직선을 랜덤하게 설정

		if (vert[i]) {	// 수직선이 있는 경우
			for (j = ++prevIdx; j <= i; j++)	// set를 나눈다.
				rows[j] = *set;
			prevIdx = i;	// prevIdx가 i를 기억
			(*set)++;	// set가 늘어나야함. (당연)
		}
	}

	for (i = ++prevIdx; i < n; i++)
		rows[i] = *set;	// 한 줄에 대하여 set 배정을 수행
	(*set)++;
}

void ofApp::randomizeV(int n) {	// 지속적 수직선 랜덤 배정 함수
	int prevSet;
	int flag = 0;	// 랜덤 할당 플래그
	int i, j;

	for (i = 0; i < n - 1; i++) {
		if (rows[i] != rows[i + 1]) {	// set가 바뀌는 지점에서
			if (last_line_flag) flag = 1;	// 마지막 라인인 경우에만 특별 분기
			else flag = rand() % 2;	// 그 외의 라인은 모두 랜덤하게 설정

			if (flag) {	// 랜덤플래그가 1일 때
				vert[i] = 0;	// 수직선이 없다고 설정

				prevSet = rows[i + 1];	// set를 끌어와서
				rows[i + 1] = rows[i];

				for (j = 0; j < n; j++)	// set를 합친다.
					if (rows[j] == prevSet)
						rows[j] = rows[i];
			}
			else
				vert[i] = 1;	// 그 외에는 수직선이 존재
		}
		else
			vert[i] = 1;	// set가 같은 경우엔 vert[i]=1로 설정
	}
}

void ofApp::randomizeH(int n, int *set) {	// 지속적 수평선 랜덤 배정 함수
	int prevSet;
	int flag = 0;	// 수평선 존재 유무를 가리킬 플래그 변수
	int i;

	prevSet = rows[0];	// 초기 set
	for (i = 0; i < n; i++) {
		horiz[i] = rand() % 2;	// 수평선 임의 배정

		if (!horiz[i])	// 없는 경우엔 플래그 변수를 설정
			flag = 1;

		if (i < n - 1) {	// 라인을 순회하는데,
			if (rows[i + 1] != prevSet) {	// set가 다르다면,
				if (!flag)	// 이때 수평선이 없는 경우
					horiz[i] = 0;	// 그대로 수평선 없게 냅두고
				else	// 수평선이 있는 경우
					flag = 0;	// 플래그 변수 해제

				prevSet = rows[i + 1];	// 앞의 set를 업데이트
			}
		}
		if ((i == n - 1) && !flag)	// 플래그가 해제되어 있고, 마지막 인덱스일 땐
			horiz[i] = 0;	// 특수하게 수평선 없게 설정

		if (horiz[i]) {	// 수평선이 있을 때는 set를 그대로 따라게 하면 된다.
			rows[i] = *set;
			(*set)++;
		}
	}
}

//------ Read 'Maze.maz' file to create a random basis graphic maze ------//
// 임의 미로 생성 기능을 통해 생성된 maze.maz 파일을 읽어 OFS 상에서 실제로 미로를
// 구축하는 함수
bool ofApp::readFile(void) {
	ifstream inputFile, tempFile;
	string line;
	int i, j;
	makemazefile();	// 우선 앞서서 임의 미로를 생성해놓고(maze.maz를 생성해놓고)
	HEIGHT = 0;

	inputFile.open("maze.maz");	// 파일을 열어 읽는다.
	tempFile.open("maze.maz");

	if (inputFile.is_open()) {	// 파일이 정상적으로 열렸다면,
		ofFile file("maze.maz"); // 한 번 더 확인
		string tempStr;

		if (!file.exists()) {	// 파일이 안열렸다면 오류 처리
			cout << "Target file does not exists." << endl;
			return false;
		}
		else {	// 파일을 찾은 경우 isOpen 플래그 설정
			cout << "We found the target file." << endl;
			isOpen = 1;
		}

		while (getline(inputFile, line)) {	// maze.maz파일을 줄별로 읽음
			WIDTH = line.length();	// 한 줄의 길이를 폭으로 기록
			HEIGHT++;	// 높이를 카운트
		}

		if (first_heap_alloc_flag) {	// 최초 게임 실행 시에만 메모리 할당
			Maze = new char*[HEIGHT];	// 그 외에는 메모리 효율성을 위해 기할당
			for (i = 0; i < HEIGHT; i++)	// 메모리를 재사용할 것
				Maze[i] = new char[WIDTH];
			lines = new struct Line[HEIGHT*WIDTH + 10];	// 적절하게 크기 배정
		}

		for (i = 0; i < HEIGHT; i++) {	// 라인 별로
			getline(tempFile, tempStr);	// 문자열을 읽어
			for (j = 0; j < WIDTH; j++)
				Maze[i][j] = tempStr[j];	// 해당 문자열을 미로 배열에 문자별 기록
		}
		shortRoute = new struct Point[10000];	// 최단 경로 배열 할당 (동적)
		allRoute = new struct Point[10000];	// 전체 경로 배열 할당 (동적)
		// 아래는 미로 배열을 기반으로 미로 그리기를 위한 기반 구축 루프
		for (i = 0; i < HEIGHT; i++) {
			for (j = 0; j < WIDTH; j++) {
				if (Maze[i][j] == '-')
					lines[linesSize++] = pathFind(i, j, 1); // 적절한 크기 설정
				else if (Maze[i][j] == '|')
					lines[linesSize++] = pathFind(i, j, 0);
			}
		}
		return true;
	}
	else {
		printf("  Needs a '.maz' extension\n");	// 에러처리
		return false;	// 실습 코드의 잔존물
	}
}

Line ofApp::pathFind(int i, int j, int flag) {	// 미로를 화면 상에 적합하게 그리
	Line path = { j,i,j,i };	// 기 위해 직선의 끝점 간격을 적절하게 조정해서 반
	// 환해주는 함수로, 해당 작업이 전체 코드에서 몇 차례 반복적으로 일어나기 때문에
	if (flag) {	// 이를 함수모듈화하였다.
		path.x1 -= 0.5;
		path.y1 += 0.5;
		path.x2 += 1.5;
		path.y2 += 0.5;
	}
	else {
		path.x1 += 0.5;
		path.y1 -= 0.5;
		path.x2 += 0.5;
		path.y2 += 1.5;
	}

	return path;	// Line 구조체 변수를 반환함.
}

//------ Main Function for 'Game Play Mode' ------//
// 1회의 게임 플레이를 전체적으로 통제하는 이번 프로젝트의 핵심 함수
// 프레임 별로, 상황 플래그 별로, 키 입력 별로 모두 다른 동작을 하는
// 함수이다. 함수에 대한 자세한 설명은 주석으로 이어간다.
void ofApp::PlayGame() {
	Point coord;	// 현위치를 기억할 변수
	coord.x = coordX;	// 현 위치 업데이트
	coord.y = coordY;
	// 목적지에 치즈를 그린다.
	cheese.draw(20 * targetX + 15, 20 * targetY + 15, 35, 35);

	if (coordX != targetX || coordY != targetY)	// 목적지에 도달하지 않은
		allRoute[aIdx++] = coord;	// 모든 이동을 allRoute배열에 기록한다.

	if (upflag)	// '상'키가 눌렸을 경우 쥐가 위를 바라보게 한다.
		ratup.draw(20 * coordX + 15, 20 * coordY + 15, 35, 35);
	else if (downflag)	// '하'키가 눌렸을 경우 쥐가 아래를 바라보게 한다.
		ratdown.draw(20 * coordX + 15, 20 * coordY + 15, 35, 35);
	else if (rightflag)	// 이하 유사 기능
		ratright.draw(20 * coordX + 15, 20 * coordY + 15, 35, 35);
	else if (leftflag)
		ratleft.draw(20 * coordX + 15, 20 * coordY + 15, 35, 35);
	printf("(%d, %d)\n", coordX, coordY);	// 현재 좌표를 콘솔 상에 표시

	if (coordX == targetX && coordY == targetY) {	// 치즈에 도달한 경우
		reachtargetflag = 1;	// 목적지 도달 플래그 설정

		gamebgm.stop();	// 게임 bgm을 끈다.
		if (!endsoundflag) {	// 정확히 '종료 시점'에 대해서 (플래그 이용)
			DFS();	// 최단 경로 계산 with DFS
			isdfs = 1;	// DFS 결과를 그리라는 명령을 draw함수에 전달
			endgame = clock();	// 게임이 종료된 시점을 기록
			playtime = (int)((endgame - startgame) / (double)1000); // 소요시간
			playtime += aIdx / 2;	// 과 전체경로 인덱스를 이용하여 점수를 계산
			if (playtime > 999)	// 점수가 999점 이상인 경우(정말 낮은 점수)
				playtime = 999;	// 무의미하다 판단하여 이때부터는 모두 999점으로 통일
			if (playtime < currentTopRanker)	// 현재 플레이 점수가 1등에 해당하면
				hooray.play();	// 1등 기념 효과음 출력
			else	// 1등이 아니면 모두 통일된 종료 사운드 실행
				endsound.play();

			currentframe = ofGetFrameNum() + 104;	// 잠시 대기를 위한 프레임 계산
			endsoundflag = 1;	// '종료 시점'만을 나누기 위한 플래그 설정
		}	// 시점을 정확히 나눈 이유 : 위의 조건문 상황이 프레임별로 반복하면 안됨.
		ofSetColor(255, 255, 255);	// 밝기 최대 설정하여
		gameover.draw(60, 50, 480, 480);	// 게임 종료 상황을 그림

		sprintf(printScore, "%d", playtime);	// 점수를 화면에 표시할 문자열 설정
		ofSetColor(0, 0, 0);	// 게임 스코어 문자열은 어둡게 표시하기 위함
		gamescore.drawString(printScore, 720, 200);	// 화면에 표시
		ofSetColor(255, 255, 255);	// 점수를 제외하고는 다시 밝게 표시하기 위함

		if (playtime < currentTopRanker)	// 1등일 때는 1등 기념 그림을 표시함.
			numberone.draw(33, 0, 900, 600);

		if ((currentframe - ofGetFrameNum()) % 105 == 0) {	// 잠시 대기 시간이 지나면
			newRank(playtime);	// 점수를 랭킹에 포함시키고
			initscrflag = 1;	// 초기화면으로 복귀
			isgame = 0;	// 1회의 플레이 종료
			menubgm.play();	// 초기화면 bgm을 다시 튼다.
		}
	}
	else {	// 목적지에 도달하지 못한 경우 (전반적인 상황)
		playtime = (int)((clock() - startgame) / (double)1000);
		playtime += aIdx / 2;	// 현 점수를
		if (playtime > 999)
			playtime = 999;
		sprintf(printScore, "%d", playtime);	// 화면에 표시한다.
		ofSetColor(100, 100, 100);	// 최종 점수 표시와 색을 구분하였다.
		gamescore.drawString(printScore, 720, 200);
		ofSetColor(255, 255, 255);
	}
}

//------ Find the shortest route when user reached the target ------//
// 목적지에 도달한 경우, 해당 목적지까지의 최단경로를 계산할 DFS 함수.
// 이때, 게임 플레이 상으로 리셋된 치즈의 최종 위치만을 판단한다.
// 즉, 최초 시작점 coordX = 0, coordY =0에서부터 종료 시점에서의 최종 치즈
// 위치까지의 최단 경로를 보이는 것이다. 중간에 지나친 목적지들은 고려하지 않음.
bool ofApp::DFS() {
	int moveOrNot;	// 움직일지 말지를 정하는 플래그 변수
	int i, j;
	struct Point p1, p2;

	if (first_heap_alloc_flag) {	// 최초 게임 시작 시의 동적할당
		Visit = new int*[HEIGHT];	// 그외에는 메모리 재사용할 것(상기)
		for (i = 0; i < HEIGHT; i++)
			Visit[i] = new int[WIDTH];
		first_heap_alloc_flag = 0;	// 이 친구가 최초 시점의 가장 마지막 할당이기
	}	// 때문에 여기에서 플래그를 해제한다.

	for (i = 0; i < HEIGHT; i++) {	// 미로 배열을 기반으로
		for (j = 0; j < WIDTH; j++) {	// 움직일 수 있는 곳과 없는 곳을 Visit배열
			if (Maze[i][j] != ' ')	// 에 기록해둔다.
				Visit[i][j] = 1;
			else
				Visit[i][j] = 0;
		}
	}
	Visit[1][1] = 1;	// 최초 시작점은 이미 방문한 것으로 여긴다.
	p1.x = 1;	// 시작점 : 이때, 중요한 점은, OFS 화면 표시 좌표와, DFS 계산 시의
	p1.y = 1;	// 좌표가 다르다는 것을 인식하는 것이다.

	shortRoute[sIdx++] = p1;	// 스택에 푸쉬

	while (sIdx) {	// 스택이 비기 전까지
		if (shortRoute[sIdx - 1].x == (targetX + 1) && shortRoute[sIdx - 1].y == (targetY + 1)) {
			printf("Reached the Destination!!\n");
			return true;	// 목적지에 도달하면 종료
		}

		moveOrNot = 0;	// 우선 이동하지 않음으로 설정
		for (i = 0; i < 4; i++) {	// 이동가능한지 확인하는 부분
			p2.x = shortRoute[sIdx - 1].x + move[i].x;	// 새 고려 위치
			p2.y = shortRoute[sIdx - 1].y + move[i].y;

			if (!Visit[p2.y][p2.x]) {	// 해당 위치가 아직 미방문 위치라면
				moveOrNot = 1;	// 이동하기로 결정!
				// 해당 이동 위치를 최단경로 스택에 푸쉬한다. 방문여부도 당연 기록
				Visit[shortRoute[sIdx - 1].y + move[i].y][shortRoute[sIdx - 1].x + move[i].x] = 1;
				shortRoute[sIdx++] = p2;
				break;
			}
		}
		if (!moveOrNot)	// 이동 가능성이 없다면 이동가능성이 있을 때까지 스택을
			sIdx--;	// 팝한다.
	}
}

void ofApp::dfsdraw() {	// DFS 결과를 토대로 최단경로를 화면에 표시하는 함수
	int wholePath = aIdx;	// 게임이 종료되는 시점에만 발동한다.
	int shortestPath = sIdx;	// 인덱스를 기억
	int i;

	for (i = 0; i < wholePath; i++) {
		allroute.addVertex(20 * allRoute[aIdx - 1].x + 30, 20 * allRoute[aIdx - 1].y + 30);
		aIdx--;	// 전체 경로를 화면에 빨간색으로 표시한다.
	}
	for (i = 0; i < shortestPath; i++) {
		shortroute.addVertex(20 * shortRoute[sIdx - 1].x + 10, 20 * shortRoute[sIdx - 1].y + 10);
		sIdx--;	// 최단 경로를 화면에 녹색으로 표시한다.
	}
	ofSetLineWidth(5);
	ofSetColor(140, 0, 0);
	allroute.draw();	// 배열을 토대로 ofPolyLine OFS 문법을 이용해 간단하게
	ofSetColor(0, 140, 0);	// 그려준다. ofPolyLine이기 때문에 오버라이드되면서
	shortroute.draw();	// 그려진다는 것을 인식한다.
}

//------ Functions for Graphic User Interface ------//
// 앞선, 임의 미로 생성 함수에서 미로를 maze.maz 파일에 문자 단위로 출력하는 함수
void ofApp::drawMaze(FILE* fp, int n) {
	int i;

	if (!first_line_flag) {	// 첫 째 라인이 아닐 때의 출력
		fprintf(fp, "+");

		for (i = 0; i < n; i++) {
			if (horiz[i])	// 수평선이 있으면 아래를 출력
				fprintf(fp, "-+");
			else	// 없으면 아래를 출력
				fprintf(fp, " +");
		}
		fprintf(fp, "\n");	// 줄넘김
	}
	fprintf(fp, "|");	// 테두리를 그린다.
	for (i = 0; i < n - 1; i++) {	// 수직선을 그리는 루프
		fprintf(fp, " ");

		if (vert[i])
			fprintf(fp, "|");
		else
			fprintf(fp, " ");	// 빈 공간
	}
	fprintf(fp, " |\n");
}

void ofApp::initScreen(void) {	// 게임의 초기화면을 그리는 함수
	gamelogo.draw(50, 0, 900, 600);	// 로고를 그려주고
	switch (selectIdx) {	// 슬롯 선택 인덱스에 따라
	case 0:	// 선택된 슬롯을 그럴듯한 게임처럼, 동적으로 크기를 키워준다.
		playbutton.draw(375, 345, 210, 110); // 즉, 선택되었음을 명시적으로
		rankbutton.draw(380, 450, 200, 100); // 표현하는 것이다.
		exitbutton.draw(380, 550, 200, 100); // 미선택 슬롯은 그대로 출력
		break;
	case 1:
		playbutton.draw(380, 350, 200, 100);
		rankbutton.draw(375, 445, 210, 110);
		exitbutton.draw(380, 550, 200, 100);
		break;
	case 2:
		playbutton.draw(380, 350, 200, 100);
		rankbutton.draw(380, 450, 200, 100);
		exitbutton.draw(375, 545, 210, 110);
	}
}

void ofApp::loadingScreen(int loadframe) { // 로딩화면을 그리는 함수
	int currentframe = ofGetFrameNum();	// 로딩화면은 프레임 범위 별로 동작이 다르다.
	int frameN = currentframe - loadframe;
	// 아이폰 로딩 모양을 본뜬 부분으로, 일반적인 게임의 로딩화면에서 아이디어를 얻었다.
	if (frameN < 15)	// 프레임별로 정해진 그림을 애니메이션처럼 보이는 것에 불과함.
		loadone.draw(0, 0, windowWidth, windowHeight);
	if (frameN >= 15 && frameN < 30)	// 즉, 실질적인 로딩과정은 아니다.
		loadtwo.draw(0, 0, windowWidth, windowHeight);
	if (frameN >= 30 && frameN < 45)
		loadthree.draw(0, 0, windowWidth, windowHeight);
	if (frameN >= 45 && frameN < 60)
		loadfour.draw(0, 0, windowWidth, windowHeight);

	if (frameN >= 60) {	// 60개의 프레임이 지나가면, 로딩을 멈추고, 진짜 게임 플레이
		isload = 0;	// 모드로 들어간다. isload를 해제하고, isgame를 설정한다.
		isgame = 1;
		gamebgm.play();	// 게임 bgm을 실행하며,
		startgame = clock();	// 소요시간 측정을 위한 기준점을 잡아주고 함수 종료
	} // 로딩화면을 도입한 이유는, 자연스러운 게임의 형태를 갖추게 하기 위함.
}

//------ Functions for Implementing Ranking System ------//
// 아래의 함수는, rank.txt 파일을 이용해 랭킹을 구축하는 함수이다.
// 즉, 외부에 저장지를 둠으로써 게임이 종료되었다 다시 실행되어도 기존의 랭킹이
// 유지된다.
void ofApp::createRankList(void) {
	int score;	// 점수 기억
	char temp[15];	// 기록날짜 기억
	FILE*fp = fopen("rank.txt", "rt");	// rank.txt 파일을 연다.
	struct Node* newNode;	// 연결리스트 구축을 위한 변수 선언부
	struct Node* currNode = head;	// currNode는 헤드를 기억

	if (fp == NULL || fscanf(fp, "%d", &numOfRanks) == EOF) {//에러처리
		printf("Ranking File Open Error !\n");
		return;
	}// 한편, rank.txt 파일은 부재하더라도 게임 실행에 문제가 되지는 않는다.
	// 랭킹이 없는 최초의 게임 실행 시점을 고려하였다.
	while (fscanf(fp, "%s %d", temp, &score) != EOF) {	// 파일을 줄별로 읽어
		newNode = (Node*)malloc(sizeof(Node));	// 노드 동적 할당
		newNode->score = score;
		strcpy(newNode->date, temp);	// 노드 구성
		newNode->link = NULL;

		if (head == NULL) {	// 최초 삽입일 때
			currentTopRanker = newNode->score;	// 탑 랭커를 기억. 맨 위가 1등이므로
			head = newNode;
			currNode = newNode;	// 머리 삽입
		}
		else {	// 노드의 꼬리 삽입과정
			currNode->link = newNode;
			currNode = newNode;
		}
	}
	fclose(fp);
}

void ofApp::writeRankFile(void) {	// 매 플레이 종료될때 rank.txt를 업데이트할 함수
	FILE* fp = fopen("rank.txt", "wt");	// 스트림을 구축한다.
	Node* currNode = head;
	currentTopRanker = currNode->score;	// 탑 랭커를 기억한다.

	fprintf(fp, "%d\n", numOfRanks);
	while (currNode) {	// rank.txt 파일에 쓴다.
		fprintf(fp, "%s %d\n", currNode->date, currNode->score);
		currNode = currNode->link;
	}
	fclose(fp);
}

void ofApp::newRank(int score) {	// 매 플레이 종료시마다 점수를 랭킹에 포함함.
	Node* newNode;
	Node* currNode = head;	// 머리에서부터 조회 시작할 것
	Node* prevNode = NULL;

	newNode = (Node*)malloc(sizeof(Node)); // 해당 시점을 기록한다.
	sprintf(newNode->date, "%d/%d,%d:%d", ofGetMonth(), ofGetDay(), ofGetHours(), ofGetMinutes());
	newNode->score = score;	// 이름 대신 시점으로 사용자를 구분하는 것

	while (currNode) {	// 적절한 위치를 선형적으로 탐색
		if (score <= currNode->score)
			break;
		prevNode = currNode;
		currNode = currNode->link;
	}

	newNode->link = currNode;	// 적절한 위치에 삽입
	if (prevNode == NULL)
		head = newNode;
	else
		prevNode->link = newNode;

	numOfRanks++;		// 연결리스트 크기를 업데이트
	writeRankFile();	// rank.txt 파일에 업데이트
}

void ofApp::showRankBoard(void) {	// 랭킹을 화면에 표시하는 함수
	char score[30];
	Node* currNode = head;
	int i = 0;

	while (currNode) {
		if (i == 5)	// 상위 5등까지만 보여준다.
			break;
		sprintf(score, "%s", currNode->date);	// 출력한다.
		rankplace.drawString(score, windowWidth / 2 - 68, 250 + 52 * i);
		sprintf(score, "%d", currNode->score);
		rankplace2.drawString(score, windowWidth / 2 + 75, 250 + 52 * i);
		currNode = currNode->link;
		i++;
	}
}

//------ Free all heap allocated memories during the process ------//
// 아래의 함수는 프로그램 최초 실행 때 플래그로 인해 분기되어 동적할당되었던
// 모든 변수에 대해 메모리 해제를 진행하는 함수이다. 만약, 최초 상황이 아닌 상황
// 에서 이 함수가 호출되면, 동적할당 해제를 제외하고, 플래그 변수 초기설정 복귀 및
// 각종 인덱스 변수 초기화 작업이 이루어진다.
void ofApp::freeMemory() {
	int i;
	struct Node * delNode;
	struct Node * currNode = head;

	first_line_flag = 1;	// 플래그를 모두 게임 첫 상태로 복구
	last_line_flag = 0;
	endsoundflag = 0;

	if (gameexit) {	// 프로그램 종료 시에만 동적할당 해제. 재사용을 위함.
		for (i = 0; i < HEIGHT; i++)
			delete[] Maze[i];
		delete[] Maze;
		for (i = 0; i < HEIGHT; i++)
			delete[] Visit[i];
		delete[] Visit;
		delete[] lines;
		delete[] shortRoute;
		delete[] allRoute;
		delete[] rows;
		delete[] vert;
		delete[] horiz;
		while (currNode) {
			delNode = currNode;
			currNode = currNode->link;
			free(delNode);
		}
		head = NULL;
	}
	linesSize = 0;
	sIdx = aIdx = isdfs = isOpen = 0;	// 인덱스 변수 및 플래그 변수 복구
	allroute.clear();	// ofPolyLine도 비워준다.
	shortroute.clear();
}