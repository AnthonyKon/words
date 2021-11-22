/**
 * Computer Programming 2 (COMP2711, COMP8801)
 * Practical 4: Words
 */

#include <iostream>
#include <string>
#include <unistd.h>
#include <algorithm>

using namespace std;

template <class T> class MyVector {
public:
    typedef T* iterator;
    MyVector(){};

    iterator begin(){
        return &items[0];
    };

    iterator end(){
        return &items[used+1];
    };

    int size(){
        return used;
    };

    iterator insert (iterator position, const T& item){
        int p = distance(begin(), position);
        allocSize();
        for (int i=size(); i>p; i--) {
            items[i] = items[i-1];
        }
        used++;
        items[p] = item;
        return &items[p];
    };

    //-----------------------------------------------------------------
    void inspect(string word) {
        allocSize();

        bool wordNeg = true;
        if(size() > 0) {
            for (int i = 0; i < size(); i++) {
                if (items[i].text == word){
                    wordNeg = false;
                    items[i].count++;
                }
            }

            if (wordNeg) {
                items[size()].count = 1;
                items[size()].text = word;
                used++;
            }
        }

       else if (size() == 0) {
           items[size()].count = 1;
           items[size()].text = word;
           used++;
       }
    }

    //---------------------------------------------------
    void allocSize() {
        int p = distance(begin(), end());
        if (p >= checkpoint){
            T *temp = items;
            checkpoint *= 2;
            items = new T[checkpoint];

            for (int i = 0; i < size()+1; i++) {
                items[i] = temp[i];
            }
            delete[] temp;
        }
    }

    //----------------------------------------------------
    void sort(){
        for (int x = 0; x < size(); x++) {
            for (int y = size() - 1; y > 0; y--) {

                string tempString;
                int count = 0;

                if (items[y-1].text > items[y].text) {
                    count = items[y].count;
                    tempString = items[y].text;
                    items[y] = items[y - 1];

                    items[y-1].count = count;
                    items[y-1].text = tempString;

                }
            }
        }
    }

    void output() {
        for (int i = 0; i < size(); i++) {
            cout << items[i].text <<": "<< items[i].count << endl;
        }
    }

private:
    int checkpoint = 5;
    T* items = new T[checkpoint];
    int used = 0;
};
//----------------------------------------------------------------------
struct WordInfo{
    string text;
    int count;
};
//-----------------------------------------------------------------------

int main(int argc, char** argv)
{
    enum {
        total,
        unique,
        individual
    } mode = total;

    for (int c; (c = getopt(argc, argv, "tui")) != -1;) {
        switch (c) {
        case 'i':
            mode = individual;
            break;

        case 't':
            mode = total;
            break;

        case 'u':
            mode = unique;
            break;
        }
    }
    argc -= optind;
    argv += optind;

    MyVector<string> words;
    MyVector<WordInfo> wordInf;
    string word;

    int count = 0, uniqueItems = 0;
    while (cin >> word) {
        switch (mode) {
            case unique:
                if (find(words.begin(), words.end(), word) == words.end()) {
                    words.insert(words.end(), word);
                    uniqueItems++;
                }
                break;

            case individual:
                wordInf.inspect(word);
                break;
        }
        count = count+1;
    }

    switch (mode) {
    case individual:
        wordInf.sort();
        wordInf.output();
        break;
    case total:
        cout << "Total: " << count << '\n';
        break;
    case unique:
        cout << "Unique: " << uniqueItems << '\n';
        break;

    }

    return 0;
}
