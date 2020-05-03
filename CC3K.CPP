// Author: ZHM 18011485
// Date:19/10/22
// ��������Ҫ��C++11���������� ������ҪUTF-8����

//#include <iostream>
//#include <fstream>
//#include <cstdlib>
//#include <string>
//#include <math.h>
//#include <cassert>
//#include <map>
//#include <sstream>
//#include <Windows.h>
//#include <ctime>
#include <bits/stdc++.h>
#include <Windows.h> 


using namespace std;
using std::map;
using std::srand;
class Player;
class Enemies;
class Dragon;
class Potion;
class Door;
class Gold;
class MerchantGold;
int numFloor = 0;
string mapList[5] = {"./source/floor1.data","./source/floor2.data","./source/floor3.data","./source/floor4.data","./source/floor5.data"};
Door *door;
int ALL_Gold = 0;  // PC��ǰ���еĽ���
string Action;   // Actionָ��PCĿǰÿһ���Ķ���
Enemies *EnemList[20];
Potion *PotionList[10];
Player *Hero;
Gold *GoldList[10];
Enemies *attEnem;
MerchantGold *MerGold[5];    // ���˶ڻ����
int MerGoldNum;
int DraGoldNum;
char Map[25][79];           // �洢���ƶ�λ�� 
char CpyMap[25][79];
char changechar[2];           // �洢Ӣ�۵�ǰ��ռ���� 
int isrestart = 1;
char ArrEnem[7] = {'H','W','E','O','L','\0','\0'};   // ֻ�е�PC�����������ڻ���ɱ������ʱ ��Ӧλ�òŻ�����Ԫ���Ա�ʵ���Զ�����
int ArrEnemNum;
char AttEnem[7] = {'H','W','E','O','L','M','D'};
// ***************���췽����λ�ñ䶯�Ĺ�ϵ����***********//
typedef struct{
	int xchange;
	int ychange;
}DireMap;
DireMap no = {-1,0},so = {1,0},ea = {0,1},we = {0,-1},ne = {-1,1},nw = {-1,-1},se = {1,1},sw = {1,-1};
map<string,DireMap>DireList;
// ******************************************************// 

class Door{
public:
	int potion_x;
	int potion_y;
};

class Gold{
public:
	int position_x;
	int position_y;
	Enemies *Master;
	char name;
	int GoldNum;
	Gold(int g,char n):name(n),GoldNum(g){}
};

class CommonGold:public Gold{
public:
	CommonGold():Gold(2,'C'){}
};

class SmallGold:public Gold{
public:
	SmallGold():Gold(1,'S'){}
};

class DragonGold:public Gold{
public:
	DragonGold():Gold(6,'D'){}
};

class MerchantGold{
public:
	int position_x;
	int position_y;
	char name = 'M';
	int GoldNum = 4;
	MerchantGold(int x,int y):position_x(x),position_y(y){}
};

class Enemies{
public:
	int HP;
	int isatt = 0;
	int Atk;
	int Defense;
	int position_x = 0;    // ��ʼ����λ�� x������ y������ 
	int position_y = 0;
	int GoldNum;
	char name;
	Enemies(int dHp, int dAtk, int dDefense):HP(dHp), Atk(dAtk), Defense(dDefense){}
	virtual void ATTACK(Player *&PC) = 0;    // �������� 
	void goUp(){
		--position_x;
	}
	void goDown(){
		++position_x;
	}
	void goLeft(){
		--position_y;
	}
	void go_Right(){
		++position_y;
	}
	virtual void Died(){
		Map[position_x][position_y] = '.';
		position_x = -100;
		position_y = -100;
	}
};

// Ӣ���� 

class Player{
public:
	int HP;
	int Atk;
	int Defense;
	int position_x = 0;    // ��ʼ����λ�� 
	int position_y = 0;
	int EffAtk = 0;          // ��ʾPotion��Ч��(��һ����ʧ) 
	int EffDef = 0;
	char name;
	Player(int dHp, int dAtk, int dDefense):HP(dHp), Atk(dAtk), Defense(dDefense),EffAtk(0),EffDef(0){}
	virtual void ATTACK(Enemies *&en) = 0; 
	// λ�ñ仯���� 
	void goUp(){
		--position_x;
		Action += "PC moves North."; 
	}
	void goDown(){
		++position_x;
		Action += "PC moves South.";
	}
	void goLeft(){
		--position_y;
		Action += "PC moves West.";
	}
	void go_Right(){
		++position_y;
		Action += "PC moves East.";
	}
	void go_NE(){
		--position_x;
		++position_y;
		Action += "PC moves North-east.";
	}
	void go_NW(){
		--position_x;
		--position_y;
		Action += "PC moves North-west.";
	}
	void go_SE(){
		++position_x;
		++position_y;
		Action = "PC moves South-east.";
	}
	void go_SW(){
		++position_x;
		--position_y;
		Action = "PC moves South-west.";
	}
	void Died(){    // ʵ��PC������ ��ѯ���Ƿ������; 
		char c;
		while(true)
		{
			cout << "PC���������Ƿ���һ������Ϸ?[Y/N]" << endl;
			cin >> c;
			switch(c){
				case 'Y':isrestart = 0;return ;
				case 'N':	
					delete []EnemList;
					delete []PotionList;
					delete []MerGold;
					delete []GoldList;
					delete door;
					delete Hero;
					exit(1);
					return ;
				default:
					cout << "ѡ����� ������ѡ��" << endl; 
					break;
			}
		}
	}
};     // PC��Enemies��ʹ�û���ָ����ͳһ����

class Shade:public Player{
public:
	Shade():Player(125, 25, 25){name = 'S';}
	void ATTACK(Enemies *&en){
		string str;
		stringstream stream;
		stream << en->name;
		str = stream.str();
		Action += "PC attack the " + str + " " + to_string(ceil((100.0 / (100 + en->Defense))*this->Atk)) + " damage.";
		if(en->name == 'M'){
			ArrEnemNum = 6;
			ArrEnem[5] = 'M';
		}
		en->HP -= ceil((100.0 / (100 + en->Defense))*this->Atk);
		if(en->HP <= 0){
			Action += str + " Died.";
		 	ALL_Gold += en->GoldNum;
		 	en->Died();
		}else{
			Action += str + " ��ʣ�� " + to_string(en->HP) + " HP."; 
		}
	}
};

class Drow:public Player{
public:
	Drow():Player(150, 25, 15){name = 'D';}
	static float magPotion;
	void ATTACK(Enemies *&en){
		string str;
		stringstream stream;
		stream << en->name;
		str = stream.str();
		Action += "PC attack the " + str + " " + to_string(ceil((100.0 / (100 + en->Defense))*this->Atk)) + " damage.";
		if(en->name == 'M'){
			ArrEnemNum = 6;
			ArrEnem[5] = 'M';
		}
		if(en->HP <= 0){
			Action += str + " Died.";
		 	ALL_Gold += en->GoldNum;
			en->Died(); 
		}else{
			Action += str + " ��ʣ�� " + to_string(en->HP) + " HP."; 
		}
	}
};

float magPotion = 1.5;

class Vampire:public Player{
public:
	Vampire():Player(50, 25, 25){name = 'V';}
	void ATTACK(Enemies *&en){
		string str;
		stringstream stream;
		stream << en->name;
		str = stream.str();
		Action += "PC attack the " + str + " " + to_string(ceil((100.0 / (100 + en->Defense))*this->Atk)) + " damage.";
		if(en->name == 'M'){
			ArrEnemNum = 6;
			ArrEnem[5] = 'M';
		}
		en->HP -= ceil((100.0 / (100 + en->Defense))*this->Atk);
		this->HP += 5;
		if(en->HP <= 0){
			Action += str + " Died.";
			ALL_Gold += en->GoldNum;
			en->Died();
		}else{
			Action += str + " ��ʣ�� " + to_string(en->HP) + " HP."; 
		}
	}
};

class Troll:public Player{
public:
	Troll():Player(120, 25, 15){name = 'T';}
	void ATTACK(Enemies *&en){
		string str;
		stringstream stream;
		stream << en->name;
		str = stream.str();
		if(en->name == 'M'){
			ArrEnemNum = 6;
			ArrEnem[5] = 'M';
		}
		Action += "PC attack the " + str + " " + to_string(ceil((100.0 / (100 + en->Defense))*this->Atk)) + " damage.";
		en->HP -= ceil((100.0 / (100 + en->Defense))*this->Atk);
		if(en->HP <= 0){
			Action += str + " Died.";
			ALL_Gold += en->GoldNum;
			en->Died();
		}else{
			Action += str + " ��ʣ�� " + to_string(en->HP) + " HP."; 
		}
	}
};

class Goblin:public Player{
public:
	Goblin():Player(110, 15, 20){name = 'G';}	
	void ATTACK(Enemies *&en){
		string str;
		stringstream stream;
		stream << en->name;
		str = stream.str();
		if(en->name == 'M'){
			ArrEnemNum = 6;
			ArrEnem[5] = 'M';
		}
		Action += "PC attack the " + str + " " + to_string(ceil((100.0 / (100 + en->Defense))*this->Atk)) + " damage.";
		en->HP -= ceil((100.0 / (100 + en->Defense))*this->Atk);
		if(en->HP <= 0){
			Action += str + " Died.";
			ALL_Gold += 5;
			en->Died();
		}else{
			Action += str + "��ʣ�� " + to_string(en->HP) + " HP."; 
		}
	}
};

// ������ 

class Human:public Enemies{
public:
	Human():Enemies(140, 20, 20){GoldNum = 4;name = 'H';}
	virtual void ATTACK(Player *&PC){
		string str;
		stringstream stream;
		stream << name;
		str = stream.str();
		if(rand()%2){
			Action += str + " attack the PC " + to_string(ceil((100.0 / (100 + PC->Defense))*this->Atk)) + " damage.";
			PC->HP -= ceil((100.0 / (100 + PC->Defense))*this->Atk);
		}
		if(PC->HP <= 0){
			PC->Died();
		}
	} 
};

class Merchant:public Enemies{   // ������ 
public:
	Merchant():Enemies(30, 70, 5){this->GoldNum = 0;name = 'M';}
	void Died(){
		MerGold[MerGoldNum] = new MerchantGold(position_x,position_y);
//		MerGold[MerGoldNum]->position_x = position_x;
//		MerGold[MerGoldNum]->position_y = position_y;
		Map[position_x][position_y] = 'G';
		++MerGoldNum;
		position_x = -100;
		position_y = -100;
	}
	virtual void ATTACK(Player *&PC){
		string str;
		stringstream stream;
		stream << name;
		str = stream.str();
		if(rand()%2){
			Action += str + " attack the PC " + to_string(ceil((100.0 / (100 + PC->Defense))*this->Atk)) + " damage.";
			PC->HP -= ceil((100.0 / (100 + PC->Defense))*this->Atk);
		}
		if(PC->HP <= 0){
			PC->Died();
		}
	}
};

class Dragon:public Enemies{
public:
	Dragon():Enemies(150, 20, 20){this->GoldNum = 0;name = 'D';}
	virtual void ATTACK(Player *&PC){
		string str;
		stringstream stream;
		stream << name;
		str = stream.str();
		if(rand()%2){
			Action += str + " attack the PC " + to_string(ceil((100.0 / (100 + PC->Defense))*this->Atk)) + " damage.";
			PC->HP -= ceil((100.0 / (100 + PC->Defense))*this->Atk);
		}
		if(PC->HP <= 0){
			PC->Died();
		}
	}
};

class Dwarf:public Enemies{
public:
	Dwarf():Enemies(100, 20, 30){name = 'W';GoldNum = rand() % 2+ 1;}
	virtual void ATTACK(Player *&PC){
		string str;
		stringstream stream;
		stream << name;
		str = stream.str();
		if(PC->name == 'V'){
			Action += name + " attack the PC 5 damage.";
			PC->HP -= 5;
		}
		if(rand()%2){
			Action += name + " attack the PC " + to_string(ceil((100.0 / (100 + PC->Defense))*this->Atk)) + " damage.";
			PC->HP -= ceil((100.0 / (100 + PC->Defense))*this->Atk);
		}
		if(PC->HP <= 0){
			PC->Died();
		}
	} 
};

class Elf:public Enemies{
public:
	Elf():Enemies(140, 30, 10){name = 'E';GoldNum = rand() % 2+ 1;}
	virtual void ATTACK(Player *&PC){
		string str;
		stringstream stream;
		stream << name;
		str = stream.str();
		int flag = 2;
		int i;
		if(PC->name == 'D'){
			flag = 1;
		}
		for(i = 0;i < flag;++i){
			if(rand()%2){
				Action += str + " attack the PC " + to_string(ceil((100.0 / (100 + PC->Defense))*this->Atk)) + " damage.";
				PC->HP -= ceil((100.0 / (100 + PC->Defense))*this->Atk);
			}
		}
		if(PC->HP <= 0){
			PC->Died();
		}
	}
};

class Orcs:public Enemies{
public:
	float KillDouble;
	Orcs():Enemies(180, 30, 25){name = 'O';KillDouble = 1;GoldNum = rand() % 2+ 1;}
	virtual void ATTACK(Player *&PC){
		string str;
		stringstream stream;
		stream << name;
		str = stream.str();
		if(rand()%2){
			if(PC->name == 'G'){
				KillDouble = 1.5;
			}
			Action += str + " attack the PC " + to_string(KillDouble *ceil((100.0 / (100 + PC->Defense))*this->Atk)) + " damage.";
			PC->HP -= KillDouble * ceil((100.0 / (100 + PC->Defense))*this->Atk);
			KillDouble = 1;
		}
		if(PC->HP <= 0){
			PC->Died();
		}
	} 
};

class Halfling:public Enemies{
public:
	Halfling():Enemies(100, 15, 20){name = 'L';GoldNum = rand() % 2+ 1;}   // ��ʼ���������� 
	virtual void ATTACK(Player *&PC){
		string str;
		stringstream stream;
		stream << name;
		str = stream.str();
		if(rand()%2){
			Action += str + " attack the PC " + to_string(ceil((100.0 / (100 + PC->Defense))*this->Atk)) + " damage.";
			PC->HP -= ceil((100.0 / (100 + PC->Defense))*this->Atk);
		}
		if(PC->HP <= 0){
			PC->Died();
		}
	} 
};

class Potion{
public:
	int position_x;
	int position_y;
	virtual void Effect(Player *&pc) = 0;
	void Used(){
		Map[position_x][position_y] = '.';
		position_x = -100;
		position_y = -100;
	}
};

class RH:public Potion{
public:
	void Effect(Player *&pc){
		if(pc->name == 'D'){
			pc->HP += 15;
		}
		pc->HP += 10;
		if(Hero->name == 'T' && Hero->HP > 120){
			Hero->HP = 120;
		}
		Used();
	}
};

class BA:public Potion{
public:
	void Effect(Player *&pc){
		if(pc->name == 'D'){
			pc->EffAtk += 8;
		}
		pc->EffAtk += 5;
		pc->Atk += pc->EffAtk;
		Used();
	}
};

class BD:public Potion{
public:
	void Effect(Player *&pc){
		if(pc->name == 'D'){
			pc->EffDef += 8;
		}
		pc->EffDef += 5;
		pc->Defense += pc->EffDef;
		Used();
	}
};

class PH:public Potion{
public:
	void Effect(Player *&pc){
		if(pc->name == 'D'){
			pc->HP -= 15;
		}
		pc->HP -= 10;
		if(pc->HP <= 0){
			pc->HP = 1;
		}
		Used();
	}
};

class WA:public Potion{
public:
	void Effect(Player *&pc){
		if(pc->name == 'D'){
			pc->EffAtk -= 8;
		}
		pc->EffAtk -= 5;
		pc->Atk += pc->EffAtk;
		Used();
	}
};

class WD:public Potion{
public:
	void Effect(Player *&pc){
		if(pc->name == 'D'){
			pc->EffDef -= 8;
		}
		pc->EffDef -= 5;
		pc->Defense += pc->EffDef;
		Used();
	}
};

void InitGame(){
	srand(time(0));
	// ���ѡ�����
	Action = "ð����ʽ��ʼ!!!";
	DraGoldNum = 0; 
	changechar[0] = {'.'};
	changechar[1] = {'.'};
	numFloor = 0;
	ArrEnem[0] = {'H'};
	ArrEnem[1] = {'W'};
	ArrEnem[2] = {'E'};
	ArrEnem[3] = {'O'};
	ArrEnem[4] = {'L'};
	ArrEnem[5] = {'\0'};
	ArrEnem[6] = {'\0'};
	ArrEnemNum = 5;
	DireList["no"] = no;
	DireList["so"] = so;
	DireList["ea"] = ea;
	DireList["we"] = we;
	DireList["ne"] = ne;
	DireList["nw"] = nw;
	DireList["se"] = se;
	DireList["sw"] = sw;
	isrestart = 1;
	char choice;
	int i = 0,j = 0;
	char c;
	int flag = 1;
	while(flag){
		cout << "Choose your race for your fight:" << endl;
		cout << "[s]hade - HP: 125, Atk: 25, Def: 25" << endl;
		cout << "[d]row - HP: 150, Atk: 25, Def: 15" << endl;
		cout << "[v]ampire - HP: 50, Atk: 25, Def: 25" << endl;
		cout << "[t]roll - HP: 120, Atk: 25, Def: 15" << endl;
		cout << "[g]oblin - HP: 110, Atk: 15, Def: 20" << endl; 
		cin >> choice;
		switch (choice){  // �û�������� 
			case 's':flag = 0;Hero = new Shade();break;
			case 'd':flag = 0;Hero = new Drow();break;
			case 'v':flag = 0;Hero = new Vampire();break;
			case 't':flag = 0;Hero = new Troll();break;
			case 'g':flag = 0;Hero = new Goblin();break;
			default:
				cout << "The race you chose has not been developed yet, please choose another one" << endl;
			}
		}
    // ��ͼ��ʼ��
	ifstream openFile(mapList[numFloor].data());
	openFile >> noskipws;
	assert(openFile.is_open());   //��ʧ��,�����������Ϣ,����ֹ�������� 
	while(!openFile.eof() && openFile.peek() != EOF){           // ��ʼ����ͼ 
		openFile >> c;
		cout << c;
		Map[i][j] = c;
		++j;
		if(j == 80){
			++i;
			j = 0;
		}
	}
	cout << endl << "Map Initialized." << endl;
	openFile.close();                                       // ��ͼ��ȡ��� 
}

// ��֪��ͼ�����е��λ�� ��ʹ�õ��λ��Ϊ�����x y���긳ֵ �Ա������λ�ñ任 
void getMap(){
	int i,j;   // i������ j������
	for(i = 0;i < 25;i++){
		for(j = 0;j < 79;++j){
			cout << Map[i][j];
		}
		cout << endl;
	}
}   // ��¼ÿһ�ִεĵ�ͼ��Ϣ

// ��ɵ�ͼ�ײ�����Ϣ��ʾ 
void ShowData(){
	cout << endl << "Race: " << Hero->name << " Gold: " << ALL_Gold <<"\t\t\t\t" << "Floor:" << numFloor + 1<< endl;               
	cout << "HP: " << Hero->HP << endl;
	cout << "Atk: " << Hero->Atk << endl;  
	cout << "Def: " << Hero->Defense << endl;
	cout << "Action:" << endl;
	cout << Action << endl;
	cout << "Type [h] for help" << endl;
}

bool goChange(Player *&p,int x,int y);

// ���ѡ���� 
void Choice(){ 
	Action = "";
	attEnem->isatt = 0;
	string ch;
	string dire;
	while(true){
		cin >> ch;
		if(ch == "no"){    // ����λ�ñ䶯 �ж��ƶ��Ƿ����
			if(goChange(Hero,Hero->position_x - 1,Hero->position_y)){
				Map[Hero->position_x][Hero->position_y] = changechar[0]; // ��ʱHero��position���Ի�δ�䶯 ����ԭ���� 
				changechar[0] = changechar[1];
				Hero->goUp();
				break;
			}
			cout << "λ�ñ䶯������." << endl; 
		}else if(ch == "so"){
			if(goChange(Hero,Hero->position_x + 1,Hero->position_y)){
				Map[Hero->position_x][Hero->position_y] = changechar[0];
				changechar[0] = changechar[1];
				Hero->goDown();
				break;
			}
			cout << "λ�ñ䶯������." << endl;;	
		}else if(ch == "ea"){   // ������ 
			if(goChange(Hero,Hero->position_x,Hero->position_y + 1)){
				Map[Hero->position_x][Hero->position_y] = changechar[0];
				changechar[0] = changechar[1];
				Hero->go_Right();
				break;
			}
			cout << "λ�ñ䶯������." << endl;
		}else if(ch == "we"){   // ������ 
			if(goChange(Hero,Hero->position_x,Hero->position_y - 1)){
				Map[Hero->position_x][Hero->position_y] = changechar[0];
				changechar[0] = changechar[1];
				Hero->goLeft();
				break;
			}
			cout << "λ�ñ䶯������." << endl;
		}else if(ch == "ne"){
			if(goChange(Hero,Hero->position_x - 1,Hero->position_y + 1)){
				Map[Hero->position_x][Hero->position_y] = changechar[0];
				changechar[0] = changechar[1];
				Hero->goUp();
				Hero->go_Right();
				break;
			}
			cout << "λ�ñ䶯������." << endl;
		}else if(ch == "nw"){
			if(goChange(Hero,Hero->position_x - 1,Hero->position_y - 1)){
				Map[Hero->position_x][Hero->position_y] = changechar[0];
				changechar[0] = changechar[1];
				Hero->goUp();
				Hero->goLeft();
				break;
			}
			cout << "λ�ñ䶯������." << endl;
		}else if(ch == "se"){
			if(goChange(Hero,Hero->position_x + 1,Hero->position_y + 1)){
				Map[Hero->position_x][Hero->position_y] = changechar[0];
				changechar[0] = changechar[1];
				Hero->goDown();
				Hero->go_Right();
				break;
			}
			cout << "λ�ñ䶯������." << endl;
		}else if(ch == "sw"){
			if(goChange(Hero,Hero->position_x + 1,Hero->position_y - 1)){
				Map[Hero->position_x][Hero->position_y] = changechar[0];
				changechar[0] = changechar[1];
				Hero->goDown();
				Hero->goLeft();
				break;
			}
			cout << "λ�ñ䶯������." << endl;
		}else if(ch == "u"){                // ʹ��ĳ�����ϵ�Potion;
			cin >> dire;              // ���뷽��
			int x = Hero->position_x + DireList[dire].xchange;
			int y = Hero->position_y + DireList[dire].ychange;
			if(Map[x][y] == 'P'){
				for(int PotionIter = 0;PotionIter < 10;++PotionIter){
					if(PotionList[PotionIter]->position_x == x && PotionList[PotionIter]->position_y){
						PotionList[PotionIter]->Effect(Hero);
						break;
					}
				}
				break;
			}else{
				cout << "λ�ò���ȷ �˴���Potion." << endl;
			}
		}else if(ch == "a"){                // ����ĳ�����ϵĵ���;
		 	cin >> dire;
		 	int EnemIter;
			int x = Hero->position_x + DireList[dire].xchange;
			int y = Hero->position_y + DireList[dire].ychange;
		 	for(EnemIter = 0;EnemIter < 7;++EnemIter){
		 		if(Map[x][y] == AttEnem[EnemIter]){     // ƥ��ɹ� 
		 			for(int i = 0;i < 20;++i){
		 				if(EnemList[i]->position_x == x && EnemList[i]->position_y == y){
		 					EnemList[i]->isatt = 1;
		 					attEnem = EnemList[i];
		 					if(EnemList[i]->name == 'L'){
		 						if(rand() % 2){
		 							Hero->ATTACK(EnemList[i]);
								}	
							 }else{
			 					Hero->ATTACK(EnemList[i]);
			 				}
							break;	
						}
					}
					break;
				}
			 }
			 if(EnemIter != 7){   // ������������� 
			 	break;
			 }
			 cout << "λ�ò���ȷ �˴��޹���." << endl;; 
		}else if(ch == "q"){
			delete []*EnemList;
			delete []*GoldList;
			delete []*MerGold;
			delete []*PotionList;
			delete door;
			delete Hero;
			exit(1);
		}else if(ch == "r"){
			isrestart = 0;
			return;
		}else if(ch == "h"){
			cout << "Directions: [no]rth, [so]uth, [ea]st, [we]st, [ne], [nw], [se], [sw] " << endl;
			cout << "a <Direction>: attack enemy at given direction" << endl;
			cout << "r: restart game" << endl;
			cout << "q: quit game" << endl;
			cout << "h: see help instructions" << endl;
		}else{
			cout << "����ָ��, ����������." << endl;
		}
	}
	if(Hero->name == 'T'){
		Hero->HP += 5;
		if(Hero->HP > 120){
			Hero->HP = 120;
		}
	}
}

bool EnemJudge(Enemies *&p,int x,int y);

void EnemChange(){            // ����λ���Զ��䶯 
	int i;
	int flag;
	int isSuccess;
	for(i = 0;i < 20;i++){
		isSuccess = 1;
		while(isSuccess){
			if(EnemList[i]->position_x <= 0 || EnemList[i]->position_y <= 0 || EnemList[i]->name == 'D' || EnemList[i]->isatt){
				break;
			}
			flag = rand() % 4;   // 0 �� 1 �� 2 �� 3 �� 
			switch(flag){
				case 0:
					if(EnemJudge(EnemList[i],EnemList[i]->position_x - 1,EnemList[i]->position_y)){
						Map[EnemList[i]->position_x][EnemList[i]->position_y] = '.';
						isSuccess = 0;
						EnemList[i]->goUp();
					}
					break;
				case 1:
					if(EnemJudge(EnemList[i],EnemList[i]->position_x + 1,EnemList[i]->position_y)){
						Map[EnemList[i]->position_x][EnemList[i]->position_y] = '.';
						isSuccess = 0;
						EnemList[i]->goDown();
					}
					break;
				case 2:
					if(EnemJudge(EnemList[i],EnemList[i]->position_x,EnemList[i]->position_y - 1)){
						Map[EnemList[i]->position_x][EnemList[i]->position_y] = '.';
						isSuccess = 0;
						EnemList[i]->goLeft();
					}
					break;
				case 3:
					if(EnemJudge(EnemList[i],EnemList[i]->position_x,EnemList[i]->position_y + 1)){
						Map[EnemList[i]->position_x][EnemList[i]->position_y] = '.';
						isSuccess = 0;
						EnemList[i]->go_Right();
					}
					break;
			}
		}
	}
}

int EatingGold(int x,int y){           // PC�Խ�Һ��� 
	int i;
	for(i = 0;i < 10;++i){
		if(GoldList[i]->position_x == x && GoldList[i]->position_y == y){
			if(GoldList[i]->name == 'D'){
				if(GoldList[i]->Master->HP <= 0){
					ALL_Gold += GoldList[i]->GoldNum;
					Map[x][y] = '@';
					GoldList[i]->position_x = -100;
					GoldList[i]->position_y = -100;
					return 1;
				}else{
					cout << "���ڻ���ʱ����ʳ�� ";
					return 0;
				}
			}else{
				ALL_Gold += GoldList[i]->GoldNum;
				Map[x][y] = '@';
				GoldList[i]->position_x = -100;
				GoldList[i]->position_y = -100;
				return 1;
			}
		}
	}
	for(i = 0;i < MerGoldNum;++i){
		if(MerGold[i]->position_x == x && MerGold[i]->position_y == y){
			ALL_Gold += MerGold[i]->GoldNum;
			Map[x][y] = '@';
			MerGold[i]->position_x = -100;
			MerGold[i]->position_y = -100;
			return 1;
		}
	}
	return 0;
}

// �ж��ƶ��Ƿ������С����
inline bool goChange(Player *&p,int x,int y) {
	if(Map[x][y] == '.' || Map[x][y] == '+' || Map[x][y] == '\\' || Map[x][y] == '#'){
		changechar[1] = Map[x][y];
		Map[x][y] = '@';   // �Ե�ͼ���и��� 
		return 1;
	}else if(Map[x][y] == 'G'){
		if(EatingGold(x,y)){
			changechar[1] = '.';
			return 1;
		}else{
			return 0;
		}
	}
	return 0;
}

inline bool EnemJudge(Enemies *&p,int x,int y){
	if(x > 0 && y > 0){
		if(Map[x][y] == '.'){
			Map[x][y] = p->name;
			return 1;
		}
	}
	return 0;
}

void getRoom(int x,int y);
void RandEnemies(){   // ��ɫ�б������� 
	int i,j;
	int flag;
	for(i = 0;i < 10;++i){
		flag = rand() % 8;
		if(flag < 5){
			GoldList[i] = new CommonGold();
		}else if(flag < 7){
			GoldList[i] = new SmallGold();
		}else{
			GoldList[i] = new DragonGold();
			EnemList[19 - DraGoldNum] = new Dragon();
			GoldList[i]->Master = EnemList[19 - DraGoldNum];
			++DraGoldNum;
		}
		flag = 1;
		while(flag){
			int x1 = rand() % 23 + 2;
			int y1 = rand() % 78 + 2;
			if(Map[x1][y1] == '.' && GoldList[i]->name == 'D'){
				map<string,DireMap>::iterator p;
				for(p = DireList.begin();p != DireList.end();p++){
					int x = x1 + p->second.xchange;
					int y = y1 + p->second.ychange;
					if(Map[x][y] == '.'){
						GoldList[i]->Master->position_x = x;
						GoldList[i]->Master->position_y = y;
						GoldList[i]->position_x = x1;
						GoldList[i]->position_y = y1;
						Map[x][y] = 'D';
						Map[x1][y1] = 'G';
						flag = 0;
						break;
					}
				}
			}else if(Map[x1][y1] == '.'){
				GoldList[i]->position_x = x1;
				GoldList[i]->position_y = y1;
				Map[x1][y1] = 'G';
				flag = 0;
				break;
			}
		}
	}
	for(i = 0;i < 20 - DraGoldNum;++i){
		flag = rand()%18;                // ��ɹ���ķ��� 
		if(flag < 4){
			EnemList[i] = new Human();
		}else if(flag < 7){
			EnemList[i] = new Dwarf();
		}else if(flag < 12){
			EnemList[i] = new Halfling();
		}else if(flag < 14){
			EnemList[i] = new Elf();
		}else if(flag < 16){
			EnemList[i] = new Orcs();
		}else if(flag < 18){
			EnemList[i] = new Merchant();
		}                      // ����������� 
		while(true){
			EnemList[i]->position_x = rand() % 23 + 2;
			EnemList[i]->position_y = rand() % 78 + 2;
			if(Map[EnemList[i]->position_x][EnemList[i]->position_y] == '.'){
				Map[EnemList[i]->position_x][EnemList[i]->position_y] = EnemList[i]->name;
				break;
			}
		}
	}
	for(i = 0;i < 10;i++){   // Potion���� 
		flag = rand() % 6;
		switch(flag){
			case 0:PotionList[i] = new RH();break;
			case 1:PotionList[i] = new BA();break;
			case 2:PotionList[i] = new BD();break;
			case 3:PotionList[i] = new PH();break;
			case 4:PotionList[i] = new WA();break;
			case 5:PotionList[i] = new WD();break;
		}
		while(true){
			PotionList[i]->position_x = rand() % 23 + 2;
			PotionList[i]->position_y = rand() % 78 + 2;
			if(Map[PotionList[i]->position_x][PotionList[i]->position_y] == '.'){
				Map[PotionList[i]->position_x][PotionList[i]->position_y] = 'P';
				break;
			}
		}
	} 
	while(true){
		Hero->position_x = rand() % 23 + 2;
		Hero->position_y = rand() % 78 + 2;
		if(Map[Hero->position_x][Hero->position_y] == '.'){
			Map[Hero->position_x][Hero->position_y] = '@';
			break;	
		}
	}
	for(int i = 0;i < 25;++i){
		for(int j = 0;j < 79;++j){
			CpyMap[i][j] = Map[i][j];
		}
	}
	getRoom(Hero->position_x,Hero->position_y);
	while(true){
		door->potion_x = rand() % 23 + 2;
		door->potion_y = rand() % 78 + 2;
		if(CpyMap[door->potion_x][door->potion_y] == '.'){
			Map[door->potion_x][door->potion_y] = '\\';
			break;
		}
	}
	attEnem = EnemList[0];
}

void JudgeNextFloor(){  // �ж��Ƿ񵽴���һ�� 
	if(Hero->position_x == door->potion_x && Hero->position_y == door->potion_y){
		if(numFloor == 4){
			string ch;
			cout << "��ϲ�� ͨ����!!" << endl;
			cout << "�Ƿ����¿�ʼ��Ϸ?[Y/N]";
			cin >> ch;
			if(ch == "Y"){
				delete []EnemList;
				delete []GoldList;
				delete []MerGold;
				delete []PotionList;
				delete door;
				delete Hero;
				isrestart = 0;
				return;
			}else{
				delete []EnemList;
				delete []GoldList;
				delete []MerGold;
				delete []PotionList;
				delete door;
				delete Hero;
				exit(1);
			}
		}
		Hero->Atk -= Hero->EffAtk;
		Hero->Defense -= Hero->EffDef;
		Hero->EffAtk = 0;    // PotionЧ������ 
		Hero->EffDef = 0;
		changechar[0] = {'.'};
		changechar[1] = {'.'};
		char c;
		int i = 0,j = 0;
		ifstream openFile(mapList[++numFloor]);
		openFile >> noskipws;
		assert(openFile.is_open());
		while(!openFile.eof() && openFile.peek() != EOF){           // ��ʼ����ͼ 
		openFile >> c;
		Map[i][j] = c;
		++j;
		if(j == 80){
			++i;
			j = 0;
			}
		}
		delete []*EnemList;
		delete []*GoldList;
		delete []*MerGold;
		delete []*PotionList;
		RandEnemies();
		Action += "PC reaches floor " + to_string(numFloor);
		openFile.close();
	}
}

void EnemAttHero(){     // �����ж���������λ�ò�ִ�й��� 
	map<string,DireMap>::iterator p;
	int x,y,i;
	for(p = DireList.begin();p != DireList.end();p++){    // ��������� ʹ��p->second.xchange��������仯�� ����Χ�˸�λ�ý��з��� 
		x = Hero->position_x + p->second.xchange;    // ��Χλ�� 
		y = Hero->position_y + p->second.ychange;
		for(int i = 0;i < ArrEnemNum;++i){     // �鿴λ���Ƿ����ڻṥ��PC�Ĺ���
			if(Map[x][y] == ArrEnem[i]){
				for(int j = 0;j < 20;j++){
					if(EnemList[j]->position_x == x && EnemList[j]->position_y == y){
						EnemList[j]->ATTACK(Hero);
						break;
					}
				}
			}
		}
		if(Map[x][y] == 'G'){    // �������ڻ��ᱻ���� 
			for(i = 0;i < 10;i++){
				if(GoldList[i]->position_x == x && GoldList[i]->position_y == y && GoldList[i]->name == 'D'){
					GoldList[i]->Master->ATTACK(Hero);     // ���ڻ�����������PC
					break; 
				}
			}		
		}
	}
}

void ArrPotion(){
	map<string,DireMap>::iterator p;
	int x,y;
	for(p = DireList.begin();p != DireList.end();p++){    // ��������� ʹ��p->second.xchange��������仯�� ����Χ�˸�λ�ý��з��� 
		x = Hero->position_x + p->second.xchange;    // ��Χλ�� 
		y = Hero->position_y + p->second.ychange;
		if(Map[x][y] == 'P')
			Action += "Hero see a unknow Potion.";
		if(Map[x][y] == 'G')
			Action += "Hero see a Gold.";
	}
}

void show(){
	cout << endl << endl << endl;
	cout << "\t\t\t\t";
	cout << "       ====       ====  ===     =   = " << endl;
	Sleep(500);
	cout << "\t\t\t\t";
	cout << "     ==         ==          ==  =  =  " << endl;
	Sleep(500);
	cout << "\t\t\t\t";
	cout << "   ==         ==            ==  = =   " << endl;
	Sleep(500);
	cout << "\t\t\t\t";
	cout << " ==         ==          ====    = =   " << endl;
	Sleep(500);
	cout << "\t\t\t\t";
	cout << "   ==         ==            ==  =  =  " << endl;
	Sleep(500);
	cout << "\t\t\t\t";
	cout << "     ==         ==          ==  =   = " << endl;
	Sleep(500);
	cout << "\t\t\t\t";
	cout << "       ====       ====  ===     =    =" << endl;
	cout << endl << endl << "\t\t\t\t\t  ���س�������......";
	getchar();
}

void getRoom(int x,int y){
	map<string,DireMap>::iterator p = DireList.begin();
	for(p;p != DireList.end();++p){
		if(CpyMap[x + p->second.xchange][y + p->second.ychange] == '.'){
			CpyMap[x + p->second.xchange][y + p->second.ychange] = '@'; 
			getRoom(x + p->second.xchange,y + p->second.ychange);
		}
	}
}

int main(){
	show();
	system("cls");
	door = new Door();
	while(true){
		InitGame();
		RandEnemies();
		getMap();
		ShowData();
		while(isrestart){	
			Choice();
			system("cls");
			JudgeNextFloor();
			EnemChange();
			EnemAttHero();
			ArrPotion();
			if(isrestart){
				getMap();
				ShowData();
			}
		}
	}
	delete []EnemList;
	delete []MerGold;
	delete []GoldList;
	delete []PotionList;
	delete door;
	delete Hero;
	return 0;
}