//EasyXͼ�ο���ʵ��ͼ�ν���
#include <iostream>//���������ͷ�ļ�
#include <graphics.h>//easyx��ͷ�ļ�
#include <string>
#include<vector>
#include<conio.h>
using namespace std;


constexpr auto swidth =  600;
constexpr auto sheight = 1100;//�����ߣ�ȫ�ֱ���

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
}//���л����ڵľ��ο��Ӣ�ۻ����ھ��ο����غ�ʱ���ж�Ϊ��ײ
//һ����ʼ����
void Welcome() {


	LPCTSTR title = _T("�ɻ���ս");//LPCTSTR��EasyX���µ��������ͣ���Ҫʹ��_T��������ת��
	LPCTSTR tplay = _T("��ʼ��Ϸ");
	LPCTSTR texit = _T("�˳���Ϸ");

	RECT tplayr, texitr;//�������־��ο�ķ�Χ
	BeginBatchDraw();
	setbkcolor(WHITE);//������ɫ����
	cleardevice();
	settextstyle(60, 0, _T("����"));//���ָ�ʽ
	settextcolor(BLACK);//������ɫ
	outtextxy(swidth / 2 - textwidth(title) / 2, sheight / 5, title);//��Ļ��һ�� - �����ȵ�һ�루����ͼ����Ƽ������ƻ�ͼ���ͣ��� 
	
	settextstyle(40, 0, _T("����"));
	tplayr.left = swidth / 2 - textwidth(tplay) / 2;
	tplayr.right = tplayr.left + textwidth(tplay);
	tplayr.top = sheight / 5*2.5;
	tplayr.bottom = tplayr.top + textheight(tplay);

	texitr.left = swidth / 2-textwidth(texit)/2;
	texitr.right = texitr.left + textheight(texit);
	texitr.top = sheight /5 * 3;
	texitr.bottom = texitr.top + textheight(texit);

	outtextxy(tplayr.left, tplayr.top,tplay);
	outtextxy(texitr.left,texitr.top, texit);//��ʼ��Ϸ���˳���Ϸ������ť�Ŀ�����
	EndBatchDraw();


	while (true) {

		ExMessage mess;
		getmessage(&mess, EX_MOUSE);//��������ʵ�������Ч��
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


//һ����Ϸ��������

void Over(unsigned long long &kill) 
{

	printf_s("o");
	TCHAR* str = new TCHAR[128];
	_stprintf_s(str, 128, _T("��ɱ����%llu"), kill);

	settextcolor(RED);//����ɱ������ɫ

	outtextxy(swidth / 2 - textwidth(str) / 2, sheight / 5, str);

	//���̵���
	LPCTSTR info = _T("��Enter����");
	settextstyle(20, 0, _T("����"));
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

//�������л���Ӣ�ۡ��ӵ���Ӣ�ۻ��͵л��ӵ�����

class BK 
{
public:
	BK(IMAGE& img)
		:img(img), y(-sheight)//��ʼ��
	{

	}

	void Show()
	{
		if (y == 0) { y = -sheight; }//����ͼƬ��λ
		y += 4;
		putimage(0, y, &img);

	}
private:
	IMAGE& img;
	int y;//���ñ���ͼƬ��ÿ�α���������y++������ͼƬ������Ļ�Ͷˣ�ͼƬ��λ�����¹���
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
		rect.bottom = sheight;//Ӣ�ۻ�����

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
		if (peekmessage(&mess, EX_MOUSE))//����¼�����Ӣ�ۻ�
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
	RECT& GetRect() { return rect; }//��ȡ���α߿�
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
		rect.bottom = 0;//�л��ľ��ο�
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
			return false;//�ɳ����棬�л�����
		}
		rect.top += 4;
		rect.bottom += 4;//�ı�л������ٶ�
		putimage(rect.left, rect.top, &img);//û�ɳ����������

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
		rect.bottom = rect.top + img.getheight();//Ӣ�۷ɻ�������ӵ���Ӣ�۷ɻ����Ϸ�
	}

	bool Show()
	{
		if (rect.bottom <= 0)//�ӵ��ɳ�����
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
		rect.bottom = rect.top + img.getheight();//�л����ӵ��ڵл����·�

	}

	bool Show()
	{

		if (rect.top >= sheight)//����������ڵ��ڽ���ĸߣ����ж��ɳ����߽�
		{
			return false;
		}

		rect.top += 5;
		rect.bottom += 5;//�л������˶��������Ϊ+���˶����겻һ�£�����ٶȲ��Ч��
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

	IMAGE heroimg, enemyimg, bkimg, bulletimg;//ͼƬ��
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
				ebs.push_back(new Enemy_Bullet(bulletimg, i->GetRect()));//�����ел�������ӵ�


			}

		}
		BeginBatchDraw();
		
		bk.Show();
		Sleep(2);//�ӳ�
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


				}//���û�ü�����Ϣһֱ����û������
				

			}

		}
		
		hp.Show();
		
		auto bsit = bs.begin();//����false����ɾ��
		while (bsit != bs.end())
		{

			if (!(*bsit)->Show())
			{
				bsit = bs.erase(bsit);
			}
			else
			{
				bsit++;//���ҳ���������������
			}
		}


		auto ebsit = ebs.begin();//Ӣ���ӵ��ı���
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
				ebsit++;//���ҳ���������������
			}
		}



		auto it = es.begin();
		while (it != es.end())//�����л�
		{
			if (RectCrashRect((*it)->GetRect(), hp.GetRect()))
			{
				is_play = hp.hurt();
				hurtlast = clock();
			}

			auto bit = bs.begin();
			while (bit != bs.end())//�����ӵ�
			{
				if (RectCrashRect((*bit)->GetRect(), (*it)->GetRect()))//��ײ�ж�Ϊ��Ϸ����
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

	//easyx��ʼ������ʼ��ͼ�ν���
	initgraph(swidth, sheight, EX_NOMINIMIZE | EX_SHOWCONSOLE);//���ô�����С������ʾ����̨����
	bool is_live = true;//��Ϸ�Ƿ����ѭ��
	while (is_live)
	{
		Welcome();
		//Play
		is_live = Play();
	}


	return 0;
};