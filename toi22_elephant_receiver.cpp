#include "elephant.h"
#include <bits/stdc++.h>
using namespace std;

#define ll long long

const int nx = 25;

std::vector<int> decode(int N, long long K)
{
    ll dp[nx][nx];
    int n;

    n = N;
    std::vector<int> group(N, 0);
    memset(dp, -1, sizeof dp);

    auto solve = [&](auto &self, int i, int j) -> long long {
        if (i == n) return dp[i][j] = 1;
        if (dp[i][j] != -1) return dp[i][j];

        ll w = self(self, i + 1, j) * j + self(self, i + 1, j + 1);

        return dp[i][j] = w;
    };

    solve(solve, 0, 0);

    set<int> s;

    for (int i = 0; i < N; i++) {

        for (int g = 0; g <= s.size(); g++) {

            ll branch;
            if (g < s.size()) {
                branch = dp[i + 1][s.size()];
            } else {
                branch = dp[i + 1][s.size() + 1];
            }

            if (K >= branch) {
                K -= branch;
                continue;
            }

            group[i] = g;

            if (g == s.size()) s.insert(g);

            break;
        }
    }

    return group;
}
