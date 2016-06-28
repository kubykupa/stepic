#include <iostream>
#include <string>
#include <vector>
#include <fstream>
#include <unistd.h>

using namespace std;

//static const string OUT_PATH = "/home/box/result.out";
static const string OUT_PATH = "./result.out";
vector<string> cmds;

string trim2(string str) {
    str.erase(str.begin(), find_if(str.begin(), str.end(),
                [](char& ch)->bool { return !isspace(ch); }));
    str.erase(find_if(str.rbegin(), str.rend(),
                [](char& ch)->bool { return !isspace(ch); }).base(), str.end());
    return str;
}

string trim(string str) {
    str.erase(0, str.find_first_not_of(' '));
    str.erase(str.find_last_not_of(' ')+1);
    return str;
}

void tokenize(const string& str, vector<string>& tokens, const string& delimiters = " ") {
    string::size_type lastPos = str.find_first_not_of(delimiters, 0);
    string::size_type pos     = str.find_first_of(delimiters, lastPos);

    while (string::npos != pos || string::npos != lastPos) {
        tokens.push_back(trim(str.substr(lastPos, pos - lastPos).c_str()));
        lastPos = str.find_first_not_of(delimiters, pos);
        pos = str.find_first_of(delimiters, lastPos);
    }
}


void run(const string& cmd) {
    vector<string> args;
    tokenize(cmd, args, " "); 

    if (args.size() > 2) {
        cerr << "Bad args count: " << cmd << endl;
    } else {
        if (args.size() == 1) {
            execlp(args[0].c_str(), args[0].c_str(), NULL);
        } else {
            cerr << args[0] << ": " << args[1] << endl;
            execlp(args[0].c_str(), args[0].c_str(), args[1].c_str(), NULL);
        }
    }
}

void run_pipes() {
    int prev_read_end = STDIN_FILENO;
    for (const auto& cmd : cmds) {
        int pfd[2]; // OMG! data writes to [1] and reads from [0] 
        pipe(pfd);

        pid_t c = fork();
        if (c == 0) {
            // CHILD
            //cerr << "child: " << getpid() << endl;

            close(STDOUT_FILENO);
            dup2(prev_read_end, STDIN_FILENO);
            dup2(pfd[1], STDOUT_FILENO);
            close(pfd[0]);
            close(pfd[1]);
            run(cmd);
        } else {
            //cerr << "new pipe: " << pfd[1] << " -> " << pfd[0] << endl;
            // PARENT 
            //close(pfd[0]);
            close(pfd[1]);

            //cerr << "parent " << getpid() << " makes child: " << c << endl;
            prev_read_end = pfd[0];
        }        
    }


    close(STDIN_FILENO);
    dup2(prev_read_end, STDIN_FILENO);
    string line;
    ofstream out(OUT_PATH, ios::out);

    cerr << endl;
    getline(std::cin, line);
    for (; line.length(); getline(std::cin, line)) {
        cerr << "FINAL: [" << line << "]" << endl;
        out << line << endl;
    }
    out.close();

    close(prev_read_end);    
}

int main(int argc, char* argv[]) {
    if (argc < 2) {
        cout << "Need cmd line" << endl;
        return 0;
    }
    string line(argv[1]);

    tokenize(line, cmds, "|");
    for (const auto& s : cmds) {
        cout << "[" << s << "]" << endl;
    }

    run_pipes(); 

    return 0;
}
