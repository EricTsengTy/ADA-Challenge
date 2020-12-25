#include <bits/stdc++.h>
#include "utils.h"

using namespace std;
using pii = pair<int, int>;

double machine(vector<Job> &jobs){
    // pair.first : job_id, pair.second : op_id
    vector<pii> avails;
    for (int i = 1; i <= n; ++i){
        for (int j = 1; j <= jobs[i].m; ++j)
            if (jobs[i].lin[j] == 0)
                avails.emplace_back(i, j);
    }
    // a random generator
    mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());
    vector<int> timeline(100000, l);
    while (!avails.empty()){
        // Pick an operation to execute randomly
        int idx = rng() % avails.size();
        auto cur = avails[idx];
        auto &op = jobs[cur.first].ops[cur.second];
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
        auto &job = jobs[cur.first];
        for (auto &nxt : job.adj[cur.second]){
            job.ops[nxt].low_t = max(job.ops[nxt].low_t, t + op.d);
            if (--job.lin[nxt] == 0)
                avails.emplace_back(cur.first, nxt);
        }
        avails.erase(avails.begin() + idx);
    }
    double max_v = 0;
    double total = 0;
    for (int i = 1; i <= n; ++i){
        double max_t = 0;
        for (int j = 1; j <= jobs[i].m; ++j){
            int st = jobs[i].ops[j].st;
            max_t = max(max_t, double(st + jobs[i].ops[j].d));
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
            cerr << max_v << ' ' << i << endl;
        }
    }
    vector<OGen> all_ops;
    genAns(max_t, all_ops);
    outAns(all_ops);
    return 0;
}
