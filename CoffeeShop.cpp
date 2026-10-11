/*
 * ============================================================
 *  COFFEE SHOP MANAGEMENT SYSTEM  (C++ | OOP + DSA)
 * ============================================================
 *  OOP used:
 *   - Abstraction     : MenuItem, User (abstract base classes)
 *   - Inheritance     : Beverage, Food  -> MenuItem
 *                       Admin, Cashier  -> User
 *   - Polymorphism    : getPrice(), getCategory(), role()
 *   - Encapsulation   : private/protected data + getters
 *   - Templates       : Queue<T>, Stack<T>
 *
 *  DSA used:
 *   - BST (Binary Search Tree)  : menu storage, search by ID  O(log n)
 *   - Linked List               : shopping cart
 *   - Queue (linked list)       : pending orders, FIFO (kitchen)
 *   - Stack (linked list)       : undo cart action + order history (LIFO)
 *   - Hash Table (chaining)     : customer loyalty by phone number
 *   - Merge Sort                : sort menu by name / price / stock
 *
 *  Compile : g++ -std=c++11 CoffeeShop.cpp -o coffeeshop
 *  Run     : ./coffeeshop
 *  Logins  : admin / admin123      cashier / cash123
 * ============================================================
 */
#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <sstream>
#include <iomanip>
#include <ctime>
using namespace std;

/* ---------------------- Input helpers ---------------------- */
static string readLine(const string& p) {
    cout << p;
    string s;
    getline(cin, s);
    if (!cin) { cout << "\nInput closed. Bye!\n"; exit(0); }
    return s;
}
static int readInt(const string& p) {
    while (true) {
        string s = readLine(p);
        try {
            size_t pos;
            int v = stoi(s, &pos);
            if (pos == s.size()) return v;
        } catch (...) {}
        cout << "  Invalid number, try again.\n";
    }
}
static double readDouble(const string& p) {
    while (true) {
        string s = readLine(p);
        try {
            size_t pos;
            double v = stod(s, &pos);
            if (pos == s.size() && v >= 0) return v;
        } catch (...) {}
        cout << "  Invalid amount, try again.\n";
    }
}
static string nowStr() {
    time_t t = time(nullptr);
    char buf[32];
    strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M", localtime(&t));
    return buf;
}

/* ====================== OOP: MENU ITEMS ====================== */
class MenuItem {                       // abstract class
protected:
    int id;
    string name;
    double basePrice;
    int stock;
public:
    MenuItem(int i, const string& n, double p, int s)
        : id(i), name(n), basePrice(p), stock(s) {}
    virtual ~MenuItem() {}

    virtual double getPrice(char size = 'M') const { return basePrice; }
    virtual string getCategory() const = 0;
    virtual bool hasSizes() const { return false; }
    virtual char typeCode() const = 0;

    int getId() const { return id; }
    string getName() const { return name; }
    double getBasePrice() const { return basePrice; }
    int getStock() const { return stock; }
    void setPrice(double p) { basePrice = p; }
    void addStock(int q) { stock += q; }
    void setStock(int q) { stock = q; }
    void reduceStock(int q) { stock -= q; }
};

class Beverage : public MenuItem {     // sizes: S / M / L
public:
    Beverage(int i, const string& n, double p, int s) : MenuItem(i, n, p, s) {}
    double getPrice(char size = 'M') const override {
        if (size == 'S') return basePrice * 0.8;
        if (size == 'L') return basePrice * 1.25;
        return basePrice;
    }
    string getCategory() const override { return "Beverage"; }
    bool hasSizes() const override { return true; }
    char typeCode() const override { return 'B'; }
};

class Food : public MenuItem {
public:
    Food(int i, const string& n, double p, int s) : MenuItem(i, n, p, s) {}
    string getCategory() const override { return "Food"; }
    char typeCode() const override { return 'F'; }
};

/* ====================== OOP: USERS ====================== */
class User {
protected:
    string uname, pass;
public:
    User(const string& u, const string& p) : uname(u), pass(p) {}
    virtual ~User() {}
    bool check(const string& u, const string& p) const { return u == uname && p == pass; }
    string getName() const { return uname; }
    virtual string role() const = 0;
    virtual bool isAdmin() const { return false; }
};
class Admin : public User {
public:
    Admin(const string& u, const string& p) : User(u, p) {}
    string role() const override { return "Admin"; }
    bool isAdmin() const override { return true; }
};
class Cashier : public User {
public:
    Cashier(const string& u, const string& p) : User(u, p) {}
    string role() const override { return "Cashier"; }
};

/* ====================== DSA: QUEUE & STACK ====================== */
template <class T>
class Queue {                          // FIFO, linked list
    struct Node { T d; Node* next; Node(const T& x) : d(x), next(nullptr) {} };
    Node *front, *rear;
    int n;
    Queue(const Queue&);
    Queue& operator=(const Queue&);
public:
    Queue() : front(nullptr), rear(nullptr), n(0) {}
    ~Queue() { while (!empty()) pop(); }
    void push(const T& x) {
        Node* nd = new Node(x);
        if (rear) rear->next = nd; else front = nd;
        rear = nd; n++;
    }
    T pop() {
        Node* t = front; T v = t->d;
        front = front->next;
        if (!front) rear = nullptr;
        delete t; n--;
        return v;
    }
    bool empty() const { return n == 0; }
    int size() const { return n; }
    vector<T> toVector() const {
        vector<T> v;
        for (Node* c = front; c; c = c->next) v.push_back(c->d);
        return v;
    }
};

template <class T>
class Stack {                          // LIFO, linked list
    struct Node { T d; Node* next; Node(const T& x, Node* nx) : d(x), next(nx) {} };
    Node* topN;
    int n;
    Stack(const Stack&);
    Stack& operator=(const Stack&);
public:
    Stack() : topN(nullptr), n(0) {}
    ~Stack() { while (!empty()) pop(); }
    void push(const T& x) { topN = new Node(x, topN); n++; }
    T pop() {
        Node* t = topN; T v = t->d;
        topN = topN->next; delete t; n--;
        return v;
    }
    bool empty() const { return n == 0; }
    int size() const { return n; }
    vector<T> toVector() const {       // top first
        vector<T> v;
        for (Node* c = topN; c; c = c->next) v.push_back(c->d);
        return v;
    }
};

/* ====================== DSA: BST (MENU) ====================== */
struct BSTNode {
    MenuItem* item;
    BSTNode *l, *r;
    BSTNode(MenuItem* i) : item(i), l(nullptr), r(nullptr) {}
};

class MenuBST {
    BSTNode* root;

    BSTNode* insert(BSTNode* n, MenuItem* it, bool& ok) {
        if (!n) { ok = true; return new BSTNode(it); }
        if (it->getId() < n->item->getId()) n->l = insert(n->l, it, ok);
        else if (it->getId() > n->item->getId()) n->r = insert(n->r, it, ok);
        return n;                                    // duplicate id -> ignored
    }
    BSTNode* removeNode(BSTNode* n, int id, bool& ok) {
        if (!n) return nullptr;
        if (id < n->item->getId()) n->l = removeNode(n->l, id, ok);
        else if (id > n->item->getId()) n->r = removeNode(n->r, id, ok);
        else {
            ok = true;
            if (!n->l) { BSTNode* r = n->r; delete n->item; delete n; return r; }
            if (!n->r) { BSTNode* l = n->l; delete n->item; delete n; return l; }
            BSTNode* m = n->r;                       // inorder successor
            while (m->l) m = m->l;
            swap(n->item, m->item);
            n->r = removeNode(n->r, id, ok);
        }
        return n;
    }
    void inorder(BSTNode* n, vector<MenuItem*>& v) const {
        if (!n) return;
        inorder(n->l, v); v.push_back(n->item); inorder(n->r, v);
    }
    void destroy(BSTNode* n) {
        if (!n) return;
        destroy(n->l); destroy(n->r);
        delete n->item; delete n;
    }
public:
    MenuBST() : root(nullptr) {}
    ~MenuBST() { destroy(root); }
    bool insert(MenuItem* it) { bool ok = false; root = insert(root, it, ok); return ok; }
    bool remove(int id) { bool ok = false; root = removeNode(root, id, ok); return ok; }
    MenuItem* find(int id) const {
        BSTNode* c = root;
        while (c) {
            if (id == c->item->getId()) return c->item;
            c = (id < c->item->getId()) ? c->l : c->r;
        }
        return nullptr;
    }
    vector<MenuItem*> toVector() const { vector<MenuItem*> v; inorder(root, v); return v; }
    int maxId() const {
        BSTNode* c = root; if (!c) return 0;
        while (c->r) c = c->r;
        return c->item->getId();
    }
};

/* ====================== DSA: MERGE SORT ====================== */
typedef bool (*Cmp)(MenuItem*, MenuItem*);
static bool byName(MenuItem* a, MenuItem* b) { return a->getName() < b->getName(); }
static bool byPriceAsc(MenuItem* a, MenuItem* b) { return a->getBasePrice() < b->getBasePrice(); }
static bool byPriceDesc(MenuItem* a, MenuItem* b) { return a->getBasePrice() > b->getBasePrice(); }
static bool byStock(MenuItem* a, MenuItem* b) { return a->getStock() < b->getStock(); }

static void mergeSort(vector<MenuItem*>& v, int l, int r, Cmp c) {
    if (l >= r) return;
    int m = (l + r) / 2;
    mergeSort(v, l, m, c);
    mergeSort(v, m + 1, r, c);
    vector<MenuItem*> t;
    int i = l, j = m + 1;
    while (i <= m && j <= r) t.push_back(c(v[j], v[i]) ? v[j++] : v[i++]);
    while (i <= m) t.push_back(v[i++]);
    while (j <= r) t.push_back(v[j++]);
    for (size_t k = 0; k < t.size(); k++) v[l + k] = t[k];
}

/* ====================== DSA: HASH TABLE (CUSTOMERS) ====================== */
struct CustNode {
    string phone, name;
    int points;
    CustNode* next;
};
class CustomerTable {
    static const int SIZE = 53;
    CustNode* table[SIZE];
    int hashFn(const string& s) const {
        unsigned long h = 5381;
        for (size_t i = 0; i < s.size(); i++) h = h * 33 + (unsigned char)s[i];
        return (int)(h % SIZE);
    }
public:
    CustomerTable() { for (int i = 0; i < SIZE; i++) table[i] = nullptr; }
    ~CustomerTable() {
        for (int i = 0; i < SIZE; i++) {
            CustNode* c = table[i];
            while (c) { CustNode* t = c; c = c->next; delete t; }
        }
    }
    CustNode* find(const string& phone) const {
        for (CustNode* c = table[hashFn(phone)]; c; c = c->next)
            if (c->phone == phone) return c;
        return nullptr;
    }
    CustNode* add(const string& phone, const string& name, int pts) {
        int h = hashFn(phone);
        CustNode* n = new CustNode{phone, name, pts, table[h]};
        table[h] = n;
        return n;
    }
    vector<CustNode*> all() const {
        vector<CustNode*> v;
        for (int i = 0; i < SIZE; i++)
            for (CustNode* c = table[i]; c; c = c->next) v.push_back(c);
        return v;
    }
};

/* ====================== ORDER + CART (LINKED LIST) ====================== */
struct OrderLine { string name; char size; int qty; double unit; };
struct Order {
    int no;
    string customer, time;
    vector<OrderLine> lines;
    double subtotal, discount, total;
};
struct UndoAction { int id; char size; int qty; };

struct CartNode {
    MenuItem* item; char size; int qty; CartNode* next;
};
class Cart {
    CartNode* head;
public:
    Cart() : head(nullptr) {}
    ~Cart() { clear(); }
    void clear() {
        while (head) { CartNode* t = head; head = head->next; delete t; }
    }
    bool empty() const { return head == nullptr; }
    int qtyOf(int id) const {
        int q = 0;
        for (CartNode* c = head; c; c = c->next) if (c->item->getId() == id) q += c->qty;
        return q;
    }
    void add(MenuItem* it, char size, int qty) {
        CartNode* c = head; CartNode* last = nullptr;
        while (c) {
            if (c->item == it && c->size == size) { c->qty += qty; return; }
            last = c; c = c->next;
        }
        CartNode* n = new CartNode{it, size, qty, nullptr};
        if (last) last->next = n; else head = n;
    }
    void reduce(int id, char size, int qty) {          // used by UNDO
        CartNode *c = head, *prev = nullptr;
        while (c) {
            if (c->item->getId() == id && c->size == size) {
                c->qty -= qty;
                if (c->qty <= 0) {
                    if (prev) prev->next = c->next; else head = c->next;
                    delete c;
                }
                return;
            }
            prev = c; c = c->next;
        }
    }
    double subtotal() const {
        double s = 0;
        for (CartNode* c = head; c; c = c->next) s += c->item->getPrice(c->size) * c->qty;
        return s;
    }
    vector<OrderLine> lines() const {
        vector<OrderLine> v;
        for (CartNode* c = head; c; c = c->next)
            v.push_back({c->item->getName(), c->item->hasSizes() ? c->size : '-', c->qty,
                         c->item->getPrice(c->size)});
        return v;
    }
    void show() const {
        if (!head) { cout << "  (cart is empty)\n"; return; }
        cout << left << setw(22) << "  Item" << setw(6) << "Size" << setw(6) << "Qty"
             << setw(10) << "Price" << "Total\n";
        for (CartNode* c = head; c; c = c->next) {
            double u = c->item->getPrice(c->size);
            cout << "  " << left << setw(20) << c->item->getName()
                 << setw(6) << (c->item->hasSizes() ? string(1, c->size) : "-")
                 << setw(6) << c->qty << setw(10) << fixed << setprecision(0) << u
                 << u * c->qty << "\n";
        }
        cout << "  Subtotal: Rs " << fixed << setprecision(0) << subtotal() << "\n";
    }
};

/* ====================== MAIN SYSTEM ====================== */
class CoffeeShop {
    MenuBST menu;
    CustomerTable customers;
    Queue<Order> pending;
    Stack<Order> served;
    vector<User*> users;
    User* current;
    int nextOrderNo;

    const string MENU_FILE = "menu.txt";
    const string CUST_FILE = "customers.txt";
    const string SALES_FILE = "sales.txt";

    /* ---------- file handling ---------- */
    void saveMenu() {
        ofstream f(MENU_FILE);
        vector<MenuItem*> v = menu.toVector();
        for (size_t i = 0; i < v.size(); i++)
            f << v[i]->typeCode() << '|' << v[i]->getId() << '|' << v[i]->getName() << '|'
              << v[i]->getBasePrice() << '|' << v[i]->getStock() << '\n';
    }
    void saveCustomers() {
        ofstream f(CUST_FILE);
        vector<CustNode*> v = customers.all();
        for (size_t i = 0; i < v.size(); i++)
            f << v[i]->phone << '|' << v[i]->name << '|' << v[i]->points << '\n';
    }
    void seedMenu() {
        menu.insert(new Beverage(101, "Espresso", 250, 50));
        menu.insert(new Beverage(102, "Cappuccino", 350, 50));
        menu.insert(new Beverage(103, "Latte", 380, 40));
        menu.insert(new Beverage(104, "Cold Coffee", 400, 40));
        menu.insert(new Beverage(105, "Mocha", 420, 30));
        menu.insert(new Food(201, "Chocolate Brownie", 300, 25));
        menu.insert(new Food(202, "Chicken Sandwich", 450, 20));
        menu.insert(new Food(203, "Blueberry Muffin", 280, 25));
        menu.insert(new Food(204, "Cheese Cake Slice", 480, 15));
        saveMenu();
    }
    void loadAll() {
        ifstream f(MENU_FILE);
        string line;
        bool any = false;
        while (getline(f, line)) {
            stringstream ss(line);
            string t, id, name, price, stock;
            getline(ss, t, '|'); getline(ss, id, '|'); getline(ss, name, '|');
            getline(ss, price, '|'); getline(ss, stock, '|');
            if (t.empty() || stock.empty()) continue;
            try {
                MenuItem* it = (t == "B")
                    ? (MenuItem*)new Beverage(stoi(id), name, stod(price), stoi(stock))
                    : (MenuItem*)new Food(stoi(id), name, stod(price), stoi(stock));
                if (!menu.insert(it)) delete it; else any = true;
            } catch (...) {}
        }
        if (!any) seedMenu();

        ifstream c(CUST_FILE);
        while (getline(c, line)) {
            stringstream ss(line);
            string ph, nm, pt;
            getline(ss, ph, '|'); getline(ss, nm, '|'); getline(ss, pt, '|');
            try { if (!ph.empty()) customers.add(ph, nm, stoi(pt)); } catch (...) {}
        }

        ifstream s(SALES_FILE);
        int count = 0;
        while (getline(s, line)) if (!line.empty()) count++;
        nextOrderNo = 1001 + count;
    }
    void logSale(const Order& o) {
        ofstream f(SALES_FILE, ios::app);
        f << o.no << '|' << o.customer << '|' << o.total << '|' << o.time << '\n';
    }

    /* ---------- display helpers ---------- */
    void printMenu(const vector<MenuItem*>& v) {
        cout << "\n" << left << setw(6) << "ID" << setw(22) << "Name" << setw(11) << "Category"
             << setw(10) << "Price" << "Stock\n";
        cout << string(55, '-') << "\n";
        for (size_t i = 0; i < v.size(); i++) {
            cout << left << setw(6) << v[i]->getId() << setw(22) << v[i]->getName()
                 << setw(11) << v[i]->getCategory() << setw(10) << fixed << setprecision(0)
                 << v[i]->getBasePrice() << v[i]->getStock() << "\n";
        }
        cout << "(Beverage price = Medium. Small -20%, Large +25%)\n";
    }
    void printReceipt(const Order& o, double paid) {
        cout << "\n==========================================\n"
             << "          COFFEE SHOP - RECEIPT\n"
             << "==========================================\n"
             << " Order #" << o.no << "   " << o.time << "\n"
             << " Customer: " << o.customer << "   Cashier: " << current->getName() << "\n"
             << "------------------------------------------\n";
        for (size_t i = 0; i < o.lines.size(); i++) {
            const OrderLine& l = o.lines[i];
            string nm = l.name + (l.size != '-' ? " (" + string(1, l.size) + ")" : "");
            cout << " " << left << setw(24) << nm << "x" << setw(3) << l.qty << right << setw(8)
                 << fixed << setprecision(0) << l.unit * l.qty << "\n";
        }
        cout << "------------------------------------------\n"
             << left << " Subtotal" << right << setw(32) << o.subtotal << "\n";
        if (o.discount > 0) cout << left << " Loyalty discount" << right << setw(25) << -o.discount << "\n";
        cout << left << " TOTAL" << right << setw(35) << o.total << "\n"
             << left << " Paid" << right << setw(36) << paid << "\n"
             << left << " Change" << right << setw(34) << paid - o.total << "\n"
             << "==========================================\n"
             << "        Thank you! Visit again :)\n\n";
    }

    /* ---------- features ---------- */
    void viewMenu() { printMenu(menu.toVector()); }

    void searchItem() {
        int id = readInt("Enter item ID: ");
        MenuItem* it = menu.find(id);
        if (!it) { cout << "  Item not found.\n"; return; }
        vector<MenuItem*> v(1, it);
        printMenu(v);
    }

    void sortMenu() {
        cout << "1) Name  2) Price low->high  3) Price high->low  4) Stock low->high\n";
        int c = readInt("Choice: ");
        Cmp fn = c == 1 ? byName : c == 2 ? byPriceAsc : c == 3 ? byPriceDesc : c == 4 ? byStock : nullptr;
        if (!fn) { cout << "  Invalid choice.\n"; return; }
        vector<MenuItem*> v = menu.toVector();
        if (!v.empty()) mergeSort(v, 0, (int)v.size() - 1, fn);
        printMenu(v);
    }

    void newOrder() {
        Cart cart;
        Stack<UndoAction> undo;
        while (true) {
            cout << "\n--- NEW ORDER ---\n1) Add item  2) Undo last add  3) View cart  4) Checkout  0) Cancel order\n";
            int c = readInt("Choice: ");
            if (c == 0) { cout << "  Order cancelled.\n"; return; }
            if (c == 1) {
                viewMenu();
                int id = readInt("Item ID: ");
                MenuItem* it = menu.find(id);
                if (!it) { cout << "  Item not found.\n"; continue; }
                char size = 'M';
                if (it->hasSizes()) {
                    string s = readLine("Size (S/M/L): ");
                    size = s.empty() ? 'M' : (char)toupper(s[0]);
                    if (size != 'S' && size != 'M' && size != 'L') { cout << "  Invalid size.\n"; continue; }
                }
                int q = readInt("Quantity: ");
                if (q <= 0) { cout << "  Quantity must be positive.\n"; continue; }
                if (cart.qtyOf(id) + q > it->getStock()) {
                    cout << "  Not enough stock! Available: " << it->getStock() - cart.qtyOf(id) << "\n";
                    continue;
                }
                cart.add(it, size, q);
                undo.push({id, size, q});
                cout << "  Added " << q << " x " << it->getName() << "\n";
            } else if (c == 2) {
                if (undo.empty()) { cout << "  Nothing to undo.\n"; continue; }
                UndoAction a = undo.pop();
                cart.reduce(a.id, a.size, a.qty);
                cout << "  Last addition undone.\n";
            } else if (c == 3) {
                cart.show();
            } else if (c == 4) {
                if (cart.empty()) { cout << "  Cart is empty.\n"; continue; }
                checkout(cart);
                return;
            } else cout << "  Invalid choice.\n";
        }
    }

    void checkout(Cart& cart) {
        cart.show();
        Order o;
        o.no = nextOrderNo;
        o.time = nowStr();
        o.lines = cart.lines();
        o.subtotal = cart.subtotal();
        o.discount = 0;
        o.customer = "Walk-in";

        CustNode* cust = nullptr;
        string phone = readLine("Customer phone (Enter to skip): ");
        if (!phone.empty()) {
            cust = customers.find(phone);
            if (!cust) {
                string nm = readLine("New customer! Name: ");
                cust = customers.add(phone, nm.empty() ? "Customer" : nm, 0);
            }
            o.customer = cust->name;
            cout << "  " << cust->name << " has " << cust->points << " loyalty points.\n";
            if (cust->points >= 50) {
                string r = readLine("Redeem 50 points for Rs 100 off? (y/n): ");
                if (!r.empty() && (r[0] == 'y' || r[0] == 'Y')) {
                    o.discount = o.subtotal < 100 ? o.subtotal : 100;
                    cust->points -= 50;
                }
            }
        }
        o.total = o.subtotal - o.discount;
        cout << "  Amount to pay: Rs " << fixed << setprecision(0) << o.total << "\n";
        double paid;
        while (true) {
            paid = readDouble("Cash received: Rs ");
            if (paid >= o.total) break;
            cout << "  Not enough cash.\n";
        }
        // deduct stock
        deductStock(cart);
        if (cust) cust->points += (int)(o.total / 100);
        printReceipt(o, paid);
        pending.push(o);
        logSale(o);
        nextOrderNo++;
        saveMenu();
        saveCustomers();
        cart.clear();
        cout << "  Order #" << o.no << " sent to the kitchen queue.\n";
    }
    void deductStock(Cart& cart) {
        vector<MenuItem*> all = menu.toVector();
        for (size_t i = 0; i < all.size(); i++) {
            int q = cart.qtyOf(all[i]->getId());
            if (q > 0) all[i]->reduceStock(q);
        }
    }

    void serveNext() {
        if (pending.empty()) { cout << "  No pending orders.\n"; return; }
        Order o = pending.pop();
        served.push(o);
        cout << "  Order #" << o.no << " (" << o.customer << ") served! Remaining in queue: "
             << pending.size() << "\n";
    }
    void viewPending() {
        vector<Order> v = pending.toVector();
        if (v.empty()) { cout << "  Queue is empty.\n"; return; }
        cout << "\nPending orders (front = next to serve):\n";
        for (size_t i = 0; i < v.size(); i++) {
            cout << "  " << i + 1 << ". #" << v[i].no << " - " << v[i].customer << " (";
            for (size_t j = 0; j < v[i].lines.size(); j++)
                cout << v[i].lines[j].qty << "x " << v[i].lines[j].name << (j + 1 < v[i].lines.size() ? ", " : "");
            cout << ") Rs " << fixed << setprecision(0) << v[i].total << "\n";
        }
    }
    void recentOrders() {
        vector<Order> v = served.toVector();
        if (v.empty()) { cout << "  No served orders this session.\n"; return; }
        cout << "\nServed orders (latest first):\n";
        for (size_t i = 0; i < v.size(); i++)
            cout << "  #" << v[i].no << "  " << v[i].customer << "  Rs " << fixed << setprecision(0)
                 << v[i].total << "  " << v[i].time << "\n";
    }
    void customerLookup() {
        string ph = readLine("Customer phone: ");
        CustNode* c = customers.find(ph);
        if (!c) { cout << "  Customer not found.\n"; return; }
        cout << "  Name: " << c->name << " | Points: " << c->points << "\n";
    }
    void salesReport() {
        ifstream f(SALES_FILE);
        string line;
        int cnt = 0; double sum = 0;
        while (getline(f, line)) {
            stringstream ss(line);
            string no, cu, tot;
            getline(ss, no, '|'); getline(ss, cu, '|'); getline(ss, tot, '|');
            try { sum += stod(tot); cnt++; } catch (...) {}
        }
        cout << "\n===== SALES REPORT =====\n"
             << " Total orders (all time) : " << cnt << "\n"
             << " Total revenue           : Rs " << fixed << setprecision(0) << sum << "\n"
             << " Pending in queue        : " << pending.size() << "\n"
             << " Served this session     : " << served.size() << "\n";
        vector<MenuItem*> v = menu.toVector();
        bool header = false;
        for (size_t i = 0; i < v.size(); i++)
            if (v[i]->getStock() <= 10) {
                if (!header) { cout << " LOW STOCK ALERT:\n"; header = true; }
                cout << "   - " << v[i]->getName() << " (" << v[i]->getStock() << " left)\n";
            }
    }

    /* ---------- admin only ---------- */
    void addItem() {
        string t = readLine("Type (B = Beverage, F = Food): ");
        if (t.empty() || (toupper(t[0]) != 'B' && toupper(t[0]) != 'F')) { cout << "  Invalid type.\n"; return; }
        string nm = readLine("Name: ");
        if (nm.empty() || nm.find('|') != string::npos) { cout << "  Invalid name.\n"; return; }
        double p = readDouble("Base price (Rs): ");
        int s = readInt("Stock: ");
        int id = menu.maxId() + 1;
        if (id < 100) id = 101;
        MenuItem* it = toupper(t[0]) == 'B' ? (MenuItem*)new Beverage(id, nm, p, s)
                                            : (MenuItem*)new Food(id, nm, p, s);
        menu.insert(it);
        saveMenu();
        cout << "  Added with ID " << id << ".\n";
    }
    void updateItem() {
        int id = readInt("Item ID: ");
        MenuItem* it = menu.find(id);
        if (!it) { cout << "  Item not found.\n"; return; }
        cout << "1) Change price  2) Add stock  3) Set stock\n";
        int c = readInt("Choice: ");
        if (c == 1) it->setPrice(readDouble("New price: "));
        else if (c == 2) it->addStock(readInt("Add quantity: "));
        else if (c == 3) it->setStock(readInt("New stock: "));
        else { cout << "  Invalid choice.\n"; return; }
        saveMenu();
        cout << "  Updated.\n";
    }
    void removeItem() {
        int id = readInt("Item ID to remove: ");
        if (menu.remove(id)) { saveMenu(); cout << "  Item removed.\n"; }
        else cout << "  Item not found.\n";
    }

    /* ---------- login + menu loop ---------- */
    bool login() {
        for (int tries = 0; tries < 3; tries++) {
            string u = readLine("Username: ");
            string p = readLine("Password: ");
            for (size_t i = 0; i < users.size(); i++)
                if (users[i]->check(u, p)) { current = users[i]; return true; }
            cout << "  Wrong credentials.\n";
        }
        return false;
    }
public:
    CoffeeShop() : current(nullptr), nextOrderNo(1001) {
        users.push_back(new Admin("admin", "admin123"));
        users.push_back(new Cashier("cashier", "cash123"));
        loadAll();
    }
    ~CoffeeShop() { for (size_t i = 0; i < users.size(); i++) delete users[i]; }

    void run() {
        cout << "\n*****************************************\n"
             << "   COFFEE SHOP MANAGEMENT SYSTEM\n"
             << "*****************************************\n";
        while (true) {
            if (!login()) { cout << "Too many failed attempts. Exiting.\n"; return; }
            cout << "\nWelcome, " << current->getName() << " (" << current->role() << ")\n";
            bool stay = true;
            while (stay) {
                cout << "\n========== MAIN MENU ==========\n"
                     << " 1) View menu          2) Search item by ID\n"
                     << " 3) Sort menu          4) New order\n"
                     << " 5) Serve next order   6) View pending queue\n"
                     << " 7) Served orders      8) Customer loyalty lookup\n"
                     << " 9) Sales report\n";
                if (current->isAdmin())
                    cout << "10) Add menu item     11) Update price/stock\n"
                         << "12) Remove menu item\n";
                cout << " 0) Logout\n";
                int c = readInt("Choice: ");
                switch (c) {
                    case 1: viewMenu(); break;
                    case 2: searchItem(); break;
                    case 3: sortMenu(); break;
                    case 4: newOrder(); break;
                    case 5: serveNext(); break;
                    case 6: viewPending(); break;
                    case 7: recentOrders(); break;
                    case 8: customerLookup(); break;
                    case 9: salesReport(); break;
                    case 10: if (current->isAdmin()) addItem(); else cout << "  Admin only!\n"; break;
                    case 11: if (current->isAdmin()) updateItem(); else cout << "  Admin only!\n"; break;
                    case 12: if (current->isAdmin()) removeItem(); else cout << "  Admin only!\n"; break;
                    case 0: stay = false; break;
                    default: cout << "  Invalid choice.\n";
                }
            }
            string again = readLine("Exit program? (y/n): ");
            if (!again.empty() && (again[0] == 'y' || again[0] == 'Y')) break;
        }
        saveMenu();
        saveCustomers();
        cout << "Goodbye!\n";
    }
};

int main() {
    CoffeeShop shop;
    shop.run();
    return 0;
}
