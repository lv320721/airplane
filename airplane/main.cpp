//EasyX图形库来实现图形界面
#include <iostream>
#include <graphics.h>//easyx的头文件
#include <string>
using namespace std;


constexpr auto swidth =  600;
constexpr auto sheight = 1100;

bool PointInRect(int x, int y, RECT& r)
{
	return (r.left <= x && x <= r.right && r.top <= y && y <= r.bottom);

}


//一个开始界面
void Welcome() {


	LPCTSTR title = _T("飞机大战");
	LPCTSTR tplay = _T("开始游戏");
	LPCTSTR texit = _T("退出游戏");

	RECT tplayr, texitr;
	BeginBatchDraw();
	setbkcolor(WHITE);
	cleardevice();
	settextstyle(60, 0, _T("黑体"));
	settextcolor(BLACK);
	outtextxy(swidth / 2 - textwidth(title) / 2, sheight / 5, title);
	
	settextstyle(40, 0, _T("黑体"));
	tplayr.left = swidth / 2 - textwidth(tplay) / 2;
	tplayr.right = tplayr.left + textwidth(tplay);
	tplayr.top = sheight / 5*2.5;
	tplayr.bottom = tplayr.top + textheight(tplay);

	texitr.left = swidth / 2-textwidth(texit)/2;
	texitr.right = texitr.left + textheight(texit);
	texitr.top = sheight /5*3;
	texitr.bottom = texitr.top + textheight(texit);

	outtextxy(tplayr.left, tplayr.top,tplay);
	outtextxy(texitr.left,texitr.top, texit);
	EndBatchDraw();


	while (true) {

		ExMessage mess;
		getmessage(&mess, EX_MOUSE);
		if (mess.lbutton)
		{
			if (PointInRect(mess.x, mess.y, tplayr))
			{
				return;
			}
			else if (PointInRect(mess.x, mess.y, texitr))
			{
				exit(0);
			}
		}

	};
	Sleep(100);

};

//背景、敌机、英雄、子弹

class BK 
{
public:
	BK(IMAGE& img)
		:img(img), y(-sheight)
	{

	}

	void Show()
	{
		if (y == 0) { y = -sheight; }
		y += 4;
		putimage(0, y, &img);

	}
private:
	IMAGE& img;
	int y;
};

class Hero 
{
public:
	Hero(IMAGE& img)
		:img(img)
	{
		rect.left = swidth / 2 - img.getwidth() / 2;
		rect.top = sheight - img.getheight();
		rect.right = rect.left = img.getwidth();
		rect.bottom = sheight;

	}
	void Show()
	{

		putimage(rect.left, rect.top, &img);

	}
	void Control()
	{
		ExMessage mess;
		if (peekmessage(&mess, EX_MOUSE))
		{
			rect.left = mess.x - img.getwidth() / 2;
			rect.top = mess.y - img.getheight() / 2;
			rect.right = rect.right = rect.left + img.getwidth();
			rect.bottom = rect.top + img.getheight();


		}

	}
private:
	IMAGE& img;
	RECT rect;

};

class Enemy
{


};




bool Play()
{

	setbkcolor(WHITE);
    cleardevice();
	bool is_play = true;

	IMAGE heroimg, enemyimg, bkimg, bulletimg;
	loadimage(&heroimg, _T("E:\\LXJ\\C++\\airplane\\airplane\\images\\me1.png"));
	loadimage(&enemyimg, _T("E:\\LXJ\\C++\\airplane\\airplane\\images\\enemy1.png"));
	loadimage(&bkimg, _T("E:\\LXJ\\C++\\airplane\\airplane\\images\\bk2.png"), swidth, sheight * 2);
	loadimage(&bulletimg, _T("E:\\LXJ\\C++\\airplane\\airplane\\images\\bullet1.png"));

	BK bk = BK(bkimg);
	Hero hp = Hero(heroimg);


	while (is_play)
	{

		BeginBatchDraw();
		
		bk.Show();
		Sleep(6);
		flushmessage();
		Sleep(2);
		hp.Control();
		
		hp.Show();
		

		EndBatchDraw();

	}
	return true;
};


int main() {

	//easyx初始化
	initgraph(swidth, sheight, EX_NOMINIMIZE | EX_SHOWCONSOLE);
	bool is_live = true;
	while (is_live)
	{
		Welcome();
		//Play
		is_live = Play();
	}


	return 0;
};