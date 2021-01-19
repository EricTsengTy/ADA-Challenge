#include <bits/stdc++.h>
#include "utils.h"

using namespace std;
using pii = pair<int, int>;
mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

/* 用來按照 weight 排序 Job */
bool cmp_weight(const Job &a, const Job &b){
    return a.w > b.w;
}

double POWER, EPS = 1e-9;
struct OpX{
    OpX(int j, int o, double w, int d):jid(j),oid(o),w(w),d(d){};
    int jid, oid;
    double w;
    int d;
    bool operator<(const OpX &obj)const{
        return pow(w, POWER) * obj.d < pow(obj.w, POWER) * d;
    }
};

double op_weight_time_first(vector<Job> &jobs){
    POWER = 2;
    priority_queue<OpX> avails;
    for (int i = 1; i <= n; ++i)
        for (int j = 1; j <= jobs[i].m; ++j) if (jobs[i].lin[j] == 0)
            avails.emplace(i, j, jobs[i].w, jobs[i].ops[j].d);
    
    vector<int> timeline(100000, l);                // 用來紀錄每段時間還剩下多少 slice
    while (!avails.empty()){
        auto top = avails.top();
        avails.pop();
        auto &job = jobs[top.jid];
        auto &op = job.ops[top.oid];
        int t = -1;
        for (int j = op.low_t; ; ++j){
            bool flag = true;
            for (int k = 0; k != op.d && flag; ++k)
                if (timeline[j + k] < op.s) flag = false;
            if (flag){
                t = j;
                break;
            }
        }
        for (int j = 0; j != op.d; ++j)
            timeline[t + j] -= op.s;
        op.st = t;
        for (auto &nxt : job.adj[top.oid]){
            job.ops[nxt].low_t = max(job.ops[nxt].low_t, t + op.d);
            if (--job.lin[nxt] == 0)
                avails.emplace(top.jid, nxt, job.w, job.ops[nxt].d);
        }
    }
    return score(jobs);
}

int main(){
    /* 一些輸入 */
    cin >> l >> n;                                  // 這個 l, n 兩個變數放在 utils.h 裡，好像有點醜但感覺比較方便
    vector<Job> jobs(n + 1);                        // 一個一堆 jobs 的 vector，他是從一開始數的
    for (int i = 1; i <= n; ++i){
        int m;
        double w;
        cin >> m >> w;                              // 讀入這個 job 的 m 和 w
        jobs[i] = Job(m, i, w);                     // 叫這個會幫你初始化，然後把剩下關於這個 job 的 input 讀進來
    }

    /* 重點應該是這裡，就是把前面讀到的 jobs 複製一份丟到你的排程 function 裡，你的 function 要把排程結果放在 jobs 裡 */
    vector<Job> max_t = jobs;                       // 複製一份 jobs (這邊叫做 max_t)
    double max_v = op_weight_time_first(max_t);     // 將這份複製的 jobs 丟到某個排程 function 裡 （這邊是 weight_first)，function 會回傳 score（可以藉由呼叫 score() 計算）
    int pre_change = 0;
    for (int i = 0; i < 100000 && (i - pre_change <= 5000); ++i){
        vector<Job> test(jobs);
        double cur_v = op_weight_time_first(test);
        if (cur_v > max_v){
            pre_change = i;
            max_v = cur_v;
            max_t = test;
            cerr << max_v << ' ' << i << endl;
        }
    }
    cerr << max_v << '\n';                          // 只是為了看一下 score 好用來判斷這個排程結果好不好

    /* 一些輸出 */
    vector<OGen> all_ops;                           // 令一個 vector<OGen> all_ops
    genAns(max_t, all_ops);                         // 將想要輸出的排程 vector<Job> max_t 和 all_ops 一起丟到 genAns 裡，它會算好東西存在 all_ops
    outAns(all_ops);                                // 輸出 all_ops
    return 0;
}