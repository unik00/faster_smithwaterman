//
// Created by huynd107 on 15/12/2020.
//

#ifndef OPT_LEVEN_OLD_H
#define OPT_LEVEN_OLD_H

namespace OldLeven {
    const int N = 1005;
    int dp[N][N];
    std::pair<int, int> trace[N][N]; // for tracing

    Answer execute(const std::string& sa, const std::string& sb) {
        std::string a = " " + sa;
        std::string b = " " + sb;

        std::pair<int, int> ans = {0, 0};
        // traditional levenshtein for benchmark
        for (int i = 1; i < int(a.size()); i++) {
            for (int j = 1; j < int(b.size()); j++) {
                if (dp[i][j - 1] > dp[i - 1][j]) {
                    dp[i][j] = dp[i][j - 1] - 1;
                    trace[i][j] = {i, j - 1};
                } else {
                    dp[i][j] = dp[i - 1][j] - 1;
                    trace[i][j] = {i - 1, j};
                }
                if (dp[i][j] < dp[i - 1][j - 1] - 1) {
                    dp[i][j] = dp[i - 1][j - 1] - 1;
                    trace[i][j] = {i - 1, j - 1};
                }
                if (a[i] == b[j] && dp[i][j] < dp[i - 1][j - 1] + 1) {
                    dp[i][j] = dp[i - 1][j - 1] + 1;
                    trace[i][j] = {i - 1, j - 1};
                }
                if (dp[i][j] < 0) {
                    dp[i][j] = 0;
                    trace[i][j] = {0, 0};
                }
                if (dp[ans.first][ans.second] < dp[i][j]) ans = {i, j};
            }
        }
        // start tracing
        std::pair<int, int> current = ans;
        std::pair<int, int> substring_a, substring_b;

        while (dp[trace[current.first][current.second].first][trace[current.first][current.second].second] != 0){
            current = trace[current.first][current.second];
        }

        substring_a = {current.first - 1, ans.first - 1};
        substring_b = {current.second - 1, ans.second - 1};
        Answer ret = Answer{dp[ans.first][ans.second], substring_a, substring_b};
        return ret;
    }
}

#endif //OPT_LEVEN_OLD_H
