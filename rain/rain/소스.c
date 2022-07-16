#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS
#include <winsock2.h>
#pragma comment(lib, "ws2_32");

#define BUFSIZE 512

void err_quit(const char* msg) {
	LPVOID IpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&IpMsgBuf, 0, NULL);
	exit(-1);

}
#include <stdio.h>
#include <stdlib.h>
#include <Windows.h>
#pragma comment (lib, "winmm.lib")
#include <mmsystem.h>;
#include <time.h>
#define key_a 0x41
#define key_d 0x44
#define key_e 0x45
#define UP 0
#define DOWN 1
#define LIFT 2
#define RIGHT 3
#define SUB 4
#define USE 5

#define WIDTH 20
#define WIDTH1 15
#define HEIGHT 11
int speed;
void textcolor(int colorNum) {
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum); //폰트색 조절
}
typedef struct
{
	int x;
	int y;

	int act;
}Rain;
typedef struct
{
	int x;
	int y;

	int act;
}Star;

typedef struct
{
	int x;
	int heart;
	int hit;
	int hitpoint;
	int score;
	int item;
	int itempoint;
	int energybar;
}Player;

typedef struct
{
	char name[21];
	int score;
}Player_Rank;


Rain rain[WIDTH];
Star star[WIDTH];
Player player;
int x = 32;
int y = 18;

int RankScore(int score, int count) { //count:0,1,2,3 <4자리 숫자를 배열에 넣는다 히히
	int num, a = 3;
	int b = 1;
	if (count != 3) {
		for (int i = 0; i < a - count; i++) {
			b *= 10;
		}
		num = (count - (count - ((count / b * 10) * (b * 10)))) / b;
	}
	else {
		num = (count % 10);
	}
	return num;
}

void Cursorview(char show) { //커서제거
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void gotoxy(int x, int y)  //좌표이동을 시키는 함수
{
	COORD pos = { 2 * x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int isKeyDown(int key) //키에따른 이동
{
	return ((GetAsyncKeyState(key) & 0x8000) != 0);
}

int mainmapDraw() {    //메인맵 구현


	gotoxy(0, 0);
	printf("\n\n\n\n");
	textcolor(3);
	printf("                                                             \n");
	printf("          ■■■■                                           \n");
	printf("         ■       ■               ■                        \n");
	printf("         ■        ■                                        \n");
	printf("         ■       ■     ■■■    ■  ■  ■■              \n");
	printf("         ■■■■      ■      ■  ■  ■      ■            \n");
	printf("         ■       ■   ■    ■■  ■  ■      ■            \n");
	printf("         ■       ■    ■■   ■  ■  ■      ■            \n");
	printf("                                                             \n");
	textcolor(7);
	gotoxy(WIDTH1 - 4, 18);
	printf("> 게임시작");
	gotoxy(WIDTH1 - 4, 19);
	printf("   조작법");
	gotoxy(WIDTH1 - 4, 20);
	printf("   랭★킹");
}

//int MainMap() {
//	while (1) {
//		while (1) {
//			int n = keycontrol(0);
//			switch (n) {
//			case UP: {
//				if (y > 18) {      //키가 18,19,20좌표로 들어가고 중심은 19
//					gotoxy(x - 2, y);
//					printf(" ");
//					gotoxy(x - 2, --y);
//					if (y == 18)
//						printf(">");
//				}
//				break;
//			}
//			case DOWN: {
//				if (y < 20) {
//					gotoxy(x - 2, y);
//					printf(" ");
//					gotoxy(x - 2, ++y);
//					printf(">");
//				}
//				break;
//			}
//			case SUB: {
//				return y - 18;
//			}
//			}
//		}
//		system("cls");
//	}
//}

void init() //초기설정값
{
	Cursorview(0);
	int i;

	for (i = 0; i < WIDTH; i++)
		rain[i].act = FALSE;

	player.x = WIDTH / 2;
	player.heart = 3;
	player.hit = 0;
	player.hitpoint = 0;
	player.score = 0;
	gotoxy(5, HEIGHT + 3);
	player.item = 0;
	player.itempoint = 0;
	speed = 100;
	player.energybar = 0;

}

void CreateRain()
{
	for (int i = 0; i < WIDTH; i++)
	{
		if (!rain[i].act)
		{
			rain[i].x = rand() % WIDTH;
			rain[i].y = HEIGHT - 1;

			rain[i].act = TRUE;

			return;
		}
	}
}


void DeleteRain_Star()
{

	for (int i = 0; i < WIDTH; i++)
	{
		if (rain[i].act && rain[i].y < 0)
		{
			rain[i].act = FALSE;
			player.score++;
		}

	}
}

int PlayerContainsRain_Star()
{

	for (int i = 0; i < WIDTH; i++)
	{
		if ((rain[i].act) && (rain[i].y == 0) && (rain[i].x == player.x))
		{
			if (player.item != 1) {
				player.heart -= 1;
				player.hit++;
				if (player.heart <= 0)
					return TRUE;
			}
		}

	}

	return FALSE;
}

void MoveRain_Star()
{
	int i;

	for (i = 0; i < WIDTH; i++)
	{
		if (rain[i].act)
		{
			rain[i].y--;
		}
	}
}

void MovePlayer()
{
	if (isKeyDown(key_a)) {
		player.x -= 1;
		if (player.energybar < 200) {
			player.energybar++;
		}
	}
	if (isKeyDown(key_d)) {
		player.x += 1;
		if (player.energybar < 200) {
			player.energybar++;
		}
	}
	if (isKeyDown(key_e)) {
		if (player.energybar == 200) {
			if (player.heart < 5) {
				player.heart++;
				player.heart++;
				player.energybar = 0;
				player.item++;
			}
		}
	}
	if (player.x < 0)
		player.x = 0;
	if (player.x > WIDTH - 1)
		player.x = WIDTH - 1;
}

int keycontrol(int playing) {
	char temp = getch();
	if (playing == 0) {
		if (temp == 'w' || temp == 'W') {

			return UP;
		}
		else if (temp == 's' || temp == 'S') {

			return DOWN;
		}
		else if (temp == 'a' || temp == 'A') {
			return LIFT;
		}
		else if (temp == 'd' || temp == 'D') {
			return RIGHT;
		}
		else if (temp == ' ') {
			return SUB;
		}
	}
	else {
		if (temp == 'a' || temp == 'A') {
			return LIFT;
		}
		else if (temp == 'd' || temp == 'D') {
			return RIGHT;
		}
	}

}

void PrintMap()
{
	system("cls");


	for (int i = 0; i < WIDTH; i++)
	{
		if (rain[i].act)
		{
			gotoxy(rain[i].x, HEIGHT - rain[i].y);
			textcolor(3);

			printf("♭");
			textcolor(7);

		}

	}

	gotoxy(player.x, HEIGHT);
	printf("□");

	gotoxy(0, HEIGHT + 1);
	for (int i = 0; i < WIDTH; i++)
		printf("▤");
	if (player.hit != 0) {
		gotoxy(player.x, HEIGHT);
		textcolor(12);
		printf("※");
		textcolor(7);
		player.hitpoint++;
		if (player.hitpoint == 4) {
			player.hit--;
			player.hitpoint = 0;
		}
	}
	else if (player.item != 0) {
		gotoxy(player.x, HEIGHT);
		textcolor(6);
		if (player.itempoint % 2 == 0) {
			printf("☆");
		}
		else {
			printf("★");
		}
		textcolor(7);
		player.itempoint++;
		if (player.itempoint == 25) {
			player.item--;
			player.itempoint = 0;
		}
	}

	gotoxy(5, HEIGHT + 4);
	int play = player.energybar / 20;
	printf("{");
	textcolor(13);
	for (int i = 0; i < play; i++) {
		printf("■");

	}
	for (int i = 10; i > play; i--) {
		printf("  ");
	}textcolor(7);  printf("}");


	if (player.heart == 3) {     //생명표시
		gotoxy(2, HEIGHT + 2);
		printf("you are heart:");
		textcolor(12);
		printf("♥♥♥");
		textcolor(7);
	}
	else if (player.heart == 4) {
		gotoxy(2, HEIGHT + 2);
		printf("you are heart:");
		textcolor(12);
		printf("♥♥♥");
		textcolor(8);
		printf("♥");
		textcolor(7);
	}
	else if (player.heart == 5) {
		gotoxy(2, HEIGHT + 2);
		printf("you are heart:");
		textcolor(12);
		printf("♥♥♥");
		textcolor(8);
		printf("♥♥");
		textcolor(7);
	}
	else if (player.heart == 2) {
		gotoxy(2, HEIGHT + 2);
		printf("you are heart:");
		textcolor(12);
		printf("♥♥");
		textcolor(7);
	}
	else if (player.heart == 1) {
		gotoxy(2, HEIGHT + 2);
		printf("you are heart:");
		textcolor(12);
		printf("♥");
		textcolor(7);
	}
	else {
		gotoxy(2, HEIGHT + 2);
		printf("you are heart:");
	}
	gotoxy(2, HEIGHT + 3);
	printf("score:%d", player.score);

	if (player.score >= 400 && player.score < 800) {  //난이도 표기
		gotoxy(21, 2);
		printf("난이도:중간");
		speed = 50;
	}
	else if (player.score >= 800 && player.score < 1000) {
		gotoxy(21, 2);
		printf("난이도:어려움");
		speed = 30;
	}
	else if (player.score >= 1000) {
		gotoxy(21, 2);
		printf("난이도:악마");
		speed = 10;
	}
	else {
		gotoxy(21, 2);
		printf("난이도:쉬움");
		speed = 100;
	}


}

MainMap() {
	while (1) {
		while (1) {
			int n = keycontrol(0);
			switch (n) {
			case UP: {
				if (y > 18) {      //키가 18,19,20좌표로 들어가고 중심은 19
					//PlaySound(TEXT("select2.wav"), NULL, SND_ASYNC);

					gotoxy(WIDTH1 - 4, y);
					printf(" ");
					gotoxy(WIDTH1 - 4, --y);
					/*if (y == 18)*/
					printf(">");
				}
				break;
			}
			case DOWN: {
				if (y < 20) {
					gotoxy(WIDTH1 - 4, y);
					printf(" ");
					gotoxy(WIDTH1 - 4, ++y);
					printf(">");
				}
				break;
			}
			case SUB: {
				return y - 18;
			}
			}
		}
		system("cls");
	}
}
int GameOver_1() {
	for (int i = 0; i < 2; i++) {
		system("cls");
		printf("\n\n\n");
		textcolor(12);
		
		printf("      .ad8ba.                        \n");
		printf("     88.    88   .a88a.   88.dYa..adYba.    adPYba.    \n");
		printf("     'Y88bbd88  88    Y8  88'  '88'   '88  88    '88  \n");
		printf("           .88  88    88  88    88     88  88PYred8' \n");
		printf("       8888P'   '8bdP'Y8  88    88     88   '8bweea\n");
		printf("                                                  \n");
		printf("      .ad88ba. `Yb   dY' adPYba.   88 .ada.      \n");
		printf("      88'  '88  '8. .8' 88    '88  88.dP'       \n");
		printf("      88    88   '8.8   88PYred8'  88P'        \n");
		printf("      '`88bd`'    'P`    '8bweea   88'        \n");
		Sleep(500);
		system("cls");
		textcolor(7);
		Sleep(500);
	}
	system("cls");
	printf("\n\n\n");
	textcolor(12);
	printf("      .ad8ba.                        \n");
	printf("     88.    88   .a88a.   88.dYa..adYba.    adPYba.    \n");
	printf("     'Y88bbd88  88    Y8  88'  '88'   '88  88    '88  \n");
	printf("           .88  88    88  88    88     88  88PYred8' \n");
	printf("       8888P'   '8bdP'Y8  88    88     88   '8bweea\n");
	printf("                                                  \n");
	printf("      .ad88ba. `Yb   dY' adPYba.   88 .ada.      \n");
	printf("      88'  '88  '8. .8' 88    '88  88.dP'       \n");
	printf("      88    88   '8.8   88PYred8'  88P'        \n");
	printf("      '`88bd`'    'P`    '8bweea   88'        \n");
	textcolor(7);
	Sleep(500);

}
void Manual() {
	system("cls");
	printf("\n\n\n");
	textcolor(7);
	printf("========================={ 조작방법 }=========================    \n");
	printf("                                                                  \n");
	printf("                                     {"); textcolor(13); printf("■■■■■■■■■■"); textcolor(7); printf("}\n");
	printf("                                                        ↖       \n");
	printf(" 플레이어->  □                   ↙           "); textcolor(6); printf("★"); textcolor(7); printf("        게이지\n");
	printf("       ▤▤▤▤▤▤▤      [select]      ▤▤▤▤▤▤▤                                    \n");
	printf("         ---이동---        --선택--      --아이템사용--                   \n");
	printf("        ←[A]  [D]→     [스페이스바]          [E]              \n");
	printf("                                                             \n");
	printf("         ※ 돌아갈려면 \"스페이스바\"를 누르세요..                    \n");
	while (1) {
		int num = keycontrol(0);
		switch (num) {
		case SUB:
			y--;
			return;
		}

	}
}
int gameover_2() {
	GameOver_1();

}
Rank_Draw() {
	system("cls");
	printf("\n\n\n");
	printf("       ========================={ 랭♣킹 }========================= \n");
	printf("         {닉네임}                      {점수}                      \n");


}

void Rank_arr() {
	int count = 0;
	int temp;

	int i = 0;
	int score[10] = { 0, };
	FILE* fp = fopen("rank.txt", "r");
	if (fp == NULL) {
		printf("랭킹 불러오기에 실패했습니다..");
	}
	Player_Rank player_rank[30] = { 0, };
	Player_Rank P_R = { 0,0 };
	for (int i = 0; i < 30; i++) {
		fscanf(fp, "%s%d", player_rank[i].name, &player_rank[i].score);
		if (player_rank[i].name != NULL) {

		}
		else {
			break;
		}
		count++;
	}
	for (int i = 0; i < 10; i++) {
		for (int j = 1; j < count - 1; j++) {
			if (player_rank[j - 1].score < player_rank[j].score) {
				P_R = player_rank[j];
				player_rank[j] = player_rank[j - 1];
				player_rank[j - 1] = P_R;

				/*temp = player_rank[j].score;
				player_rank[j].score = player_rank[j - 1].score;
				player_rank[j - 1].score = temp;*/


			}
		}
	}
	for (int i = 0; i < 9; i++) {
		if (i < 1) {
			textcolor(6);
			gotoxy(4, 5 + i);
			printf("★");
			textcolor(3);
		}
		gotoxy(5, 5 + i);
		printf("%s", player_rank[i].name);
		gotoxy(20, 5 + i);
		printf("%d", player_rank[i].score);
		textcolor(7);
	}
	/*for (int i = 0; i < 5; i++)
	{
		name=(char*)malloc(sizeof(char) * 21);
		fgets(name, 21, fp);
		rank3[i] = name;
		printf("%s", name);
	}*/
	fclose(fp);

}
void Rank_System() {

	Rank_Draw();
	
	int retval;
	//원속 초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;
	//MessageBox(NULL, "원속 초기화 성공", "알림", MB_OK);

	//socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
	//MessageBox(NULL, "TCP 소켓성공", "알림", MB_OK);

	//connect()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = inet_addr("127.0.0.1");
	serveraddr.sin_port = htons(9000);

	connect(sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));


	SOCKET client_sock = 0;
	SOCKADDR_IN clientaddr;
	int addrlen;

	char buf[BUFSIZE + 1];
	retval = recv(sock, buf, BUFSIZE, 0);
	
	int s = 0;
	char namebuf[21] = { 0, };
	char scorebuf[5] = { 0, };
	int j;
	for (int i = 0; i < 10; i++) {
		for ( j = 0; buf[s] != ' '; j++) {
			namebuf[j] = buf[s++];
		}
		namebuf[++j] = NULL;
		buf[s++];

		for (int a = 0; a < 4; a++) {
			scorebuf[a] = buf[s++];
		}
		scorebuf[4] = NULL;
		buf[s++];
		gotoxy(8, 6+i);
		printf("%s      %s", namebuf, scorebuf);
		printf("alsdfh");
	}

	gotoxy(0, 15);
	printf("       ============================================================ \n");
	char name[21] = "";
	printf("\n       랭킹을 등록하시겠습니까 ? (Y / N)");
	while (1) {
		int num1 = keycontrol(0);
		if (num1 == 'Y' || num1 == 'y') {
			printf("\n       닉네임을 적어주세요:");
			scanf("%s", name);
			//FILE* fp = fopen("rank.txt", "a+");              //타입을 a+(전에 있던 데이터를 덮어쓰는게 아니라 있던 데이터에 추가함)로 설정해 전에있던 랭킹들과 새로 추가된 랭킹들이 있을 수 있게했다.
			//if (fp == NULL) {
			//	printf("       파일 로딩에 실패했습니다.");
			//}
			//printf("\n       닉네임을 적어주세요:");
			//원속 초기화
			
			//MessageBox(NULL, "원속 초기화 성공", "알림", MB_OK);

			//socket()
			
			//MessageBox(NULL, "TCP 소켓성공", "알림", MB_OK);

			//connect()
			

			

			
			name[strlen(name)] = 0 ;
			char buf[BUFSIZE + 1];
			sprintf(buf, "%s",name);
			int k = 0;
			char buf_a[BUFSIZE + 1];
			
				for (int j = 0; name[j] != NULL; j++) {
					buf_a[k++] = name[j];
				}
				buf_a[k++] = ' ';

				for (int a = 0; a < 4; a++) {
					buf_a[k++] = RankScore(player.score,a);
				}
				buf_a[k++] = ' ';
			
			//send()
			send(sock, buf, strlen(buf), 0);
			
			char buf_1[BUFSIZE + 1];
			int a = player.score;
			char score[5];
			
			
			
			//closesocket()
			closesocket(sock);

			//원속 종료
			WSACleanup();
			//name문자열배열에 닉네임을 저장하고 name배열에 문자열을 fprintf를 이용해서 파일에 저장해준다.
			//fprintf(fp, "%s\n%d\n", name, player.score);

			//fclose(fp);
			break;
		}
		else if (num1 == 'n' || num1 == 'N') {
			break;
		}
	}
	printf("\n      ※ \"스페이스바\"를 누르면 메인메뉴로 이동합니다...");


	while (1) {
		int num = keycontrol(0);
		if (num == SUB) {
			y = 18;
			return;
		}
	}
}
void main()
{

	char key; init();

	//PlaySound(TEXT("bgm2.wav"), NULL, SND_ASYNC | SND_LOOP);
	while (1) {
		system("cls");
		mainmapDraw();
		Sleep(200);
		int movemap = MainMap();
		switch (movemap) {
		case 0: {
			PlaySound(NULL, 0, 0);
			//PlaySound(TEXT("bgm3.wav"), NULL, SND_ASYNC | SND_LOOP);
			player.score = 0;
			player.energybar = 0;
			do
			{
				srand((int)malloc(NULL));

				CreateRain();
				MoveRain_Star();
				DeleteRain_Star();

				MovePlayer();

				PrintMap();

				Sleep(speed);
			} while (!(PlayerContainsRain_Star()));
			PrintMap();
			player.heart = 3;
			PlaySound(NULL, 0, 0);
			//PlaySound(TEXT("bgm2.wav"), NULL, SND_ASYNC | SND_LOOP);
			Sleep(1060);
			gameover_2();


			break;
		}
		case 1: {//조작법 창
			Manual();
			break;
		}
		case 2: {
			Rank_System();
		}
		}
	}

}
