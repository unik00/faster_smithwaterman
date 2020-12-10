#include <iostream>
#include <cstring>
#include <fstream>
#include <cassert>
#include <unistd.h>

#include "vn_lang_tool.hpp"

using namespace std;

const int N = 1005;
int dp[N][N];


int max(const int &a, const int &b){
    return a > b ? a : b;
}

int min(const int &a, const int &b){
    return a < b ? a : b;
}

int old_leven(string sa, string sb){
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

    return ans;
}

int w[N];
bool in_b[int(1e6)];
char a[N];

int attempt(string sa, string sb){
    string b = " " + sb;
    string _a = sa;

    for(int i = 1; i < (int)b.size(); i++) {
        in_b[b[i]] = true;
    }
    int ans = 0;
    int cur_w = 0, m = 0;

    for(auto i : _a){
        if (in_b[i]) {
            w[++m] = cur_w;
            a[m] = i;
            cur_w = 0;
        }
        else {
            cur_w++;
        }
    }

    for(int i = 1; i <= m; i++){
        for(int j = 1; j < int(b.size()); j++){
            if (dp[i][j - 1] - 1 > dp[i-1][j] - 1 - w[i]) {
                dp[i][j] = dp[i][j - 1] - 1;
            }
            else {
                dp[i][j] = dp[i - 1][j] - 1;
            }

            if (dp[i][j] < dp[i-1][j-1] - 1 - w[i]){
                dp[i][j] = dp[i-1][j-1] - 1 - w[i];
            }

            if (a[i] == b[j]) {
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
    return ans;
}

void test_single(string a, string b){


    clock_t start;
    double duration;

    start = clock();
    int old_ans = old_leven(a, b);
    cout << old_ans << endl;
    double old_duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    cout<< "Old time: " << old_duration <<'\n';


    start = clock();
    int new_ans =attempt(a, b);
    cout << new_ans << endl;
    double new_duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    cout<< "Attempt time: "<< new_duration <<'\n';




    cout << "Percentage: " << new_duration / old_duration * 100 <<  "%" << endl;
    cout << "**********************\n";
    assert(new_ans == old_ans);
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

    string a = "a", b = "b";

    a = "biznm";
    b = "bienm";
    test_single(a, b);

    a = "giaoc";
    b = "ngoc";
    test_single(a, b);
    a = "số 16 nguỵ như kon tum thanh xuân hn nhân chính hà đông hà nội hà nội";
    b = "nhan chinh";
    test_single(a, b);

    a = "chua lang dong da ha noi ha noi";
    b = "^láng hạ|^lh";
    test_single(a, b);

    a = "chua lang dong da ha noi ha noi";
    b = "lh";
    test_single(a, b);


    a = "193 tl 19 tạp hoá văn cường gặp chị anh thạnh lộc gò vấp hồ chí minh hồ chí minh";
    b = "^phường 16|^p1";
    test_single(a, b);

    a = "351 100 nơ trang long nơ trang long phường 13 gò vấp hồ chí minh hồ chí minh";
    b = "bình thạnh|bt";
    test_single(a, b);

    a = "710 69 lũy bán bích f tân thành q tân phú hcm tân thành tân bình hồ chí minh hồ chí minh";
    b = "phu thanh";
    test_single(a, b);

    a = "351 100 nơ trang long nơ trang long phường 13 gò vấp hồ chí minh hồ chí minh";
    b = "bt";
    test_single(a, b);

    a = "710 69 lũy bán bích f tân thành q tân phú hcm tân thành tân bình hồ chí minh hồ chí minh\n";
    b = "^tân thành";
    test_single(a, b);

    a = "công ty showa gloves 152 2 tổ 11 khu phố 1 b an phú thuận an bình dương bình dương";
    b = "^an phú|^ap";
    test_single(a, b);

    //
//    ifstream fin("test_1.txt");
//    string s;
//    while (getline(fin, s)){
//        cout << s << endl;
//        s = VnLangTool::lower_root(s);
//        int sub_len = rand() % 15 + 5;
//
//            string b = gen_random(sub_len);
//            b = VnLangTool::lower_root(b);
//            cout << s << "\n" << b << endl;
//            test_single(s, b);
//    }
    return 0;
}
