#include <bits/stdc++.h>

using namespace std;
using pii = pair<int, int>;

int l, n;       // # of slices, # of jobs

struct Operation{
    int s, d, p;
    pair<int, long long> stat;   // start time, slice used(represented by bits)
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

double machine(vector<Job> &jobs){
    // pair.first : {job_id, op_id}, pair.second = least start time
    vector<pair<pii, int>> avails;
    for (int i = 1; i <= n; ++i){
        for (int j = 1; j <= jobs[i].m; ++j)
            if (jobs[i].lin[j] == 0)
                avails.emplace_back(pii(i, j), 0);
    }
    // a random generator
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    vector<long long> timeline(100000, (1LL << l) - 1);
    while (!avails.empty()){
        // Pick an operation to execute randomly
        int idx = rng() % avails.size();
        auto cur = avails[idx];
        auto &op = jobs[cur.first.first].ops[cur.first.second];
        int t = -1;
        // Find an appropriate place for the operation
        for (int i = cur.second; ; ++i){
            long long flag = (1LL << l) - 1;
            for (int j = 0; j != op.d && flag; ++j){
                if (i + j > timeline.size()) cout << "ERROR" << ' ' << i + j << endl, exit(0);
                flag &= timeline[i + j];
            }
            cout << __builtin_popcount(flag) << ' ' << op.s << endl;
            if (__builtin_popcount(flag) >= op.s){
                t = i;
                break;
            }
        }
        // Pick which slices to use
        op.stat = {t, (1LL << l) - 1};
        for (int i = t; i != t + op.d; ++i)
            op.stat.second &= timeline[i];
        // Subtract extra slices (by keep removing the last 1 bit)
        while (__builtin_popcount(op.stat.second) > op.s)
            op.stat.second &= (op.stat.second - 1);
        // Subtract used slices from timeline
        for (int i = t; i != t + op.d; ++i)
            timeline[i] &= (~op.stat.second);
        auto &job = jobs[cur.first.first];
        for (auto &nxt : job.adj[cur.first.second]){
            if (--job.lin[nxt] == 0)
                avails.emplace_back(pii(cur.first.first, nxt), t + op.d);
        }   
        avails.erase(avails.begin() + idx);
    }
    double max_v = 0;
    double total = 0;
    for (int i = 1; i <= n; ++i){
        double max_t = 0;
        for (int j = 1; j <= jobs[i].m; ++j){
            auto &stat = jobs[i].ops[j].stat;
            max_t = max(max_t, double(stat.first + jobs[i].ops[j].d));
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
    double max_v = machine(max_t);
    for (int i = 0; i != 20000; ++i){
        vector<Job> test(jobs);
        double cur_v = machine(test);
        if (cur_v > max_v){
            max_v = cur_v;
            max_t = test;
            cout << max_v << ' ' << i << endl;
        }
    }
    for (int i = 1; i <= n; ++i){
        for (int j = 1; j <= max_t[i].m; ++j){
            auto &stat = max_t[i].ops[j].stat;
            cout << stat.first;
            for (int k = 0; k != l; ++k)
                if (stat.second & (1LL << k))
                    cout << ' ' << (k + 1);
            cout << '\n';
        }
    }
    cout << max_v << '\n';
    return 0;
}
