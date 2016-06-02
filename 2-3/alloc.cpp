#include <iostream>
#include <cstring>

class SmallAllocator {
public:
    using sizet = unsigned int;
private:
    struct Header {
        union {
            sizet mValue;
            struct {
                unsigned mIsFree : 1;
                unsigned mSize : sizeof(sizet) * 8 - 1;
            };
        };
    };
private:
    enum {
        HEADER_SIZE = sizeof(Header),
        TOTAL_SIZE = 1024 * 1024
    };
    char mRaw[TOTAL_SIZE];

private:
    Header& getHeader(char* ptr, bool atBlockBegin = true) {
        return *(Header*) (ptr - HEADER_SIZE * (atBlockBegin ? 0 : 1));
    }

    void bind(char* ptr, sizet size) {
        Header& header = getHeader(ptr);
        sizet prevSize = header.mSize;
        
        header.mIsFree = false;
        header.mSize = size;

        sizet tailSize = prevSize - size;
        if (tailSize > 0) {
            Header& next = getHeader(ptr + size);
            next.mIsFree = true;
            next.mSize = tailSize;
        }
    }

public:
    SmallAllocator() {
        Header& header = getHeader(mRaw);
        header.mIsFree = true;
        header.mSize = TOTAL_SIZE;
    }

    void* Alloc(sizet size) {
        for (char* ptr = mRaw; ptr < mRaw + TOTAL_SIZE;) {
            Header& header = getHeader(ptr);
            if (header.mIsFree && size + HEADER_SIZE <= header.mSize) {
                bind(ptr, size + HEADER_SIZE);

                return ptr + HEADER_SIZE;
            }
            ptr += header.mSize;
        }
        return nullptr;
    }

    void Free(void *ptr) {
        Header& header = getHeader((char*) ptr, false);
        header.mIsFree = true;
    };

    void *ReAlloc(void *ptr, sizet size) {
        Header& prev = getHeader((char*) ptr, false);

        void* newPtr = Alloc(size);
        if (newPtr == nullptr) {
            return nullptr;
        }
        memcpy((char*) newPtr, (char*) ptr, std::min(size, (sizet) (prev.mSize - HEADER_SIZE)));
        prev.mIsFree = true;
        return newPtr;
    };

};


void test() {
    SmallAllocator A1;
    int * A1_P1 = (int *) A1.Alloc(sizeof(int));
    A1_P1 = (int *) A1.ReAlloc(A1_P1, 2 * sizeof(int));
    A1.Free(A1_P1);

    SmallAllocator A2;
    int * A2_P1 = (int *) A2.Alloc(10 * sizeof(int));
    for(unsigned int i = 0; i < 10; i++) A2_P1[i] = i;
    for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 1" << std::endl;

    int * A2_P2 = (int *) A2.Alloc(10 * sizeof(int));
    for(unsigned int i = 0; i < 10; i++) A2_P2[i] = -1;
    for(unsigned int i = 0; i < 10; i++) if(A2_P1[i] != i) std::cout << "ERROR 2" << std::endl;
    for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 3" << std::endl;

    A2_P1 = (int *) A2.ReAlloc(A2_P1, 20 * sizeof(int));
    for(unsigned int i = 10; i < 20; i++) A2_P1[i] = i;
    for(unsigned int i = 0; i < 20; i++) if(A2_P1[i] != i) std::cout << "ERROR 4" << std::endl;
    for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 5" << std::endl;

    A2_P1 = (int *) A2.ReAlloc(A2_P1, 5 * sizeof(int));
    for(unsigned int i = 0; i < 5; i++) if(A2_P1[i] != i) std::cout << "ERROR 6" << std::endl;
    for(unsigned int i = 0; i < 10; i++) if(A2_P2[i] != -1) std::cout << "ERROR 7" << std::endl;
    A2.Free(A2_P1);
    A2.Free(A2_P2);
}

int main() {
    test();
    return 0;
}
