#include <iostream>
#include <cstring>
#include <fstream>
#include <cassert>
#include <unistd.h>

#include "lib/vn_lang_tool.hpp"

using namespace std;

const int N = 1005;
int dp[N][N];
double new_duration = 0;
double old_duration = 0;

int max(const int &a, const int &b){
    return a > b ? a : b;
}

int min(const int &a, const int &b){
    return a < b ? a : b;
}

int find_substring(string str, string pattern) {

    // Step 0. Should not be empty string
    if( str.size() == 0 || pattern.size() == 0)
        return -1;

    // Step 1. Compute failure function
    int failure[pattern.size()];
    std::fill( failure, failure+pattern.size(), -1);

    for(int r=1, l=-1; r<pattern.size(); r++) {

        while( l != -1 && pattern[l+1] != pattern[r])
            l = failure[l];

        // assert( l == -1 || pattern[l+1] == pattern[r]);
        if( pattern[l+1] == pattern[r])
            failure[r] = ++l;
    }

    // Step 2. Search pattern
    int tail = -1;
    for(int i=0; i<str.size(); i++) {

        while( tail != -1 && str[i] != pattern[tail+1])
            tail = failure[tail];

        if( str[i] == pattern[tail+1])
            tail++;

        if( tail == pattern.size()-1)
            return i - tail;
    }

    return -1;
}

int old_leven(string sa, string sb){
    clock_t start;
    start = clock();

//    if (find_substring(sa, sb) == sb.length()) {
//        cout << sa<<endl <<sb<<endl;
//        double s_old_duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
//        old_duration += s_old_duration;
//        return sb.length();
//    }

    string a = " " + sa;
    string b = " " + sb;

    // traditional smith waterman for benchmark
    int ans = 0;
     for(int i = 1; i < int(a.size()); i++){
        for(int j = 1; j < int(b.size()); j++){
            if (dp[i][j-1] > dp[i-1][j]){
                dp[i][j] = dp[i][j - 1] - 1;
//                cout << i << " " << j << " " << i << " "<< j - 1 << endl;
            }
            else dp[i][j] = dp[i - 1][j] - 1;
            if (dp[i][j] < dp[i-1][j-1]-1) {
                dp[i][j] = dp[i - 1][j - 1] - 1;
//                cout << i << " " << j << " " << i-1 << " "<< j - 1 << endl;

            }
            if (a[i] == b[j] && dp[i][j] <dp[i-1][j-1] + 1){
                dp[i][j] = dp[i-1][j-1] + 1;
//                cout << i << " " << j << " " << i-1 << " "<< j - 1 << endl;

            }
            if (dp[i][j] < 0) dp[i][j] = 0;
            //            cout << i << " " << j << " " << dp[i][j] << endl;
            if (ans < dp[i][j]) ans=dp[i][j];
        }
    }
    for(int i = 0; i <= int(a.size()); i++) dp[i][0] = 0;
    for(int j = 0; j < int(b.size()); j++) dp[0][j] = 0;
    double s_old_duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    old_duration += s_old_duration;
    return ans;
}

int w[N];
bool in_b[int(1e6)];
char a[N];

int attempt(string sa, string sb){
    clock_t start;
    start = clock();

    if (find_substring(sa, sb) != -1) {
//        cout << sa<<endl <<sb<<endl;
        double s_new_duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
        new_duration += s_new_duration;
        return sb.length();
    }
    string b = " " + sb;
    string _a = sa;

    for(int i = 1; i < (int)b.size(); i++) {
        in_b[b[i]] = true;
    }
    int ans = 0;
    int cur_w = 0, m = 0;
    for(int i = 0; i < (int)_a.size(); i++){
        if (in_b[_a[i]]) {
            w[++m] = cur_w;
            a[m] = _a[i];
            cur_w = 0;
        }
        else {
            cur_w++;
        }
    }

    int dq[b.size()], end = -1, front = 0;

    for(int i = 1; i <= m; i++){
        end = -1;
        front = 0;
        for(int j = 1; j < int(b.size()); j++){
            if (dp[i][j - 1] - 1 > dp[i-1][j] - w[i] - 1) {
                dp[i][j] = dp[i][j - 1] - 1;
            }
            else {
                dp[i][j] = dp[i - 1][j] - w[i] - 1;
            }

            while (front <= end && dp[i-1][dq[end]] < dp[i-1][j-1]) end--;
            dq[++end] = j - 1;
            while (front <= end && dq[front] < j - 1 - w[i]) front++;

            int new_v = max(0, dp[i - 1][dq[front]] - w[i]) + ((a[i] == b[j]) ? 1 : -1);
            if (dp[i][j] < new_v){
                dp[i][j] = new_v;
            }

            if (dp[i][j] < 0) dp[i][j] = 0;
            if (ans < dp[i][j]) ans = dp[i][j];
        }
    }

    // reset
    for(int i = 0; i <= m; i++) dp[i][0] = 0;
    for(int j = 0; j < int(b.size()); j++) dp[0][j] = 0;

    for(int i = 1; i < (int)b.size(); i++) {
        in_b[b[i]] = false;
        w[i] = 0;
    }

    double s_new_duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    new_duration += s_new_duration;

    return ans;
}


string gen_random(const int len) {

    string tmp_s;
    static const char alphanum[] =
            "abc";

//    srand( (unsigned) time(NULL) * getpid());

    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];


    return tmp_s;

}

void full_test(){
    VnLangTool::init("", true);
    string a, b;

    ifstream fin("test_1.txt");
    vector<int> res1, res2;

    while (getline(fin, a)){
        getline(fin, b);
        b.erase(std::remove(b.begin(), b.end(), '^'), b.end());
//        a = VnLangTool::lower_root(a);
//        b = VnLangTool::lower_root(b);
//        int x = old_leven(a, b);
//        int y = attempt(a, b);
//        cout << a << endl << b << endl;
//        cout << x << " " << y << endl;
//        assert(x == y);
        auto bs = VnLangTool::split_str(b, "|");
        for(auto s: bs) res1.push_back(old_leven(a, s));
//        res1.push_back(old_leven(a,b));
    }
    fin.close();
    fin.open("test_1.txt");
    while (getline(fin, a)){
        getline(fin, b);
        b.erase(std::remove(b.begin(), b.end(), '^'), b.end());

//        a = VnLangTool::lower_root(a);
//        b = VnLangTool::lower_root(b);
        auto bs = VnLangTool::split_str(b, "|");
        for(auto s: bs) res2.push_back(attempt(a, s));
//        cout << a << endl << b << endl;
//        res2.push_back(attempt(a,b));
    }

    assert(res1.size() == res2.size());
    for(int i = 0 ; i < (int)res1.size(); i++)
        assert(res1[i] == res2[i]);
    cout << "Percentage: " << new_duration / old_duration * 100 <<  "%" << endl;
    cout << new_duration / res2.size() << " " << old_duration / res1.size() << endl;

//    for(int i = 0; i < int(1e6); i++){
//        a = gen_random(20);
//        b = gen_random(6);
//        cout << a << endl << b << endl;
//        int ans1= attempt(a, b);
//        int ans2 =  old_leven(a, b);
//        cout << "???????" << ans1 << " " <<ans2<< endl;
//        assert(ans1==ans2);
//    }

    cout << new_duration << " " << old_duration << endl;
}
int main() {
    full_test();

    return 0;
}
