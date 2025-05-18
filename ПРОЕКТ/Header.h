#include <iostream> // ����������� ���������� ��� ������ � ��������
#include <windows.h> // ����������� ���������� ��� ������ � WinAPI
#include <conio.h> // ����������� ���������� ��� ������ � �����������
using namespace std; 

enum Color : short { // ����� ������
	BLACK, DARKBLUE, DARKGREEN, TURQUOISE, DARKRED,
	PURPLE, DARKYELLOW, GREY, DARKGREY, BLUE, GREEN,
	CYAN, RED, PINK, YELLOW, WHITE
};

enum Key : short { // ���� ������
	LEFT = 75, RIGHT = 77, UP = 72, DOWN = 80,
	ENTER = 13, SPACE = 32, ESCAPE = 27, BACKSPACE = 8
};

void APPLE_SETUP(const int width, const int height, int* X_apple, int* Y_apple, int* array_X, int* array_Y, int* length) { // ��������� ��������� "������"
	do // ���� ������ ���������� ������ �������� ������� - �� ����� ��� �� ��������� �� "�������"
	{
		*X_apple = rand() % (width - 2) + 1; // ��������� ��������� "������" �� ��� �������
		*Y_apple = rand() % (height - 2) + 1; // ��������� ��������� "������" �� ��� �������
	} while (*X_apple != array_X[*length - 1] && *Y_apple != array_Y[*length - 1]); // �������� �� ���������� � "�������"
}

void DRAWING_FRAME(const int* height, const int* width, HANDLE h) { // ��������� ����� ����
	SetConsoleTextAttribute(h, 4); // ��������� �����, ������� �������� ����� ����
	for (int y = 0; y < *height; y++) // ����������� ������� ���� �� ��������� �����
	{
		for (int x = 0; x < *width; x++)
		{
			char s;
			if (x == 0 && y == 0) // � ������� ����� ���� ���� - ������ ���������������� ����
				s = '#'; // 218
			else if (x == 0 && y == *height - 1) // ������ ����� ����
				s = '#'; // 192
			else if (y == 0 && x == *width - 1) // ������� ������ ����
				s = '#'; // 191
			else if (y == *height - 1 && x == *width - 1) // ������ ������ ����
				s = '#'; // 217
			else if (y == 0 || y == *height - 1) // ������� � ������ ������� ����
				s = '#'; // 196
			else if (x == 0 || x == *width - 1) // ����� � ������ ������� ����
				s = '#'; // 179
			else s = ' '; // �� ���� ��������� ������� ������ ���� ������ ������ (�������� ������ ������� ����)
			cout << s; // ������� ������
		}
		cout << "\n";
	}
}

void INFO_PANEL(COORD& info_panel, HANDLE h, int max_length, int length, const int width) { // ��������� �������������� ������
	SetConsoleOutputCP(1251);
	info_panel.X = width + 5;
	info_panel.Y = 0;
	SetConsoleCursorPosition(h, info_panel);
	SetConsoleTextAttribute(h, Color::RED);
	cout << "������������ ����� ������: ";
	SetConsoleTextAttribute(h, Color::WHITE);
	cout << max_length;

	info_panel.Y = 2;
	SetConsoleCursorPosition(h, info_panel);
	SetConsoleTextAttribute(h, Color::RED);
	cout << "�������� ����� ������: ";
	SetConsoleTextAttribute(h, Color::WHITE);
	cout << length;
	SetConsoleOutputCP(866);
}

void UPDATE_LENGTH(COORD info_panel, HANDLE h, int length, int width) { // ���������� ����� "������" � �������������� ������
	info_panel.X = width + 5;
	info_panel.Y = 2;
	SetConsoleCursorPosition(h, info_panel); 
	SetConsoleOutputCP(1251);
	SetConsoleTextAttribute(h, Color::RED);
	cout << "�������� ����� ������: ";
	SetConsoleOutputCP(866);
	SetConsoleTextAttribute(h, Color::WHITE);
	cout << length;
	SetConsoleTextAttribute(h, Color::GREEN);
}

void MAIN_CYCLE(const int max_length, char snake, char apple, char head, int sleep_time, int dx, int dy, int* array_X, int* array_Y, int* length, const int width, const int height, HANDLE h, int* X_apple, int* Y_apple, COORD c, COORD info_panel) { // �������� ���� ����
	bool flag = true; // ���������� ��� ���������� ����� �����

	do // ���������� ���� ����
	{
		Sleep(sleep_time); // �������� ������ ��������� �� �������� ����� ��������

		if (_kbhit()) // ���������, ���� �� ������ �����-���� ������� � ��������� � ��������� � ������ ������
		{
			int k = _getch(); // ��������� ��� ������� �� ������
			if (k == 0 || k == 224) // ���� ������ ��� - ���������������, ��������� ������ ���
				k = _getch();
			switch (k) // ���������� ��� ������� ������� ������ ��������� ������
			{
			case Key::DOWN: // ���� ���� ������ ������� ����
				dy = 1; // �� ���������� �� ��� ������� ����� �������������
				dx = 0; // �� ��� ������� ���������� �������
				break;
			case Key::UP: // ���� �����
				dy = -1; // ���������� �������� �������������� ������
				dx = 0;
				break;
			case Key::LEFT: // ���� �����
				dy = 0;
				dx = -1;
				break;
			case Key::RIGHT: // ���� ������
				dy = 0;
				dx = 1;
				break;
			case Key::ESCAPE: // ���� ���� ������ ������� ESC
				flag = false; // ������������� ������ �� ����, ���� ��������� ����� ��������
				break;
			}
		}

		int X = array_X[*length - 1] + dx; // ���������� �������� �������� ������ "������" ����� ��������
		int Y = array_Y[*length - 1] + dy; // �� �� ����� ��� ��������
		if (X == 0 || X == width - 1 || Y == 0 || Y == height - 1) // �������� �� ���������� ������ ����
		{
			flag = false; // ���� ��� - ������ ��������� ����������� ���������� �����
		}
		else if (X == *X_apple && Y == *Y_apple) // �������� �� ���������� "������"
		{
			c.X = array_X[*length - 1]; // ��������� � ������ ��������� ������� ������ "������"
			c.Y = array_Y[*length - 1];
			SetConsoleCursorPosition(h, c); // ��������� ������� � ��� �������
			cout << snake; // ����������� ������� ���� "������"

			(*length)++; // ���������� ����� "������" (������ ����������)

			UPDATE_LENGTH(info_panel, h, *length, width);

			c.X = array_X[*length - 1] = X; // ��������� � ������� ������� ������ ����� "������"
			c.Y = array_Y[*length - 1] = Y;
			SetConsoleCursorPosition(h, c); // ��������� ���� �������
			cout << head; // � ����������� ��� ������� ������ "������"

			if (*length == max_length) // ��������, �������� �� ����� "������" ������ ������������� ��������
			{
				break; // ���� ��� - ������ ��������� ���� 
			}

			int i; // ���������� ��� �������� ���������� ������� "������", �� ����������� � �������� "������"
			do {
				*X_apple = rand() % (width - 2) + 1; // ��������� ����� ��������� "������"
				*Y_apple = rand() % (height - 2) + 1;
				i = 0; // ��������� ����� ������������� ���������
				for (; i < *length; i++) // ������ ����� �� ������ ����������
					if (*X_apple == array_X[i] && *Y_apple == array_Y[i]) // ���� ���������� �������
						break; // �� ��������� ���� for
			} while (i < *length); // ����� ����� ��������� ������������, ���� ����� ������������� ��������� ������ ����� "������"

			c.X = *X_apple; // ��������� � ������ ��������� ����� ���������� ������� "������"
			c.Y = *Y_apple;
			SetConsoleCursorPosition(h, c); // �������� ���� �������
			SetConsoleTextAttribute(h, Color::RED); // ��������� ����� � �������
			cout << apple; // ����������� ������� "������"
			SetConsoleTextAttribute(h, Color::GREEN); // �������� ��������� ����� � ������� - ��� ����������� ����������� "������"
		}
		else // ������, ����� ������ "������" ��������� �� ����� ������ �������
		{
			int i = 1; // ���������� �� ���������� �������, �� ����������� � ����� �������� - ����� ������ "������"
			for (; i < *length; i++)
				if (X == array_X[i] && Y == array_Y[i]) // ���� ���������� ������� � ����� - �����������
					break;
			if (i < *length) // ���� ����� ������������� ������� ������ ����� "������" - �� ��������� �������� ���� ����
				break;
			else // � ����� ��������� ��������� ������ "������"
			{
				c.X = array_X[0]; // ������������� � ������ ��������� ������� ������ "������"
				c.Y = array_Y[0];
				SetConsoleCursorPosition(h, c); // ������� ���� ������
				cout << ' '; // � ���������� ������ (������� ������)

				if (*length > 1) // ���� ����� ������ ������ 
				{
					c.X = array_X[*length - 1]; // ������������� � ������ ��������� ���������� ������� ������ "������"
					c.Y = array_Y[*length - 1];
					SetConsoleCursorPosition(h, c);  // ������� ���� ������
					putchar(snake); // ������� ������ ���� "������"
				}

				for (int i = 0; i < *length - 1; i++) // ��������� ���� ������ ��������� ������� "������"
				{
					array_X[i] = array_X[i + 1]; // ������������ ��� ������ - ����� ����������
					array_Y[i] = array_Y[i + 1];
				}

				c.X = array_X[*length - 1] = X; // ������������� ����� ������� ������ "������"
				c.Y = array_Y[*length - 1] = Y;
				SetConsoleCursorPosition(h, c); // ������� ���� �������
				cout << head; // ���������� ������ ������ "������"
			}
		}
	} while (flag); // ������� �� �����, ���� �������� ����������� ����������
}

void GAME_OVER(HANDLE h) { // ���������� ����
	// ������� ������� � ����� �������
	SetConsoleOutputCP(1251); // ��������� ������� �������� ��� ����������� ����������� ���������
	system("cls"); // ������� ������� 
	COORD END; // �������� ������� ��� �������� ��������� ���������� ����
	END.X = 52;
	END.Y = 15;

	SetConsoleCursorPosition(h, END); // ��������� ������� � �������� ����������
	SetConsoleTextAttribute(h, Color::RED); // ��������� ����� ������
	cout << "���� ��������!\n";
	_getch(); // �������� ������� �������
}