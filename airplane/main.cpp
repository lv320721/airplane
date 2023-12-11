//EasyX图形库来实现图形界面
#include <iostream>//输入输出流头文件
#include <graphics.h>//easyx的头文件
#include <string>
#include<vector>
#include<conio.h>
using namespace std;


constexpr auto swidth =  600;
constexpr auto sheight = 1100;//定义宽高，全局变量

constexpr unsigned int SHP = 4;
constexpr auto hurttime = 1000;//ms

bool PointInRect(int x, int y, RECT& r)
{
	return (r.left <= x && x <= r.right && r.top <= y && y <= r.bottom);

}

bool RectCrashRect(RECT &r1, RECT &r2)
{
	RECT r;
	r.left = r1.left - (r2.right - r2.left);
	r.right = r1.right;
	r.top = r1.top - (r2.bottom - r2.top);
	r.bottom = r1.bottom;

	return (r.left < r2.left && r2.left <= r.right && r.top <= r2.top && r2.top <= r.bottom);
}//当敌机所在的矩形框和英雄机所在矩形框有重合时，判定为相撞
//一个开始界面
void Welcome() {


	LPCTSTR title = _T("飞机大战");//LPCTSTR：EasyX中新的数据类型，需要使用_T进行类型转换
	LPCTSTR tplay = _T("开始游戏");
	LPCTSTR texit = _T("退出游戏");

	RECT tplayr, texitr;//定义文字矩形框的范围
	BeginBatchDraw();
	setbkcolor(WHITE);//背景颜色设置
	cleardevice();
	settextstyle(60, 0, _T("黑体"));//文字格式
	settextcolor(BLACK);//文字颜色
	outtextxy(swidth / 2 - textwidth(title) / 2, sheight / 5, title);//屏幕的一半 - 自身宽度的一半（具体图形设计见软件设计画图解释）， 
	
	settextstyle(40, 0, _T("黑体"));
	tplayr.left = swidth / 2 - textwidth(tplay) / 2;
	tplayr.right = tplayr.left + textwidth(tplay);
	tplayr.top = sheight / 5*2.5;
	tplayr.bottom = tplayr.top + textheight(tplay);

	texitr.left = swidth / 2-textwidth(texit)/2;
	texitr.right = texitr.left + textheight(texit);
	texitr.top = sheight /5 * 3;
	texitr.bottom = texitr.top + textheight(texit);

	outtextxy(tplayr.left, tplayr.top,tplay);
	outtextxy(texitr.left,texitr.top, texit);//开始游戏和退出游戏两个按钮的框架设计
	EndBatchDraw();


	while (true) {

		ExMessage mess;
		getmessage(&mess, EX_MOUSE);//阻塞函数实现鼠标点击效果
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

	}
	

};


//一个游戏结束界面

void Over(unsigned long long &kill) 
{

	printf_s("o");
	TCHAR* str = new TCHAR[128];
	_stprintf_s(str, 128, _T("击杀数：%llu"), kill);

	settextcolor(RED);//“击杀数”颜色

	outtextxy(swidth / 2 - textwidth(str) / 2, sheight / 5, str);

	//键盘导航
	LPCTSTR info = _T("按Enter返回");
	settextstyle(20, 0, _T("黑体"));
	outtextxy(swidth - textwidth(info), sheight - textheight(info), info);

	while (true)
	{
		ExMessage mess;
		getmessage(&mess, EX_KEY);
		if (mess.vkcode == 0x0D)
		{

			return;
		}


	}


}

//背景、敌机、英雄、子弹（英雄机和敌机子弹）类

class BK 
{
public:
	BK(IMAGE& img)
		:img(img), y(-sheight)//初始化
	{

	}

	void Show()
	{
		if (y == 0) { y = -sheight; }//背景图片归位
		y += 4;
		putimage(0, y, &img);

	}
private:
	IMAGE& img;
	int y;//设置背景图片，每次背景滚动，y++当背景图片到达屏幕低端，图片归位再重新滚动
};

class Hero 
{
public:
	Hero(IMAGE& img)
		:img(img), HP(SHP)
	{
		rect.left = swidth / 2 - img.getwidth() / 2;
		rect.top = sheight - img.getheight();
		rect.right = rect.left = img.getwidth();
		rect.bottom = sheight;//英雄机坐标

	}
	void Show()
	{
		setlinecolor(RED);
		setlinestyle(PS_SOLID, 4);

		putimage(rect.left, rect.top, &img);
		line(rect.left, rect.top - 5, rect.left + (img.getwidth() / SHP * HP), rect.top - 5);
	}
	void Control()
	{
		ExMessage mess;
		if (peekmessage(&mess, EX_MOUSE))//鼠标事件控制英雄机
		{
			rect.left = mess.x - img.getwidth() / 2;
			rect.top = mess.y - img.getheight() / 2;
			rect.right = rect.right = rect.left + img.getwidth();
			rect.bottom = rect.top + img.getheight();


		}

	}

	bool hurt()
	{
		HP--;
		return (HP == 0) ? false : true;
	}
	RECT& GetRect() { return rect; }//获取矩形边框
private:
	IMAGE& img;
	RECT rect;
	unsigned int HP;
};

class Enemy
{
public:
	Enemy(IMAGE& img, int x, IMAGE*& boom)
		:img(img), isdie(false), boomsum(0)
	{
		selfboom[0] = boom[0];
		selfboom[1] = boom[1];
		selfboom[2] = boom[2];
		rect.left = x;
		rect.right = rect.left + img.getwidth();
		rect.top = -img.getheight();
		rect.bottom = 0;//敌机的矩形框
	}
	
	bool Show()
	{

		if (isdie)
		{
			if (boomsum == 3)
			{
				return false;
			}
			putimage(rect.left, rect.top, selfboom + boomsum);
			boomsum++;

			return true;
		}
		if (rect.top >= sheight)
		{ 
			return false;//飞出画面，敌机销毁
		}
		rect.top += 4;
		rect.bottom += 4;//改变敌机下落速度
		putimage(rect.left, rect.top, &img);//没飞出画面就下落

		return true;
	}
	void Isdie()
	{

		isdie = true;
	}
	RECT& GetRect() { return rect; }
private:
	IMAGE& img;
	RECT rect;
	IMAGE selfboom[3];


	bool isdie;
	int boomsum;


};

class Bullet
{

public:
	Bullet(IMAGE& img, RECT pr)
		:img(img)
	{
		rect.left = pr.left + (pr.right - pr.left) / 2 - img.getwidth() / 2;
		rect.right = rect.left + img.getwidth();
		rect.top = pr.top - img.getwidth();
		rect.bottom = rect.top + img.getheight();//英雄飞机发射的子弹在英雄飞机的上分
	}

	bool Show()
	{
		if (rect.bottom <= 0)//子弹飞出画面
		{
			return false;
		}

		rect.top -= 3;
		rect.bottom -= 3;
		putimage(rect.left, rect.top, &img);

		return true;
     }
	RECT& GetRect() { return rect; }
protected:

	IMAGE& img;
	RECT rect;



};

class Enemy_Bullet :public Bullet
{
public:
	Enemy_Bullet(IMAGE& img, RECT pr)
		:Bullet(img, pr)
	{
		rect.left = pr.left + (pr.right - pr.left) / 2 - img.getwidth() / 2;
		rect.right = rect.left + img.getwidth();
		rect.top = pr.bottom;
		rect.bottom = rect.top + img.getheight();//敌机的子弹在敌机的下方

	}

	bool Show()
	{

		if (rect.top >= sheight)//如果顶部大于等于界面的高，就判定飞出来边界
		{
			return false;
		}

		rect.top += 5;
		rect.bottom += 5;//敌机向下运动，坐标改为+，运动坐标不一致，造成速度差的效果
		putimage(rect.left, rect.top, &img);

		return true;

	}

};

bool AddEnemy(vector<Enemy*>& es, IMAGE& enemyimg, IMAGE* boom)
{
	Enemy* e =new Enemy(enemyimg, abs(rand()) & (swidth - enemyimg.getwidth()), boom);
	
	for (auto& i : es)
	{

		if (RectCrashRect(i->GetRect(), e->GetRect()))
		{

			delete e;
			return false;
		}


	}
	es.push_back(e);
	return true;
}


bool Play()
{

	setbkcolor(WHITE);
    cleardevice();
	bool is_play = true;

	IMAGE heroimg, enemyimg, bkimg, bulletimg;//图片库
	IMAGE eboom[3];
	loadimage(&heroimg, _T("E:\\LXJ\\C++\\airplane\\airplane\\images\\me1.png"));
	loadimage(&enemyimg, _T("E:\\LXJ\\C++\\airplane\\airplane\\images\\enemy1.png"));
	loadimage(&bkimg, _T("E:\\LXJ\\C++\\airplane\\airplane\\images\\bk2.png"), swidth, sheight * 2);
	loadimage(&bulletimg, _T("E:\\LXJ\\C++\\airplane\\airplane\\images\\bullet1.png"));

	loadimage(&eboom[0], _T("E:\\LXJ\\C++\\airplane\\airplane\\images\\enemy1_down2.png"));
	loadimage(&eboom[1], _T("E:\\LXJ\\C++\\airplane\\airplane\\images\\enemy1_down3.png"));
	loadimage(&eboom[2], _T("E:\\LXJ\\C++\\airplane\\airplane\\images\\enemy1_down4.png"));

	BK bk = BK(bkimg);
	Hero hp = Hero(heroimg);

	vector<Enemy*> es;
	vector<Bullet*> bs;
	vector<Enemy_Bullet*> ebs;
	int bsing = 0;

	clock_t hurtlast = clock();
	unsigned long long kill = 0;
	for (int i = 0; i < 5; i++)
	{
		AddEnemy(es, enemyimg, eboom);
		

	}


	while (is_play)
	{
		bsing++;
		if (bsing % 10 == 0)
		{
			bs.push_back(new Bullet(bulletimg, hp.GetRect()));
			
		}

		if (bsing == 60)
		{
			bsing = 0;
			for (auto& i : es)
			{
				ebs.push_back(new Enemy_Bullet(bulletimg, i->GetRect()));//给所有敌机都添加子弹


			}

		}
		BeginBatchDraw();
		
		bk.Show();
		Sleep(2);//延迟
		flushmessage();
		Sleep(2);
		hp.Control();
		if (_kbhit())
		{
			char v = _getch();
			if (v == 0x20)
			{
				Sleep(500);
				while(true)
				{
					if (_kbhit())
					{
						v = _getch();
						if (v == 0x20)
						{
							break;
						}
					}
					Sleep(16);


				}//如果没用键盘消息一直动，没有阻塞
				

			}

		}
		
		hp.Show();
		
		auto bsit = bs.begin();//返回false进行删除
		while (bsit != bs.end())
		{

			if (!(*bsit)->Show())
			{
				bsit = bs.erase(bsit);
			}
			else
			{
				bsit++;//解决页面出界会结束的问题
			}
		}


		auto ebsit = ebs.begin();//英雄子弹的遍历
		while (ebsit != ebs.end())
		{

			if (!(*ebsit)->Show())
			{
				ebsit = ebs.erase(ebsit);
			}
			else
			{
				if (RectCrashRect((*ebsit)->GetRect(), hp.GetRect()))
				{
					if (clock() - hurtlast >= hurttime)
					{

						is_play = hp.hurt();
						hurtlast = clock();
					}


				}
				ebsit++;//解决页面出界会结束的问题
			}
		}



		auto it = es.begin();
		while (it != es.end())//遍历敌机
		{
			if (RectCrashRect((*it)->GetRect(), hp.GetRect()))
			{
				is_play = hp.hurt();
				hurtlast = clock();
			}

			auto bit = bs.begin();
			while (bit != bs.end())//遍历子弹
			{
				if (RectCrashRect((*bit)->GetRect(), (*it)->GetRect()))//相撞判定为游戏结束
				{
					delete(*it);
					it = es.erase(it);
					it = es.begin();
					delete(*bit);
					bs.erase(bit);

					kill++;
					break;

				}
				bit++;

			}
			if (!(*it)->Show())
			{
				delete(*it);
				it = es.erase(it);
				it = es.begin();
			}
			it++;

		}
		for (int i = 0; i < 5 - es.size(); i++)
		{
			AddEnemy(es, enemyimg, eboom);

		}
		EndBatchDraw();

	}
	printf_s("e");
	Over(kill);
	return true;
};


int main() {

	//easyx初始化，初始化图形界面
	initgraph(swidth, sheight, EX_NOMINIMIZE | EX_SHOWCONSOLE);//禁用窗口最小化，显示控制台窗口
	bool is_live = true;//游戏是否进行循环
	while (is_live)
	{
		Welcome();
		//Play
		is_live = Play();
	}


	return 0;
};