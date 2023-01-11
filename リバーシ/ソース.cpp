//[1]�w�b�_�[���C���N���[�h����
#include<conio.h>
#include<stdlib.h>
#include<stdio.h>
#include<vector>//�x�N�^�[�w�b�_�[���C���N���[�h����
#include<time.h>//���ԊǗ��w�b�_�[���C���N���[�h����


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

//�Q�[�����[�h�̎�ނ��`����
enum
{
	MODE_1P,	//AI�Ƃ̑ΐ탂�[�h
	MODE_2P,	//�l�ԓ��m�̑ΐ탂�[�h
	MODE_MAX	//���[�h�̐�
};

//[4]�ϐ���錾����
//�^�[���̐F���`����
char colorNames[][5 + 1] = {
	"Black",//COLOR_BLACK
	"White"//COLOR_WHITE
};

//���[�h�̖��O��錾����
const char* modeNames[] = {
	"�P�o �f�`�l�d",//MODE_1P
	"�Q�o �f�`�l�d",//MODE_2P
};

//���݂̃Q�[�����[�h��錾����
int mode;

//�Z�����`����
int cells[BOARD_HEIGHT][BOARD_WIDTH];

//�J�[�\�����`����
int cursorX, cursorY;

//AI�̍��W���`����
int AIx, AIy;

//�^�[�����`����
int turn;

//�x�N�g���\���̂�錾����
typedef struct 
{
	int x, y;
}VEC2;

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

//�e�^�[�����v���C���[�̓����錾����
bool isPlayer[COLOR_MAX];

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
			if((x<0) || (x> BOARD_WIDTH) || (y<0) || (y> BOARD_HEIGHT))
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
				break;
			}
		}

	}
	return false;
}

//�ՖʑS�̂Œu����ꏊ�����邩�`�F�b�N����
bool checkCanPutAll(int _color) {
	for (int y = 0; y < BOARD_HEIGHT; y++)
		for (int x = 0; x < BOARD_WIDTH; x++)
			//�����u����ꏊ������Βu����Ƃ���
			if (checkCanPut(_color, x, y, false))
				return true;	
	//�u����ꏊ���Ȃ����false�̂܂�
	return false;
}

//�`��֐�
void drawBoard() {
	//�`�揈��
	system("cls");
	//�Ֆʂ��`����
	for (int y = 0; y < BOARD_HEIGHT; y++) {
		for (int x = 0; x < BOARD_WIDTH; x++)
			//�v���C���[�̒S�����ǂ������肷��
			
				//�J�[�\���̐ݒ�
			if ((x == cursorX) && (y == cursorY)) {
				if (isPlayer[turn])
					printf("��");
			}
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
}

//���[�h�I����ʂ��쐬����
void SelectMode() {
	//�Q�[�����[�h������������
	mode = MODE_1P;
	//���[�v
	while (1)
	{
		//��ʂ��N���A����
		system("cls");

		//���b�Z�[�W��\������
		printf("���[�h���@�I������\n��������\n");

		printf("\n\n");//2�s�󂯂�

		//���ׂẴ��[�h�𔽕�����
		for (int i = 0; i < MODE_MAX; i++)
		{
			//���݂̃��[�h�ɂ̓J�[�\�����A����ȊO�ɂ̓X�y�[�X��`�悷��
			printf("%s ", (i == mode) ? "��" : "�@");

			printf("%s\n", modeNames[i]);//���[�h�̖��O��`�悷��

			printf("\n");//1�s�󂯂�
		}

		switch (_getch())
		{
		case 'w':	//w�L�[�������ꂽ��
			mode--; //�O�̃��[�h�ɐ؂�ւ���
			break;
		case 's':	//s�L�[�������ꂽ��
			mode++; //���̃��[�h�ɐ؂�ւ���
			break;
		default://���̑��̃L�[�������ꂽ��

			//�I�����ꂽ���[�h�ŕ��򂷂�
			switch (mode)
			{
			case MODE_1P://AI�Ƒΐ킷�郂�[�h

				isPlayer[COLOR_BLACK] = true; //�����v���C���[�ɂ���
				isPlayer[COLOR_WHITE] = false;//�����v���C���[�ɂ��Ȃ�

				break;
			case MODE_2P://�l�ԓ��m�̑ΐ탂�[�h

				//���҂��v���C���[�S���ɂ���
				isPlayer[COLOR_BLACK] = isPlayer[COLOR_WHITE] = true; 

				break;
			}

			return;//���[�h�I���𔲂���
		}
	}
}

//���C��
int main() {
	start://�J�n���x��
		;//��

	SelectMode();//���[�h�I������֐����Ăяo��

	//�Ֆʂ����Z�b�g����
	for (int y = 0; y < BOARD_HEIGHT; y++)
		for (int x = 0; x < BOARD_WIDTH; x++)
			cells[y][x] = COLOR_NONE;

	//�����ʒu��ݒ肷��
	cells[3][3] = cells[4][4] = COLOR_WHITE;
	cells[3][4] = cells[4][3] = COLOR_BLACK;

	//�����U�ɂ���
	turn = 0;

	//�΂��u���Ȃ��Ƃ����t���O
	bool cantPut = false;//�ʏ��false�ɂ��Ă���

	//���[�v
	while (1)
	{
		//����������������
		srand((unsigned int)time(NULL));
		//�`�悷��
		drawBoard();
		
		//�΂��u���Ȃ��t���O���������Ƃ�
		if (cantPut)
			printf("Can't put!\n");
		else
			//���݂̃^�[����\������
			printf("%s turn \n", colorNames[turn]);
		
		//cantPut�t���O���I������
		cantPut = false;

		//���݂̃^�[���̒S�����v���C���[���ǂ������肷��
		if (isPlayer[turn])
		{
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

				//�΂��u����ꍇ
				checkCanPut(turn, cursorX, cursorY, true);

				//��L�ȊO�̃L�[�������ƌ��݂̃^�[���̐΂�z�u����
				cells[cursorY][cursorX] = turn;
				//�^�[����؂�ւ���
				turn ^= 1;

				//�����̃^�[���Œu���Ȃ������ꍇ(�p�X�̏���)
				if (!checkCanPutAll(turn)) {
					//�^�[����؂�ւ���
					turn ^= 1;
				}
				break;
			}
		}
		//���݂̃^�[���̒S�����v���C���[�łȂ��Ȃ�
		else
		{
			//AI�̍��W���u���郂�m�ɂȂ�܂Ń��[�v����
			while (!checkCanPut(turn, AIx, AIy, false))
			{
				//AI�̍��W�������_���Ŏ擾����
				int AIx = rand() % BOARD_WIDTH;
				int AIy = rand() % BOARD_HEIGHT;
				//AI�̍��W���u���郂�m�ɂȂ�����
				if (checkCanPut(turn, AIx, AIy, false))
				{
					//�΂��u������
					checkCanPut(turn, AIx, AIy, true);
					break;
				}
				//�����̃^�[���Œu���Ȃ��ꍇ(�p�X�̏���)
				if (!checkCanPutAll(turn)) {
					//�^�[����؂�ւ���
					turn ^= 1;
					break;
				}
			};

			//�^�[����؂�ւ���
			turn ^= 1;
					
		}

		//�͈͊O�ɃJ�[�\�����s���Ȃ��悤�ɂ���
		if (cursorX < 0)
			cursorX++;
		else if (cursorX >= BOARD_WIDTH)
			cursorX--;
		else if (cursorY < 0)
			cursorY++;
		else if (cursorY >= BOARD_HEIGHT)
			cursorY--;


		//���������u���Ȃ��Ȃ����ꍇ�Q�[�����I������
		if ((!checkCanPutAll(COLOR_BLACK)) && (!checkCanPutAll(COLOR_WHITE))) {
			//�ĕ`�悷��
			drawBoard();
			
			//�F�̌��̏W�v������֐�
			int count[COLOR_MAX] = {};
			for (int y = 0; y < BOARD_HEIGHT; y++)
				for (int x = 0; x < BOARD_WIDTH; x++)
					//�΂��u����ĂȂ����count��ǉ�����
					if (cells[y][x] != COLOR_NONE)
						count[cells[y][x]]++;

			//�΂̏W�v���I����Ă���ĕ`�悷��
			drawBoard();
			for (int i = 0; i < COLOR_MAX; i++)
				//�������ꂼ��̐���`�悷��
				printf("%s:%d\n", colorNames[i],count[i]);

			//���������̏ꍇ
			if (count[COLOR_BLACK] == count[COLOR_WHITE])
				printf("Draw!\n");
			//�ǂ��炩�̕������������ꍇ
			else {
				if(count[COLOR_BLACK] > count[COLOR_WHITE])
					printf("%s\n", colorNames[COLOR_BLACK]);
				else
					printf("%s\n", colorNames[COLOR_WHITE]);

				printf("Won!\n");
			}

			_getch();
			goto start;//�J�n���x���ɃW�����v����
		}
	}
}