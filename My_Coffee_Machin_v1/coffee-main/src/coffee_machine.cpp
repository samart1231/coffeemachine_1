#include "coffee_machine.h"
#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>
#include <map>
#include <string>
#include <sstream>
#include <vector>
#include <variant>

void CoffeeMachine::showMenu() {
    std::cout << "Welcome to the Coffee Machine!" << std::endl;
    std::cout << "1. Coffee Menu" << std::endl;
    std::cout << "2. Bread Menu" << std::endl;
    std::cout << "3. View Cart" << std::endl;
    std::cout << "4. Checkout" << std::endl;
    std::cout << "5. Exit" << std::endl;
}
void CoffeeMachine::showBreadMenu() {
    std::cout << "Bread Menu:" << std::endl;
    std::cout << "1. Whole Wheat - $2.00" << std::endl;
    std::cout << "2. Croissant - $2.50" << std::endl;
    std::cout << "3. Bagel - $1.50" << std::endl;
    std::cout << "4. Whole Wheat Bread - $2.00" << std::endl;
    std::cout << "5. Naan - $2.50" << std::endl;
    std::cout << "6. Pita - $1.50" << std::endl;
    std::cout << "7. Back to Main Menu" << std::endl;
}
void CoffeeMachine::addItemToCart(const std::string &item, double price) {
    cart.emplace_back(item, price);
    std::cout << item << " added to your cart for $"
              << std::fixed << std::setprecision(2) << price << "." << std::endl;
}
void CoffeeMachine::writeCartToFile(const std::string &filename, double totalCost) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Unable to create file for cart details." << std::endl;
        return;
    }
    for (const auto& item : cart) {
        file << item.first << "," << std::fixed << std::setprecision(2) << item.second << std::endl;
    }
    file << "Total," << std::fixed << std::setprecision(2) << totalCost << std::endl;
    file.close();
    std::cout << "Cart details written to " << filename << std::endl;
}
void CoffeeMachine::generatePaymentQR() {
    const std::string cartFile = "../scripts/cart.txt";
    double totalCost = 0.0;

    // Write cart details to a file
    std::ofstream file(cartFile);
    if (!file) {
        std::cerr << "Error: Unable to create file for cart details." << std::endl;
        return;
    }
    for (const auto& item : cart) {
        file << item.first << "," << std::fixed << std::setprecision(2) << item.second << std::endl;
        totalCost += item.second;
    }
    file << "Total," << std::fixed << std::setprecision(2) << totalCost << std::endl;
    file.close();

    // Execute the Python script with the cart file as an argument
    std::string command = "python ../scripts/generate_payment_qr.py " + cartFile;
    int result = std::system(command.c_str());
    if (result == -1) {
        std::cerr << "Error: Failed to execute the system command." << std::endl;
    } else if (result != 0) {
        std::cerr << "Error: Python script returned a non-zero exit code." << std::endl;
    } else {
        std::cout << "Payment QR code generated successfully." << std::endl;
    }
}
void CoffeeMachine::showCart() {
    std::cout << "Your cart contains:" << std::endl;
    for (const auto& item : cart) {
        std::cout << "- " << item.first << ": $" << std::fixed << std::setprecision(2) << item.second << std::endl;
    }if (cart .empty()) {
        std::cout << "-- Your cart is empty. --\n***** Please add the product to your cart. *****" << std::endl;
    } else {

        double totalCost = 0.0;
        for (const auto& item : cart) {
            totalCost += item.second;
        }
        std::cout << "Total: $" << std::fixed << std::setprecision(2) << totalCost << std::endl;
    }

}
void CoffeeMachine::handleCoffeeSelection(const std::string &coffee, double basePrice) {
    int typeChoice;
    std::cout << "Select coffee type: " << std::endl;
    std::cout << "1. Hot (+$0.00)" << std::endl;
    std::cout << "2. Iced (+$0.50)" << std::endl;
    std::cout << "Select an option: ";
    std::cin >> typeChoice;

    double typeCost = (typeChoice == 2) ? 0.50 : 0.00;
    std::string type = (typeChoice == 2) ? "Iced" : "Hot";

    int sizeChoice;
    std::cout << "Select cup size: " << std::endl;
    std::cout << "1. Small (+$0.00)" << std::endl;
    std::cout << "2. Medium (+$0.50)" << std::endl;
    std::cout << "3. Large (+$1.00)" << std::endl;
    std::cout << "Select an option: ";
    std::cin >> sizeChoice;

    double sizeCost = (sizeChoice == 2) ? 0.50 : (sizeChoice == 3) ? 1.00 : 0.00;

    int milkSugarChoice;
    std::cout << "Select milk and sugar option: " << std::endl;
    std::cout << "1. Sugar (+$0.20)" << std::endl;
    std::cout << "2. Milk (+$0.30)" << std::endl;
    std::cout << "3. Milk and Sugar (+$0.50)" << std::endl;
    std::cout << "4. None (+$0.00)" << std::endl;
    std::cout << "Select an option: ";
    std::cin >> milkSugarChoice;

    double milkSugarCost = (milkSugarChoice == 1) ? 0.20 :
                           (milkSugarChoice == 2) ? 0.30 :
                           (milkSugarChoice == 3) ? 0.50 : 0.00;
    std::string milkSugar = (milkSugarChoice == 1) ? "Sugar" :
                            (milkSugarChoice == 2) ? "Milk" :
                            (milkSugarChoice == 3) ? "Milk and Sugar" : "None";

    double totalCost = basePrice + typeCost + sizeCost + milkSugarCost;

    std::string description = coffee + " (" + type + ", ";
    description += (sizeChoice == 1) ? "Small" : (sizeChoice == 2) ? "Medium" : "Large";
    if (milkSugarChoice != 4) description += ", " + milkSugar;
    description += ")";

    addItemToCart(description, totalCost);
}

void CoffeeMachine::loadBreadStockFromFile(const std::string &filename, std::map<std::string, int> &breadStock) {
    std::ifstream file(filename);
    if (!file) {
    std::cerr << "Error: Unable to open stock file: " << filename << std::endl;
    return;
    }
    std::string line, bread;
    int stock;
    while (std::getline(file, line)) {
    std::istringstream ss(line);
    if (std::getline(ss, bread, ',') && ss >> stock) {
                   breadStock[bread] = stock;
          }
    }
   file.close();
 }

void CoffeeMachine::saveBreadStockToFile(const std::string &filename, const std::map<std::string, int> &breadStock) {
    std::ofstream file(filename);
    if (!file) {
        std::cerr << "Error: Unable to save stock to file: " << filename << std::endl;
        return;
    }

    for (const auto &item : breadStock) {
        file << item.first << "," << item.second << std::endl;
    }
    file.close();
}

void CoffeeMachine::handleBreadSelection() {
    std::map<std::string, int> breadStock;
    loadBreadStockFromFile("../src/bread_stock.txt", breadStock);

    int breadChoice;
    do {
        showBreadMenu();
        std::cout << "Select a bread: ";
        std::cin >> breadChoice;

        std::string selectedBread;
        double price = 0.0;

        switch (breadChoice) {
            case 1:
                selectedBread = "Whole Wheat";
            price = 2.00;
            std::cout << "Stock remaining for Whole Wheat is : " << breadStock[selectedBread];
            if (breadStock[selectedBread] <= 1) {
                std::cout << "pc" << std::endl;
            }if (breadStock[selectedBread] > 1) {
                std::cout << "pcs" << std::endl;
            }
                break;
            case 2:
                selectedBread = "Croissant";
                price = 2.50;
                std::cout << "Stock remaining for Whole Wheat is : " << breadStock[selectedBread];
            if (breadStock[selectedBread] <= 1) {
                std::cout << "pc" << std::endl;
            }if (breadStock[selectedBread] > 1) {
                std::cout << "pcs" << std::endl;
            }
                break;
            case 3:
                selectedBread = "Bagel";
                price = 1.50;
                std::cout << "Stock remaining for Whole Wheat is : " << breadStock[selectedBread];
            if (breadStock[selectedBread] <= 1) {
                std::cout << "pc" << std::endl;
            }if (breadStock[selectedBread] > 1) {
                std::cout << "pcs" << std::endl;
            }
                break;
            case 4:
                selectedBread = "Whole Wheat Bread";
                price = 2.00;
                std::cout << "Stock remaining for Whole Wheat is : " << breadStock[selectedBread];
            if (breadStock[selectedBread] <= 1) {
                std::cout << "pc" << std::endl;
            }if (breadStock[selectedBread] > 1) {
                std::cout << "pcs" << std::endl;
            }
                break;
            case 5:
                selectedBread = "Naan";
                price = 2.50;
                std::cout << "Stock remaining for Whole Wheat is : " << breadStock[selectedBread];
            if (breadStock[selectedBread] <= 1) {
                std::cout << "pc" << std::endl;
            }if (breadStock[selectedBread] > 1) {
                std::cout << "pcs" << std::endl;
            }
                break;
            case 6:
                selectedBread = "Pita";
                price = 1.50;
                std::cout << "Stock remaining for Whole Wheat is : " << breadStock[selectedBread];
            if (breadStock[selectedBread] <= 1) {
                std::cout << "pc" << std::endl;
            }if (breadStock[selectedBread] > 1) {
                std::cout << "pcs" << std::endl;
            }
                break;
            case 7:
                std::cout << "Returning to main menu..." << std::endl;
                return;
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
                continue;
        }

        int quantity;
        std::cout << "Enter quantity for " << selectedBread << ": ";
        std::cin >> quantity;

        if (quantity <= 0) {
            std::cout << "Invalid quantity. Please try again." << std::endl;
            continue;
        }

        if (breadStock[selectedBread] >= quantity) {
            int confirmation;
            std::cout << "Are you sure you want to add " << selectedBread << " x" << quantity
                      << " for $" << std::fixed << std::setprecision(2) << price * quantity << "?" << std::endl;
            std::cout << "1. Confirm" << std::endl;
            std::cout << "2. Back to Main Menu." << std::endl;
            std::cout << "Select an option: ";
            std::cin >> confirmation;

            if (confirmation == 1) {
                addItemToCart(selectedBread + " x" + std::to_string(quantity), price * quantity);
                breadStock[selectedBread] -= quantity; // Reduce stock only after confirmation
                std::cout << selectedBread << " added to your cart." << std::endl;
                std::cout << "Stock remaining for " << selectedBread << ": " << breadStock[selectedBread] << std::endl;

                // Save updated stock to file
                saveBreadStockToFile("../src/bread_stock.txt", breadStock);
            } else if (confirmation == 2) {
                std::cout << selectedBread << " x" << quantity << " was not added to your cart." << std::endl;
            } else {
                std::cout << "Invalid option. Returning to the previous menu." << std::endl;
            }
        } else {
            std::cout << "Sorry, not enough stock for " << selectedBread << ". Available: " << breadStock[selectedBread] << std::endl;
        }
    } while (breadChoice != 7);
}
void CoffeeMachine::checkout() {
    if (cart.empty()) {
        std::cout << "Your cart is empty. Add items before checking out." << std::endl;
        return;
    }

    double totalCost = 0.0;
    std::cout << "Cart Summary:" << std::endl;
    for (const auto&[fst, snd] : cart) {
        std::cout << "- " << fst << " - $" << std::fixed << std::setprecision(2) << snd << std::endl;
        totalCost += snd;
    }
    std::cout << "Total: $" << std::fixed << std::setprecision(2) << totalCost << std::endl;
    generatePaymentQR();
    cart.clear();
    std::cout << "Thank you for your purchase!" << std::endl;
}
void CoffeeMachine::run() {
    int choice;
    do {
        showMenu();
        std::cout << "Select an options: ";
        std::cin >> choice;

        switch (choice) {
            case 1: {
                int coffeeChoice;
                std::cout << "Coffee Menu:" << std::endl;
                std::cout << "1. Espresso - $2.50" << std::endl;
                std::cout << "2. Cappuccino - $3.00" << std::endl;
                std::cout << "3. Latte - $3.50" << std::endl;
                std::cout << "4. Americano - $4.00" << std::endl;
                std::cout << "5. Back to Main Menu" << std::endl;
                std::cout << "Select a coffee: ";
                std::cin >> coffeeChoice;

                std::string coffee;
                double basePrice;

                switch (coffeeChoice) {
                    case 1:
                        coffee = "Espresso";
                        basePrice = 2.50;
                        break;
                    case 2:
                        coffee = "Cappuccino";
                        basePrice = 3.00;
                        break;
                    case 3:
                        coffee = "Latte";
                        basePrice = 3.50;
                        break;
                    case 4:
                        coffee = "Americano";
                    basePrice = 4.00;
                    break;
                    case 5:
                        std::cout << "Returning to main menu..." << std::endl;
                    return run();
                    default:
                        std::cout << "Invalid coffee selection. Returning to main menu." << std::endl;
                        continue;
                }

                handleCoffeeSelection(coffee, basePrice);
                break;
            }
            case 2:
                handleBreadSelection();
                break;
            case 3:
                showCart();
                break;
            case 4:
                checkout();
                break;
            case 5:
                std::cout << "Goodbye!" << std::endl;
                break;
            default:
                std::cout << "Invalid option. Please try again." << std::endl;
        }
    } while (choice != 4);
}


