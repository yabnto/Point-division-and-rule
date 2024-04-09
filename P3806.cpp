#include <iostream>
#include <vector>

using namespace std;
using pii = pair<int, int>;

const int MaxN = 1e4 + 10, MaxM = 1e8 + 10;

struct S {
  bool cnt[MaxM];
  int sz[MaxN], st[MaxN], k[MaxN], tot, n, m, ans;
  vector<pii> g[MaxN];
  bool vis[MaxN], flag[MaxM];

  S() {
    tot = n = m = ans = 0;
    for (int i = 0; i < MaxN; i++) {
      cnt[i] = sz[i] = st[i] = vis[i] = k[i] = 0;
    }
  }

  int find_fatbigest(int x, int fa) {
    sz[x] = 1;
    int maxs = 0, res = -1;
    for (auto i : g[x]) {
      if (i.first == fa || vis[i.first]) continue;
      res = find_fatbigest(i.first, x);
      if (res != -1) {
        return res;
      }
      sz[x] += sz[i.first], maxs = max(maxs, sz[i.first]);
    }
    maxs = max(maxs, n - sz[x]);
    if (maxs * 2 <= n) {
      res = x;
      sz[fa] = n - sz[x];
    }
    return res;
  }

  void G(int x, int fa, int sum) {
    st[++tot] = sum;
    for (int i = 1; i <= m; i++) {
      if (sum <= k[i]) {
        flag[k[i]] |= cnt[k[i] - sum] + (sum == k[i]);
      }
    }
    for (auto i : g[x]) {
      if (i.first == fa || vis[i.first]) continue;
      G(i.first, x, sum + i.second);
    }
  }

  void DFS(int x) {
    for (auto i : g[x]) {
      if (vis[i.first]) continue;
      int tmp = tot;
      G(i.first, x, i.second);
      for (int i = tmp + 1; i <= tot; i++) {
        cnt[st[i]] = 1;
      }
    }
    for (int i = 1; i <= tot; i++) {
      cnt[st[i]] = 0;
    }
    tot = 0;
    vis[x] = 1;
    for (auto i : g[x]) {
      if (vis[i.first]) continue;
      n = sz[i.first];
      DFS(find_fatbigest(i.first, x));
    }
  }

  void solve(int len, int q, int x[], int a[][3]) {
    n = len, m = q;
    for (int i = 1; i <= m; i++) {
      k[i] = x[i];
    }
    for (int i = 1; i < n; i++) {
      g[a[i][0]].push_back({a[i][1], a[i][2]});
      g[a[i][1]].push_back({a[i][0], a[i][2]});
    }
    DFS(find_fatbigest(1, 0));
  }
};

int a[MaxN][3], k[MaxN], n, m;
S t;

int main() {
  ios::sync_with_stdio(0), cin.tie(0);
  cin >> n >> m;
  for (int i = 1; i < n; i++) {
    cin >> a[i][0] >> a[i][1] >> a[i][2];
  }
  for (int i = 1; i <= m; i++) {
    cin >> k[i];
  }      
  t.solve(n, m, k, a);
  for (int i = 1; i <= m; i++) {
    cout << (t.flag[k[i]] ? "AYE" : "NAY") << '\n';
  }
  return 0;
}
