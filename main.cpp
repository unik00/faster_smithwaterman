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

int old_leven(string sa, string sb){
    clock_t start;
    double duration;
    start = clock();

    string a = " " + sa;
    string b = " " + sb;

    // traditional smith waterman for benchmark
    int ans = 0;
     for(int i = 1; i < int(a.size()); i++){
        for(int j = 1; j < int(b.size()); j++){
            dp[i][j] = max(dp[i][j-1], dp[i-1][j]) - 1;
            dp[i][j] = max(dp[i][j], dp[i-1][j-1] - 1);
            if (a[i] == b[j]) dp[i][j] = max(dp[i][j], dp[i-1][j-1] + 1);
            dp[i][j] = max(dp[i][j], 0);
//            cout << i << " " << j << " " << dp[i][j] << endl;
            ans = max(ans, dp[i][j]);
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
    double duration;
    start = clock();

    string b = " " + sb;
    string _a = sa;

    for(int i = 1; i < (int)b.size(); i++) {
        in_b[b[i]] = true;
    }
    int ans = 0;
    int cur_w = 0, m = 0;

    for(auto c : _a){
//        if (c == ' ' && a[m] == ' ') continue;
        if (in_b[c]) {
            w[++m] = cur_w;
            a[m] = c;
            cur_w = 0;
        }
        else {
            cur_w++;
        }
    }
//    for (int i = 1; i <=m; i++) cout <<a[i]; cout<<endl;
//    for (int i = 1; i <=m; i++) cout <<w[i] << " "; cout<<endl;
//    cout << b << endl;

    for(int i = 1; i <= m; i++){
        for(int j = 1; j < int(b.size()); j++){
            if (dp[i][j - 1] - 1 > dp[i-1][j] - 1 - w[i]) {
                dp[i][j] = dp[i][j - 1] - 1;
            }
            else {
                dp[i][j] = dp[i - 1][j] - 1 - w[i];
            }

            if (dp[i][j] < dp[i-1][j-1] - 1 - w[i]){
                dp[i][j] = dp[i-1][j-1] - 1 - w[i];
            }

            if (a[i] == b[j]) {
                // we need to query max(dp[i-1][j-1-w[i] to j-1]), can we do this faster?
                for(int l = 0, min_ = min(w[i], j - 1); l <= min_; l++) {
                    if (dp[i][j] < max(0, dp[i - 1][j - 1 - l] - w[i]) + 1){
                        dp[i][j] = max(0, dp[i - 1][j - 1 - l] - w[i]) + 1;
                    }
                }
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
            "abcdeqphngo";

//    srand( (unsigned) time(NULL) * getpid());

    tmp_s.reserve(len);

    for (int i = 0; i < len; ++i)
        tmp_s += alphanum[rand() % (sizeof(alphanum) - 1)];


    return tmp_s;

}

int main() {
    VnLangTool::init("", true);
    string a, b;

    ifstream fin("test_1.txt");
    vector<int> res1, res2;

    while (getline(fin, a)){
        getline(fin, b);
        a = VnLangTool::lower_root(a);
        b = VnLangTool::lower_root(b);
        res2.push_back(attempt(a, b));
//        res1.push_back(old_leven(a, b));


    }
    fin.close();
/*
    fin.open("test_1.txt");
    while (getline(fin, a)){
        getline(fin, b);
        a = VnLangTool::lower_root(a);
        b = VnLangTool::lower_root(b);
        res1.push_back(old_leven(a, b));
    }

    assert(res1.size() == res2.size());
    for(int i = 0 ; i < (int)res1.size(); i++)
        assert(res1[i] == res2[i]);
//    cout << "Percentage: " << new_duration / old_duration * 100 <<  "%" << endl;
*/
    cout << new_duration << " " << old_duration << endl;

    return 0;
}
