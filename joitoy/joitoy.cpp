#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>
#include "joitoy.h" // Убедитесь, что здесь описана структура Toy с полями: id, name, category, price, quantity, next
#include <Windows.h>

#define NOMINMAX

using namespace std;

Toy* firstitem = nullptr;
int nextId = 1;

// --- ФУНКЦИИ ---

void AddData() {
    system("cls");
    cout << "=== ДОБАВИТЬ ИГРУШКУ ===" << endl;

    Toy* toy = new Toy();
    toy->id = nextId++;
    cout << "ID: " << toy->id << endl;

    cout << "Название: ";
    cin.ignore(); // Очищаем буфер перед getline
    getline(cin, toy->name);

    cout << "\nКатегория:" << endl;
    cout << "1) Мягкая" << endl;
    cout << "2) Пазл" << endl;
    cout << "3) Конструктор" << endl;
    cout << "4) Электронные" << endl;
    cout << "5) Антистресс" << endl;
    cout << "6) Другое" << endl;
    cout << "Выберите категорию (цифра): ";

    char catChoice = _getch();
    cout << catChoice << "\n";

    switch (catChoice) {
    case '1': toy->category = "мягкая"; break;
    case '2': toy->category = "пазл"; break;
    case '3': toy->category = "конструктор"; break;
    case '4': toy->category = "электронные"; break;
    case '5': toy->category = "антистресс"; break;
    case '6': toy->category = "другое"; break;
    default:
        cout << "Неверный выбор! Категория установлена как 'другое'." << endl;
        toy->category = "другое";
        _getch();
        break;
    }

    cout << "Цена: ";
    cin >> toy->price;

    cout << "Количество: ";
    cin >> toy->quantity;

    // Добавляем в начало списка (проще и быстрее, чем искать конец)
    toy->next = firstitem;
    firstitem = toy;

    cout << "\nИгрушка успешно добавлена!" << endl;
    _getch();
}

void PrintData() {
    system("cls");
    cout << "=== СПИСОК ИГРУШЕК ===\n\n";

    if (firstitem == nullptr) {
        cout << "Список пуст!\n";
    }
    else {
        Toy* current = firstitem;
        int count = 0;
        while (current != nullptr) {
            cout << "[" << ++count << "] ID: " << current->id << endl;
            cout << "    Название: " << current->name << endl;
            cout << "    Категория: " << current->category << endl;
            cout << "    Цена: " << current->price << " руб." << endl;
            cout << "    Количество: " << current->quantity << " шт." << endl;
            cout << "-------------------\n";
            current = current->next;
        }
    }
    cout << "\nНажмите любую клавишу...";
    _getch();
}

void EditData() {
    system("cls");
    cout << "=== РЕДАКТИРОВАНИЕ ===\n\n";

    int id;
    cout << "Введите ID игрушки: ";
    cin >> id;

    Toy* current = firstitem;
    while (current != nullptr) {
        if (current->id == id) {
            cout << "\nНашли: " << current->name << "\n";
            cout << "Старая цена: " << current->price << ". Новая цена: ";
            cin >> current->price;
            cout << "Старое кол-во: " << current->quantity << ". Новое кол-во: ";
            cin >> current->quantity;
            cout << "\nИзменено!\n";
            _getch();
            return;
        }
        current = current->next;
    }

    cout << "Игрушка с ID " << id << " не найдена!\n";
    _getch();
}

void DeleteData() {
    system("cls");
    cout << "=== УДАЛЕНИЕ ===\n\n";

    int id;
    cout << "Введите ID игрушки: ";
    cin >> id;

    if (firstitem == nullptr) {
        cout << "Список пуст!\n";
        _getch();
        return;
    }

    // Удаляем голову
    if (firstitem->id == id) {
        Toy* temp = firstitem;
        firstitem = firstitem->next;
        delete temp;
        cout << "Удалено!\n";
        _getch();
        return;
    }

    // Ищем элемент перед удаляемым
    Toy* current = firstitem;
    while (current->next != nullptr && current->next->id != id) {
        current = current->next;
    }

    if (current->next != nullptr) {
        Toy* temp = current->next;
        current->next = current->next->next;
        delete temp;
        cout << "Удалено!\n";
    }
    else {
        cout << "Не найдено!\n";
    }
    _getch();
}

void PrintFilteredData() {
    system("cls");
    cout << "=== ПОИСК ПО КАТЕГОРИИ ===\n\n";

    string category;
    cout << "Введите категорию (например, мягкая): ";
    cin.ignore();
    getline(cin, category);

    Toy* current = firstitem;
    bool found = false;

    while (current != nullptr) {
        // Приводим к нижнему регистру для лучшего поиска (опционально, сейчас простое сравнение)
        if (current->category == category) {
            cout << "\nID: " << current->id << " | Название: " << current->name << " | Цена: " << current->price << endl;
            found = true;
        }
        current = current->next;
    }

    if (!found) cout << "Ничего не найдено!\n";
    cout << "\nНажмите любую клавишу...";
    _getch();
}

// 💾 ИСПРАВЛЕННОЕ СОХРАНЕНИЕ (используем '|' как разделитель вместо пробела)
bool SaveData() {
    ofstream file("toys.txt");
    if (!file.is_open()) return false;

    Toy* current = firstitem;
    while (current != nullptr) {
        // Формат: ID|Name|Category|Price|Quantity
        file << current->id << "|"
            << current->name << "|"
            << current->category << "|"
            << current->price << "|"
            << current->quantity << "\n";
        current = current->next;
    }
    file.close();
    return true;
}

// 📂 ИСПРАВЛЕННАЯ ЗАГРУЗКА (читаем до символа '|')
bool LoadData() {
    ifstream file("toys.txt");
    if (!file.is_open()) return false;

    // Очищаем текущий список перед загрузкой, если нужно
    while (firstitem != nullptr) {
        Toy* temp = firstitem;
        firstitem = firstitem->next;
        delete temp;
    }

    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        Toy* toy = new Toy();

        // Парсим строку вручную, так как формат ID|Name|Cat|Price|Qty
        size_t pos = 0;
        string token;
        int field = 0;

        // Вспомогательная функция для получения следующего поля
        auto getNextField = [&]() {
            size_t delimiterPos = line.find('|', pos);
            if (delimiterPos == string::npos) {
                token = line.substr(pos);
                pos = line.length();
            }
            else {
                token = line.substr(pos, delimiterPos - pos);
                pos = delimiterPos + 1;
            }
            return token;
            };

        try {
            toy->id = stoi(getNextField());
            toy->name = getNextField();
            toy->category = getNextField();
            toy->price = stod(getNextField()); // или stof, зависит от типа price
            toy->quantity = stoi(getNextField());

            toy->next = nullptr;
            // Вставляем в начало списка
            toy->next = firstitem;
            firstitem = toy;

            if (toy->id >= nextId) nextId = toy->id + 1;
        }
        catch (...) {
            // Если строка битая, пропускаем её
            continue;
        }
    }
    file.close();
    return true;
}

void PrintHelp() {
    system("cls");
    cout << "=== СПРАВКА ===\n\n";
    cout << "1. Добавить игрушку\n";
    cout << "2. Показать все\n";
    cout << "3. Редактировать (цена/кол-во)\n";
    cout << "4. Удалить по ID\n";
    cout << "5. Поиск по категории\n";
    cout << "6. Сохранить в файл\n";
    cout << "8. Выход (автосохранение)\n\n";
    _getch();
}

char MainMenu() {
    system("cls");
    cout << "=== МАГАЗИН ИГРУШЕК ===\n\n";
    cout << "1. Добавить игрушку\n";
    cout << "2. Показать все игрушки\n";
    cout << "3. Редактировать\n";
    cout << "4. Удалить\n";
    cout << "5. Поиск по категории\n";
    cout << "6. Сохранить\n";
    cout << "7. Справка\n";
    cout << "8. Выход\n\n";
    cout << "Выбор: ";

    char choice = _getch();
    cout << choice << "\n";

    if (choice < '1' || choice > '8') {
        cout << "Неверный выбор!\n";
        _getch();
        return '0';
    }
    return choice;
}

void HandleEvents() {
    char choice = 0;
    while (choice != '8') {
        choice = MainMenu();
        switch (choice) {
        case '1': AddData(); break;
        case '2': PrintData(); break;
        case '3': EditData(); break;
        case '4': DeleteData(); break;
        case '5': PrintFilteredData(); break;
        case '6':
            if (SaveData()) cout << "Сохранено!\n";
            else cout << "Ошибка сохранения!\n";
            _getch();
            break;
        case '7': PrintHelp(); break;
        case '8': break;
        case '0': break;
        }
    }
}

int main() {
    SetConsoleOutputCP(1251); // Для русской кодировки в консоли
    HandleEvents();
    SaveData(); // Автосохранение при выходе

    // Очистка памяти
    while (firstitem != nullptr) {
        Toy* temp = firstitem;
        firstitem = firstitem->next;
        delete temp;
    }
    return 0;
}