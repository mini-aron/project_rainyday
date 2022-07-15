#define _WINSOCK_DEPRECATED_NO_WARNINGS
#define _CRT_SECURE_NO_WARNINGS

#include <winsock2.h>
#include<stdio.h>
#include<stdlib.h>
#pragma comment(lib, "ws2_32");

#define BUFSIZE 512

typedef struct
{
	char name[21];
	int score[5];
}Player_Rank;
int count = 0;
Player_Rank player_rank[100];
void Rank_Arrr() {
	 count = 0;
	int temp;

	int i = 0;
	int score[10] = { 0, };
	

	Player_Rank P_R = { 0,0 };
	for (int i = 0; i < 100; i++) {
		if (player_rank[i].name[0] != NULL)
			count += 1;
		else {
			break;
		}
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
}


int RankScore(int score,int count) { //count:0,1,2,3
	int num, a = 3;
	int b=1;
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
void err_quit(const char* msg) {
	LPVOID IpMsgBuf;
	FormatMessage(
		FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM,
		NULL, WSAGetLastError(),
		MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT),
		(LPTSTR)&IpMsgBuf, 0, NULL);
	exit(-1);

}

int main(int argc, char* argv[]) {
	int retval;

	//원속초기화
	WSADATA wsa;
	if (WSAStartup(MAKEWORD(2, 2), &wsa) != 0)
		return -1;
	//MessageBox(NULL, "원속 초기화 성공", "알림", MB_OK);

	//socket()
	SOCKET listen_sock = socket(AF_INET, SOCK_STREAM, 0);
	if (listen_sock == INVALID_SOCKET) err_quit("socket()");
	printf("소켓이 생성되었습니다\n");
	//MessageBox(NULL, "TCP 소켓성공", "알림", MB_OK);

	//bind()
	SOCKADDR_IN serveraddr;
	ZeroMemory(&serveraddr, sizeof(serveraddr));
	serveraddr.sin_family = AF_INET;
	serveraddr.sin_addr.s_addr = htonl(INADDR_ANY);
	serveraddr.sin_port = htons(9000);

	bind(listen_sock, (SOCKADDR*)&serveraddr, sizeof(serveraddr));
	printf("Bind 완료 되었습니다.\n");
	while (1) {

		//listen()
		listen(listen_sock, SOMAXCONN);
		printf("connect 연결을 기다리는 중..\n");

		//통신에 사용할 변수
		SOCKET client_sock;
		SOCKADDR_IN clientaddr;
		int addrlen;
		char buf[BUFSIZE + 1];

		//accept()
		addrlen = sizeof(clientaddr);
		printf("w");
		char buf_5[BUFSIZE + 1];
		//대기대기기기ㅣ딛ㄱ
		client_sock = accept(listen_sock, (SOCKADDR*)&clientaddr, &addrlen);
		int count_1 = 0;
		
		printf("요청을 받았습니다.\n");
		int k = 0;
		char buf_a[BUFSIZE + 1];
		 for(int i = 0; i < 10; i++) {
			for (int j = 0; player_rank[i].name[j] != NULL; j++) {
				buf_a[k++] = player_rank[i].name[j];
			}
			buf_a[k++] = ' ';
			
			for (int a = 0; a < 4; a++) {
				buf_a[k++] = player_rank[i].score[a];
			}
			buf_a[k++] = ' ';
		}
		 
		send(client_sock, buf_a, strlen(buf_a), 0);
		printf("--------------");

		//recv()
		retval = recv(client_sock, buf, BUFSIZE, 0);
		printf("메세지를 수신하였습니다.\n");
		//받는 데이터 출력
		buf[retval] = '\0';



		printf("[TCP/%s:%d] %s ",
			inet_ntoa(clientaddr.sin_addr),
			ntohs(clientaddr.sin_port), buf);
		
		/*for (int i = 0; i < strlen(buf); i++) {
			if (buf != NULL)
				player_rank[count].name[i] = buf[i];
			else
				break;
		}*/
		
		Rank_Arrr();
		printf("==========================\n");
		for (int i = 0; i < 5; i++) {
			printf("%s %d\n", player_rank[i].name, player_rank[i].score);
		}printf("==========================\n");
		printf("\ncount:%d\n", count);
		
		

	
		/*FILE* fp = fopen("rank.txt", "r");
		if (fp == NULL) {
			printf("랭킹 불러오기에 실패했습니다..");
		}
		else {
			fprintf("%s %s", buf,  atoi( buf_4));
			fclose(fp);
		}*/



		//Rank_Arrr(buf_1);
		client_sock = 0;
	}
		//send(listen_sock, buf_1, strlen(buf), 0);
		//printf("메세지를 보냅니다.\n");

		//closesocket
		closesocket(listen_sock);

		//원속 종료
		WSACleanup();
	
	
	return 0;

}
