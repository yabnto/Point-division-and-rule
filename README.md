点分治是树分治的一种形式，通常用来求满足某种要求的路径数量。

### 引入
有 $n$ 个数，问是否存在一个 $l, r$ 使得区间和为 $k$，强行用分治做，可以将数组分成两半，递归后处理左边 $l$ 右边 $r$，然后就用前缀和加 $map$ 加归并的并做就可以了。

### 思路

可以将这个思路放到树上，分为过当前点和不过当前点，不过当前点直接递归处理，过当前点可以往下找，然后用一个桶记录这种路径的长度，让找到的路径长度去匹配一个，然后发现这样子会炸，因为每次都要跑子树大小次，所以找重心跑，子树大小每次除 $2$ 成功 $O(nlogn)$

### code
```cpp
#include <iostream>
#include <vector>

using namespace std;

const int MaxN = 50010;

int cnt[MaxN], sz[MaxN], st[MaxN], tot, n, k, ans;
vector<int> g[MaxN];
bool vis[MaxN];

int find_fatbigest(int x, int fa) {
  sz[x] = 1;
  int maxs = 0, res = -1;
  for (int i : g[x]) {
    if (i == fa || vis[i]) continue;
    res = find_fatbigest(i, x);
    if (res != -1) {
      return res;
    }
    sz[x] += sz[i], maxs = max(maxs, sz[i]);
  }
  maxs = max(maxs, n - sz[x]);
  if (maxs * 2 <= n) {
    res = x;
    sz[fa] = n - sz[x];
  }
  return res;
}

void G(int x, int fa, int sum) {
  if (sum > k) {
    return;
  }
  st[++tot] = sum;
  ans += cnt[k - sum] + (sum == k);
  for (int i : g[x]) {
    if (i == fa || vis[i]) continue;
    G(i, x, sum + 1);
  }
}

void DFS(int x) {
  for (int i : g[x]) {
    if (vis[i]) continue;
    int tmp = tot;
    G(i, x, 1);
    for (int i = tmp + 1; i <= tot; i++) {
      cnt[st[i]]++;
    }
  }
  for (int i = 1; i <= tot; i++) {
    cnt[st[i]]--;
  }
  tot = 0;
  vis[x] = 1;
  for (int i : g[x]) {
    if (vis[i]) continue;
    n = sz[i];
    DFS(find_fatbigest(i, x));
  }
}

int main() {
  cin >> n >> k;
  for (int i = 1, u, v; i < n; i++) {
    cin >> u >> v;
    g[u].push_back(v);
    g[v].push_back(u);
  }
  DFS(find_fatbigest(1, 0));
  cout << ans << endl;
  return 0;
}
```

## P3806 【模板】点分治 1
考虑离线，将所有的 $k$ 弄下来，然后统计答案是统计 $m$ 次，因为 $m$ 小的可怜，所以能过。
### code
```cpp
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
```
