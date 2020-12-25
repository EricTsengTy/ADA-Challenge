#ifndef UTILS_H
#define UTILS_H
#include <bits/stdc++.h>
using namespace std;
using pii = pair<int, int>;

int l, n;       // # of slices, # of jobs

/* Operation */
struct Operation{
    int s, d, p;                // operation 的基本參數
    int st;                     // operation 的開始時間，用來紀錄單個operation經過排程後要從何時開始
    int low_t = 0;              // operation 開始時間的 lower bound（因為一個operation必須在某些operation做完才可以做，所以才有這個東東）
};

/* Job */
struct Job{
    vector<Operation> ops;      // operations of the job （紀錄每個 job 的 operations）
    vector<vector<int>> adj;    // use for topological order （利用圖中 adjacent list 的概念來處理 operations 之間的先後關係）
    vector<int> lin;            // 用來紀錄一個 operation 還需要多少其他的 operations 做完才可以做，其實一開始就是 operation 的 p 的值
    int m, id;                  // # of ops, job's id （operations 的數目、job 的 id（id是按照輸入順序））
    double w;                   // weight
    int min_t;                  // min time when only this job

    Job() = default;
    Job(int m, int id, double w):ops(m + 1), adj(m + 1), lin(m + 1, 0), m(m), id(id), w(w){
        input_op();
    };
    /* 用來按照輸入順序排序 Job */
    bool operator<(const Job &obj)const{
        return id < obj.id;
    }
    /* 負責讀入除了 m, w 以外的資料，並同時計算每個點的入度 */
    void input_op(){
        for (int i = 1; i <= m; ++i){
            cin >> ops[i].s >> ops[i].d >> ops[i].p;
            for (int j = 0; j != ops[i].p; ++j){
                int a;
                cin >> a;
                adj[a].push_back(i);
                ++lin[i];
            }
        }
    }
    /* (不太重要可以跳過，只是為了實作weight-div-time.cpp）試著算出當只有這個 job 要跑得時候，他要跑多久，並將這個值存在 min_t 裡 */
    void calc_min_t(){
        Job job = *this;
        min_t = 0;
        vector<int> avails;
        for (int j = 1; j <= m; ++j)
            if (lin[j] == 0)
                avails.emplace_back(j);
        vector<int> timeline(100000, l);

        while (!avails.empty()){
            // Pick an operation to execute randomly (just pick 0)
            int idx = 0;
            auto cur = avails[idx];
            auto &op = ops[cur];
            int t = -1;
            // Find an appropriate place for the operation
            for (int i = op.low_t; ; ++i){
                bool flag = true;
                for (int j = 0; j != op.d && flag; ++j){
                    if (i + j > timeline.size()) cout << "ERROR" << '\n';
                    if (timeline[i + j] < op.s) flag = false;
                }
                if (flag){
                    t = i;
                    break;
                }
            }
            // Pick which slices to use
            op.st = t;
            for (int i = t; i != t + op.d; ++i)
                timeline[i] -= op.s;
            for (auto &nxt : adj[cur]){
                ops[nxt].low_t = max(ops[nxt].low_t, t + op.d);
                if (--lin[nxt] == 0)
                    avails.emplace_back(nxt);
            }
            avails.erase(avails.begin() + idx);
            min_t = max(min_t, t + op.d - 1);
        }
        int tmp = min_t;
        *this = job;
        min_t = tmp;
    }
};

/* 用來產生Output的一個struct（一個 operation 一個 OGen） */
struct OGen{
    int st, len, id, s;     // 開始時間、持續時間start time, duration, op_id, slice_needed
    long long s_mask;       // 用 bitset 的概念來存一個 operation 用了哪些 slice，假如第 k 個 bit 被設成 1 就代表它用了第 k 個 slice
    OGen (int st, int len, int s, int id):st(st),len(len),s(s),id(id),s_mask((1LL << l) - 1){};
    /* 用來按照開始時間排序 */
    bool operator<(const OGen &obj)const{
        return st < obj.st;
    }
    /* 用來按照 id （輸入順序）排序 */
    bool operator>(const OGen &obj)const{
        return id < obj.id;
    }
};

/* 將按照jobs排程時需要產生出的 output 資訊（主要是每個 operation 需要的用到哪些slice） 存在 all_ops */
void genAns(vector<Job> &jobs, vector<OGen> &all_ops){
    for (int i = 1, id = 0; i <= n; ++i){
        for (int j = 1; j <= jobs[i].m; ++j, ++id){
            auto &op = jobs[i].ops[j];
            all_ops.emplace_back(op.st, op.d, op.s, id);
        }
    }
    sort(all_ops.begin(), all_ops.end());                       // 先按照開始時間排序，然後一個一個去選他們需要的slice
    vector<long long> timeline(100000, (1LL << l) - 1);         // 用來紀錄每個時間點有哪些 slice 還可以用
    for (auto &op : all_ops){
        for (int i = 0; i != op.len; ++i)
            op.s_mask &= timeline[op.st + i];
        while (__builtin_popcountll(op.s_mask) > op.s)
            op.s_mask &= (op.s_mask - 1);
        for (int i = 0; i != op.len; ++i)
            timeline[op.st + i] &= (~op.s_mask);
    }
    sort(all_ops.begin(), all_ops.end(), greater<OGen>());      // 排序回原本的樣子（按照輸入順序）
}

/* 輸出答案 */
void outAns(vector<OGen> &all_ops){
    for (auto &op : all_ops){
        cout << op.st;
        for (int i = 0; i != l; ++i)                            // 看一下 op.s_mask 有什麼東西被設成 1 然後輸出被設成 1 的那些 slice
            if (op.s_mask & (1LL << i)) cout << ' ' << i + 1;
        cout << '\n';
    }
}

/* 計算按照 jobs 的排程方式的分數會是多少 */
double score(vector<Job> &jobs){
    double max_v = 0, total = 0;
    for (int i = 1; i <= n; ++i){
        double max_t = 0;
        for (int j = 1; j <= jobs[i].m; ++j){
            auto &op = jobs[i].ops[j];
            max_t = max(max_t, double(op.st + op.d));
        }
        total += max_t * jobs[i].w;
        max_v = max(max_v, max_t);
    }
    total += max_v;
    return -total;
}
#endif