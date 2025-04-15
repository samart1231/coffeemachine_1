#ifndef COFFEE_MACHINE_H
#define COFFEE_MACHINE_H

#include <map>
#include <vector>
#include <string>
#include <utility>

class CoffeeMachine {
private:
    std::vector<std::pair<std::string, double>> cart;

    static void showMenu();
    static void showBreadMenu();
    void addItemToCart(const std::string &item, double price);
    void writeCartToFile(const std::string &filename, double totalCost);
    void generatePaymentQR();
    void showCart();
    void handleCoffeeSelection(const std::string &coffee, double basePrice);

    static void loadBreadStockFromFile(const std::string &filename, std::map<std::string, int> &breadStock);

    static void saveBreadStockToFile(const std::string &filename, const std::map<std::string, int> &breadStock);

    void handleBreadSelection();
    void checkout();
public:
    void run();
};

#endif // COFFEE_MACHINE_H