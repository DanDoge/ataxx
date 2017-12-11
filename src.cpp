#include <graphics.h>
#include <conio.h>
#include <Windows.h>
#include <stdio.h>


int getblacknum();
int getwhitenum();
void newinitialize();
void initializegame();
void checklegal(MOUSEMSG m);
void move(int x, int y);
void affect(int x, int y, int mode);
void drawsums();
int checkwinlose();
void decide();
int getgain(int x1, int y1, int x2, int y2, int color, int times);
int userlegal();
void gethint();
void gethuiqi();
void savemap();
void saveloadgame(int mode);

int map[8][8][8] = {};
RECT startgame = { 100,425,233,455 };
RECT loadgame = { 233, 425, 366, 455 };
RECT savegame = { 366, 425, 500, 455 };
RECT huiqi = { 100, 455,233,475 };
RECT hint = { 233,455,366,475 };
RECT quitgame = { 366,455,500,475 };
RECT help = { 450,350,550,400 };
RECT title = { 425, 50, 625, 100 };
int now[2] = {};
int whitestart[2] = {};
int recentmap[8][8] = {};
int maxtimes = 2;

int main(){
	newinitialize();

	MOUSEMSG m;
	while (true) {
		m = GetMouseMsg();

		if (m.mkLButton == true && m.x < 233 && m.x > 100 && m.y > 425 && m.y < 455) {

			initializegame();

			while (true) {
				m = GetMouseMsg();
				if (m.mkLButton == true) {
					if (m.x < 233 && m.x > 100 && m.y > 455 && m.y < 475) {
						gethuiqi();
					}
					if (m.x < 366 && m.x > 233 && m.y > 425 && m.y < 455) {
						saveloadgame(1);
					}

					if (m.x < 500 && m.x > 366 && m.y > 425 && m.y < 455) {
						saveloadgame(-1);
					}
					if (m.x < 400 && m.x > 50 && m.y > 50 && m.y < 400) {
						if (getpixel(m.x, m.y) == BLACK && m.y % 50 && m.x % 50) {
							checklegal(m);
						}
						else if (getpixel(m.x, m.y) == GREEN) {
							savemap();
							move(m.x, m.y);
							drawsums();
							decide();
							drawsums();
						}
					}

					if (m.x > 366 && m.x < 500 && m.y > 455 && m.y < 475) {
						return 0;
					}

					if (m.x > 233 && m.x < 366 && m.y > 455 && m.y < 475) {
						gethint();
					}

					if (userlegal() == -1) {
						if (getblacknum() > getwhitenum()) {
							drawtext(_T("��Ӯ��������"), &help, DT_CENTER);
							Sleep(1000);
							drawtext(_T("˫���ؿ���Ϸ"), &help, DT_CENTER);
							break;
						}
						else {
							drawtext(_T("�ܱ�Ǹ������"), &help, DT_CENTER);
							Sleep(1000);
							drawtext(_T("˫���ؿ���Ϸ"), &help, DT_CENTER);
							break;
						}
					}

					if (checkwinlose() == 1) {
						drawtext(_T("��Ӯ��������"), &help, DT_CENTER);
						Sleep(1000);
						drawtext(_T("˫���ؿ���Ϸ"), &help, DT_CENTER);
						break;
					}
					else if (checkwinlose() == -1) {
						drawtext(_T("�ܱ�Ǹ������"), &help, DT_CENTER);
						Sleep(1000);
						drawtext(_T("˫���ؿ���Ϸ"), &help, DT_CENTER);
						break;
					}

					if (getblacknum() + getwhitenum() == 49) {
						if (getblacknum() > getwhitenum()) {
							drawtext(_T("��Ӯ��������"), &help, DT_CENTER);
							Sleep(1000);
							drawtext(_T("˫���ؿ���Ϸ"), &help, DT_CENTER);
							break;
						}
						else {
							drawtext(_T("�ܱ�Ǹ������"), &help, DT_CENTER);
							Sleep(1000);
							drawtext(_T("˫���ؿ���Ϸ"), &help, DT_CENTER);
							break;
						}
					}

				}

			}

			while (true) {
				m = GetMouseMsg();
				if (m.mkLButton == true && m.x < 500 && m.x > 366 && m.y > 455 && m.y < 475) {
					return 0;
				}
				else if (m.mkLButton == true) {
					break;
				}
			}

		}
		else if(m.mkLButton == true && m.x < 500 && m.x > 366 && m.y > 455 && m.y < 475){
			return 0;
		}
	}

	system("pause");
    return 0;
}

void affect(int x, int y, int mode) {
	if (mode == 1) {
		for (int i = x - 1; i <= x + 1; i += 1) {
			for (int j = y - 1; j <= y + 1; j += 1) {
				if (getpixel(i * 50 + 25, j * 50 + 25) == WHITE) {
					setfillcolor(BLACK);
					fillcircle(i * 50 + 25, j * 50 + 25, 20);
				}
			}
		}
	}
	else {
		if (x > 0 && y > 0) {
			for (int i = x - 50; i <= x + 50; i += 50) {
				for (int j = y - 50; j <= y + 50; j += 50) {
					if (getpixel(i, j) == BLACK && i > 50 && i < 400 && j > 50 && j < 400) {
						setfillcolor(WHITE);
						fillcircle(i, j, 20);
					}
				}
			}
		}
	}
}

void move(int x, int y) {
	int i = x / 50;
	int j = y / 50;
	affect(i, j, 1);
	if (abs(i - now[0]) <= 1 && abs(j - now[1]) <= 1) {
		setfillcolor(BLACK);
		fillcircle(i*50+25, j*50+25, 20);
	}
	else {
		setfillcolor(BLACK);
		fillcircle(i * 50 + 25, j * 50 + 25, 20);
		setfillcolor(BROWN);
		solidpolygon((POINT*)map[now[0]][now[1]], 4);
	}
	for (int i = 1; i < 8; i += 1) {
		for (int j = 1; j < 8; j += 1) {
			if (getpixel(i * 50 + 25, j * 50 + 25) != WHITE
				&& getpixel(i * 50 + 25, j * 50 + 25) != BLACK) {
				setfillcolor(BROWN);
				solidpolygon((POINT*)map[i][j], 4);
			}
			else if (getpixel(i * 50 + 25, j * 50 + 25) == BLACK) {
				setfillcolor(BROWN);
				solidpolygon((POINT*)map[i][j], 4);
				setfillcolor(BLACK);
				fillcircle(i * 50 + 25, j * 50 + 25, 20);
			}
		}
	}
}

void checklegal(MOUSEMSG m) {
	for (int i = 1; i < 8; i += 1) {
		for (int j = 1; j < 8; j += 1) {
			if (getpixel(i * 50 + 25, j * 50 + 25) == GREEN) {
				setfillcolor(BROWN);
				solidpolygon((POINT*)map[i][j], 4);
			}
		}
	}
	if ( m.x < 400 && m.y < 400 && m.x > 50 && m.y >50) {
		int x = m.x / 50;
		int y = m.y / 50;

		if (getpixel(x * 50 + 25, y * 50 + 25) == BLACK) {
			now[0] = x;
			now[1] = y;
			for (int i = 1; i < 8; i += 1) {
				for (int j = 1; j < 8; j += 1) {
					if (getpixel(i * 50 + 25, j * 50 + 25) != WHITE
						&& getpixel(i * 50 + 25, j * 50 + 25) != BLACK) {
						setfillcolor(BROWN);
						solidpolygon((POINT*)map[i][j], 4);
					}
					else if (getpixel(i * 50 + 25, j * 50 + 25) == BLACK) {
						setfillcolor(BROWN);
						solidpolygon((POINT*)map[i][j], 4);
						setfillcolor(BLACK);
						fillcircle(i * 50 + 25, j * 50 + 25, 20);
					}
				}
			}
			for (int i = x - 2; i <= x + 2; i += 1) {
				for (int j = y - 2; j <= y + 2; j += 1) {
					if (getpixel(i * 50 + 25, j * 50 + 25) == BROWN) {
						setfillcolor(GREEN);
						solidpolygon((POINT*)map[i][j], 4);
					}
				}
			}
		}
		else {
			//drawtext(_T("�����²��˰�"), &help, DT_CENTER);
		}
	}
	else{
		//drawtext(_T("���ܵ����˰�"), &help, DT_CENTER);
	}
}

int userlegal() {
	for (int i = 75; i <= 375; i += 50) {
		for (int j = 75; j <= 375; j += 50) {
			if (getpixel(i, j) == BLACK) {
				for (int p = i - 100; p <= i + 100; p += 50) {
					for (int q = j - 100; q <= j + 100; q += 50) {
						if (getpixel(p, q) == BROWN || getpixel(p, q) == GREEN) {
							return 1;
						}
					}
				}
			}
		}
	}

	return -1;
}

void gethint() {
	int beststart[2] = { -1,-1 };
	int bestend[2] = { -1,-1 };
	int bestcnt = 0;
	for (int i = 75; i <= 375; i += 50) {
		for (int j = 75; j <= 375; j += 50) {
			if (getpixel(i, j) == BLACK) {
				for (int k = max(75, i - 100); k <= min(375, i + 100); k += 50) {
					for (int l = max(75, j - 100); l <= min(375, j + 100); l += 50) {
						if (getpixel(k, l) == BROWN || getpixel(k, l) == GREEN) {
							int tempcnt = 0;
							for (int p = k - 50; p <= k + 50; p += 50) {
								for (int q = l - 50; q <= l + 50; q += 50) {
									if (getpixel(p, q) == WHITE) {
										tempcnt += 1;
									}
								}
							}
							if (tempcnt >= bestcnt) {
								beststart[0] = i;
								beststart[1] = j;
								bestend[0] = k;
								bestend[1] = l;
								bestcnt = tempcnt;
							}
						}
					}
				}
			}
		}
	}
	if (getpixel(bestend[0], bestend[1]) == BROWN) {
		for (int i = 0; i < 3; i += 1) {
			setfillcolor(BROWN);
			fillcircle(beststart[0], beststart[1], 20);
			Sleep(200);
			setfillcolor(BLACK);
			fillcircle(bestend[0], bestend[1], 20);
			Sleep(200);
			setfillcolor(BROWN);
			solidpolygon((POINT*)map[bestend[0] / 50][bestend[1] / 50], 4);
			setfillcolor(BLACK);
			fillcircle(beststart[0], beststart[1], 20);
		}
	}
	else {
		for (int i = 0; i < 3; i += 1) {
			setfillcolor(BROWN);
			fillcircle(beststart[0], beststart[1], 20);
			Sleep(200);
			setfillcolor(BLACK);
			fillcircle(bestend[0], bestend[1], 20);
			Sleep(200);
			setfillcolor(GREEN);
			solidpolygon((POINT*)map[bestend[0] / 50][bestend[1] / 50], 4);
			setfillcolor(BLACK);
			fillcircle(beststart[0], beststart[1], 20);
		}
	}
}

int getgain(int x1, int y1, int x2, int y2, int color, int times) {
	if (times == maxtimes) {
		return 0;
	}
	else {
		int nowmap[8][8] = {};
		for (int i = 1; i < 8; i += 1) {
			for (int j = 1; j < 8; j += 1) {
				if (getpixel(i * 50 + 25, j * 50 + 25) == BLACK) {
					nowmap[i][j] = 1;
				}
				else if (getpixel(i * 50 + 25, j * 50 + 25) == WHITE) {
					nowmap[i][j] = -1;
				}
			}
		}
		if (abs(x1 - x2) > 1 || abs(y1 - y2) > 1) {
			nowmap[x1][y1] = 0;
		}
		nowmap[x2][y2] = color;
		for (int i = max(1, x2 - 1); i <= min(7, x2 + 1); i += 1) {
			for (int j = max(1, y2 - 1); j <= min(7, y2 + 1); j += 1) {
				if (nowmap[i][j] + nowmap[x2][y2] == 0) {
					nowmap[i][j] = nowmap[x2][y2];
				}
			}
		}


		int maxcnt = 0;

		for (int i = 1; i < 8; i += 1) {
			for (int j = 1; j < 8; j += 1) {
				if (nowmap[i][j] == color) {

					for (int k = max(1, i - 2); k <= min(7, i + 2); k += 1) {
						for (int l = max(1, j - 2); l <= min(7, j + 2); l += 1) {
							if (nowmap[k][l] == 0) {
								int tempcnt = 0;
								for (int p = max(1, k - 1); p <= min(7, k + 1); p += 1) {
									for (int q = max(1, l - 1); q <= min(7, l + 1); q += 1) {
										if (nowmap[p][q] == -color) {
											tempcnt += (5 - times);
										}
									}
								}
								if (max(abs(i - k), abs(j - l)) > 1) {
									tempcnt -= 5;
								}
								tempcnt -= getgain(i, j, k, l, -color, times + 1);
								maxcnt = max(maxcnt, tempcnt);
							}
						}
					}
				}
			}
		}
		return maxcnt;
	}


}

void decide() {
	int maxgain = -1 << 30;
	int start[2] = {-1, -1};
	int end[2] = {-1, -1};
	for (int i = 75; i <= 375; i += 50) {
		for (int j = 75; j <= 375; j += 50) {
			if (getpixel(i, j) == WHITE) {
				for (int p = i - 100; p <= i + 100; p += 50) {
					for (int q = j - 100; q <= j + 100; q += 50) {
						if (getpixel(p, q) == BROWN) {
							int tempcnt = 0;
							for (int l = p - 50; l <= p + 50; l += 50) {
								for (int r = q - 50; r <= q + 50; r += 50) {
									if (getpixel(l, r) == BLACK) {
										tempcnt += 10;
									}
								}
							}
							if (abs(p - i) >= 100 || abs(q - j) >= 100) {
								tempcnt -= 10;
							}
							tempcnt -= getgain(i / 50, j / 50, p / 50, q / 50, -1, 0);
							if (tempcnt > maxgain) {
								start[0] = i;
								start[1] = j;
								end[0] = p;
								end[1] = q;
								maxgain = tempcnt;
							}
						}
					}
				}
			}
		}
	}
	if (start[0] > 0 && start[1] > 0) {

		for (int i = 0; i < 3; i += 1) {
			setfillcolor(BROWN);
			fillcircle(start[0], start[1], 20);
			Sleep(200);
			setfillcolor(WHITE);
			fillcircle(start[0], start[1], 20);
			Sleep(200);
		}

		if (abs(start[0] - end[0]) <= 50 && abs(start[1] - end[1]) <= 50) {
			setfillcolor(WHITE);
			fillcircle(end[0], end[1], 20);
		}
		else {
			setfillcolor(WHITE);
			fillcircle(end[0], end[1], 20);
			setfillcolor(BROWN);
			solidpolygon((POINT*)map[start[0] / 50][start[1] / 50], 4);
		}

		affect(end[0], end[1], 0);
	}
}

void gethuiqi() {
	for (int i = 1; i <= 7; i += 1) {
		for (int j = 1; j <= 7; j += 1) {

			setfillcolor(BROWN);
			solidpolygon((POINT*)map[i][j], 4);

			if (recentmap[i][j] == -1) {
				setfillcolor(WHITE);
				fillcircle(i * 50 + 25, j * 50 + 25, 20);
			}
			else if(recentmap[i][j] == 1){
				setfillcolor(BLACK);
				fillcircle(i * 50 + 25, j * 50 + 25, 20);
			}
		}
	}
}

void savemap() {
	for (int i = 1; i <= 7; i += 1) {
		for (int j = 1; j <= 7; j += 1) {
			if (getpixel(i * 50 + 25, j * 50 + 25) == BLACK) {
				recentmap[i][j] = 1;
			}
			else if (getpixel(i * 50 + 25, j * 50 + 25) == WHITE) {
				recentmap[i][j] = -1;
			}
			else {
				recentmap[i][j] = 0;
			}
		}
	}
}



void saveloadgame(int mode) {
	if (mode == 1) {
		FILE *fp = fopen("save.in", "r");
		if (fp) {
			for (int i = 1; i <= 7; i += 1) {
				for (int j = 1; j <= 7; j += 1) {
					int num = 0;
					fscanf(fp, "%d", &num);
					setfillcolor(BROWN);
					solidpolygon((POINT*)map[i][j], 4);
					switch (num) {
					case 1: setfillcolor(BLACK);
						fillcircle(i * 50 + 25, j * 50 + 25, 20);
						break;
					case -1:setfillcolor(WHITE);
						fillcircle(i * 50 + 25, j * 50 + 25, 20);
						break;
					}
				}
			}
			fclose(fp);
			drawsums();
		}
		else {
			drawtext(_T("û�д浵"), &loadgame, DT_CENTER);
			Sleep(500);
			drawtext(_T("SO SAD"), &loadgame, DT_CENTER);
			Sleep(500);
			drawtext(_T("�����浵"), &loadgame, DT_CENTER);
		}
	}
	else {
		FILE *fp = fopen("save.in", "w+");
		if (fp) {
			for (int i = 1; i <= 7; i += 1) {
				for (int j = 1; j <= 7; j += 1) {
					if (getpixel(i * 50 + 25, j * 50 + 25) == WHITE) {
						fprintf(fp, "%d ", -1);
					}
					else if (getpixel(i * 50 + 25, j * 50 + 25) == BLACK) {
						fprintf(fp, "%d ", 1);
					}
					else {
						fprintf(fp, "%d ", 0);
					}
				}
				fprintf(fp, "\n");
			}
			fclose(fp);
			drawtext(_T("�浵�ɹ�"), &savegame, DT_CENTER);
			Sleep(1000);
			drawtext(_T("�洢��Ϸ"), &savegame, DT_CENTER);
		}
		else {
			drawtext(_T("�洢ʧ��"), &savegame, DT_CENTER);
			Sleep(500);
			drawtext(_T("SO SAD"), &savegame, DT_CENTER);
			Sleep(500);
			drawtext(_T("�洢��Ϸ"), &savegame, DT_CENTER);
		}
	}
}

int checkwinlose() {
	if (getblacknum() == 0) {
		return -1;
	}
	else if (getwhitenum() == 0) {
		return 1;
	}
	else {
		return 0;
	}
}

int getblacknum() {
	int cnt = 0;
	for (int i = 75; i <= 375; i += 50) {
		for (int j = 75; j <= 375; j += 50) {
			cnt += (getpixel(i, j) == BLACK);
		}
	}

	return cnt;
}

int getwhitenum() {
	int cnt = 0;
	for (int i = 75; i <= 375; i += 50) {
		for (int j = 75; j <= 375; j += 50) {
			cnt += (getpixel(i, j) == WHITE);
		}
	}

	return cnt;
}

void drawsums() {
	setfillcolor(LIGHTGRAY);
	POINT ini[4] = { {430,300}, {430, 100}, {530, 100}, {530, 300} };
	solidpolygon(ini, 4);

	setfillcolor(BLACK);
	POINT blacksum[4] = { { 430, 300 },{ 430, 300 - getblacknum() * 4 },
						{ 470, 300 - getblacknum() * 4 },{ 470, 300 } };
	solidpolygon(blacksum, 4);
	setfillcolor(WHITE);
	POINT whitesum[4] = { { 490, 300 },{ 490, 300 - getwhitenum() * 4 },
						{ 530, 300 - getwhitenum() * 4 },{ 530, 300 } };
	solidpolygon(whitesum, 4);
}

void initializegame() {

	setfillcolor(BROWN);
	for (int i = 1; i < 8; i += 1) {
			for (int j = 1; j < 8; j += 1) {
				solidpolygon((POINT*)map[i][j], 4);
			}
		}

	setfillcolor(BLACK);
	fillcircle(75, 75, 20);
	fillcircle(375, 375, 20);

	setfillcolor(WHITE);
	fillcircle(75, 375, 20);
	fillcircle(375, 75, 20);

	drawsums();

	RECT blacktext = { 430, 320, 470, 370 };
	drawtext(_T("����"), &blacktext, DT_CENTER);
	RECT whitetext = { 490, 320, 530, 370 };
	drawtext(_T("����"), &whitetext, DT_CENTER);

	setfillcolor(BLACK);
	POINT blacksum[4] = { {430, 300}, {430, 300 - getblacknum() * 5 }, {470, 300 - getblacknum() * 5 }, {470, 300} };
	solidpolygon(blacksum, 4);
	setfillcolor(WHITE);
	POINT whitesum[4] = { { 490, 300 },{ 490, 300 - getblacknum() * 5 },{ 530, 300 - getblacknum() * 5 },{ 530, 300 } };
	solidpolygon(whitesum, 4);

	drawtext(_T("�����ƶ�����"), &help, DT_CENTER);
}

void newinitialize() {
	initgraph(640, 480);
	setbkcolor(LIGHTGRAY);
	cleardevice();

	settextstyle(50, 0, _T("����"));
	settextcolor(BLACK);
	drawtext(_T("ͬ����"), &title, DT_CENTER);
	settextstyle(16, 0, _T("����"));

	setcolor(BLACK);
	line(100, 420, 500, 420);
	line(100, 450, 500, 450);
	line(100, 480, 500, 480);
	line(100, 420, 100, 480);
	line(500, 420, 500, 480);
	line(233, 420, 233, 480);
	line(366, 420, 366, 480);
	line(100, 480, 500, 480);

	setfillcolor(BLACK);
	fillrectangle(95, 415, 505, 420);
	fillrectangle(95, 415, 100, 480);
	fillrectangle(500, 415, 505, 480);

	for (int i = 50; i <= 400; i += 50) {
		line(50, i, 400, i);
		line(i, 50, i, 400);
	}
	line(40, 40, 40, 410);
	line(40, 40, 410, 40);
	line(410, 40, 410, 410);
	line(40, 410, 410, 410);

	setfillcolor(BLACK);
	fillrectangle(41, 41, 409, 49);
	fillrectangle(41, 41, 49, 409);
	fillrectangle(41, 399, 409, 409);
	fillrectangle(399, 41, 409, 409);

	for (int i = 1; i < 8; i += 1) {
		for (int j = 1; j < 8; j += 1) {
			map[i][j][0] = i * 50 + 1;
			map[i][j][1] = j * 50 + 1;
			map[i][j][2] = i * 50 + 50 - 1;
			map[i][j][3] = j * 50 + 1;
			map[i][j][4] = i * 50 + 50 - 1;
			map[i][j][5] = j * 50 + 50 - 1;
			map[i][j][6] = i * 50 + 1;
			map[i][j][7] = j * 50 + 50 - 1;
		}
	}

	setfillcolor(BROWN);
	for (int i = 1; i < 8; i += 1) {
		for (int j = 1; j < 8; j += 1) {
			solidpolygon((POINT*)map[i][j], 4);
		}
	}

	drawtext(_T("��ʼ��Ϸ"), &startgame, DT_CENTER);
	drawtext(_T("�����浵"), &loadgame, DT_CENTER);
	drawtext(_T("�洢��Ϸ"), &savegame, DT_CENTER);
	drawtext(_T("������ʾ"), &hint, DT_CENTER);
	drawtext(_T("Ҫ����ô"), &huiqi, DT_CENTER);
	drawtext(_T("�˳���Ϸ"), &quitgame, DT_CENTER);
}
