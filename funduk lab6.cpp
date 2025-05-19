#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;

// Самописный вектор
template <typename T>
class vektor {
private:
    T* data;
    int capacity;
    int count;

public:
    vektor() : data(nullptr), capacity(0), count(0) {}

    ~vektor() {
        delete[] data;
    }

    void push_back(const T& value) {
        if (count >= capacity) {
            int new_capacity = max(1, capacity * 2);
            T* new_data = new T[new_capacity];
            for (int i = 0; i < count; i++) {
                new_data[i] = data[i];
            }
            delete[] data;
            data = new_data;
            capacity = new_capacity;
        }
        data[count++] = value;
    }

    T& operator[](int index) {
        return data[index];
    }

    const T& operator[](int index) const {
        return data[index];
    }

    int size() const {
        return count;
    }

    void resize(int new_size) {
        if (new_size > capacity) {
            T* new_data = new T[new_size];
            for (int i = 0; i < count; i++) {
                new_data[i] = data[i];
            }
            delete[] data;
            data = new_data;
            capacity = new_size;
        }
        count = new_size;
    }
};

class PhoneNumber {
public:
    int area_code;  
    int number;     

    bool operator<(const PhoneNumber& other) const {
        return area_code == other.area_code ? number < other.number : area_code < other.area_code;
    }

    bool operator>(const PhoneNumber& other) const {
        return !(*this < other) && !(*this == other);
    }

    bool operator==(const PhoneNumber& other) const {
        return area_code == other.area_code && number == other.number;
    }

    bool operator!=(const PhoneNumber& other) const {
        return area_code != other.area_code || number != other.number;
    }

    friend ostream& operator<<(ostream& os, const PhoneNumber& phone) {
        os << phone.area_code << " " << phone.number;
        return os;
    }

    friend istream& operator>>(istream& is, PhoneNumber& phone) {
        is >> phone.area_code >> phone.number;
        return is;
    }
};

class Full_Name {
public:
    string last_name;   
    string first_name;  
    string second_name; 

    string union_name() const {
        return last_name + first_name + second_name;
    }

    int size() const {
        return last_name.size() + first_name.size() + second_name.size();
    }

    bool operator>(const Full_Name& other) const {
        return union_name() > other.union_name();
    }

    bool operator<(const Full_Name& other) const {
        return union_name() < other.union_name();
    }

    bool operator==(const Full_Name& other) const {
        return union_name() == other.union_name();
    }

    bool operator!=(const Full_Name& other) const {
        return !(*this == other);
    }

    friend ostream& operator<<(ostream& os, const Full_Name& name) {
        os << name.last_name << " " << name.first_name << " " << name.second_name;
        return os;
    }

    friend istream& operator>>(istream& is, Full_Name& name) {
        is >> name.last_name >> name.first_name >> name.second_name;
        return is;
    }
};

class Adress {
public:
    string city;           
    string street;        
    int building_number;  
    int flat;            

    string union_adress() const {
        return city + street + to_string(building_number) + to_string(flat);
    }

    bool operator==(const Adress& other) const {
        return union_adress() == other.union_adress();
    }

    bool operator>(const Adress& other) const {
        return union_adress() > other.union_adress();
    }

    bool operator<(const Adress& other) const {
        return union_adress() < other.union_adress();
    }

    bool operator!=(const Adress& other) const {
        return !(*this == other);
    }

    friend ostream& operator<<(ostream& os, const Adress& addr) {
        os << addr.city << " " << addr.street << " " << addr.building_number << " " << addr.flat;
        return os;
    }

    friend istream& operator>>(istream& is, Adress& addr) {
        is >> addr.city >> addr.street >> addr.building_number >> addr.flat;
        return is;
    }
};

struct Data {
    PhoneNumber phone;  
    Full_Name name;     
    Adress adres;       
    int number;         
    int pos;            // Позиция в исходном файле

    void print() const {
        cout << "Номер телефона: " << phone << endl;
        cout << "ФИО: " << name << endl;
        cout << "Адрес: " << adres << endl;
        cout << "Номер заявки: " << number << endl;
        cout << "Позиция в исходном файле: " << pos << endl;
        cout << "-----------------------------" << endl;
    }
};


class HashTable {
    struct Entry {
        string str_key;
        int int_key;
        Data data;
        int status; // 0 - свободно, 1 - занято
    };

    Entry* table;
    int size;

    // Хеш-функция (середина квадрата)
    int hash(const string& str_key, const int& int_key) const {
        unsigned h = 0;
        for (char c : str_key) h = h * 31 + c;
        h += int_key;
        h *= h;
        string s = to_string(h);
        int mid = s.length() / 2;
        string part = s.substr(max(0, mid - 1), 3);

        return stoi(part) % size;
    }

    // Линейная адресация
    int line_adresation(int hash, int attempt) const {
        return (hash + attempt) % size;
    }

public:
    HashTable(int sz) : size(sz) {
        table = new Entry[size];
        for (int i = 0; i < size; ++i)
            table[i].status = 0;
    }

    ~HashTable() {
        delete[] table;
    }

    bool insert(const Data& d) {
        string key = d.name.union_name();
        int h = hash(key, d.number);

        for (int i = 0; i < size; ++i) {
            int idx = line_adresation(h, i);

            if (table[idx].status == 0) {
                table[idx] = { key,d.number, d, 1 };
                return true;
            }

            if (table[idx].str_key == key && table[idx].int_key == d.number) {
                return false;
            }
        }
        return false; 
    }

    const Data* search(const string& name, int req_num, int& steps) const {
        string key = name;
        int h = hash(key, req_num);
        steps = 0;

        for (int i = 0; i < size; ++i) {
            steps++;
            int idx = line_adresation(h, i);

            if (table[idx].status == 0) return nullptr;
            if (table[idx].status == 1 && table[idx].str_key == key && table[idx].int_key == req_num) {
                return &table[idx].data;
            }
        }
        return nullptr;
    }

    bool remove(const string& name, int req_num) {
        string key = name;
        int h = hash(key, req_num);

        for (int i = 0; i < size; ++i) {
            int idx = line_adresation(h, i);

            if (table[idx].status == 0) return false;
            if (table[idx].status == 1 && table[idx].str_key == key && table[idx].int_key == req_num) {
                table[idx].status = 0;
                return true;
            }
        }
        return false;
    }

    void saveToFile(const string& filename) const {
        ofstream out(filename);
        if (!out) {
            cerr << "Ошибка открытия файла для записи\n";
            return;
        }

        for (int i = 0; i < size; ++i) {
            if (table[i].status == 1) {
                const Data& d = table[i].data;
                out << d.phone << " "
                    << d.name << " "
                    << d.adres << " "
                    << d.number << " "
                    << d.pos << "\n";
            }
        }
    }

    void loadFromFile(const string& filename) {
        ifstream in(filename);
        if (!in) {
            cerr << "Ошибка открытия файла для чтения\n";
            return;
        }

        Data d;
        int pos = 0;
        while (in >> d.phone >> d.name >> d.adres >> d.number) {
            d.pos = pos++;
            insert(d);
        }
    }

    void print() const {
        for (int i = 0; i < size; ++i) {
            if (table[i].status == 1) {
                cout << "Ячейка " << i << ":\n";
                table[i].data.print();
            }
        }
    }
};

int main() {
    setlocale(LC_ALL, "ru");
    HashTable ht(100);

    ht.loadFromFile("input.txt");

    int steps;
    const Data* found = ht.search("DdbbdBdbbdAeccd", 42, steps);
    if (found) {
        cout << "Найдено за " << steps << " шагов:\n";
        found->print();
    }
    else {
        cout << "Не найдено\n";
    }

    ht.saveToFile("output.txt");

    return 0;
}