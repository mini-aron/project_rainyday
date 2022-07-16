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
	SetConsoleTextAttribute(GetStdHandle(STD_OUTPUT_HANDLE), colorNum); //��Ʈ�� ����
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

int RankScore(int score, int count) { //count:0,1,2,3 <4�ڸ� ���ڸ� �迭�� �ִ´� ����
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

void Cursorview(char show) { //Ŀ������
	HANDLE hConsole;
	CONSOLE_CURSOR_INFO ConsoleCursor;

	hConsole = GetStdHandle(STD_OUTPUT_HANDLE);

	ConsoleCursor.bVisible = show;
	ConsoleCursor.dwSize = 1;

	SetConsoleCursorInfo(hConsole, &ConsoleCursor);
}

void gotoxy(int x, int y)  //��ǥ�̵��� ��Ű�� �Լ�
{
	COORD pos = { 2 * x, y };
	SetConsoleCursorPosition(GetStdHandle(STD_OUTPUT_HANDLE), pos);
}

int isKeyDown(int key) //Ű������ �̵�
{
	return ((GetAsyncKeyState(key) & 0x8000) != 0);
}

int mainmapDraw() {    //���θ� ����


	gotoxy(0, 0);
	printf("\n\n\n\n");
	textcolor(3);
	printf("                                                             \n");
	printf("          �����                                           \n");
	printf("         ��       ��               ��                        \n");
	printf("         ��        ��                                        \n");
	printf("         ��       ��     ����    ��  ��  ���              \n");
	printf("         �����      ��      ��  ��  ��      ��            \n");
	printf("         ��       ��   ��    ���  ��  ��      ��            \n");
	printf("         ��       ��    ���   ��  ��  ��      ��            \n");
	printf("                                                             \n");
	textcolor(7);
	gotoxy(WIDTH1 - 4, 18);
	printf("> ���ӽ���");
	gotoxy(WIDTH1 - 4, 19);
	printf("   ���۹�");
	gotoxy(WIDTH1 - 4, 20);
	printf("   ����ŷ");
}

//int MainMap() {
//	while (1) {
//		while (1) {
//			int n = keycontrol(0);
//			switch (n) {
//			case UP: {
//				if (y > 18) {      //Ű�� 18,19,20��ǥ�� ���� �߽��� 19
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

void init() //�ʱ⼳����
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

			printf("��");
			textcolor(7);

		}

	}

	gotoxy(player.x, HEIGHT);
	printf("��");

	gotoxy(0, HEIGHT + 1);
	for (int i = 0; i < WIDTH; i++)
		printf("��");
	if (player.hit != 0) {
		gotoxy(player.x, HEIGHT);
		textcolor(12);
		printf("��");
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
			printf("��");
		}
		else {
			printf("��");
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
		printf("��");

	}
	for (int i = 10; i > play; i--) {
		printf("  ");
	}textcolor(7);  printf("}");


	if (player.heart == 3) {     //����ǥ��
		gotoxy(2, HEIGHT + 2);
		printf("you are heart:");
		textcolor(12);
		printf("������");
		textcolor(7);
	}
	else if (player.heart == 4) {
		gotoxy(2, HEIGHT + 2);
		printf("you are heart:");
		textcolor(12);
		printf("������");
		textcolor(8);
		printf("��");
		textcolor(7);
	}
	else if (player.heart == 5) {
		gotoxy(2, HEIGHT + 2);
		printf("you are heart:");
		textcolor(12);
		printf("������");
		textcolor(8);
		printf("����");
		textcolor(7);
	}
	else if (player.heart == 2) {
		gotoxy(2, HEIGHT + 2);
		printf("you are heart:");
		textcolor(12);
		printf("����");
		textcolor(7);
	}
	else if (player.heart == 1) {
		gotoxy(2, HEIGHT + 2);
		printf("you are heart:");
		textcolor(12);
		printf("��");
		textcolor(7);
	}
	else {
		gotoxy(2, HEIGHT + 2);
		printf("you are heart:");
	}
	gotoxy(2, HEIGHT + 3);
	printf("score:%d", player.score);

	if (player.score >= 400 && player.score < 800) {  //���̵� ǥ��
		gotoxy(21, 2);
		printf("���̵�:�߰�");
		speed = 50;
	}
	else if (player.score >= 800 && player.score < 1000) {
		gotoxy(21, 2);
		printf("���̵�:�����");
		speed = 30;
	}
	else if (player.score >= 1000) {
		gotoxy(21, 2);
		printf("���̵�:�Ǹ�");
		speed = 10;
	}
	else {
		gotoxy(21, 2);
		printf("���̵�:����");
		speed = 100;
	}


}

MainMap() {
	while (1) {
		while (1) {
			int n = keycontrol(0);
			switch (n) {
			case UP: {
				if (y > 18) {      //Ű�� 18,19,20��ǥ�� ���� �߽��� 19
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
	printf("========================={ ���۹�� }=========================    \n");
	printf("                                                                  \n");
	printf("                                     {"); textcolor(13); printf("�����������"); textcolor(7); printf("}\n");
	printf("                                                        ��       \n");
	printf(" �÷��̾�->  ��                   ��           "); textcolor(6); printf("��"); textcolor(7); printf("        ������\n");
	printf("       �ǢǢǢǢǢǢ�      [select]      �ǢǢǢǢǢǢ�                                    \n");
	printf("         ---�̵�---        --����--      --�����ۻ��--                   \n");
	printf("        ��[A]  [D]��     [�����̽���]          [E]              \n");
	printf("                                                             \n");
	printf("         �� ���ư����� \"�����̽���\"�� ��������..                    \n");
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
	printf("       ========================={ ����ŷ }========================= \n");
	printf("         {�г���}                      {����}                      \n");


}

void Rank_arr() {
	int count = 0;
	int temp;

	int i = 0;
	int score[10] = { 0, };
	FILE* fp = fopen("rank.txt", "r");
	if (fp == NULL) {
		printf("��ŷ �ҷ����⿡ �����߽��ϴ�..");
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
			printf("��");
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
	//���� �ʱ�ȭ
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;
	//MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);

	//socket()
	SOCKET sock = socket(AF_INET, SOCK_STREAM, 0);
	if (sock == INVALID_SOCKET) err_quit("socket()");
	//MessageBox(NULL, "TCP ���ϼ���", "�˸�", MB_OK);

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
	printf("\n       ��ŷ�� ����Ͻðڽ��ϱ� ? (Y / N)");
	while (1) {
		int num1 = keycontrol(0);
		if (num1 == 'Y' || num1 == 'y') {
			printf("\n       �г����� �����ּ���:");
			scanf("%s", name);
			//FILE* fp = fopen("rank.txt", "a+");              //Ÿ���� a+(���� �ִ� �����͸� ����°� �ƴ϶� �ִ� �����Ϳ� �߰���)�� ������ �����ִ� ��ŷ��� ���� �߰��� ��ŷ���� ���� �� �ְ��ߴ�.
			//if (fp == NULL) {
			//	printf("       ���� �ε��� �����߽��ϴ�.");
			//}
			//printf("\n       �г����� �����ּ���:");
			//���� �ʱ�ȭ
			
			//MessageBox(NULL, "���� �ʱ�ȭ ����", "�˸�", MB_OK);

			//socket()
			
			//MessageBox(NULL, "TCP ���ϼ���", "�˸�", MB_OK);

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

			//���� ����
			WSACleanup();
			//name���ڿ��迭�� �г����� �����ϰ� name�迭�� ���ڿ��� fprintf�� �̿��ؼ� ���Ͽ� �������ش�.
			//fprintf(fp, "%s\n%d\n", name, player.score);

			//fclose(fp);
			break;
		}
		else if (num1 == 'n' || num1 == 'N') {
			break;
		}
	}
	printf("\n      �� \"�����̽���\"�� ������ ���θ޴��� �̵��մϴ�...");


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
		case 1: {//���۹� â
			Manual();
			break;
		}
		case 2: {
			Rank_System();
		}
		}
	}

}
