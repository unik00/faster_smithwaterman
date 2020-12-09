#include <iostream>
#include <cstring>

using namespace std;

const int N = 1005;
int dp[N][N];

int old_leven(string a, string b){
    // traditional smith waterman for benchmark
    a = " " + a;
    b = " " + b;
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
    for(int i = 0; i < int(a.size()); i++) for(int j = 0; j < int(b.size()); j++) dp[i][j] = 0;
    return ans;
}

int w[N];
bool in_b[N];
char a[N];

int attempt(string _a, string b){
    b = " " + b;
    for(int i = 1; i < (int)b.size(); i++) {
        in_b[b[i]] = true;
    }
    int ans = 0;
    int cur_w = 0, m = 0;

    for(char i : _a){
        if (in_b[i]) {
            w[++m] = cur_w;
            a[m] = i;
            cur_w = 0;
        }
        else {
            cur_w++;
        }
    }
//    for(int i = 0; i < (int)_a.size(); i++) cout << w[i] << " ";
//    cout << endl;

    for(int i = 1; i <= m; i++){
        for(int j = 1; j < int(b.size()); j++){
            dp[i][j] = max(dp[i][j - 1] - 1, dp[i-1][j] - 1 - w[i]);

            dp[i][j] = max(dp[i][j], dp[i-1][j-1] - 1 - w[i]);

            if (a[i] == b[j]) {
                if (dp[i - 1][j - 1] == 0){
                    dp[i][j] = max(dp[i][j], dp[i-1][j-1] + 1);
                }
                else{
                    for(int l = 0, min_ = min(w[i], j - 2); l <= min_; l++) {
                        dp[i][j] = max(dp[i][j], dp[i - 1][j - 1 - l] + 1 - w[i]);
                    }
                }
            }
            dp[i][j] = max(dp[i][j], 0);
//            cout << i << " " << j << " " << dp[i][j] << endl;
            ans = max(ans, dp[i][j]);
        }
    }

    // reset
    for(int i = 0; i <= m; i++) for(int j = 0; j < int(b.size()); j++) dp[i][j] = 0;
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
    cout << old_leven(a, b) << endl;
    double old_duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    cout<< "Old time: " << old_duration <<'\n';

    start = clock();
    cout << attempt(a, b) << endl;
    double new_duration = ( clock() - start ) / (double) CLOCKS_PER_SEC;
    cout<< "Attempt time: "<< new_duration <<'\n';
    cout << "Percentage: " << new_duration / old_duration * 100 <<  "%" << endl;
    cout << "**********************\n";
}

int main() {
    string a = "a", b = "b";

    a = "biznm";
    b = "bienm";
    test_single(a, b);

    a = "bizznxh";
    b = "bienxh";
    test_single(a, b);

    a = "nha so 8 ngo 269 pho thanh nhan phuong thanh nhan quan hai ba trung ha noi";
    b = "hai ba trung";
    test_single(a, b);


    a = "nha so 8 ngo 269 pho thanh nhan phuong thanh nhan quan hai ba trung ha noi";
    b = "pho thah nkan";
    test_single(a, b);

    a = "so 100 thach loi thanh xuan soc son ha noi";
    b = "thanh loi";
    test_single(a, b);

    a = "số nhà 16 ngõ 435 đường xuân đỉnh đường xuân đỉnh quận bắc từ liêm thành phố hà nội xuân đỉnh phường xuân đỉnh quận từ liêm hà nội,Hà Nội";
    b = "xuan dinh";
    test_single(a, b);

    a = "lê ngọc huyền số 9 đường nguyễn thị sóc ấp bắc lân xã bà điểm huyện hóc môn tphcm xã bà điểm huyện hóc môn tp hồ chí minh,TP Hồ Chí Minh";
    b = "huyen hoc mon";
    test_single(a, b);

    return 0;
}
