//
// Created by huynd107 on 15/12/2020.
//

#ifndef OPT_LEVEN_NEW_H
#define OPT_LEVEN_NEW_H

#include "answer.h"

namespace NewLeven {
    const int N = 1005;
    const int ALPHABET_SIZE = 256;
    int dp[N][N]; // for dynamic programming memorization
    std::pair<int, int> trace[N][N]; // for tracing
    int w[N]; // for saving number of consecutive delete chars
    bool in_b[ALPHABET_SIZE];

    int max(const int &a, const int &b) {
        return a > b ? a : b;
    }

    int find_substring(const std::string &str, const std::string &pattern) {
        // Step 0. Should not be empty string
        if (str.empty() || pattern.empty())
            return -1;

        // Step 1. Compute failure function
        int failure[pattern.size()];
        std::fill(failure, failure + pattern.size(), -1);

        for (int r = 1, l = -1; r < pattern.size(); r++) {

            while (l != -1 && pattern[l + 1] != pattern[r])
                l = failure[l];

            // assert( l == -1 || pattern[l+1] == pattern[r]);
            if (pattern[l + 1] == pattern[r])
                failure[r] = ++l;
        }

        // Step 2. Search pattern
        int tail = -1;
        for (int i = 0; i < str.size(); i++) {

            while (tail != -1 && str[i] != pattern[tail + 1])
                tail = failure[tail];

            if (str[i] == pattern[tail + 1])
                tail++;

            if (tail == pattern.size() - 1)
                return i - tail;
        }

        return -1;
    }

    Answer execute(const std::string& sa, const std::string& sb) {
        int tmp = find_substring(sa, sb);
        if (tmp != -1) {
            Answer ans = Answer{(int)sb.length(), {tmp, tmp + sb.length() - 1}, {0, sb.length() - 1}};
//            std::cout << ans.first_substring.first << " " << ans.first_substring.second << std::endl;

            return ans;
        }
        std::string b = " " + sb;
        std::string a = " " + sa;

        int n = (int)b.size() - 1;
        for (int i = 1; i <= n; i++) {
            in_b[b[i]] = true;
        }
        std::pair<int, int> ans = {0, 0};
        int cur_w = 0, m = 0;
        for (int i = 1; i < (int)a.size(); i++) {
            if (in_b[a[i]]) {
                w[++m] = cur_w;
                a[m] = a[i];
                cur_w = 0;
            } else {
                cur_w++;
            }
        }

        int dq[b.size()], end = -1, front = 0;

        for (int i = 1; i <= m; i++) {
            end = -1;
            front = 0;
            for (int j = 1; j <= n; j++) {
                if (dp[i][j - 1] - 1 > dp[i - 1][j] - w[i] - 1) {
                    dp[i][j] = dp[i][j - 1] - 1;
                    trace[i][j] = {i, j - 1};
                } else {
                    dp[i][j] = dp[i - 1][j] - w[i] - 1;
                    trace[i][j] = {i - 1, j};
                }

                while (front <= end && dp[i - 1][dq[end]] <= dp[i - 1][j - 1]) end--;
                dq[++end] = j - 1;
                while (front <= end && dq[front] < j - 1 - w[i]) front++;

                int new_v = max(0, dp[i - 1][dq[front]] - w[i]) + ((a[i] == b[j]) ? 1 : -1);
                if (dp[i][j] < new_v) {
                    dp[i][j] = new_v;
                    if (dp[i - 1][dq[front]] <= w[i]) trace[i][j] = {0, 0};
                    else trace[i][j] = {i - 1, dq[front]};
                }
                if (dp[i][j] < 0) {
                    dp[i][j] = 0;
                    trace[i][j] = {0, 0}; // special value indicating that tracing is over
                }
                if (dp[ans.first][ans.second] < dp[i][j]) ans = {i, j};
            }
        }

        // start tracing
        std::pair<int, int> current = ans;
        std::pair<int, int> substring_a, substring_b;

        while (trace[current.first][current.second].first!=0&&trace[current.first][current.second].second!=0){
            if (trace[current.first][current.second].first==0&&trace[current.first][current.second].second==0)
                break;
            current = trace[current.first][current.second];
//            std::cout << current.first << " " << current.second << std::endl;
//            std::cout << dp[current.first][current.second] << std::endl;
        }

        std::pair<int, int> original_a, original_b;
        original_a = {current.first, ans.first};
        original_b = {current.second, ans.second};

//        for(int i = 1; i <= m; i++) std::cout << a[i]; std::cout << std::endl;
//        for(int i = 1; i <= n; i++) std::cout << b[i]; std::cout << std::endl;
//        std::cout << original_a.first << " " << original_a.second << std::endl;
//        std::cout << original_b.first << " " << original_b.second << std::endl;


        for(int i = 1; i <= current.first; i++){
            original_a.first += w[i];
            original_a.second += w[i];
        }

        for(int i = current.first + 1; i <= ans.first; i++){
            original_a.second += w[i];
        }

        original_a.first -= 1;
        original_a.second -= 1;
        original_b.first -= 1;
        original_b.second -= 1;

//        std::cout << "original_a: " << original_a.first << " " << original_a.second << std::endl;
//        std::cout << "original_b: " << original_b.first << " " << original_b.second << std::endl;

        // reset
        for (int i = 1; i <= n; i++) {
            in_b[b[i]] = false;
            w[i] = 0;
        }
        return Answer{dp[ans.first][ans.second], original_a, original_b};
    }
}
#endif //OPT_LEVEN_NEW_H
