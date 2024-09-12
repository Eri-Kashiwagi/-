#include<stdio.h>
#include<graphics.h>
#include"tools.h"
#include<time.h>
#include<math.h>
#include<mmsystem.h>
#include"vector2.h"
#pragma comment(lib,"winmm.lib")
#define win_width 900
#define win_height 600
#define zm_max 10 
enum { wandou, xiangrikui, shirenhua, zhiwucount };//ֲ������
enum { GOING, WIN, FAIL };//��Ϸ״̬����
enum { sunshinedown, sunshinegrond, sunshinecollect, sunshineproduct };//����״̬
//����ͼƬ
IMAGE imgbg;

//������ͼƬ
IMAGE imgbar;

//ֲ�￨ƬͼƬ
IMAGE imgcard[zhiwucount];

//ֲ��ͼƬ
IMAGE* imgzhiwu[zhiwucount][20];

//����ͼƬ
IMAGE imgsunshineball[29];

//��ʬͼƬ
IMAGE imgzm[22];

//��ʬ����ͼƬ
IMAGE imgzmdead[20];

//��ʬ��ֲ��ͼƬ
IMAGE imgzmeat[21];

//��ʬվ��ͼƬ
IMAGE imgzmstand[11];

//�㶹�ӵ�����ͼƬ
IMAGE imgbulletnor;

//�㶹�ӵ�ը��ͼƬ
IMAGE imgbullblast[4];

//��������
int curx=-100, cury=-100;

//����ֲ��
int curzhiwu;

//������������
int sunshinesum = 50;

//����ɱ����ʬ����
int killcount;

//��ʬ����
int zmcount;


//���ֲ�����
class music {
public:
	//���ű�������
	static void bg_begin() {
		mciSendString("play res/bg.mp3", 0, 0, 0);
		mciSendString("setaudio res/bg.mp3 volume to 500", NULL, 0, NULL);
	}
	//�رձ�������
	static void bg_close() {
		mciSendString("close res/bg.mp3", NULL, 0, NULL);
	}
	//����ʤ������
	static void win() {
		mciSendString("play res/win.mp3", 0, 0, 0);
	}
	//����ʧ������
	static void lose() {
		mciSendString("play res/lose.mp3", 0, 0, 0);
	}
	//�����ռ���������
	static void collectsunshine() {
		PlaySound("res/sunshine.wav", NULL, SND_FILENAME | SND_ASYNC);
	}
};

//��Ϸ
class game {
private:
	//��Ϸ״̬
	static int status;
public:
	//��ѯ��Ϸ״̬
	static int getstatus() {
		return status;
	}
	//�޸���Ϸ״̬
	static void changestatus(int n) {
		status = n;
	}

	//�ж��ļ��ܷ��
	static bool fileexist(const char* name);

	//��Ϸ��ʼ��
	static void init();

	//��ѯ��Ϸ�Ƿ����
	static bool checkover();
};
int game::status = 0;

//enum { wandou, xiangrikui, shirenhua, zhiwucount };
//enum {GOING , WIN, FAIL};
//IMAGE imgbg;
//IMAGE imgbar;
//IMAGE imgcard[zhiwucount];
//IMAGE* imgzhiwu[zhiwucount][20];
//int curx, cury;
//int curzhiwu;
//int sunshine=50;
//int killcount;
//int zmcount;

//ֲ����
class zhiwu {
public:
	int type;//ֲ������
	int frameindex;//֡��
	bool catched;//�Ƿ񱻽�ʬץס
	int deadtime;//����ʱ��

	int timer;//����ֲ��֡��
	int x, y;//ֲ��λ��

	int shootime;//�ӵ����ʱ��
};
//��ͼ�ϵ�ֲ������
zhiwu map[3][9];
//enum{sunshinedown,sunshinegrond,sunshinecollect,sunshineproduct};

//����
class sunshineball {
public:
	int x, y;
	int frameindex;//֡��
	int desty;//Ŀ��λ��
	bool used;//�Ƿ�������״̬
	int timer;//����֡��
	float xoff;//�ƶ�
	float yoff;//�ƶ�

	float t;
	vector2 p1, p2, p3, p4;
	vector2 pcur;//����λ��
	float speed;//�ƶ��ٶ�
	int status;//״̬


};
sunshineball balls[10];//����������
//IMAGE imgsunshineball[29];



//��ʬ
class zm {
public:
	int x, y;//��ʬλ��
	int frameindex;//֡��
	bool used;//�Ƿ�������״̬
	int speed;//�ƶ��ٶ�
	int row;//����һ��
	int blood;//Ѫ��
	bool dead;//�Ƿ�����
	bool eating;//�Ƿ��ڳ�ֲ��
};
zm zms[10];
//IMAGE imgzm[22];
//IMAGE imgzmdead[20];
//IMAGE imgzmeat[21];
//IMAGE imgzmstand[11];

//�ӵ�
class bullet{
public:
	int x, y;//λ��
	int row;//����
	bool used;//�Ƿ�������״̬
	int speed;//�ƶ��ٶ�
	bool blast;//�Ƿ�ը��
	int frameindex;//֡��

};
bullet bullets[30];
//IMAGE imgbulletnor;
//IMAGE imgbullblast[4];

//�û���Ϊ
class user {
public:
	static void click();//�û����
};

//��Ⱦ
class render {
public:
	//��ʾ��ʼ�˵�
	static void startUI();

	//��ʾ��Ϸ����
	static void viewscence();

	//��ʾ�������»�
	static void barsdown();

	//��ʾ��Ϸ����
	static void game_content();

};
 
//����
class create {
public:
	//���ɽ�ʬ
	static void zm();

	//��������
	static void sunshine();

	//�����ӵ�
	static void bullet();
};

//����
class update {
public:
	//������Ϸ��������
	static void game_content();

	//����ֲ��
	static void zhiwu();

	//���½�ʬ
	static void zm();

	//��������
	static void sunshine();

	//�����ӵ�
	static void bullet();
};

//�ռ�
class collect {
public:
	//�ռ�����
	static void sunshine(ExMessage* msg);
};

//��ײ
class collision {
public:
	//��ײ���
	static void check();
};
bool game::fileexist(const char* name) {
	FILE* fp = fopen(name, "r");//���ļ�
	if (fp == NULL)return false;
	else {
		fclose(fp);
		return true;
	}
}
void game::init() {
	loadimage(&imgbg, "res/bg.jpg");
	loadimage(&imgbar, "res/bar5.png");
	char name[64];
	memset(imgzhiwu, 0, sizeof(imgzhiwu));
	memset(map, 0, sizeof(map));
	memset(balls, 0, sizeof(balls));
	memset(zms, 0, sizeof(zms));
	memset(bullets, 0, sizeof(bullets));
	for (int i = 0; i < zhiwucount; i++) {
		sprintf_s(name, sizeof(name), "res/Cards/card_%d.png", i + 1);
		loadimage(&imgcard[i], name);

		for (int j = 0; j < 20; j++) {
			sprintf_s(name, sizeof(name), "res/zhiwu/%d/%d.png", i, j + 1);
			if (game::fileexist(name)) {
				imgzhiwu[i][j] = new IMAGE;
				loadimage(imgzhiwu[i][j], name);


			}
			else break;
		}
	}
	killcount = 0;
	zmcount = 0;
	game::changestatus(GOING);
	curzhiwu = 0;
	for (int i = 0; i < 29; i++) {
		sprintf_s(name, sizeof(name), "res/sunshine/%d.png", i + 1);
		loadimage(&imgsunshineball[i], name);


	}
	srand(time(NULL));//�����������

	initgraph(win_width, win_height, 1);//����һ��ͼ����
	//����������������
	LOGFONT f;
	gettextstyle(&f);
	f.lfHeight = 30;
	f.lfWeight = 15;
	strcpy(f.lfFaceName, "Segoe UI Black");
	f.lfQuality = ANTIALIASED_QUALITY;
	settextstyle(&f);
	setbkmode(TRANSPARENT);
	setcolor(BLACK);
	//��ʼ���ӵ�
	loadimage(&imgbulletnor, "res/bullets/bullet_normal.png");
	loadimage(&imgbullblast[3], "res/bullets/bullet_blast.png");
	for (int i = 0; i < 3; i++) {
		float k = (i + 1) * 0.2;
		loadimage(&imgbullblast[i], "res/bullets/bullet_blast.png", imgbullblast[3].getwidth() * k, imgbullblast[3].getheight() * k, true);
	}

	//��ʼ����ʬ
	for (int i = 0; i < 22; i++) {
		sprintf_s(name, sizeof(name), "res/zm/%d.png", i + 1);
		loadimage(&imgzm[i], name);
	}
	for (int i = 0; i < 20; i++) {
		sprintf_s(name, sizeof(name), "res/zm_dead/%d.png", i + 1);
		loadimage(&imgzmdead[i], name);
	}

	for (int i = 0; i < 21; i++) {
		sprintf_s(name, sizeof(name), "res/zm_eat/%d.png", i + 1);
		loadimage(&imgzmeat[i], name);
	}

	for (int i = 0; i < 11; i++) {
		sprintf_s(name, sizeof(name), "res/zm_stand/%d.png", i + 1);
		loadimage(&imgzmstand[i], name);
	}
}
////��Ϸ��ʼ��
//void gameInit() {
//	loadimage(&imgbg, "res/bg.jpg");
//	loadimage(&imgbar, "res/bar5.png");
//	char name[64];
//	memset(imgzhiwu, 0, sizeof(imgzhiwu));
//	memset(map, 0, sizeof(map));
//	memset(balls, 0, sizeof(balls));
//	for (int i = 0; i < zhiwucount; i++) {
//		sprintf_s(name, sizeof(name), "res/Cards/card_%d.png",i+1);
//		loadimage(&imgcard[i], name);
//
//		for (int j = 0; j < 20; j++) {
//			sprintf_s(name, sizeof(name), "res/zhiwu/%d/%d.png", i, j + 1);
//			if (fileexist(name)) {
//				imgzhiwu[i][j] = new IMAGE;
//				loadimage(imgzhiwu[i][j], name);
//
//
//			}
//			else break;
//		}
//	}
//	killcount = 0;
//	zmcount = 0;
//	game::changestatus(GOING);
//
//
//	curzhiwu = 0;
//	for (int i = 0; i < 29; i++) {
//		sprintf_s(name, sizeof(name), "res/sunshine/%d.png", i + 1);
//		loadimage(&imgsunshineball[i], name);
//
//
//	}
//
//	srand(time(NULL));
//
//	initgraph(win_width, win_height,1);
//	LOGFONT f;
//	gettextstyle(&f);
//	f.lfHeight = 30;
//	f.lfWeight = 15;
//	strcpy(f.lfFaceName, "Segoe UI Black");
//	f.lfQuality = ANTIALIASED_QUALITY;
//	settextstyle(&f);
//	setbkmode(TRANSPARENT);
//	setcolor(BLACK);
//
//
//	//��ʼ����ʬ
//	memset(zms, 0, sizeof(zms));
//	for (int i = 0; i < 22; i++) {
//		sprintf_s(name, sizeof(name), "res/zm/%d.png", i + 1);
//		loadimage(&imgzm[i], name);
//	}
//
//	loadimage(&imgbulletnor, "res/bullets/bullet_normal.png");
//	memset(bullets, 0, sizeof(bullets));
//
//	loadimage(&imgbullblast[3], "res/bullets/bullet_blast.png");
//	for (int i = 0; i < 3; i++) {
//		float k = (i + 1) * 0.2;
//		loadimage(&imgbullblast[i], "res/bullets/bullet_blast.png", imgbullblast[3].getwidth() * k, imgbullblast[3].getheight() * k, true);
//
//
//
//	}
//
//	for (int i = 0; i < 20; i++) {
//		sprintf_s(name, sizeof(name), "res/zm_dead/%d.png", i + 1);
//		loadimage(&imgzmdead[i], name);
//	}
//
//	for (int i = 0; i < 21; i++) {
//		sprintf_s(name, sizeof(name), "res/zm_eat/%d.png", i + 1);
//		loadimage(&imgzmeat[i], name);
//	}
//
//	for (int i = 0; i < 11; i++) {
//		sprintf_s(name, sizeof(name), "res/zm_stand/%d.png", i + 1);
//		loadimage(&imgzmstand[i], name);
//	}
//}

//void drawzm() {
//	int zmmax = sizeof(zms) / sizeof(zms[0]);
//	for (int i = 0; i < zmmax; i++) {
//		if (zms[i].used) {
//			/*IMAGE* img = &imgzm[zms[i].frameindex];*/
//			//IMAGE* img = (zms[i].dead)?imgzmdead:imgzm;
//			IMAGE* img = NULL;
//			if (zms[i].dead)img = imgzmdead;
//			else if (zms[i].eating)img = imgzmeat;
//			else img = imgzm;
//
//			img += zms[i].frameindex;
//			putimagePNG(zms[i].x, zms[i].y - img->getheight(), img);
//		}
//	}
//
//	/*static int cnt = 0;
//	cnt++;
//	if (cnt >=0) {
//		cnt = 0;
//		for (int i = 0; i < zmmax; i++) {
//			if (zms[i].used) {
//				if (zms[i].dead) {
//					zms[i].frameindex++;
//					if (zms[i].frameindex >= 20) {
//						zms[i].used = false;
//					}
//				}
//				else zms[i].frameindex = (zms[i].frameindex + 1) % 22;
//			}
//		}
//	}*/
//}
//

void render::game_content(){
	BeginBatchDraw();
	putimage(-112, 0, &imgbg);//��Ⱦ����
	putimagePNG(250, 0, &imgbar);//��Ⱦ������

	//��Ⱦֲ�￨Ƭ
	for (int i = 0; i < zhiwucount; i++) {
		int x = 333 + i * 65;
		int y = 10;
		putimagePNG(x, y, &imgcard[i]);
	}

	//��Ⱦ��ͼ�ϵ�ֲ��
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			if (map[i][j].type > 0) {
				/*int x = 256 + j * 81;
				int y = 179 + i * 102 + 14;*/
				int zhiwutype = map[i][j].type - 1;
				int index = map[i][j].frameindex;
				/*putimagePNG(x, y, imgzhiwu[zhiwutype][index]);*/
				putimagePNG(map[i][j].x, map[i][j].y, imgzhiwu[zhiwutype][index]);
			}
		}
	}
	//��Ⱦ����϶���ֲ��
	if (curzhiwu > 0) {
		IMAGE* img = imgzhiwu[curzhiwu - 1][0];
		putimagePNG(curx - img->getwidth() / 2, cury - img->getheight() / 2, img);
	}

	//��Ⱦ�ӵ�
	int ballmax = sizeof(balls) / sizeof(balls[0]);
	for (int i = 0; i < ballmax; i++) {
		/*if (balls[i].used || balls[i].xoff) {
			IMAGE* img = &imgsunshineball[balls[i].frameindex];
			putimagePNG(balls[i].x, balls[i].y, img);
		}*/
		if (balls[i].used) {
			IMAGE* img = &imgsunshineball[balls[i].frameindex];
			putimagePNG(balls[i].pcur.x, balls[i].pcur.y, img);
		}
	}

	//��Ⱦ��������
	char scoreText[8];
	sprintf_s(scoreText, sizeof(scoreText), "%d", sunshinesum);
	if (sunshinesum == 0) {
		outtextxy(288, 67, scoreText);
	}
	else if (sunshinesum < 100) {
		outtextxy(283 , 67, scoreText);
	}
	else if (sunshinesum < 1000) {
		outtextxy(278, 67, scoreText);
	}
	else outtextxy(273, 67, scoreText);

	//��Ⱦ��ʬ
	int zmmax = sizeof(zms) / sizeof(zms[0]);
	for (int i = 0; i < zmmax; i++) {
		if (zms[i].used) {
			/*IMAGE* img = &imgzm[zms[i].frameindex];*/
			//IMAGE* img = (zms[i].dead)?imgzmdead:imgzm;
			IMAGE* img = NULL;
			if (zms[i].dead)img = imgzmdead;
			else if (zms[i].eating)img = imgzmeat;
			else img = imgzm;

			img += zms[i].frameindex;
			putimagePNG(zms[i].x, zms[i].y - img->getheight(), img);
		}
	}

	//��Ⱦ�ӵ�
	int bulletmax = sizeof(bullets) / sizeof(bullets[0]);
	for (int i = 0; i < bulletmax; i++){
		if (bullets[i].used) {
			if (bullets[i].blast) {
				IMAGE* img = &imgbullblast[bullets[i].frameindex];
				putimagePNG(bullets[i].x, bullets[i].y, img);
			}
			else {
				putimagePNG(bullets[i].x, bullets[i].y, &imgbulletnor);
			}


		}
	}

	EndBatchDraw();
}
void collect::sunshine(ExMessage* msg) {
	int ballmax = sizeof(balls) / sizeof(balls[0]);
	int w = imgsunshineball[0].getwidth();
	int h = imgsunshineball[0].getheight();
	for (int i = 0; i < ballmax; i++) {
		if (balls[i].used) {
			/*int x = balls[i].x;
			int y = balls[i].y;*/
			int x = balls[i].pcur.x;
			int y = balls[i].pcur.y;
			if (msg->x > x && msg->x<x + w && msg->y >y && msg->y < y + h) {
				/*balls[i].used = false;*/
				balls[i].status = sunshinecollect;
				//sunshine += 25;
			/*	mciSendString("play res/sunshine.mp3", 0, 0,0);*/
				/*PlaySound("res/sunshine.wav", NULL, SND_FILENAME | SND_ASYNC);*/
				music::collectsunshine();
				//float desy = 0;
				//float desx = 256;
				//float angle = atan((y - desy) / (x - desx));
				//balls[i].xoff = 4 * cos(angle);
				//balls[i].yoff = 4 * sin(angle);
				balls[i].p1 = balls[i].pcur;
				balls[i].p4 = vector2(252, 0);
				balls[i].t = 0;
				float distance = dis(balls[i].p1 - balls[i].p4);
				float off =8;

				balls[i].speed = 1.0/(distance/off);
				break;
			}
			
		}
	}
}


void user::click() {
	ExMessage msg;//�����״̬
	static int status = 0;
	if (peekmessage(&msg)) {
		if (msg.message == WM_LBUTTONDOWN) {//���������
			if (msg.x > 338 && msg.x < 338 + 65 * zhiwucount && msg.y < 96) {
				int index = (msg.x - 338) / 65;
				//printf("%d\n", index);
				if (index == 0&&sunshinesum<100) {
					return;
				}
				else if (index == 0 && sunshinesum >=100) {
					sunshinesum -= 100;
				}
				else if (index == 1 && sunshinesum < 50) {
					return;
				}
				else if (index == 1 && sunshinesum >= 50) {
					sunshinesum -= 50;
				}
				status = 1;
				curzhiwu = index+1;

			}
			else {
				collect::sunshine(&msg);

			}


		}
		else if (msg.message == WM_MOUSEMOVE&&status==1) {//�ƶ����
			curx = msg.x;
			cury = msg.y;


		}
		else if (msg.message == WM_LBUTTONUP&&status==1) {//�ٴε��������
			if (msg.x > 256 - 112 && msg.x < 900 - 30 && msg.y > 179 && msg.y < 489) {
				int row = (msg.y - 179) / 102;
				int col = (msg.x - 256+112) / 81;
				//printf("%d %d\n", row, col);
				if (map[row][col].type == 0) {
					map[row][col].type = curzhiwu;
					map[row][col].frameindex = 0;
					map[row][col].shootime = 0;
					map[row][col].x = 256-112 + col * 81;
					map[row][col].y = 179 + row * 102 + 14;

				}
				curzhiwu = 0;
				status = 0;
				curx = -100;
				cury = -100;
			}

		}



	}
}

void create::sunshine() {
	//����֡��
	static int cnt = 0;
	static int fee = 200;
	cnt++;
	if (cnt >= fee) {
		fee = 200 + rand() % 200;
		cnt = 0;
		int ballmax = sizeof(balls) / sizeof(balls[0]);
		int i;
		for (i = 0; i < ballmax && balls[i].used; i++);
		if (i >= ballmax)return;
		balls[i].used = true;
		balls[i].frameindex = 0;
		/*balls[i].x = 260 + rand() % (900 - 260);
		balls[i].y = 60;
		balls[i].desty = 200 + (rand() % 4) * 90;*/
		balls[i].timer = 0;
		//balls[i].xoff = 0;
		//balls[i].yoff = 0;
		//���տ�����������ĳ�ʼ��
		balls[i].status = sunshinedown;
		balls[i].t = 0;
		balls[i].p1 = vector2(260-112 + rand() % (900 - 260+112), 60);
		balls[i].p4 = vector2(balls[i].p1.x, 200 + (rand() % 4) * 90);
		int off = 2;
		float distance = balls[i].p4.y - balls[i].p1.y;
		balls[i].speed = 1.0 / (distance / off);

		//���տ���������
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 9; j++) {
				if (map[i][j].type == xiangrikui + 1) {
					map[i][j].timer++;
					if (map[i][j].timer >= 2) {
						map[i][j].timer = 0;
						int k;
						for (k = 0; k < ballmax && balls[k].used; k++);
						if (k >= ballmax)return;
						balls[k].used = true;
						balls[k].p1 = vector2(map[i][j].x, map[i][j].y);
						int w = (100 + rand() % 50)*(rand()%2?1:-1);
						balls[k].p4 = vector2(map[i][j].x+w,map[i][j].y+imgzhiwu[xiangrikui][0]->getheight()-imgsunshineball[0].getheight());
						balls[k].p2 = vector2(balls[k].p1.x + w * 0.3, balls[k].p1.y - 100);
						balls[k].p3 = vector2(balls[k].p1.x + w * 0.7, balls[k].p1.y - 100);
						balls[k].status = sunshineproduct;
						balls[k].speed = 0.05;
						balls[k].t = 0;



					}
				}
			}
		}


	}


}
void update::sunshine() {
	int ballmax = sizeof(balls) / sizeof(balls[0]);
	//�������������ռ�����
	for (int i = 0; i < ballmax; i++) {
		if (balls[i].used) {
			balls[i].frameindex = (balls[i].frameindex + 1) % 29;
			//�������¹���
			if (balls[i].status == sunshinedown) {
				sunshineball* sun = &balls[i];
				sun->t += sun->speed;
				sun->pcur = sun->p1 + sun->t * (sun->p4 - sun->p1);
				if (sun->t >= 1) {
					sun->status = sunshinegrond;
					sun->timer = 0;
				}


			}
			//�����ڵ���
			else if (balls[i].status == sunshinegrond) {
				balls[i].timer++;
				if (balls[i].timer > 150) {
					balls[i].used = false;
					balls[i].timer = 0;
				}


			}
			//�������ռ�
			else if (balls[i].status == sunshinecollect) {
				sunshineball* sun = &balls[i];
				sun->t += sun->speed;
				sun->pcur = sun->p1 + sun->t * (sun->p4 - sun->p1);
				if (sun->t > 1) {
					sun->used = false;
					sunshinesum += 25;
				}


			}
			//���տ�����������
			else if (balls[i].status == sunshineproduct) {
				sunshineball* sun = &balls[i];
				sun->t += sun->speed;
				sun->pcur = calcBezierPoint(sun->t, sun->p1, sun->p2, sun->p3, sun->p4);
				if (sun->t > 1) {
					sun->status = sunshinegrond;
					sun->timer = 0;
				}



			}


			//	if(balls[i].timer==0)balls[i].y += 2;
			//	if (balls[i].y >= balls[i].desty) {
			//		balls[i].timer++;
			//		if(balls[i].timer>100)balls[i].used = false;

			//	}
			//}
			//else if (balls[i].xoff) {
			//	float desy = 0;
			//	float desx = 256;
			//	float angle = atan((balls[i].y - desy) / (balls[i].x - desx));
			//	balls[i].xoff = 4 * cos(angle);
			//	balls[i].yoff = 4 * sin(angle);

			//	balls[i].x -= balls[i].xoff;
			//	balls[i].y -= balls[i].yoff;
			//	if (balls[i].y < 0 || balls[i].x < 262) {
			//		balls[i].xoff = 0;
			//		balls[i].yoff = 0;
			//		sunshine += 25;
			//	}
			//}
		}
	}
}

void create::zm() {
	//���ƽ�ʬ��������
	if (zmcount >= zm_max) {
		return;
	}
	static int  zmfee = 100;
	static int cnt = 0;
	cnt++;
	if (cnt > zmfee) {
		cnt = 0;
		zmfee = rand() % 200 + 200;
		int i;
		int zmmax = sizeof(zms) / sizeof(zms[0]);
		for (i = 0; i < zmmax && zms[i].used; i++);
		if (i >= zmmax)return;
		else {
			memset(&zms[i], 0, sizeof(zms[i]));
			zms[i].used = true;
			zms[i].x = win_width;
			zms[i].row = rand() % 3;
			zms[i].y = 172 + (1 + zms[i].row) * 100;
			zms[i].speed = 1;
			zms[i].blood = 100;
			zms[i].dead = false;
			zmcount++;
		}
	}

}

void update::zm() {
	static int ans = 0;
	ans++;

	int zmmax = sizeof(zms) / sizeof(zms[0]);
	if (ans >= 3) {
		ans = 0;
		//��ʬ����
		for (int i = 0; i < zmmax; i++) {
			if (zms[i].used) {
				zms[i].x -= zms[i].speed;
				if (zms[i].x < 56) {
					/*printf("GAME OVER\n");
					MessageBox(NULL, "over", "over", 0);
					exit(0);*/
					game::changestatus(FAIL);
				}
			}
		}
	}
	static int cnt = 0;
	cnt++;
	if (cnt >= 4) {
		cnt = 0;
		for (int i = 0; i < zmmax; i++) {
			if (zms[i].used) {
				if (zms[i].dead) {//����֡
					zms[i].frameindex++;
					if (zms[i].frameindex >= 20) {
						zms[i].used = false;
						killcount++;
						if (killcount == zm_max) {
							game::changestatus(WIN);
						}
					}
				}
				else if (zms[i].eating) {//��ֲ��֡
				     zms[i].frameindex = (zms[i].frameindex + 1) % 21;
				}
				else zms[i].frameindex = (zms[i].frameindex + 1) % 22;//�ƶ�֡
			}
		}
	}


}



void create::bullet() {
	static int ans = 0;
	ans++;
	if (ans < 3)return;
	ans = 0;
	int line[3] = { 0 };
	int zmmax = sizeof(zms) / sizeof(zms[0]);
	int dangerx = win_width - imgzm[0].getwidth()+100;
	int bulletmax = sizeof(bullets) / sizeof(bullets[0]);
	for (int i = 0; i < zmmax; i++) {
		if (zms[i].used && zms[i].x < dangerx) {
			line[zms[i].row] = 1;
		}
	}
	//��ʼ���ӵ�
	for (int i = 0; i < 3; i++) {
		for (int j = 0; j < 9; j++) {
			if (map[i][j].type == 1&&line[i]) {
				map[i][j].shootime++;
				if (map[i][j].shootime >= 50) {
					map[i][j].shootime = 0;
					int k;
					for (k = 0; k < bulletmax && bullets[k].used; k++);
					if (k < bulletmax) {
						bullets[k].used = true;
						bullets[k].row = i;
						bullets[k].speed = 4;
						bullets[k].blast = false;
						bullets[k].frameindex = 0;


					    int zwx = 256-112 + j * 81;
						int zwy = 179 + i * 102 + 14;
						bullets[k].x = zwx + imgzhiwu[map[i][j].type - 1][0]->getwidth() - 10;
						bullets[k].y = zwy + 5;
					}
				}
			}
		}
	}


}
void update::bullet() {
	static int ans = 0;
	ans++;
	if (ans < 3)return;
	ans = 0;
	int cnt = sizeof(bullets) / sizeof(bullets[0]);
	for (int i = 0; i < cnt; i++) {
		if (bullets[i].used) {
			bullets[i].x += bullets[i].speed;
			if (bullets[i].x > win_width) {
				bullets[i].used = false;
			}

			if (bullets[i].blast) {//�Ƿ�ը
				bullets[i].frameindex++;
				if (bullets[i].frameindex >= 4) {
					bullets[i].used = false;
				}
			}

		}
	}



}
void collision::check() {
	int bcnt = sizeof(bullets) / sizeof(bullets[0]);
	int zcnt = sizeof(zms) / sizeof(zms[0]);
	for (int i = 0; i < bcnt; i++) {
		if (bullets[i].used == false || bullets[i].blast)continue;

		for (int k = 0; k < zcnt; k++) {
			if (zms[k].used == false)continue;
			int x1 = zms[k].x + 80;
			int x2 = zms[k].x + 110;
			int x = bullets[i].x;
			if (!zms[k].dead&&bullets[i].row==zms[k].row && x > x1 && x < x2) {
				zms[k].blood -= 20;
				bullets[i].blast = true;
				bullets[i].speed = 0; 

				if (zms[k].blood <= 0) {
					zms[k].dead = true;
					zms[k].speed = 0;
					zms[k].frameindex = 0;
				}
				break;
			}
		}
	}

	int zcount = sizeof(zms) / sizeof(zms[0]);
	for (int i = 0; i < zcnt; i++) {
		if (zms[i].dead)continue;
		int row = zms[i].row;
		for (int k = 0; k < 9; k++) {
			if (map[row][k].type == 0) {
				continue;
			}
			else {
				int zhiwux = 256-112 + k * 81;
				int x1 = zhiwux + 10;
				int x2 = zhiwux + 60;
				int x3 = zms[i].x + 80;
				if (x3 > x1 && x3 < x2) {
					if (map[row][k].catched) {
						map[row][k].deadtime++;
						if (map[row][k].deadtime > 60) {
							map[row][k].deadtime = 0;
							map[row][k].type = 0;
							zms[i].eating = false;
							zms[i].frameindex = 0;
							zms[i].speed = 1;
						}
					}
					else {
						map[row][k].catched = true;
						map[row][k].deadtime=0;
						zms[i].eating = true;
						zms[i].speed = 0;
						zms[i].frameindex = 0;
					}
				}
			}
		}
	}


}

void update::zhiwu() {
	static int cnt = 0;
	cnt++;
	if (cnt > 2) {
		cnt = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 9; j++) {
				if (map[i][j].type > 0) {
					map[i][j].frameindex++;
					int index = map[i][j].frameindex;
					int zhiwutype = map[i][j].type - 1;
					if (imgzhiwu[zhiwutype][index] == NULL) {
						map[i][j].frameindex = 0;
					}
				}
			}
		}
	}
}
void update::game_content() {
	/*static int cnt=0;
	cnt++;
	if (cnt > 2) {
		cnt = 0;
		for (int i = 0; i < 3; i++) {
			for (int j = 0; j < 9; j++) {
				if (map[i][j].type > 0) {
					map[i][j].frameindex++;
					int index = map[i][j].frameindex;
					int zhiwutype = map[i][j].type - 1;
					if (imgzhiwu[zhiwutype][index] == NULL) {
						map[i][j].frameindex = 0;
					}
				}
			}
		}
	}*/
	update::zhiwu();
	create::sunshine();
	update::sunshine();

	create::zm();
	update::zm();

	create::bullet();
	update::bullet();

	collision::check();
}
int flag1 = 0;
void render::startUI() {
	IMAGE imgmenu, imgmenu1, imgmenu2;
	int flag = 0;
	loadimage(&imgmenu, "res/menu.png");
	loadimage(&imgmenu1, "res/menu1.png");
	loadimage(&imgmenu2, "res/menu2.png");
	while (1) {
		BeginBatchDraw();
		putimage(0, 0, &imgmenu);
		putimagePNG(475, 75, flag == 0 ? &imgmenu1 : &imgmenu2);
		ExMessage msg;
		if (peekmessage(&msg)) {
			if (msg.message == WM_LBUTTONDOWN && msg.x > 474 && msg.x < 774 && msg.y>75 && msg.y < 215) {
				flag = 1;

			}
			else if (msg.message == WM_LBUTTONUP && flag == 1) {
				EndBatchDraw();
				break;
			}
			else if (msg.message == WM_CLOSE) {
				flag1 = 1;
				break;
			}
		}
		EndBatchDraw();
	}
}  



void render::viewscence() {
	int xmin = win_width - imgbg.getwidth();
	vector2 points[9] = { {550,80},{530,160},{630,170},{530,200},{525,270},		//9����ʬվλ
		{565,370},{605,340},{705,280},{690,340} };
	int index[9];
	for (int i = 0; i < 9; i++) {
		index[i] = rand() % 11;
	}
	int cnt = 0;
	for (int x = 0; x >= xmin; x -= 2) {
		BeginBatchDraw();
		putimage(x, 0, &imgbg);
		cnt++;
		for (int k = 0; k < 9; k++) {
			putimagePNG(points[k].x - xmin + x, points[k].y, &imgzmstand[index[k]]);
			if (cnt >= 10) {
				index[k] = (index[k] + 1) % 11;
			}
		}
		if (cnt >= 10)cnt = 0;
		EndBatchDraw();
		Sleep(5);
	}
	for (int i = 0; i < 100; i++) {
		BeginBatchDraw();
		putimage(xmin, 0, &imgbg);
		for (int k = 0; k < 9; k++) {
			putimagePNG(points[k].x, points[k].y, &imgzmstand[index[k]]);
			index[k] = (index[k] + 1) % 11;
		}
		EndBatchDraw();
		Sleep(30);
	}
	cnt = 0;
	for (int x = xmin; x <= -112; x += 2) {
		BeginBatchDraw();
		putimage(x, 0, &imgbg);
		cnt++;
		for (int k = 0; k < 9; k++) {
			putimagePNG(points[k].x - xmin + x, points[k].y, &imgzmstand[index[k]]);
			if (cnt >= 10) {
				index[k] = (index[k] + 1) % 11;
			}
		}
		if (cnt >= 10)cnt = 0;
		EndBatchDraw();
		Sleep(5);
	}
}


void render::barsdown() {
	int height = imgbar.getheight();
	for (int y = -height; y <= 0; y++) {
		BeginBatchDraw();
		putimage(-112, 0, &imgbg);
		putimagePNG(250, y, &imgbar);
		for (int i = 0; i < zhiwucount; i++) {
			int x = 338 + i * 65;
			int Y = 6 + y;
			putimagePNG(x, Y, &imgcard[i]);
		}


		EndBatchDraw();
	}
}
bool game::checkover() {
	if (game::getstatus() == WIN) {
		/*mciSendString("close res/bg.mp3", NULL, 0, NULL);*/
		music::bg_close();
		Sleep(600);
		loadimage(0, "res/win2.png");
		/*mciSendString("play res/win.mp3", 0, 0, 0);*/
		music::win();
		return true;
	}
	else if (game::getstatus() == FAIL) {
		/*mciSendString("close res/bg.mp3", NULL, 0, NULL);*/
		music::bg_close();
		Sleep(600);
		loadimage(0, "res/fail2.png");
		/*mciSendString("play res/lose.mp3", 0, 0, 0);*/
		music::lose();
		return true;
	}
	else {
		return false;
	}
}
int main(void) {
	/*gameInit();*/
	game::init();
	music::bg_begin();
	/*mciSendString("play res/bg.mp3", 0, 0, 0); 
	mciSendString("setaudio res/bg.mp3 volume to 500", NULL, 0, NULL);*/
	render::startUI();
	if (flag1 == 1)return 0;
	render::viewscence();
	render::barsdown();
	int timer = 0;
	while (1) {
		timer += getDelay();
		user::click();
		if (timer < 10) {	//��������ֲ����Ⱦʱ��
			continue;
		}
		timer = 0;
		render::game_content();
		update::game_content();	
		if (game::checkover())break;
		
	}
	system("pause");
	return 0;
}
