#include <iostream>
#include <fstream>
#include <cmath>
#include <exception>
#include <vector>
#include <string>
#include <sstream>

using namespace std;

// Визначення винятку для файлу, що не знайдено
class FileNotFoundException : public exception {
    string message;
public:
    FileNotFoundException(const string& filename) {
        message = "File not found: " + filename;
    }
    const char* what() const noexcept override {
        return message.c_str();
    }
};

// Визначення винятку для ділення на нуль
class DivisionByZeroException : public exception {
public:
    const char* what() const noexcept override {
        return "Division by zero error";
    }
};

// Прототипи функцій
double Krn(double x, double y, double z);
double Rrz(double x, double y, double z);
double Qrz(double x, double y);
double Srs(double x, double y, double z);
double Srz(double x, double y, double z);
double T(double x);
double U(double x);
double fun(double x, double y, double z);

// Глобальні змінні для збереження даних з файлів
vector<pair<double, double>> dataT;
vector<pair<double, double>> dataU;

// Функція для зчитування даних з файлу
void readData(const string& filename, vector<pair<double, double>>& data) {
    ifstream file(filename);
    if (!file.is_open()) {
        throw FileNotFoundException(filename);
    }
    double x, t, u;
    while (file >> x >> t >> u) {
        data.emplace_back(x, t);
    }
    file.close();
}

// Головна функція
int main() {
    double x, y, z;
    cout << "Enter values for x, y, z: ";
    cin >> x >> y >> z;

    try {
        // Завантаження даних з файлів
        readData("dat_X_1_1.dat", dataT);
        readData("dat_X_1_00.dat", dataU);
        readData("dat_X_00_1.dat", dataU);

        double result = fun(x, y, z);
        cout << "The value of fun(x, y, z) is: " << result << endl;
    }
    catch (const exception& e) {
        cerr << "Error: " << e.what() << endl;
    }

    return 0;
}

// Визначення функцій

double Krn(double x, double y, double z) {
    return 73.1389 * Rrz(x, y, z) + 14.838 * Rrz(z - y, z, y);
}

double Rrz(double x, double y, double z) {
    if (x > y) {
        return x * x * Qrz(x - x, y);
    }
    else {
        return y + y * Qrz(y, x);
    }
}

double Qrz(double x, double y) {
    if (fabs(x) < 1) {
        return y * Srs(x, y, y);
    }
    else {
        return y * Srs(y, x, y);
    }
}

double Srs(double x, double y, double z) {
    if (z > y && z * z + x * y > 0) {
        return Srz(x, y, z) + y * sqrt(2 + x * y);
    }
    else {
        return x + x * y * z * (z * y);
    }
}

double Srz(double x, double y, double z) {
    if (z > y) {
        return Srs(x, y, z) + y * sqrt(2 + x * y);
    }
    else {
        return x + x * y * z * (z * y);
    }
}

// Функція для інтерполяції даних
double interpolate(const vector<pair<double, double>>& data, double x) {
    if (x <= data.front().first) return data.front().second;
    if (x >= data.back().first) return data.back().second;
    for (size_t i = 1; i < data.size(); ++i) {
        if (x < data[i].first) {
            double x1 = data[i - 1].first, y1 = data[i - 1].second;
            double x2 = data[i].first, y2 = data[i].second;
            return y1 + (y2 - y1) * (x - x1) / (x2 - x1);
        }
    }
    return 0;
}

double T(double x) {
    return interpolate(dataT, x);
}

double U(double x) {
    return interpolate(dataU, x);
}

// Основна функція, яка використовує інші функції
double fun(double x, double y, double z) {
    try {
        return x * Krn(x, y, z) + y * Krn(x, z, y) - z * Krn(x, z, y);
    }
    catch (const DivisionByZeroException& e) {
        cerr << e.what() << endl;
        // Реалізуйте логіку обробки ділення на нуль на основі наданого алгоритму
        return 0; // Повернення значення за замовчуванням
    }
    catch (const FileNotFoundException& e) {
        cerr << e.what() << endl;
        // Реалізуйте логіку обробки файлу, який не знайдено, на основі наданого алгоритму
        return 0; // Повернення значення за замовчуванням
    }
}
