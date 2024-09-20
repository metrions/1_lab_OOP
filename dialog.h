#ifndef DIALOG_H
#define DIALOG_H

#include <iostream>
#include "structures.h"
#include "function.h"

using namespace std;

void otvechalka_density(double f){
    cout << "Плотность равна = " << f << endl;
}

void otvechalka_properties(double* mas){
    cout << "Матожидание = " << mas[0] << endl;
    cout << "Дисперсия = " << mas[1] << endl;
    cout << "Коэффициент ассиметрии = " << mas[2] << endl;
    cout << "Экцесс = " << mas[3] << endl;
}


void sprashivalka_main(int key){
    float v;
    float u;
    float l;
    float x;
    do {
        cout << "Введи параметр формы v (v > 0): " << endl;
        cin >> v;
        if (v <= 0) {
            cout << "Ошибка: параметр формы v должен быть больше 0. Попробуй снова." << endl;
        }
    } while (v <= 0);

    // Ввод параметра сдвига u
    cout << "Введи параметр сдвига u: " << endl;
    cin >> u;

    // Валидация параметра масштаба l
    do {
        cout << "Введи параметр масштаба l (l > 0): " << endl;
        cin >> l;
        if (l <= 0) {
            cout << "Ошибка: параметр масштаба l должен быть больше 0. Попробуй снова." << endl;
        }
    } while (l <= 0);

    if (key == 1) {
        cout << "Аргумент x" << endl;
        cin >> x;

        otvechalka_density(density_main(v, u, l, x));
    }
    else if (key == 2){
        cout << "Теоретические" << endl;
        int size = 10000000;
        double* mas = create_pool(size, 0, 1, v);
        otvechalka_properties(properties_main(v, u, l));
        cout << "\n";
        cout << "Практические" << endl;
        otvechalka_properties(properties_main_practic(mas, size, v, u, l));
    }
    else if (key == 3){
        cout << "Введи объем выборки" << endl;
        int size;
        cin >> size;
        double* mas = create_pool(size, 0, 1, v);
        double* array = array_destinity(v, u, l, mas, size);
        double* newArray = density_empire(mas, size);
        // write_to_file(mas, array_destinity(v, u, l, mas, size), size);
        write_to_file(mas, array, size, 1);
        write_to_file(mas, newArray, size, 0);
        cout << "Теоретические характеристики" << endl;
        otvechalka_properties(properties_main(v, u, l));
        cout << endl << "Практические характеристики" << endl;
        otvechalka_properties(properties_main_practic(mas, size, v, u, l));
        cout << endl << "Эмпирические характеристики" << endl;
        otvechalka_properties(properties_empire(size, newArray));
    }
}

void sprashivalka_mix(int key){
    float v1;
    float u1;
    float l1;
    float x1;
    do {
        cout << "Введи параметр формы v1 (v > 0): " << endl;
        cin >> v1;
        if (v1 <= 0) {
            cout << "Ошибка: параметр формы v должен быть больше 0. Попробуй снова." << endl;
        }
    } while (v1 <= 0);

    // Ввод параметра сдвига u
    cout << "Введи параметр сдвига u1: " << endl;
    cin >> u1;

    // Валидация параметра масштаба l
    do {
        cout << "Введи параметр масштаба l1 (l1 > 0): " << endl;
        cin >> l1;
        if (l1 <= 0) {
            cout << "Ошибка: параметр масштаба l должен быть больше 0. Попробуй снова." << endl;
        }
    } while (l1 <= 0);

    float v2;
    float u2;
    float l2;
    float x2;
    do {
        cout << "Введи параметр формы v2 (v > 0): " << endl;
        cin >> v2;
        if (v2 <= 0) {
            cout << "Ошибка: параметр формы v должен быть больше 0. Попробуй снова." << endl;
        }
    } while (v2 <= 0);

    // Ввод параметра сдвига u
    cout << "Введи параметр сдвига u2: " << endl;
    cin >> u2;

    // Валидация параметра масштаба l
    do {
        cout << "Введи параметр масштаба l2 (l1 > 0): " << endl;
        cin >> l1;
        if (l1 <= 0) {
            cout << "Ошибка: параметр масштаба l2 должен быть больше 0. Попробуй снова." << endl;
        }
    } while (l1 <= 0);


    float p;
    cout << "параметр p" << endl;
    cin >> p;
    
    if (key == 1){
        float x;
        cout << "параметр x" << endl;
        cin >> x;
        otvechalka_density(density_mix(v1, u1, l1, v1, u2, l2, x, p));
    }
    else if (key == 2){
        cout << "Теоретические" << endl;
        int size = 100000;

        otvechalka_properties(properties_mix_theory(v1, u1, l1, v1, u2, l2, p));
        cout << "\n" << endl;
        cout << "Практические" << endl;

        otvechalka_properties(properties_mix_practic(size, v1, u1, l1, v1, u2, l2, p));
    }
    else if (key == 3){
        cout << "Введи объем выборки" << endl;
        int size;
        cin >> size;
        double* mas = create_pool_mix(size, p, v1, v2);
        double* array = array_destinity_mix(v1, u1, l1, v2, u2, l2, p, mas, size);
        double* newArray = density_empire(mas, size);

        write_to_file(mas, array, size, 1);
        write_to_file(mas, newArray, size, 0);
        cout << "Теоретические характеристики" << endl;
        otvechalka_properties(properties_mix_theory(v1, u1, l1, v1, u2, l2, p));
        cout << endl << "Практические характеристики" << endl;
        otvechalka_properties(properties_mix_practic(size, v1, u1, l1, v1, u2, l2, p));
        cout << endl << "Эмпирические характеристики" << endl;
        otvechalka_properties(properties_empire(size, newArray));
    }
}


void sprashivalka(){
    int n;
    int i;
    cout << "Доступный функционал (цифра - функционал)" << endl;
    cout << "1 - Распределение джонсона \n"
         << "2 - Распределение в виде смеси \n";
    do {
        cin >> n;
        if (n <= 0) {
            cout << "Ошибка" << endl;
        }
    } while (n <= 0);

    cout << "Доступный функционал (цифра - функционал)" << endl;
    cout << "1 - Вычислить значений плотности распределения по заданному аргументу\n"
         << "2 - Вычислить математическое ожидание, дисперсию, коэффициент асимметрии и эксцесса (в виде одной функции)\n"
         << "3 - Моделирование случайной величины + Эмпирическое распредление" << endl;
    do {
        cin >> i;
        if (i <= 0) {
            cout << "Ошибка" << endl;
        }
    } while (i <= 0);

    switch (n)
    {
    case 1:
        sprashivalka_main(i);
        break;
    case 2:
        sprashivalka_mix(i); 
        break;
    default:
        cout << "Неверное значение.";
        break;
    }
}

void otvechalka(double value, int key){
    switch (key)
    {
    case 1:
        cout << "Значение плотности распределения Джонсона = ";
        break;
    case 2:
        cout << "распределением в виде смеси двух основных распределений с параметрами (μ1, λ1, ν1) и (μ2, λ2, ν2) и параметром смеси p = ";
        break;
    default:
        break;
    }
}

#endif // STRUCTURES_H