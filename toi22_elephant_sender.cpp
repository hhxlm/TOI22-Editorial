#include "elephant.h"
#include <bits/stdc++.h>
using namespace std;

#define ll long long

const int nx = 25;

long long encode(int N)
{
    int pa[nx];
    map<int,int> mp;
    int ct = 0;
    ll dp[nx][nx];
    int n;

    for (int i = 0; i < N; i++) pa[i] = i;

    auto fp = [&](auto &self, int n) -> int{
        if (pa[n] == n) return n;
        return pa[n] = self(self, pa[n]);
    };

    auto unite = [&](auto &self, int u, int v) {
        int pu = fp(fp, u), pv = fp(fp, v);
        if (pu == pv) return;
        pa[pu] = pv;
    };

    auto solve = [&](auto &self, int i, int j) -> long long {
        if (i == n) return dp[i][j] = 1;
        if (dp[i][j] != -1) return dp[i][j];

        ll w = self(self, i + 1, j) * j + self(self, i + 1, j + 1);

        return dp[i][j] = w;
    };

    n = N;
    for (int i = 0; i < N; i++) {
        for (int j = i + 1; j < N; j++) {
            if (same_group(i, j)) unite(unite, i, j);
        }
    }

    for (int i = 0; i < N; i++) {
        int p = fp(fp, i);
        if (mp.find(p) == mp.end()) {
            mp[p] = ct++;
        }
    }

    set<int> s;

    memset(dp, -1, sizeof dp);

    solve(solve, 0, 0);

    ll K = 0;

    for (int i = 0; i < N; i++) {
        
            if (s.find(mp[pa[i]]) != s.end()) {
                K += dp[i + 1][s.size()] * (ll)mp[pa[i]];
            } else {
                K += dp[i + 1][s.size()] * (ll)(s.size());
                s.insert(mp[pa[i]]);
            }
        
    }

    return K;
}


