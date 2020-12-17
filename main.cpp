#include <iostream>
#include <cstring>
#include <fstream>
#include <cassert>
#include <unistd.h>

#include "lib/vn_lang_tool.hpp"
#include "new.h"
#include "old.h"

using namespace std;

const int N = 1005;
double new_duration = 0;
double old_duration = 0;


string gen_random(const int len) {
    string tmp_s;
    static const char alphanum[] =
            "abc";
    tmp_s.reserve(len);
    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];
    return tmp_s;
}

void random_test(){
    string a, b;
    for(int i = 0; i < int(1e6); i++){
        a = gen_random(100);
        b = gen_random(6);
        cout << a << endl << b << endl;
        Answer ans1 = NewLeven::execute(a, b);
        Answer ans2 = OldLeven::execute(a, b);
//        cout << ans1 << " " << ans2 << endl;
        assert(ans1==ans2);
    }
}

void full_test(){
    VnLangTool::init("", true);
    string a, b;

    ifstream fin("test_1.txt");
    vector<Answer> res1, res2;

    while (getline(fin, a)){
        getline(fin, b);
//        a = VnLangTool::lower_root(a);
//        b = VnLangTool::lower_root(b);

//        cout << a << endl << b << endl;
        b.erase(std::remove(b.begin(), b.end(), '^'), b.end());

        auto bs = VnLangTool::split_str(b, "|");
        for(auto s: bs) {
            clock_t start = clock();
            Answer tmp = OldLeven::execute(a, s);
            double s_old_duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

            //            if (tmp.score != NewLeven::execute(a, s).score){
//                Answer new_l = NewLeven::execute(a, s);
//                cout << "***\n";
//                cout << a << endl << s << endl;
//                cout << tmp.score << " " << new_l.score << endl;
//                cout << tmp.first_substring.first << " " << tmp.first_substring.second<< endl;
//                cout << new_l.first_substring.first << " " << new_l.first_substring.second<< endl;
//                exit(0);
//            }
            res1.push_back(tmp);
            old_duration += s_old_duration;
        }
    }
    fin.close();

    fin.open("test_1.txt");
    while (getline(fin, a)){
        getline(fin, b);
        b.erase(std::remove(b.begin(), b.end(), '^'), b.end());
        auto bs = VnLangTool::split_str(b, "|");
        for(auto s: bs) {
            clock_t start = clock();
            Answer tmp = NewLeven::execute(a, s);
            double s_new_duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;

            res2.push_back(tmp);
            new_duration += s_new_duration;

        }
    }

    assert(res1.size() == res2.size());
    for(int i = 0 ; i < (int)res1.size(); i++) {
//        cout << res1[i].score << " " << res2[i].score << endl;
        assert(res1[i] == res2[i]);
    }
    cout << "Percentage: " << new_duration / old_duration * 100 <<  "%" << endl;
    cout << new_duration / res2.size() << " " << old_duration / res1.size() << endl;


    cout << new_duration << " " << old_duration << endl;
}
int main() {
    full_test();
    return 0;
}
