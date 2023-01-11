//[1]ヘッダーをインクルードする
#include<conio.h>
#include<stdlib.h>
#include<stdio.h>
#include<vector>//ベクターヘッダーをインクルードする
#include<time.h>//時間管理ヘッダーをインクルードする


//[2]定数を定義する
#define BOARD_WIDTH		8
#define BOARD_HEIGHT	8

//[3]列数を定義する
//石の定義
enum
{
	COLOR_NONE=-1,
	COLOR_BLACK=0,
	COLOR_WHITE=1,
	COLOR_MAX
};

//チェックする方向を定義する
enum {
	DIRECTION_UP,
	DIRECTION_UP_LEFT,
	DIRECTION_LEFT,
	DIRECTION_DOWN_LEFT,
	DIRECTION_DOWN,
	DIRECTION_DOWN_RIGHT,
	DIRECTION_RIGHT,
	DIRECTION_UP_RIGHT,
	DIRECTION_MAX
};

//ゲームモードの種類を定義する
enum
{
	MODE_1P,	//AIとの対戦モード
	MODE_2P,	//人間同士の対戦モード
	MODE_MAX	//モードの数
};

//[4]変数を宣言する
//ターンの色を定義する
char colorNames[][5 + 1] = {
	"Black",//COLOR_BLACK
	"White"//COLOR_WHITE
};

//モードの名前を宣言する
const char* modeNames[] = {
	"１Ｐ ＧＡＭＥ",//MODE_1P
	"２Ｐ ＧＡＭＥ",//MODE_2P
};

//現在のゲームモードを宣言する
int mode;

//セルを定義する
int cells[BOARD_HEIGHT][BOARD_WIDTH];

//カーソルを定義する
int cursorX, cursorY;

//AIの座標を定義する
int AIx, AIy;

//ターンを定義する
int turn;

//ベクトル構造体を宣言する
typedef struct 
{
	int x, y;
}VEC2;

//チェックする方向のベクトルを定義する
int directions[][2] = {
	{0,-1},// DIRECTION_UP
	{-1,-1},// DIRECTION_UP_LEFT
	{-1,0},// DIRECTION_LEFT
	{-1,1},// DIRECTION_DOWN_LEFT
	{0,1},// DIRECTION_DOWN
	{1,1},// DIRECTION_DOWN_RIGHT
	{1.0},// DIRECTION_RIGHT
	{1,-1},// DIRECTION_UP_RIGHT
};

//各ターンがプレイヤー歌道か宣言する
bool isPlayer[COLOR_MAX];

//石が置けるか判定する(+turnOverフラグを行う)
bool checkCanPut(int _color, int _x, int _y,bool _turnOver) {
	//COLOR_NONEでないところは石が置けないようにする
	if (cells[_y][_x] != COLOR_NONE)
		return false;

	//全方向のチェックを行う
	for (int i = 0; i < DIRECTION_MAX; i++) {
		//ローカル変数のx,yを定義
		int x = _x, y = _y;

		x += directions[i][0];
		y += directions[i][1];
		//隣のマスが相手の色でなければ
		if (cells[y][x] != (_color ^ 1))
			continue;//その方向のチェックは終了

		//上のifに引っかからなかった場合(隣のマスが相手の色である)
		//範囲外までチェックを行う
		while (1){
			x += directions[i][0];
			y += directions[i][1];

			//範囲外になった時ループを抜ける
			if((x<0) || (x> BOARD_WIDTH) || (y<0) || (y> BOARD_HEIGHT))
				break;
			
			//石がなかった時ループを抜ける
			if (cells[y][x] == COLOR_NONE)
				break;
			 
			//自分の色だった場合(挟んでいる場合)
			if (cells[y][x] == _color){
				//ひっくり返しフラグが建っていない場合
				if(!_turnOver)
					return true;

				//新たなローカル変数のx,yを定義
				int x2 = _x, y2 = _y;
				while (1) {
					//色を現在の色に塗り替える
					cells[y2][x2] = _color;

					x2 += directions[i][0];
					y2 += directions[i][1];

					//囲まれている部分のチェックが終わった場合
					if ((x2 == x) && (y2 == y))
						break;
				}
				break;
			}
		}

	}
	return false;
}

//盤面全体で置ける場所があるかチェックする
bool checkCanPutAll(int _color) {
	for (int y = 0; y < BOARD_HEIGHT; y++)
		for (int x = 0; x < BOARD_WIDTH; x++)
			//もし置ける場所があれば置けるとする
			if (checkCanPut(_color, x, y, false))
				return true;	
	//置ける場所がなければfalseのまま
	return false;
}

//描画関数
void drawBoard() {
	//描画処理
	system("cls");
	//盤面を定義する
	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++)
			//プレイヤーの担当かどうか判定する
			
				//カーソルの設定
			if ((x == cursorX) && (y == cursorY)) {
				if (isPlayer[turn])
					printf("◎");
			}
			else {
				switch (cells[y][x])
				{
				case COLOR_NONE:	printf("・");	break;//セルが無い場合
				case COLOR_BLACK:	printf("○");	break;//セルが黒
				case COLOR_WHITE:	printf("●");	break;//セルが白
				}
			}


		printf("\n");//改行
	}
}

//モード選択画面を作成する
void SelectMode() {
	//ゲームモードを初期化する
	mode = MODE_1P;
	//ループ
	while (1)
	{
		//画面をクリアする
		system("cls");

		//メッセージを表示する
		printf("モードを　選択して\nください\n");

		printf("\n\n");//2行空ける

		//すべてのモードを反復する
		for (int i = 0; i < MODE_MAX; i++)
		{
			//現在のモードにはカーソルを、それ以外にはスペースを描画する
			printf("%s ", (i == mode) ? "＞" : "　");

			printf("%s\n", modeNames[i]);//モードの名前を描画する

			printf("\n");//1行空ける
		}

		switch (_getch())
		{
		case 'w':	//wキーが押されたら
			mode--; //前のモードに切り替える
			break;
		case 's':	//sキーが押されたら
			mode++; //次のモードに切り替える
			break;
		default://その他のキーが押されたら

			//選択されたモードで分岐する
			switch (mode)
			{
			case MODE_1P://AIと対戦するモード

				isPlayer[COLOR_BLACK] = true; //黒をプレイヤーにする
				isPlayer[COLOR_WHITE] = false;//白をプレイヤーにしない

				break;
			case MODE_2P://人間同士の対戦モード

				//両者をプレイヤー担当にする
				isPlayer[COLOR_BLACK] = isPlayer[COLOR_WHITE] = true; 

				break;
			}

			return;//モード選択を抜ける
		}
	}
}

//メイン
int main() {
	start://開始ラベル
		;//空文

	SelectMode();//モード選択する関数を呼び出す

	//盤面をリセットする
	for (int y = 0; y < BOARD_HEIGHT; y++)
		for (int x = 0; x < BOARD_WIDTH; x++)
			cells[y][x] = COLOR_NONE;

	//初期位置を設定する
	cells[3][3] = cells[4][4] = COLOR_WHITE;
	cells[3][4] = cells[4][3] = COLOR_BLACK;

	//黒を先攻にする
	turn = 0;

	//石が置けないというフラグ
	bool cantPut = false;//通常はfalseにしておく

	//ループ
	while (1)
	{
		//乱数を初期化する
		srand((unsigned int)time(NULL));
		//描画する
		drawBoard();
		
		//石が置けないフラグが建ったとき
		if (cantPut)
			printf("Can't put!\n");
		else
			//現在のターンを表示する
			printf("%s turn \n", colorNames[turn]);
		
		//cantPutフラグを終了する
		cantPut = false;

		//現在のターンの担当がプレイヤー化どうか判定する
		if (isPlayer[turn])
		{
			//カーソルの操作
			switch (_getch())
			{
			case 'w':	cursorY--; break;
			case 's':	cursorY++; break;
			case 'a':	cursorX--; break;
			case 'd':	cursorX++; break;
			default:
				//石が置けない場所の場合
				if (!checkCanPut(turn, cursorX, cursorY, false)) {
					cantPut = true;//cantPutをtrueにする
					break;
				}

				//石が置ける場合
				checkCanPut(turn, cursorX, cursorY, true);

				//上記以外のキーを押すと現在のターンの石を配置する
				cells[cursorY][cursorX] = turn;
				//ターンを切り替える
				turn ^= 1;

				//自分のターンで置けなかった場合(パスの処理)
				if (!checkCanPutAll(turn)) {
					//ターンを切り替える
					turn ^= 1;
				}
				break;
			}
		}
		//現在のターンの担当がプレイヤーでないなら
		else
		{
			//AIの座標が置けるモノになるまでループする
			while (!checkCanPut(turn, AIx, AIy, false))
			{
				//AIの座標をランダムで取得する
				int AIx = rand() % BOARD_WIDTH;
				int AIy = rand() % BOARD_HEIGHT;
				//AIの座標が置けるモノになったら
				if (checkCanPut(turn, AIx, AIy, false))
				{
					//石が置ける状態
					checkCanPut(turn, AIx, AIy, true);
					break;
				}
				//自分のターンで置けない場合(パスの処理)
				if (!checkCanPutAll(turn)) {
					//ターンを切り替える
					turn ^= 1;
					break;
				}
			};

			//ターンを切り替える
			turn ^= 1;
					
		}

		//範囲外にカーソルが行かないようにする
		if (cursorX < 0)
			cursorX++;
		else if (cursorX >= BOARD_WIDTH)
			cursorX--;
		else if (cursorY < 0)
			cursorY++;
		else if (cursorY >= BOARD_HEIGHT)
			cursorY--;


		//白も黒も置けなくなった場合ゲームを終了する
		if ((!checkCanPutAll(COLOR_BLACK)) && (!checkCanPutAll(COLOR_WHITE))) {
			//再描画する
			drawBoard();
			
			//色の個数の集計をする関数
			int count[COLOR_MAX] = {};
			for (int y = 0; y < BOARD_HEIGHT; y++)
				for (int x = 0; x < BOARD_WIDTH; x++)
					//石が置かれてなければcountを追加する
					if (cells[y][x] != COLOR_NONE)
						count[cells[y][x]]++;

			//石の集計が終わってから再描画する
			drawBoard();
			for (int i = 0; i < COLOR_MAX; i++)
				//黒白それぞれの数を描画する
				printf("%s:%d\n", colorNames[i],count[i]);

			//引き分けの場合
			if (count[COLOR_BLACK] == count[COLOR_WHITE])
				printf("Draw!\n");
			//どちらかの方が数が多い場合
			else {
				if(count[COLOR_BLACK] > count[COLOR_WHITE])
					printf("%s\n", colorNames[COLOR_BLACK]);
				else
					printf("%s\n", colorNames[COLOR_WHITE]);

				printf("Won!\n");
			}

			_getch();
			goto start;//開始ラベルにジャンプする
		}
	}
}