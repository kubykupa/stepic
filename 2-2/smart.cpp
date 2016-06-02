#include <iostream>
#include <string>

class StringPointer {
private:
    std::string* mPtr;
    bool isOwner;
public:
    StringPointer(std::string* strPtr = NULL)
        : mPtr(strPtr == NULL ? new std::string() : strPtr)
        , isOwner(strPtr == NULL)
    {
    }
    ~StringPointer() {
        if (isOwner) {
            delete mPtr;
        }
        mPtr = NULL;
    }
    operator std::string*() {
        return mPtr;
    }
    std::string* operator->() const {
        return mPtr;
    }
};


int main() {
    std::string s1 = "Hello, world!";

    StringPointer sp1(&s1);
    StringPointer sp2(NULL);

    std::cout << sp1->length() << std::endl;
    std::cout << *sp1 << std::endl;
    std::cout << sp2->length() << std::endl;
    std::cout << *sp2 << std::endl;
    return 0;
}
