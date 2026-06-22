#include <bits/stdc++.h>
using namespace std;

#define ll long long

const int nx = 2e5 + 5;
const ll inf = 1e18;

int n;
int a[nx], opt[nx];
ll offset = 0;
ll dp[nx];
ll tree_val[nx];

struct Data {
    ll minLeft = inf, minRight = inf;
    int optLeft, optRight;
    Data(ll x=inf, int i=-1): minLeft(x), minRight(x), optLeft(i), optRight(i) {};
};

Data merge(Data &A, Data B) {
    Data ans;
    if (A.minLeft <= B.minLeft) ans.minLeft = A.minLeft, ans.optLeft = A.optLeft;
    else ans.minLeft = B.minLeft, ans.optLeft = B.optLeft;
    if (A.minRight <= B.minRight) ans.minRight = A.minRight, ans.optRight = A.optRight;
    else ans.minRight = B.minRight, ans.optRight = B.optRight;
    return ans;
}

Data tree[4*nx];

void update(int idx, int l, int r, int x, ll val, int xx) {
    if (l == r) {
        Data ans;
        ans.minLeft = val - a[xx];
        ans.optLeft = xx;
        ans.minRight = val + a[xx];
        ans.optRight = xx;
        tree[idx] = merge(tree[idx], ans);
        return;
    }
    int mid = (l + r) / 2;
    if (x <= mid) update(idx*2, l, mid, x, val, xx);
    else update(idx*2+1, mid + 1, r, x, val, xx);
    tree[idx] = merge(tree[idx*2], tree[idx*2+1]);
}

Data query(int idx, int l, int r, int ql, int qr) {
    if (r < ql || l > qr) return Data();
    if (ql <= l && r <= qr) return tree[idx];
    int mid = (l + r) / 2;
    Data left = query(idx*2, l, mid, ql, qr);
    Data right = query(idx*2+1, mid + 1, r, ql, qr);
    return merge(left, right);
}



std::vector<int> assign_notes(std::vector<int> A) {
    n = A.size();
    for (int i = 1; i <= n; i++) {
        a[i] = A[i - 1];
    }
    sort(A.begin(), A.end());
    A.erase(unique(A.begin(), A.end()), A.end());

    for (int i = 1; i <= n - 1; i++) {
        int idx = lower_bound(A.begin(), A.end(), a[i + 1]) - A.begin() + 1;
        Data left = query(1, 1, n, 1, idx);
        Data right = query(1, 1, n, idx, n);
        dp[i] = offset, opt[i + 1] = 0;
        if (left.minLeft + offset + a[i + 1] < dp[i]) {
            dp[i] = left.minLeft + offset + a[i + 1];
            opt[i + 1] = left.optLeft;
        }
        if (right.minRight + offset - a[i + 1] < dp[i]) {
            dp[i] = right.minRight + offset - a[i + 1];
            opt[i + 1] = right.optRight;
        }
        
        idx = lower_bound(A.begin(), A.end(), a[i]) - A.begin() + 1;
        offset += abs(a[i + 1] - a[i]);
        tree_val[i] = dp[i] - offset;
        update(1, 1, n, idx, dp[i] - offset, i);
    }

    vector<int> V(n + 1);
    V[n] = 1;
    int cur = 0;
    ll min_cost = offset;
    for (int i = 1; i < n; i++) {
        if (tree_val[i] + offset < min_cost) {
            min_cost = tree_val[i] + offset;
            cur = i;
        }
    }
    for (int i = n; i > 1; i--) {
        if (cur == i - 1) {
            V[i - 1] = V[i] ^ 1;
            cur = opt[i];
        } else {
            V[i - 1] = V[i];
        }
    }
    V.erase(V.begin());
    return V;
}

