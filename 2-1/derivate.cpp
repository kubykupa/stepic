#include <iostream>
#include <fstream>
#include <map>
#include <functional>
#include <sstream>


template <typename T>
T fromString(const std::string& s) {
    std::istringstream iss(s);
    T res;
    iss >> res;
    return res;
}

std::string derivative(const std::string& polynomial) {
    struct token {long c, p;};
    auto next = [&polynomial](size_t& from) {
        size_t L = polynomial.length();
        if (from >= L) {
            return token{0, 0};
        }
        long c = 0;
        size_t i = from;
        bool hasX = false;
        for (; i < L; ++i) {
            const char s = polynomial[i];
            if (s == '+' || s == '-') {
                if (i - from) {
                    c = fromString<long>(polynomial.substr(from, i - from));
                    from = i;
                    return token{c, 0};
                }
            } else if (s == '*') {
                c = fromString<long>(polynomial.substr(from, i - from));
                i += 2; // skip *x
                hasX = true;
                break;
            } else if (s == 'x') {
                c = (i - from == 1 && polynomial[i-1] == '-') ? -1 : 1;
                ++i;    // skip x
                hasX = true;
                break;
            }
        }
        if (!hasX) {
            c = fromString<long>(polynomial.substr(from));
            from = L;
            return token{c, 0};
        }
        if (i < L && polynomial[i] == '^')
            ++i;
        from = i;
        while (i < L && polynomial[i] != '-' && polynomial[i] != '+') {
            ++i;
        }
        long p = (from == i) ? 1 : fromString<long>(polynomial.substr(from, i - from));
        from = i;
        return token{c, p};
    };

    // make power -> coef map
    std::map<long, long, std::greater<long>> p2c;
    size_t pos = 0;
    do {
        token t = next(pos);
        //cout << t.c << ", " << t.p << endl;
        if (t.p == 0)
            continue;

        t = token{t.c * t.p, t.p - 1};
        auto i = p2c.find(t.p);
        if (i != p2c.end()) {
            i->second += t.c;     
        } else {
            p2c[t.p] = t.c;
        }
    } while (pos < polynomial.length());

    // formating result
    if (p2c.empty()) {
        return "0";
    }

    std::ostringstream ss;
    for (auto i = p2c.begin(); i != p2c.end(); i++) {
        if (i->first == 0) {
            // 0 power
            if (i != p2c.begin())
                ss << "+";
            ss << i->second;
        } else {
            if (i->second < 0) {
                if (i->second == -1)
                    ss << "-x";
            } else if (i != p2c.begin()) {
                ss << "+";
            }

            if (i->second == 1) {
                ss << "x";
            } else {
                ss << i->second << "*x";
            }

            // !0 power
            if (i->first != 1) {
               ss << "^"<< i->first;
            }
        }
    }
    return ss.str();
}


int main() {
    using namespace std;

    const char* input = "input.txt";

    ifstream infile(input, ios::in); 
    if (!infile.is_open()) {
        cout << "file not opened: " << input << endl;
        return -1;
    }

    string polinom, answer;
    while (getline(infile, polinom) && getline(infile, answer)) {
        if (polinom.length() && polinom[0] == '#')
            continue;
        string res = derivative(polinom);
        cout << polinom << " -> " << answer << " ? " << res << " " <<   (res == answer ? "[ok]" : "[fail]") << endl;
    }
    infile.close();
    return 0;
}
