#ifndef FUNCTION_H
#define FUNCTION_H

#include <fstream>
#include <iostream>

using namespace std;

// Моделирование случайной величины для основного распределения
float modeling_main(float v) {
    double r1; 
    do r1 = (double)rand() / RAND_MAX; 
    while (r1 == 0. || r1 == 1.);
    
    double r2; 
    do r2 = (double)rand() / RAND_MAX; 
    while (r2 == 0. || r2 == 1.);
    
    double z = sqrt(-2 * log(r1)) * cos(2 * M_PI * r2);

    float x = sinh(z / v);
    return x;
}

// Моделирование случайной величины для смеси
double modeling_mix(float p, float v1, float v2){
    double r;  
    double x;
    do r=(double)rand()/RAND_MAX; 
    while(r==0.||r==1.);
    if (r > p){
        x = modeling_main(v1);
    }
    else {
        x = modeling_main(v2);
    }
    return x;
}

// Вычисление плотности для основного распределения
double density_main(float v, float u, float l, float x){
    double f;
    double answer = (x - u) / l;
    double part1 = (v / sqrt((2 * M_PI)));
    double part2 = 1.0 / (sqrt(answer * answer + 1.0));
    double part3 = exp((-1.0) * v * v / 2.0 * pow(log(answer + sqrt(answer*answer +1.0)), 2));
    f = part1 * part2 * part3 / l;
    return f;
}

// Вычисление плотности для смеси
double density_mix(float v1, float u1, float l1,
                    float v2, float u2, float l2, float x,
                    float p){
    return (1-p) * density_main(v1, u1, l1, x) + p * density_main(v2, u2, l2, x);
}

// Массив, где x->f(x) для основного распредления
double* array_destinity(float v, float u, float l, double* mas, int size){
    double* fx = new double[size];
    for (int i=0; i< size; i++){
        fx[i] = density_main(v, u, l, mas[i]);
    }
    return fx;
}

// Массив, где x->f(x) для смеси
double* array_destinity_mix(float v1, float u1, float l1,
                            float v2, float u2, float l2, float p,
                            double* mas, int size){
    double* fx = new double[size];
    for (int i=0; i< size; i++){
        fx[i] = (1.0-p) * density_main(v1, u1, l1, mas[i]) + p * density_main(v2, u2, l2, mas[i]);
    }
    return fx;
}

// Возвращает значение плотности для эмпирического распределения
double* density_empire(double* mas, int size) {
    int k = log2(size) + 1;
    double min = 9999999, max = -9999999;
    for (int i = 0; i < size; i++) {
        if (mas[i] > max) max = mas[i];
        if (mas[i] < min) min = mas[i];
    }

    double delta = (max - min) / k;
    double* array = new double[k]();
    
    for (int i = 0; i < size; i++) {
        int m = int((mas[i] - min) / delta);
        if (m >= k) m = k - 1;
        array[m]++;
    }
    
    for (int i = 0; i < k; i++) {
        array[i] *= 1.0 / ((double) size * delta);
    }

    double* ans = new double[size];
    for (int i = 0; i < size; i++) {
        int m = int((mas[i] - min) / delta);
        if (m >= k) m = k - 1;
        ans[i] = array[m];
    }

    delete[] array;
    return ans;
}

// Создание выборки из случайных величин для основного распределения
double* create_pool(int size, double min, double max,
                            float v){
    double* mas = new double[size];
    for (int i=0; i<size; i++){
        mas[i] = modeling_main(v);
    }
    return mas;
}

// Создание выборки из случайных величин для смеси
double* create_pool_mix(int size, float p, float v1, float v2){
    double* mas = new double[size];
    for (int i=0; i<size; i++){
        mas[i] = modeling_mix(p, v1, v2);
    }
    return mas;
}

// Запись в файл для петухона
void write_to_file(double* x, double* y, int size, int type){
    std::ofstream out; 
    if (type == 0){
        out.open("data.txt");
    }
    else if (type == 1) out.open("practic.txt");
    out << x[0] << " " << y[0];
    for (int i=1; i<size; i++){
        out << endl;
        out << x[i] << " " << y[i];
    }
}

// Вычисление характеристик для основного распределения
double* properties_main(float v, float u, float l){
    double math = u;
    double w = exp(2 / v / v) * l * l;
    double disp = (w-1) / 2;
    double y2 = (w * w + 2 * w - 3) / 2;
    double* mas = new double[4];
    mas[0] = math; mas[1] = disp; mas[2] = 0; mas[3] = y2;
    return mas;
}

// Вычисление мат ожидания
double math_solve(double* mas, float v, int size){
    double math = 0;
    for (int i =0; i< size; i++){
        math += mas[i] * 1.0 / size;
    }
    return math;
}

// Вычисление моментов
double calculate_moment(const double* mas, float v, int size, double math, int order) {
    double sum = 0;
    double* newArray = new double[size];
    for (int i = 0; i < size; ++i) {
        newArray[i] = std::pow(mas[i] - math, order);
    }
    return math_solve(newArray, v, size);
}

// Вычисление характеристик на практике для основного распределения
double* properties_main_practic(double* mas, int size, float v, float u, float l){
    double math = math_solve(mas, v, size);
    
    // Дисперсия
    double disp = calculate_moment(mas, v, size, math, 2);
    
    // Асимметрия
    double y1 = calculate_moment(mas, v, size, math, 3) / std::pow(disp, 1.5);
    
    // Эксцесс
    double y2 = calculate_moment(mas, v, size, math, 4) / std::pow(disp, 2) - 3;
    
    // Создание массива для результатов
    double* ans = new double[4];
    
    // Заполнение результатов с учетом масштабирования
    ans[0] = math * l + u;      // Математическое ожидание
    ans[1] = disp * l * l;      // Дисперсия
    ans[2] = y1;                // Асимметрия
    ans[3] = y2;                // Эксцесс
    
    // Возвращение результата
    return ans;
}

// Вычисление характеристик на практике для смеси
double* properties_mix(double* props1, double* props2, float p){
    double* props = new double[4];

    double math = props1[0] * p + props2[0] * (1 - p);
    double disp = p * (props1[0] * props1[0] + props1[1]) + (1-p) * (props2[0] * props2[0] + props2[1]) - math * math;
    double y1 = 1 / pow(disp, 1.5) * (p * (pow(props1[0] - math, 3) + 3 * (props1[0] - math) * props1[1] + pow(props1[1], 1.5) * props1[2])) +
    ((1-p) * (pow(props2[0] - math, 3) + 3 * (props2[0] - math) * props2[1] + pow(props2[1], 1.5) * props2[2]));
    double y2 = (p * (pow(props1[0] - math, 4) + 6 * pow(props1[0] - math, 2)) * props1[1] + 4 * (props1[0] - math) * pow(props1[1], 1.5) + props1[2] + pow(props1[1], 2) * (props1[3] + 3))
    + ((1-p) * (pow(props2[0] - math, 4) + 6 * pow(props2[0] - math, 2)) * props2[1] + 4 * (props2[0] - math) * pow(props2[1], 1.5) + props2[2] + pow(props2[1], 2) * (props2[3] + 3));
    props[0] = math;
    props[1] = disp;
    props[2] = y1;
    props[3] = y2;

    return props;
}

// Вычисление теоретических характеристик для смеси
double* properties_mix_theory(float v1, float u1, float l1,
                        float v2, float u2, float l2,
                        float p){
    
    double* props1 = properties_main(v1, u1, l1);
    double* props2 = properties_main(v2, u2, l2);
    return properties_mix(props1, props2, p);
}

// Вычисление характеристик на практике для смеси
double* properties_mix_practic(int size, float v1, float u1, float l1,
                        float v2, float u2, float l2,
                        float p){
    double* props1 = properties_main_practic(create_pool(size, 0, 1, v1), size, v1, u1, l1);
    double* props2 = properties_main_practic(create_pool(size, 0, 1, v2), size, v2, u2, l2);
    return properties_mix(props1, props2, p);
}

// Вычисление характеристик на практике для эмпирической функции
double* properties_empire(int size, double* array) {
    double math = 0;
    double disp = 0;
    
    // Математическое ожидание
    for (int i = 0; i < size; i++) {
        math += array[i];
    }
    math /= size;  // M_x

    // Дисперсия (несмещённая оценка)
    for (int i = 0; i < size; i++) {
        disp += pow(array[i] - math, 2);
    }
    disp /= (size - 1);  // D_x
    
    // Коэффициент асимметрии
    double y1 = 0;
    for (int i = 0; i < size; i++) {
        y1 += pow(array[i] - math, 3);
    }
    y1 = (y1 / size) / pow(disp, 1.5);  // γ1
    
    // Коэффициент эксцесса
    double y2 = 0;
    for (int i = 0; i < size; i++) {
        y2 += pow(array[i] - math, 4);
    }
    y2 = (y2 / size) / pow(disp, 2) - 3;  // γ2
    
    // Результат
    double* mas = new double[4];
    mas[0] = math;   // Математическое ожидание
    mas[1] = disp;   // Дисперсия
    mas[2] = y1;     // Коэффициент асимметрии
    mas[3] = y2;     // Коэффициент эксцесса
    
    return mas;
}


#endif // FUNCTION_H