#include <bits/stdc++.h>

using namespace std;
using pii = pair<int, int>;

int l, n;       // # of slices, # of jobs

struct Operation{
    int s, d, p;
    int st;                     // start time
    int low_t = 0;              // lower bound of start time
};

struct Job{
    vector<Operation> ops;      // operations of the job
    vector<vector<int>> adj;    // use for topological order
    vector<int> lin;            // in degree of each node
    int m, id;                  // # of ops, job's id
    double w;                   // weight

    Job() = default;
    Job(int m, int id, double w):ops(m + 1), adj(m + 1), lin(m + 1, 0), m(m), id(id), w(w){
        input_op();
    };
    bool operator<(const Job &obj)const{
        return id < obj.id;
    }
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
};

bool cmp_weight(const Job &a, const Job &b){
    return a.w > b.w;
}

struct OGen{
    int st, len, id, s;     // start time, duration, op_id, slice_needed, 
    long long s_mask;       // slice_used(bitmask)
    OGen (int st, int len, int s, int id):st(st),len(len),s(s),id(id),s_mask((1LL << l) - 1){};
    bool operator<(const OGen &obj)const{
        return st < obj.st;
    }
    bool operator>(const OGen &obj)const{
        return id < obj.id;
    }
};

void genAns(vector<OGen> &all_ops){
    sort(all_ops.begin(), all_ops.end());
    vector<long long> timeline(100000, (1LL << l) - 1);
    for (auto &op : all_ops){
        for (int i = 0; i != op.len; ++i)
            op.s_mask &= timeline[op.st + i];
        while (__builtin_popcountll(op.s_mask) > op.s)
            op.s_mask &= (op.s_mask - 1);
        for (int i = 0; i != op.len; ++i)
            timeline[op.st + i] &= (~op.s_mask);
    }
    sort(all_ops.begin(), all_ops.end(), greater<OGen>());
}

double weight_first(vector<Job> &jobs){
    sort(jobs.begin() + 1, jobs.end(), cmp_weight);
    vector<int> timeline(100000, l);
    for (int i = 1; i <= n; ++i){
        queue<int> avails;
        auto &job = jobs[i];
        for (int j = 1; j <= job.m; ++j){
            auto &op = job.ops[j];
            if (job.lin[j] == 0)
                avails.push(j);    //zzzzzzzxxxxvdsdhello
        }
        while (!avails.empty()){
            int top = avails.front();
            avails.pop();
            auto &op = job.ops[top];
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
            for (auto &nxt : job.adj[top]){
                job.ops[nxt].low_t = max(job.ops[nxt].low_t, t + op.d);
                if (--job.lin[nxt] == 0)
                    avails.push(nxt);
            }
        }
    }
    sort(jobs.begin() + 1, jobs.end());
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

int main(){
    cin >> l >> n;
    vector<Job> jobs(n + 1);
    for (int i = 1; i <= n; ++i){
        int m;
        double w;
        cin >> m >> w;
        jobs[i] = Job(m, i, w);
    }
    vector<Job> max_t = jobs;
    double max_v = weight_first(max_t);
    cerr << max_v << '\n';
    vector<OGen> all_ops;
    for (int i = 1, id = 0; i <= n; ++i){
        for (int j = 1; j <= max_t[i].m; ++j, ++id){
            auto &op = max_t[i].ops[j];
            all_ops.emplace_back(op.st, op.d, op.s, id);
        }
    }
    genAns(all_ops);
    for (auto &op : all_ops){
        cout << op.st;
        for (int i = 0; i != l; ++i)
            if (op.s_mask & (1LL << i)) cout << ' ' << i + 1;
        cout << '\n';
    }
    return 0;
}