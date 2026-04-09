// main.cpp - Учёт игрушек с персональными базами данных
#include <iostream>
#include <conio.h>
#include <fstream>
#include <string>
#include <vector>
#include <windows.h>
#include "joitoy.h"

using namespace std;

Toy* firstitem = nullptr;     // Указатель на первый элемент связного списка игрушек
int nextId = 1;               // Счётчик для генерации уникальных ID новых игрушек
string currentUser = "";      // Логин текущего авторизованного пользователя

//ВСПОМОГАТЕЛЬНЫЕ ФУНКЦИИ

string GetUserDBFile(const string& login) {

    string safeLogin = login;
    for (char& c : safeLogin) {
        if (c == '\\' || c == '/' || c == ':' || c == '*' ||
            c == '?' || c == '"' || c == '<' || c == '>' || c == '|') {
            c = '_';
        }
    }
    return "toys_" + safeLogin + ".txt";
}

string GetCurrentDBFile() {
    return GetUserDBFile(currentUser);
}

string FindUserPassword(const string& login) {
    ifstream file("users.txt");
    if (!file.is_open()) return "";

    string line;
    while (getline(file, line)) {
        size_t pos = line.find('|');
        if (pos != string::npos) {
            string fileLogin = line.substr(0, pos);
            if (fileLogin == login) {
                string password = line.substr(pos + 1);
                file.close();
                return password;
            }
        }
    }
    file.close();
    return "";
}

bool UserExists(const string& login) {
    return !FindUserPassword(login).empty();
}

bool CheckCredentials(const string& login, const string& password) {
    string storedPassword = FindUserPassword(login);
    return !storedPassword.empty() && storedPassword == password;
}

bool RegisterUser(const string& login, const string& password) {
    if (login.empty() || password.empty() || UserExists(login))
        return false;

    ofstream file("users.txt", ios::app);
    if (!file.is_open()) return false;

    file << login << "|" << password << "\n";
    return file.good();
}

//МЕНЮ АВТОРИЗАЦИИ 

bool REGISTER() {
    SetConsoleOutputCP(1251);
    system("cls");
    SetTextColor(crt::COLOR_YELLOW);
    cout << "=== РЕГИСТРАЦИЯ ===" << endl;
    ResetColor();

    string login, password, confirm;

    cout << "\nПридумайте логин: ";
    cin.clear();
    cin.sync();
    getline(cin, login);

    if (login.empty()) {
        SetTextColor(crt::COLOR_LIGHT_RED);
        cout << "Ошибка: логин не может быть пустым!" << endl;
        ResetColor();
        _getch();
        return false;
    }

    if (UserExists(login)) {
        SetTextColor(crt::COLOR_LIGHT_RED);
        cout << "Ошибка: пользователь с таким логином уже существует!" << endl;
        ResetColor();
        _getch();
        return false;
    }

    cout << "Придумайте пароль: ";
    getline(cin, password);

    if (password.empty()) {
        SetTextColor(crt::COLOR_LIGHT_RED);
        cout << "Ошибка: пароль не может быть пустым!" << endl;
        ResetColor();
        _getch();
        return false;
    }

    cout << "Подтвердите пароль: ";
    getline(cin, confirm);

    if (password != confirm) {
        SetTextColor(crt::COLOR_LIGHT_RED);
        cout << "Ошибка: пароли не совпадают!" << endl;
        ResetColor();
        _getch();
        return false;
    }

    if (RegisterUser(login, password)) {
        SetTextColor(crt::COLOR_LIGHT_GREEN);
        cout << "\nРегистрация успешна! Теперь вы можете войти." << endl;
        ResetColor();
        _getch();
        return true;
    }
    else {
        SetTextColor(crt::COLOR_LIGHT_RED);
        cout << "\nОшибка при сохранении данных!" << endl;
        ResetColor();
        _getch();
        return false;
    }
}

bool LOGIN_IN() {
    SetConsoleOutputCP(1251);
    system("cls");
    SetTextColor(crt::COLOR_YELLOW);
    cout << "=== АВТОРИЗАЦИЯ ===" << endl;
    cout << "1) Войти" << endl;
    cout << "2) Регистрация" << endl;
    cout << "3) Выход" << endl;
    cout << "\nВыбор: ";
    ResetColor();

    char choice = _getch();
    cout << choice << "\n\n";

    string login, password;

    switch (choice) {
    case '1':
        cout << "Логин: ";
        cin.clear();
        cin.sync();
        getline(cin, login);
        cout << "Пароль: ";
        getline(cin, password);

        if (CheckCredentials(login, password)) {
            currentUser = login;  

            SetTextColor(crt::COLOR_LIGHT_GREEN);
            cout << "\nУспешный вход, нажмите любую клавишу чтобы продолжить... " << endl;
            ResetColor();
            _getch();
            return true;
        }
        else {
            SetTextColor(crt::COLOR_LIGHT_RED);
            cout << "\nНеверный логин или пароль!" << endl;
            ResetColor();
            _getch();
            return false;
        }

    case '2': 
        REGISTER();
        LOGIN_IN();
        return true;

    case '3':
        SetTextColor(crt::COLOR_LIGHT_RED);
        cout << "\nВыход из программы..." << endl;
        ResetColor();
        Sleep(1000);
        return false;

    default:
        SetTextColor(crt::COLOR_LIGHT_RED);
        cout << "\nНеверный выбор!" << endl;
        ResetColor();
        _getch();
        return false;
    }
}

//ФУНКЦИИ УПРАВЛЕНИЯ ИГРУШКАМИ

void AddData() {
    SetConsoleOutputCP(1251);
    system("cls");
    SetTextColor(crt::COLOR_LIGHT_MAGENTA);
    cout << " ДОБАВИТЬ ИГРУШКУ " << endl;
    ResetColor();
    Toy* toy = new Toy();
    toy->id = nextId++;
    SetTextColor(crt::COLOR_MAGENTA);
    cout << "ID: " << toy->id << endl;
    ResetColor();
    cin.clear();  

    SetTextColor(crt::COLOR_MAGENTA);
    cout << "Название: ";
    ResetColor();
    getline(cin, toy->name);

    if (toy->name.empty()) {
        SetTextColor(crt::COLOR_LIGHT_RED);
        cout << "Ошибка: название не может быть пустым!" << endl;
        ResetColor();
        _getch();
        delete toy;
        return;
    }

    SetTextColor(crt::COLOR_MAGENTA);
    cout << "\nКатегория:" << endl;
    ResetColor();
    cout << "1) Мягкая\n2) Пазл\n3) Конструктор\n4) Электронные\n5) Антистресс\n6) Другое\n";
    cout << "Выберите категорию (цифра): ";

    char catChoice = _getch(); //Выбор
    cout << catChoice << "\n";

    switch (catChoice) {
    case '1': toy->category = "мягкая"; break;
    case '2': toy->category = "пазл"; break;
    case '3': toy->category = "конструктор"; break;
    case '4': toy->category = "электронные"; break;
    case '5': toy->category = "антистресс"; break;
    case '6': toy->category = "другое"; break;
    default:
        SetTextColor(crt::COLOR_LIGHT_RED);
        cout << "Неверный выбор! Категория: 'другое'." << endl;
        ResetColor();
        toy->category = "другое";
        _getch();
        break;
    }

    SetTextColor(crt::COLOR_MAGENTA);
    cout << "Цена: ";
    ResetColor();
    cin >> toy->price;

    SetTextColor(crt::COLOR_MAGENTA);
    cout << "Количество: ";
    ResetColor();
    cin >> toy->quantity;

    //очищаем буфер после числового ввода
    cin.ignore(1000, '\n');

    toy->next = firstitem;
    firstitem = toy;

    SetTextColor(crt::COLOR_LIGHT_GREEN);
    cout << "\nИгрушка успешно добавлена!" << endl;
    ResetColor();
    _getch();
}

void SearchById() {
    SetConsoleOutputCP(1251);
    system("cls");
    SetTextColor(crt::COLOR_YELLOW);
    cout << " ПОИСК ПО ID \n\n";
    ResetColor();

    int searchId;
    SetTextColor(crt::COLOR_LIGHT_CYAN);
    cout << "Введите ID игрушки: ";
    ResetColor();
    cin >> searchId;

    Toy* current = firstitem;
    bool found = false;

    while (current) {
        if (current->id == searchId) {
            SetTextColor(crt::COLOR_LIGHT_GREEN);
            cout << "\nНайдено:\n";
            ResetColor();
            cout << "ID: " << current->id << "\n"
                << "Название: " << current->name << "\n"
                << "Категория: " << current->category << "\n"
                << "Цена: " << current->price << " руб.\n"
                << "Количество: " << current->quantity << " шт.\n";
            found = true;
            break;
        }
        current = current->next;
    }

    if (!found) {
        SetTextColor(crt::COLOR_LIGHT_RED);
        cout << "\nИгрушка с ID " << searchId << " не найдена!\n";
        ResetColor();
    }
    cout << "\nНажмите любую клавишу...";
    _getch();
}

void SearchByCategory() {
    SetConsoleOutputCP(1251);
    system("cls");
    SetTextColor(crt::COLOR_YELLOW);
    cout << " ПОИСК ПО КАТЕГОРИИ \n\n";
    ResetColor();

    const vector<string> categories = {
        "мягкая", "пазл", "конструктор",
        "электронные", "антистресс", "другое"
    };

    cout << "Доступные категории:\n";
    for (int i = 0; i < 6; ++i) {
        cout << "  " << (i + 1) << ". " << categories[i] << "\n";
    }
    cout << "\n";
     
    int choice;
    cout << "Введите номер категории (1-6): ";

    while (!(cin >> choice) || choice < 1 || choice > 6) {
        SetTextColor(crt::COLOR_LIGHT_RED);
        cout << " Ошибка: введите число от 1 до ! ";
        ResetColor();
        cin.clear();
        cin.ignore(1000, '\n');
        cout << "Повторите ввод: ";
    }
    cin.ignore(1000, '\n');

    string selectedCategory = categories[choice - 1];

    Toy* current = firstitem;
    bool found = false;
    int count = 0;

    while (current) {
        if (current->category == selectedCategory) {
            if (!found) {
                SetTextColor(crt::COLOR_LIGHT_GREEN);
                cout << "\nНайдено в категории \"" << selectedCategory << "\":\n";
                ResetColor();
                found = true;
            }
            count++;
            cout << "[" << count << "] ID: " << current->id
                << " | " << current->name
                << " | " << current->price << " руб."
                << " (в наличии: " << current->quantity << ")\n";
        }
        current = current->next;
    }

    if (!found) {
        SetTextColor(crt::COLOR_LIGHT_RED);
        cout << "\nНичего не найдено в категории \"" << selectedCategory << "\"\n";
        ResetColor();
    }
    cout << "\nНажмите любую клавишу...";
    _getch();
}

void SearchByPrice() {
    SetConsoleOutputCP(1251);
    system("cls");
    SetTextColor(crt::COLOR_YELLOW);
    cout << " ПОИСК ПО ЦЕНЕ \n\n";
    ResetColor();

    double minPrice, maxPrice;

    cout << "Минимальная цена: ";
    cin >> minPrice;
    cout << "Максимальная цена: ";
    cin >> maxPrice;

    if (minPrice > maxPrice) {
        double temp = minPrice;
        minPrice = maxPrice;
        maxPrice = temp;
    }

    Toy* current = firstitem;
    bool found = false;
    int count = 0;

    while (current) {
        if (current->price >= minPrice && current->price <= maxPrice) {
            if (!found) {
                SetTextColor(crt::COLOR_LIGHT_GREEN);
                cout << "\nНайдено:\n";
                ResetColor();
                found = true;
            }
            count++;
            cout << "[" << count << "] ID: " << current->id
                << " | " << current->name
                << " | " << current->price << " руб."
                << " | " << current->category << "\n";
        }
        current = current->next;
    }

    if (!found) {
        SetTextColor(crt::COLOR_LIGHT_RED);
        cout << "\nНет игрушек в диапазоне "
            << minPrice << " - " << maxPrice << " руб.\n";
        ResetColor();
    }
    cout << "\nНажмите любую клавишу...";
    _getch();
}

void PrintData() {
    SetConsoleOutputCP(1251);
    SetTextColor(crt::COLOR_YELLOW);
    system("cls");
    cout << " СПИСОК ИГРУШЕК [" << currentUser << "]\n\n";  //Показываем текущего пользователя
    ResetColor();

    if (!firstitem) {
        SetTextColor(crt::COLOR_LIGHT_RED);
        cout << "Список пуст! Добавьте первую игрушку.\n";
        ResetColor();
    }
    else {
        Toy* current = firstitem;
        int count = 0;
        while (current) {
            cout << "[" << ++count << "] ID: " << current->id << "\n"
                << "    Название: " << current->name << "\n"
                << "    Категория: " << current->category << "\n"
                << "    Цена: " << current->price << " руб.\n"
                << "    Количество: " << current->quantity << " шт.\n"
                << "-------------------\n";
            current = current->next;
            if (count % 5 == 0)
            {
                SetTextColor(crt::COLOR_YELLOW);
                cout << "Чтобы увидеть следующую часть списка, нажмите любую клавишу...";
                ResetColor();
                _getch();
              
            }
        }
    }
    cout << "\nНажмите любую клавишу...";
    _getch();
}

void EditData() {
    SetConsoleOutputCP(1251);
    system("cls");
    SetTextColor(crt::COLOR_YELLOW);
    cout << " РЕДАКТИРОВАНИЕ \n\n";
    ResetColor();

    int id;
    cout << "Введите ID игрушки: ";
    cin >> id;
    cin.ignore(1000, '\n');

    Toy* current = firstitem;
    while (current) {
        if (current->id == id) {
            cout << "\nНашли: " << current->name << "\n";
            cout << "Старая цена: " << current->price << " -> Новая цена: ";
            cin >> current->price;
            cout << "Старое кол-во: " << current->quantity << " -> Новое кол-во: ";
            cin >> current->quantity;
            cin.ignore(1000, '\n');  //Добавить очистку буфера
            return;
        }
        current = current->next;
    }
    SetTextColor(crt::COLOR_LIGHT_RED);
    cout << " Игрушка с ID " << id << " не найдена!\n";
    ResetColor();
    _getch();
}

void DeleteData() {
    SetConsoleOutputCP(1251);
    system("cls");
    SetTextColor(crt::COLOR_YELLOW);
    cout << " УДАЛЕНИЕ \n\n";
    ResetColor();

    int id;
    cout << "Введите ID игрушки: ";
    cin >> id;

    if (!firstitem) {
        SetTextColor(crt::COLOR_LIGHT_RED);
        cout << "Список пуст!\n";
        ResetColor();
        _getch();
        return;
    }

    if (firstitem->id == id) {
        Toy* temp = firstitem;
        firstitem = firstitem->next;
        delete temp;
        SetTextColor(crt::COLOR_LIGHT_GREEN);
        cout << " Удалено!\n";
        ResetColor();
        _getch();
        return;
    }

    Toy* current = firstitem;
    while (current->next && current->next->id != id)
        current = current->next;

    if (current->next) {
        Toy* temp = current->next;
        current->next = current->next->next;
        delete temp;
        SetTextColor(crt::COLOR_LIGHT_GREEN);
        cout << " Удалено!\n";
        ResetColor();
    }
    else {
        SetTextColor(crt::COLOR_LIGHT_RED);
        cout << " Не найдено!\n";
        ResetColor();
    }
    _getch();
}

bool SaveData() {
    string filename = GetCurrentDBFile();
    ofstream file(filename);
    if (!file.is_open()) {
        SetTextColor(crt::COLOR_LIGHT_RED);
        cout << "Ошибка: не удалось открыть файл " << filename << endl;
        ResetColor();
        return false;
    }
    Toy* current = firstitem;
    while (current) {
        file << current->id << "|" << current->name << "|"
            << current->category << "|" << current->price << "|"
            << current->quantity << "\n";
        current = current->next;
    }
    file.close();
    return true;
}
bool LoadData() {
    string filename = GetCurrentDBFile();
    ifstream file(filename);

    // Если файл не существует — это нормально для нового пользователя
    if (!file.is_open()) {
        // Создаём пустой список для нового пользователя
        firstitem = nullptr;
        nextId = 1;
        return true;
    }

    // Очистка текущего списка
    while (firstitem) {
        Toy* temp = firstitem;
        firstitem = firstitem->next;
        delete temp;
    }
    int maxId = 0;
    string line;
    while (getline(file, line)) {
        if (line.empty()) continue;

        Toy* toy = new Toy();
        size_t pos = 0;
        string token;

        auto getNextField = [&]() {
            size_t delim = line.find('|', pos);
            if (delim == string::npos) {
                token = line.substr(pos);
                pos = line.length();
            }
            else {
                token = line.substr(pos, delim - pos);
                pos = delim + 1;
            }
            return token;
            };

        try {
            toy->id = stoi(getNextField());
            toy->name = getNextField();
            toy->category = getNextField();
            toy->price = stod(getNextField());
            toy->quantity = stoi(getNextField());
            toy->next = firstitem;
            firstitem = toy;

            if (toy->id > maxId) maxId = toy->id;
        }
        catch (...) {
            delete toy;
            continue;
        }
    }
    file.close();
    nextId = maxId + 1;
    return true;
}

void PrintHelp() {
    system("cls");
    SetTextColor(crt::COLOR_LIGHT_CYAN);
    cout << " СПРАВКА [Пользователь: " << currentUser << "]\n\n"
        << "1. Добавить игрушку\n"
        << "2. Показать все игрушки\n"
        << "3. Редактировать (цена/кол-во)\n"
        << "4. Удалить по ID\n"
        << "5. Поиск по категории\n"
        << "6. Поиск по ID\n"
        << "7. Поиск по цене\n"
        << "8. Сохранить в файл\n"
        << "9. Справка\n"
        << "0. Выход (автосохранение)\n";
    ResetColor();
    SetTextColor(crt::COLOR_LIGHT_GREEN);
    cout << "\n Данные хранятся в: " << GetCurrentDBFile() << "\n\n";
    ResetColor();
    _getch();
}
char MainMenu() {
    system("cls");
    SetTextColor(crt::COLOR_LIGHT_GREEN);
    cout << " УЧЕТ ТОВАРОВ В МАГАЗИНЕ ИГРУШЕК \n";
    ResetColor();
    SetTextColor(crt::COLOR_GREEN);
    cout << "====================================\n";
    ResetColor();
    cout << "Пользователь: " << currentUser << "\n";
    cout << "База данных: " << GetCurrentDBFile() << "\n\n";  //Показываем файл БД
    SetTextColor(crt::COLOR_LIGHT_CYAN);
    cout << "         МЕНЮ\n\n";
    ResetColor();
    cout << "1. Добавить игрушку\n"
        << "2. Показать все игрушки\n"
        << "3. Редактировать\n"
        << "4. Удалить по ID\n"
        << "5. Поиск по категории\n"
        << "6. Поиск по ID\n"
        << "7. Поиск по цене\n"
        << "8. Сохранить в файл\n"
        << "9. Справка\n"
        << "0. Выход (автосохранение)\n\n"
        << "Выбор: ";
    ResetColor();
    char choice = _getch();
    cout << choice << "\n";

    if (choice < '0' || choice > '9') {
        SetTextColor(crt::COLOR_LIGHT_RED);
        cout << " Неверный выбор!\n";
        ResetColor();
        _getch();
        return '0';
    }
    return choice;
}
void HandleEvents() {
    char choice = 0;
    while (choice != '0') {
        choice = MainMenu();
        switch (choice) {
        case '1': AddData(); break;
        case '2': PrintData(); break;
        case '3': EditData(); break;
        case '4': DeleteData(); break;
        case '5': SearchByCategory(); break;
        case '6': SearchById(); break;
        case '7': SearchByPrice(); break;
        case '8':
            if (SaveData()) {
                SetTextColor(crt::COLOR_LIGHT_GREEN);
                cout << " Сохранено в " << GetCurrentDBFile() << "!\n";
                ResetColor();
            }
            else {
                SetTextColor(crt::COLOR_LIGHT_RED);
                cout << " Ошибка сохранения!\n";
                ResetColor();
            }
            _getch();
            break;
        case '9': PrintHelp(); break;
        case '0':
            SetTextColor(crt::COLOR_YELLOW);
            cout << "\nСохранение перед выходом...\n";
            ResetColor();
            SaveData();
            break;
        }
    }
}

int main() {
    SetConsoleOutputCP(65001);  // UTF-8 для вывода
    SetConsoleCP(65001);         // UTF-8 для ввода
    setlocale(LC_ALL, "ru_RU.UTF-8");

    // SetConsoleOutputCP(1251);
    // SetConsoleCP(1251);
    // setlocale(LC_ALL, "Russian");
   // Авторизация
    if (!LOGIN_IN()) {
        cout << "\nПрограмма завершена.\n";
        _getch();
        return 0;
    }
    LoadData();
    HandleEvents();
    SaveData();

    while (firstitem) {
        Toy* temp = firstitem;
        firstitem = firstitem->next;
        delete temp;
    }
    cout << "\nСпасибо за работу, " << currentUser << "!\n";
    cout << "Нажмите любую клавишу для завершения...";
    _getch();
    return 0;
}

