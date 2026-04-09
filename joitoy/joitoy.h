
#pragma once       
#ifndef JOITOY_H        
#define JOITOY_H        
#include <string>      
#include <Windows.h>    

#define NOMINMAX       
                        

using namespace std;    

namespace crt {
    const int COLOR_BLACK = 0;  
    const int COLOR_BLUE = 1;  
    const int COLOR_GREEN = 2;  
    const int COLOR_CYAN = 3; 
    const int COLOR_RED = 4; 
    const int COLOR_MAGENTA = 5;  
    const int COLOR_BROWN = 6; 
    const int COLOR_GRAY = 7; 
    const int COLOR_DARK_GRAY = 8; 
    const int COLOR_LIGHT_BLUE = 9;  
    const int COLOR_LIGHT_GREEN = 10; 
    const int COLOR_LIGHT_CYAN = 11; 
    const int COLOR_LIGHT_RED = 12; 
    const int COLOR_LIGHT_MAGENTA = 13; 
    const int COLOR_YELLOW = 14; 
    const int COLOR_WHITE = 15; 
}
struct Toy {
    int id;            
    string name;         
    string category;    
    double price;       
    int quantity;       
    Toy* next;          

    Toy() : id(0), price(0), quantity(0), next(nullptr) {}
};
inline void SetTextColor(int color, int background = crt::COLOR_BLACK) {
    HANDLE hConsole = GetStdHandle(STD_OUTPUT_HANDLE);  
    SetConsoleTextAttribute(hConsole, color | (background << 4)); 
}
inline void ResetColor() {
    SetTextColor(crt::COLOR_GRAY, crt::COLOR_BLACK);
}
bool UserExists(const string& login);
bool RegisterUser(const string& login, const string& password);
bool CheckCredentials(const string& login, const string& password);
bool REGISTER();

#endif 
