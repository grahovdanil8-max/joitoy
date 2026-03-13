
#include <iostream>
#include <stack>
#include <iostream>

using namespace std;
int main()
{
	setlocale(LC_ALL, "rus");
  
	stack <int> steck;
	int i = 0;

	cout << "Ввидите 6 чисел ";
	while (i != 6) {
		int a;
		cin >> a;

		steck.push(a);
		i++;
	}

	if (steck.empty()) cout << " Стек не пуст ";

	cout << " Вверхний элемент стека: " << steck.top() << endl;
	cout << " Удаление верхнего элемента : " << endl;

	steck.pop(); // удаление верхнего элемента стека

	cout << " новый верхний элемент " << steck.top();

	system("pause");
	return 0;

}

