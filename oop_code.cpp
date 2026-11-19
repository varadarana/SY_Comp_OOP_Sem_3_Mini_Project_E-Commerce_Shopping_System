
#include <iostream>
#include <string>
#include <fstream>
#include <stdexcept>
using namespace std;

// Base Class
class Product{
private:
    string name;
    int price;
public:
    Product(string n, int p){
        name = n;
        price = p;
    }
    string getName() const{
        return name;
    }
    int getPrice() const{
        return price;
    }
    virtual void display(ostream &out) const = 0;
    virtual ~Product() {}

    friend ostream& operator<<(ostream &out, const Product &p){
        p.display(out);
        return out;
    }
};

// Derived Class 1
class Clothes : public Product{
public:
    Clothes(string n, int p) : Product(n, p) {}
    void display(ostream &out) const override{
        out << getName() << " - Rs. " << getPrice()
            << " | Sizes: S, M, L";
    }
};

// Derived Class 2
class Electronics : public Product{
public:
    Electronics(string n, int p) : Product(n, p) {}
    void display(ostream &out) const override{
        out << getName() << " - Rs. " << getPrice();
    }
};

// Cart Structure
struct CartItem{
    Product *product;
    int quantity;
    string size;
};

// Number Validation
int getInteger(){
    string input;
    while (true){
        getline(cin, input);
        try{
            if (input.empty())
                throw invalid_argument("Empty input");
            int number = 0;

            for (int i = 0; i < input.length(); i++) {
                if (input[i] < '0' || input[i] > '9')
                    throw invalid_argument("Invalid input");
                int digit = input[i] - '0';
                if (number > (2147483647 - digit) / 10)
                    throw invalid_argument("Number too large");
                number = number * 10 + digit;
            }
            return number;
        }
        catch (invalid_argument &e){
            cout << "Enter a valid number: ";
        }
    }
}

// Yes/No Input
int getYesNo()
{
    int choice;

    while (true)
    {
        choice = getInteger();

        if (choice == 0 || choice == 1)
            return choice;

        cout << "Enter 1 for Yes or 0 for No: ";
    }
}

// Customer Details
void getCustomerDetails(string &name, string &mobile, string &address){
    do{
        cout << "Enter Customer Name: ";
        getline(cin, name);
    } while (name.empty());

    while (true) {
        cout << "Enter 10-digit Mobile Number: ";
        getline(cin, mobile);

        bool valid = mobile.length() == 10;

        for (int i = 0; i < mobile.length(); i++) {
            if (mobile[i] < '0' || mobile[i] > '9')
                valid = false;
        }

        if (valid)
            break;
        cout << "Invalid mobile number. Try again.\n";
    }

    do{
        cout << "Enter Address: ";
        getline(cin, address);
    } while (address.empty());
}

// Display Products
void viewProducts(Product *products[]){
    cout << "\n---------- CLOTHES ----------\n";

    for (int i = 0; i < 3; i++)
        cout << i + 1 << ". " << *products[i] << endl;

    cout << "\n-------- ELECTRONICS --------\n";

    for (int i = 3; i < 6; i++)
        cout << i + 1 << ". " << *products[i] << endl;
}

// Add Products to Cart
void addToCart(Product *products[], CartItem *&cart, int &count){
    int choice = 1;

    while (choice == 1) {
        viewProducts(products);

        cout << "\nEnter Product Number: ";
        int id = getInteger();

        if (id < 1 || id > 6) {
            cout << "Invalid product number.\n";
            continue;
        }

        string size = "";

        if (id >= 1 && id <= 3){
            cout << "Select Size (S/M/L): ";
            getline(cin, size);
        }

        cout << "Enter Quantity: ";
        int qty = getInteger();

        if (qty == 0) {
            cout << "Quantity must be greater than zero.\n";
            continue;
        }

        CartItem *temp = new CartItem[count + 1];

        for (int i = 0; i < count; i++)
            temp[i] = cart[i];

        temp[count].product = products[id - 1];
        temp[count].quantity = qty;
        temp[count].size = size;

        delete[] cart;
        cart = temp;
        count++;

        cout << "Product added successfully.\n";

        cout << "\nAdd another product? (1 = Yes, 0 = No): ";
        choice = getYesNo();
    }
}

// View Cart
void viewCart(CartItem *cart, int count){
    if (count == 0){
        cout << "\nYour cart is empty.\n";
        return;
    }

    int total = 0;

    cout << "\n----------- SHOPPING CART -----------\n";

    for (int i = 0; i < count; i++){
        int amount = cart[i].product->getPrice() * cart[i].quantity;

        cout << i + 1 << ". "
             << cart[i].product->getName();

        if (cart[i].size != "")
            cout << " | Size: " << cart[i].size;

        cout << " | Quantity: " << cart[i].quantity
             << " | Amount: Rs. " << amount << endl;

        total += amount;
    }
    cout << "Total Amount: Rs. " << total << endl;
}

// Remove Products from Cart
void removeFromCart(CartItem *&cart, int &count){
    if (count == 0){
        cout << "\nYour cart is empty.\n";
        return;
    }

    viewCart(cart, count);

    cout << "\nHow many products do you want to remove? ";
    int removeCount = getInteger();

    if (removeCount == 0 || removeCount > count) {
        cout << "Invalid number of products.\n";
        return;
    }

    for (int r = 0; r < removeCount; r++){
        viewCart(cart, count);

        cout << "\nEnter product number to remove: ";
        int id = getInteger();

        if (id < 1 || id > count) {
            cout << "Invalid product number. Try again.\n";
            r--;
            continue;
        }

        CartItem *temp = new CartItem[count - 1];
        int j = 0;

        for (int i = 0; i < count; i++){
            if (i != id - 1)
                temp[j++] = cart[i];
        }

        delete[] cart;
        cart = temp;
        count--;

        cout << "Product removed successfully.\n";
    }
}

// Generate Invoice
void generateInvoice(string name, string mobile, string address,CartItem *&cart, int &count){
    if (count == 0){
        cout << "\nYour cart is empty. Cannot generate invoice.\n";
        return;
    }

    fstream file("orders.txt", ios::out | ios::app);

    if (!file) {
        cout << "Unable to open invoice file.\n";
        return;
    }

    int total = 0;

    cout << "\n========== INVOICE ==========\n";
    file << "\n========== INVOICE ==========\n";

    cout << "Customer Name: " << name << endl;
    cout << "Mobile Number: " << mobile << endl;
    cout << "Address: " << address << endl;

    file << "Customer Name: " << name << endl;
    file << "Mobile Number: " << mobile << endl;
    file << "Address: " << address << endl;

    cout << "\nProduct Details:\n";
    file << "\nProduct Details:\n";

    for (int i = 0; i < count; i++){
        int amount = cart[i].product->getPrice() * cart[i].quantity;

        cout << cart[i].product->getName();

        file << cart[i].product->getName();

        if (cart[i].size != "") {
            cout << " | Size: " << cart[i].size;
            file << " | Size: " << cart[i].size;
        }

        cout << " | Quantity: " << cart[i].quantity
             << " | Amount: Rs. " << amount << endl;

        file << " | Quantity: " << cart[i].quantity
             << " | Amount: Rs. " << amount << endl;

        total += amount;
    }

    cout << "\nTotal Amount: Rs. " << total << endl;
    file << "\nTotal Amount: Rs. " << total << endl;

    cout << "========== THANK YOU ==========\n";
    file << "========== THANK YOU ==========\n";

    file.close();

    delete[] cart;
    cart = NULL;
    count = 0;

    cout << "\nInvoice saved in orders.txt\n";
}

// Main Function
int main(){
    string name, mobile, address;

    cout << "===== E-COMMERCE SHOPPING SYSTEM =====\n";

    getCustomerDetails(name, mobile, address);

    Clothes c1("Henley Top", 599);
    Clothes c2("Ethnic Red Bandhani Anarkali Set", 1499);
    Clothes c3("White Formal Shirt", 799);

    Electronics e1("iPhone", 69999);
    Electronics e2("Earbuds", 1499);
    Electronics e3("Laptop", 54999);

    Product *products[6] = {&c1, &c2, &c3, &e1, &e2, &e3};

    CartItem *cart = NULL;
    int count = 0;
    int choice;

    do  {
        cout << "\n========== MAIN MENU ==========\n";
        cout << "1. View Products\n";
        cout << "2. Add Products to Cart\n";
        cout << "3. View Shopping Cart\n";
        cout << "4. Remove Products from Cart\n";
        cout << "5. Generate Invoice & Checkout\n";
        cout << "6. Exit\n";
        cout << "Enter your choice: ";

        choice = getInteger();

        switch (choice) {
            case 1:
                viewProducts(products);
                break;

            case 2:
                addToCart(products, cart, count);
                break;

            case 3:
                viewCart(cart, count);
                break;

            case 4:
                removeFromCart(cart, count);
                break;

            case 5:
                generateInvoice(name, mobile, address, cart, count);
                break;

            case 6:
                cout << "\nThank you for visiting!\n";
                break;

            default:
                cout << "Invalid choice. Try again.\n";
        }

    } while (choice != 6);

    delete[] cart;

    return 0;
}