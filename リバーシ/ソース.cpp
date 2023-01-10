//[1]�w�b�_�[���C���N���[�h����
#include<conio.h>
#include<stdlib.h>
#include<stdio.h>

//[2]�萔���`����
#define BOARD_WIDTH		8
#define BOARD_HEIGHT	8

//[3]�񐔂��`����
//�΂̒�`
enum
{
	COLOR_NONE=-1,
	COLOR_BLACK=0,
	COLOR_WHITE=1,
	COLOR_MAX
};

//�`�F�b�N����������`����
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

//[4]�ϐ����`����
//�^�[���̐F���`����
char colorNames[][5 + 1] = {
	"Black",//COLOR_BLACK
	"White"//COLOR_WHITE
};

//�Z�����`����
int cells[BOARD_HEIGHT][BOARD_WIDTH];

//�J�[�\�����`����
int cursorX, cursorY;

//�^�[�����`����
int turn;

//�`�F�b�N��������̃x�N�g�����`����
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

//�΂��u���邩���肷��(+turnOver�t���O���s��)
bool checkCanPut(int _color, int _x, int _y,bool _turnOver) {
	//COLOR_NONE�łȂ��Ƃ���͐΂��u���Ȃ��悤�ɂ���
	if (cells[_y][_x] != COLOR_NONE)
		return false;

	//�S�����̃`�F�b�N���s��
	for (int i = 0; i < DIRECTION_MAX; i++) {
		//���[�J���ϐ���x,y���`
		int x = _x, y = _y;

		x += directions[i][0];
		y += directions[i][1];
		//�ׂ̃}�X������̐F�łȂ����
		if (cells[y][x] != (_color ^ 1))
			continue;//���̕����̃`�F�b�N�͏I��

		//���if�Ɉ���������Ȃ������ꍇ(�ׂ̃}�X������̐F�ł���)
		//�͈͊O�܂Ń`�F�b�N���s��
		while (1){
			x += directions[i][0];
			y += directions[i][1];

			//�͈͊O�ɂȂ��������[�v�𔲂���
			if((x<=0) || (x>= BOARD_WIDTH) || (y<0) || (y>= BOARD_HEIGHT))
				break;
			
			//�΂��Ȃ����������[�v�𔲂���
			if (cells[y][x] == COLOR_NONE)
				break;
			 
			//�����̐F�������ꍇ(����ł���ꍇ)
			if (cells[y][x] == _color){
				//�Ђ�����Ԃ��t���O�������Ă��Ȃ��ꍇ
				if(!_turnOver)
					return true;

				//�V���ȃ��[�J���ϐ���x,y���`
				int x2 = _x, y2 = _y;
				while (1) {
					//�F�����݂̐F�ɓh��ւ���
					cells[y2][x2] = _color;

					x2 += directions[i][0];
					y2 += directions[i][1];

					//�͂܂�Ă��镔���̃`�F�b�N���I������ꍇ
					if ((x2 == x) && (y2 == y))
						break;
				}
			}
		}
	}
	return false;
}

//���C��
int main() {
	//�Ֆʂ����Z�b�g����
	for (int y = 0; y < BOARD_HEIGHT; y++)
		for (int x = 0; x < BOARD_WIDTH; x++)
			cells[y][x] = COLOR_NONE;

	//�����ʒu��ݒ肷��
	cells[3][3] = cells[4][4] = COLOR_WHITE;
	cells[3][4] = cells[4][3] = COLOR_BLACK;

	//�΂��u���Ȃ��Ƃ����t���O
	bool cantPut = false;//�ʏ��false�ɂ��Ă���

	//���[�v
	while (1)
	{
		//�`�揈��
		system("cls");
		//�Ֆʂ��`����
		for (int y = 0; y < BOARD_HEIGHT; y++){
			for (int x = 0; x < BOARD_WIDTH; x++)
				//�J�[�\���̐ݒ�
				if ((x == cursorX) && (y == cursorY))
					printf("��");
				else {
					switch (cells[y][x])
					{
					case COLOR_NONE:	printf("�E");	break;//�Z���������ꍇ
					case COLOR_BLACK:	printf("��");	break;//�Z������
					case COLOR_WHITE:	printf("��");	break;//�Z������
					}
				}
					
			printf("\n");//���s
		}
		//�΂��u���Ȃ��t���O���������Ƃ�
		if (cantPut)
			printf("Can't put!\n");
		else
			//���݂̃^�[����\������
			printf("%s turn \n", colorNames[turn]);
		
		//cantPut�t���O���I������
		cantPut = false;

		//�J�[�\���̑���
		switch (_getch())
		{
		case 'w':	cursorY--; break;
		case 's':	cursorY++; break;
		case 'a':	cursorX--; break;
		case 'd':	cursorX++; break;
		default:
			//�΂��u���Ȃ��ꏊ�̏ꍇ
			if (!checkCanPut(turn, cursorX, cursorY, false)) {
				cantPut = true;//cantPut��true�ɂ���
				break;
			}

			//
			checkCanPut(turn, cursorX, cursorY, true);

			//��L�ȊO�̃L�[�������ƌ��݂̃^�[���̐΂�z�u����
			cells[cursorY][cursorX] = turn;
			//�^�[����؂�ւ���
			turn ^= 1;
			break;
		}
	}
}