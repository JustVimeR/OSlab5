#include <iostream>
#include <thread>
#include <atomic>
#include <chrono>

// Функція для розрахунку факторіалу числа
int f(int x) {
    if (x < 0) return 0;
    long long factorial = 1;
    for (int i = 1; i <= x; ++i) {
        factorial *= i;
        std::this_thread::sleep_for(std::chrono::milliseconds(100));
    }
    return factorial > 0 ? 1 : 0;
}

// Функція для перевірки, чи є число простим
int g(int x) {
    if (x <= 1) return 0;
    for (int i = 2; i * i <= x; ++i) {
        if (x % i == 0) return 0;
        std::this_thread::sleep_for(std::chrono::milliseconds(50));
    }
    return 1;
}

// Головна програма
int main() {
    int x;
    std::cout << "Enter the value of x: ";
    std::cin >> x;

    std::atomic<bool> result_f{ false }, result_g{ false };
    std::atomic<bool> computed_f{ false }, computed_g{ false };

    // Поток для f(x)
    std::thread thread_f([&]() {
        int res = f(x);
        result_f = res != 0;
        computed_f = true;
        });

    // Поток для g(x)
    std::thread thread_g([&]() {
        int res = g(x);
        result_g = res != 0;
        computed_g = true;
        });

    // Перевірка стану обчислень
    while (!computed_f || !computed_g) {
        std::this_thread::sleep_for(std::chrono::seconds(10)); // Чекаємо 10 секунд

        if (!computed_f && !computed_g) {
            std::cout << "Both calculations are still ongoing.\n";
        }
        else if (!computed_f) {
            std::cout << "The calculation of f(x) is still in progress.\n";
        }
        else if (!computed_g) {
            std::cout << "The computation of g(x) is still in progress.\n";
        }

        std::cout << "1) Continue calculation, 2) Stop, 3) Continue without asking more: ";
        int choice;
        std::cin >> choice;
        if (choice == 2) break;
        if (choice == 3) {
            while (!computed_f || !computed_g) {
                std::this_thread::sleep_for(std::chrono::seconds(10));
            }
            break;
        }
    }

    bool final_result = result_f && result_g;
    if (computed_f && computed_g) {
        std::cout << "Result f(x) && g(x): " << final_result << std::endl;
    }
    else {
        std::cout << "The calculation was interrupted." << std::endl;
    }

    thread_f.join();
    thread_g.join();
    return 0;
}


