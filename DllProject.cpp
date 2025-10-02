// lab4evm.cpp : Этот файл содержит функцию "main". Здесь начинается и заканчивается выполнение программы.
//

#include <iostream>
#include <Windows.h>
#include <locale.h>
using namespace std;

typedef void (*SSEFUNCType)(int, float*, float*, float*, float*);

void SSEFUNC_CPP(int N, float* x, float* b, float* e, float* res)
{
	for (int i = 0; i < N; i++)
	{
		res[i] = sqrt(pow(x[i], 3)) + b[i] / e[i];
	}
}

float MaxError(float* res1, float* res2, int N)
{
	float max = -1;
	for (int i = 0; i < N; i++)
	{
		if (fabs(res1[i] - res2[i]) > max) max = fabs(res1[i] - res2[i]);
	}
	return max;
}

float MinError(float* res1, float* res2, int N)
{
	float min = 1000;
	for (int i = 0; i < N; i++)
	{
		if (fabs(res1[i] - res2[i]) < min) min = fabs(res1[i] - res2[i]);
	}
	return min;
}

float AverError(float* res1, float* res2, int N)
{
	float s = 0;
	for (int i = 0; i < N; i++)
	{
		s += fabs(res1[i] - res2[i]);
	}
	return s / N;
}

//extern "C" __declspec(dllimport) void(*Func)(float* res2, int N, float* x, float* b, float* e);

int main()
{
	//setlocale(LC_ALL, "Russian");
	HMODULE hLib;
	//SetCurrentDirectory((LPCWSTR)L"C://Users//тр//Documents//Универ//4 семестр//ЭВМ 4//лаб 4");
	hLib = LoadLibrary(L"C:\\Users\\muhab\\Documents\\RadASM\\Masm\\Projects\\dll_library\\dll_library.dll");
	if (hLib == NULL)
	{
		DWORD error = GetLastError();
		cout << error << endl;
		cout << "Error";
		return 1;
	}
	SSEFUNCType SSEFUNC = reinterpret_cast<SSEFUNCType>(GetProcAddress(hLib, "SSEFUNC"));
	if (SSEFUNC == NULL)
	{
		DWORD error = GetLastError();
		cout << error << endl;
		cout << "Error loading function\n";
		FreeLibrary(hLib);
		return 1;
	}
	float MaxEr, MinEr, AverEr;
	long N;
	cout << "Enter array's length N: " << endl;
	cin >> N;
	float *x, *b, *e, *res, *res2, *aligned_x, *aligned_b, *aligned_e, *aligned_res, *aligned_res2;
	x = new float[N + 16];
	aligned_x = (float*)(((int)x / 16 + 1) * 16);
	b = new float[N + 16];
	aligned_b = (float*)(((int)b / 16 + 1) * 16);
	e = new float[N + 16];
	aligned_e = (float*)(((int)e / 16 + 1) * 16);
	res = new float[N + 16];
	aligned_res = (float*)(((int)res / 16 + 1) * 16);
	res2 = new float[N + 16];
	aligned_res2 = (float*)(((int)res2 / 16 + 1) * 16);
	for (int i = 0; i < N; i++)
	{
		aligned_x[i] = 20 + 5.2f * i;
		aligned_b[i] = 40 + 8.4f * i;
		aligned_e[i] = 60 + 9.8f * i;
	}
	SSEFUNC_CPP(N, aligned_x, aligned_b, aligned_e, aligned_res);
	SSEFUNC(N, aligned_res2, aligned_x, aligned_b, aligned_e);
	MaxEr = MaxError(aligned_res, aligned_res2, N);
	AverEr = AverError(aligned_res, aligned_res2, N);
	MinEr = MinError(aligned_res, aligned_res2, N);
	cout.precision(20);
	cout << "Maximum error: " << MaxEr << endl;
	cout << "Average error: " << AverEr << endl;
	cout << "Minimum error: " << MinEr << endl;
	for (int i = 0; i < N; i++)
		cout << i << ")" << aligned_res[i] << "   " << aligned_res2[i] << endl;
	FreeLibrary(hLib);
	delete[] x;
	delete[] b;
	delete[] e;
	delete[] res;
	delete[] res2;
	return 0;
}

// Запуск программы: CTRL+F5 или меню "Отладка" > "Запуск без отладки"
// Отладка программы: F5 или меню "Отладка" > "Запустить отладку"

// Советы по началу работы 
//   1. В окне обозревателя решений можно добавлять файлы и управлять ими.
//   2. В окне Team Explorer можно подключиться к системе управления версиями.
//   3. В окне "Выходные данные" можно просматривать выходные данные сборки и другие сообщения.
//   4. В окне "Список ошибок" можно просматривать ошибки.
//   5. Последовательно выберите пункты меню "Проект" > "Добавить новый элемент", чтобы создать файлы кода, или "Проект" > "Добавить существующий элемент", чтобы добавить в проект существующие файлы кода.
//   6. Чтобы снова открыть этот проект позже, выберите пункты меню "Файл" > "Открыть" > "Проект" и выберите SLN-файл.
