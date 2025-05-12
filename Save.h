#pragma once
struct GameElements {
    int x;
    int y;
    int val;
    GameElements* next;

    GameElements(int x, int y, int val) : x(x), y(y), val(val), next(nullptr) {}
};

class Save {
private:
    string name;
    string TimeStamp;
    int score;
    GameElements* root;

    string intToStr(int num) {
        string res;
        if (num == 0) return "0";
        while (num > 0) {
            res = char('0' + (num % 10)) + res;
            num /= 10;
        }
        return res;
    }

    string pad2(int num) {
        string s = intToStr(num);
        return (s.length() < 2 ? "0" + s : s);
    }

    string getCurrentTimestamp() {
        time_t now = time(nullptr);
        tm t;
        localtime_s(&t, &now);


        return intToStr(1900 + t.tm_year) + "-" + pad2(1 + t.tm_mon) + "-" + pad2(t.tm_mday) +
            " " + pad2(t.tm_hour) + ":" + pad2(t.tm_min) + ":" + pad2(t.tm_sec);

    }

public:
    Save() {
        name = "";
        TimeStamp = "";
        root = nullptr;
        score = 0;
    }

    Save(string n, int s) {
        score = s;
        name = n;
        TimeStamp = getCurrentTimestamp();
        root = nullptr;
    }

    void addElement(int x, int y, int val) {
        GameElements* newNode = new GameElements(x, y, val);
        newNode->next = root;
        root = newNode;
    }
    void serialize(const string& filename) {
        ofstream out(filename.c_str());
        out << name << endl;
        out << score << endl;
        out << TimeStamp << endl;

        GameElements* current = root;
        while (current) {
            out << current->x << " " << current->y << " " << current->val << endl;
            current = current->next;
        }
        out.close();
    }

    void deserialize(const string& filename) {
        ifstream in(filename.c_str());
        if (!in) return;
        string px;
        getline(in, name);
        getline(in, TimeStamp);
        getline(in, px);
        score = stoi(px);
        root = nullptr;
        int x, y, val;
        while (in >> x >> y >> val) {
            grid[x][y] = val;
            addElement(x, y, val);
        }
        in.close();
    }
    string sanitize(const string& s) {
        string result;
        for (char c : s) {
            if ((c >= 'a' && c <= 'z') ||
                (c >= 'A' && c <= 'Z') ||
                (c >= '0' && c <= '9')) {
                result += c;
            }
        }
        return result;
    }

    string getSaveID() {
        // Combine name and sanitized timestamp
        return sanitize(name) + sanitize(TimeStamp);
    }

    void printSave() {
        cout << "Name: " << name << endl;
        cout << "Time: " << TimeStamp << endl;
        GameElements* current = root;
        while (current) {
            cout << "(" << current->x << ", " << current->y << ") = " << current->val << endl;
            current = current->next;
        }
    }
};