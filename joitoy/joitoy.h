#pragma once
#include <string>
using namespace std;
#ifndef JOITOY_H //›“Œ .h
#define JOITOY_H
struct Toy {
    int id;
    string name;
    string category;
    double price;
    int quantity;
    Toy* next;

    Toy() : id(0), price(0), quantity(0), next(nullptr) {}
};
#endif