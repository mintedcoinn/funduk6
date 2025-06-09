#include <iostream>
#include <fstream>
#include <string>
#include <algorithm>

using namespace std;


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
    int pos = -1;            // Позиция в исходном файле

    void print() const {
        cout << "Номер телефона: " << phone << endl;
        cout << "ФИО: " << name << endl;
        cout << "Адрес: " << adres << endl;
        cout << "Номер заявки: " << number << endl;
        (pos + 1 > 0) ? cout << "Позиция в исходном файле: " << pos + 1 << endl : cout << "Строка вставлена вручную" << endl;
        cout << "-----------------------------" << endl;
    }
    friend ostream& operator<<(ostream& os, const Data& d) {
        os << d.phone<<" " << d.name << " " << d.adres << " " << d.number;
        return os;
    }

    friend istream& operator>>(istream& is, Data& d) {
        is >> d.phone;
        is >> d.name;
        is >> d.adres;
        is >> d.number;
        return is;
    }
};


class key_ {
public:
    string str_key;
    int int_key;

    key_() : str_key(""), int_key(0) {}
    key_(const string& s, int i) : str_key(s), int_key(i) {}

    bool operator==(const key_ other) const{
        return this->str_key == other.str_key && this->int_key == other.int_key;
    }
};


class HashTable {

    struct Entry {
        key_ key;
        Data data;
        int status; // 0 - свободно, 1 - занято
    };

    Entry* table;
    int size;

    // Хеш-функция (середина квадрата)
    int hash(const key_& key) const {
        unsigned long long h = 0;
        for (char c : key.str_key) h = h * 31 + c;
        //cout << h << endl;
        h += key.int_key;
        //cout << h << endl;
        h *= h;
        //cout << h << endl;
        string s = to_string(h);
        int mid = s.length() / 2;
        //cout << mid << endl;
        string part = s.substr(mid - 1, 3);
        //cout << part<<endl;
        return stoi(part) % size;
    }
    /*№	str_key	int_key	Формула	h	h²	Хеш (% 100)
1	"aaa"	27154	96321 + 27154 = 123475	123475	15246075625	7
2	"aab"	27153	96322 + 27153 = 123475	123475	15246075625	7
3	"abb"	27122	96353 + 27122 = 123475	123475	15246075625	7
*/

    // Линейная адресация
    int line_adresation(int hash, int attempt) const {
        return (hash + attempt) % size;
    }

public:
    HashTable(unsigned int sz) : size(sz) {

        table = new Entry[size];

        for (unsigned int i = 0; i < size; ++i) {
            table[i].status = 0;
        }
    }

    ~HashTable() {
        delete[] table;
    }

    bool insert(const Data& d) {
        key_ key(d.name.union_name(),d.number);
        int h = hash(key);

        for (int i = 0; i < size; ++i) {
            int idx = line_adresation(h, i);

            if (table[idx].status == 0) {
                table[idx] = { key, d, 1 };
                return true;
            }

            if (table[idx].key == key) {
                return false;
            }
        }
        return false; 
    }

    const Data* search(key_& key, int& steps) const {
        int h = hash(key);
        steps = 0;

        for (int i = 0; i < size; ++i) {
            steps++;
            int idx = line_adresation(h, i);

            if (table[idx].status == 0) return nullptr;
            if (table[idx].status == 1 && table[idx].key == key) {
                cout << "Найдено в ячейке: "<< idx << endl;
                return &table[idx].data;
            }
        }
        return nullptr;
    }

    bool remove(key_& key) {
        int h = hash(key);

        for (int i = 0; i < size; ++i) {
            int idx = line_adresation(h, i);

            if (table[idx].status == 0) return false;

            if (table[idx].status == 1 && table[idx].key == key) {
                table[idx].status = 0;

                for (int jdx =size - 1; jdx > idx; jdx--) {
                    //int jdx = line_adresation(h, j);
                    if (table[jdx].status == 1 && hash(table[jdx].key) == h) {
                        table[idx].key = table[jdx].key;
                        table[idx].data = table[jdx].data;
                        table[idx].status = 1;
                        table[jdx].status = 0;
                    }
                }
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

key_ input_from_keyboard() {
    Full_Name key;
    int int_key = 0;
    cin >> key;
    cin >> int_key;
    string str_key = key.union_name();
    
    key_ ready_key(str_key, int_key);
    
    return ready_key;
}

void menu() {
   
    cout << "1. Поиск по ключу" << endl;
    cout << "2. Вставить элемент" << endl;
    cout << "3. Удалить элемент" << endl;
    cout << "4. Считать из файла" << endl;
    cout << "5. Записать в файл" << endl;
    cout << "6. Вывести таблицу" << endl;
    cout << "7. Выход" << endl;
}

int main() {
    setlocale(LC_ALL, "ru");

    unsigned int size = 0;
    cout << "Введите размер хеш-таблицы: ";
    cin >> size;
    HashTable ht(size);
    bool loaded = false;

    int choice;
    do {
        menu();
        cout << "Выберите действие: ";
        cin >> choice;

        switch (choice) {
        case 1: { // Поиск по ключу
            cout << "Введите ключ поиска (ФИО и номер заявки):" << endl;
            key_ key = input_from_keyboard();
            int steps;
            const Data* found = ht.search(key, steps);
            if (found) {
                cout << "Найдено за " << steps << " шагов:\n";
                found->print();
            }
            else {
                cout << "Не найдено\n";
            }
            break;
        }
        case 2: { // Вставка элемента
            cout << "Введите данные для вставки (телефон, ФИО, адрес, номер заявки):" << endl;
            Data d;
            cin >> d;
            if (ht.insert(d)) {
                cout << "Вставлено успешно" << endl;
            }
            else {
                cout << "Не удалось вставить" << endl;
            }
            break;
        }
        case 3: { // Удаление элемента
            cout << "Введите ключ удаления (ФИО и номер заявки):" << endl;
            key_ key = input_from_keyboard();
            if (ht.remove(key)) {
                cout << "Удалено успешно" << endl;
                break;
            }
            else {
                cout << "Не найдено для удаления" << endl;
                break;
            }
            
        }
        case 4: { // Загрузка из файла
            if (!loaded) {
                ht.loadFromFile("input.txt");
                loaded = true;
                cout << "Данные загружены из файла" << endl;
            }
            else {
                cout << "Данные уже были загружены ранее" << endl;
            }
            break;
        }
        case 5: { // Сохранение в файл
            ht.saveToFile("output.txt");
            cout << "Данные сохранены в файл output.txt" << endl;
            break;
        }
        case 6: { // Вывод таблицы
            cout << "Содержимое хеш-таблицы:" << endl;
            ht.print();
            break;
        }
        case 7: { // Выход
            cout << "Завершение программы" << endl;
            break;
        }
        default: {
            cout << "Неверный выбор, попробуйте снова" << endl;
            break;
        }
        }
        cout << "_____________________________" << endl;
    } while (choice < 7);

    return 0;
}