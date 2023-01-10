//[1]ヘッダーをインクルードする
#include<conio.h>
#include<stdlib.h>
#include<stdio.h>

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

//[4]変数を定義する
//ターンの色を定義する
char colorNames[][5 + 1] = {
	"Black",//COLOR_BLACK
	"White"//COLOR_WHITE
};

//セルを定義する
int cells[BOARD_HEIGHT][BOARD_WIDTH];

//カーソルを定義する
int cursorX, cursorY;

//ターンを定義する
int turn;

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
			if((x<=0) || (x>= BOARD_WIDTH) || (y<0) || (y>= BOARD_HEIGHT))
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
			}
		}
	}
	return false;
}

//メイン
int main() {
	//盤面をリセットする
	for (int y = 0; y < BOARD_HEIGHT; y++)
		for (int x = 0; x < BOARD_WIDTH; x++)
			cells[y][x] = COLOR_NONE;

	//初期位置を設定する
	cells[3][3] = cells[4][4] = COLOR_WHITE;
	cells[3][4] = cells[4][3] = COLOR_BLACK;

	//石が置けないというフラグ
	bool cantPut = false;//通常はfalseにしておく

	//ループ
	while (1)
	{
		//描画処理
		system("cls");
		//盤面を定義する
		for (int y = 0; y < BOARD_HEIGHT; y++){
			for (int x = 0; x < BOARD_WIDTH; x++)
				//カーソルの設定
				if ((x == cursorX) && (y == cursorY))
					printf("◎");
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
		//石が置けないフラグが建ったとき
		if (cantPut)
			printf("Can't put!\n");
		else
			//現在のターンを表示する
			printf("%s turn \n", colorNames[turn]);
		
		//cantPutフラグを終了する
		cantPut = false;

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

			//
			checkCanPut(turn, cursorX, cursorY, true);

			//上記以外のキーを押すと現在のターンの石を配置する
			cells[cursorY][cursorX] = turn;
			//ターンを切り替える
			turn ^= 1;
			break;
		}
	}
}