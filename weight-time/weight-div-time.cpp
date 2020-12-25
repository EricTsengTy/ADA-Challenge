#include <bits/stdc++.h>
#include "utils.h"
using namespace std;
using pii = pair<int, int>;

bool cmp_weight_div_time(const Job &a, const Job &b){
    return a.w * b.min_t > b.w * a.min_t;
}

double weight_time_first(vector<Job> &jobs){
    sort(jobs.begin() + 1, jobs.end(), cmp_weight_div_time);
    vector<int> timeline(100000, l);
    for (int i = 1; i <= n; ++i){
        queue<int> avails;
        auto &job = jobs[i];
        for (int j = 1; j <= job.m; ++j){
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
        jobs[i].calc_min_t();
    }
    vector<Job> max_t = jobs;
    double max_v = weight_time_first(max_t);
    cerr << max_v << '\n';
    vector<OGen> all_ops;
    genAns(max_t, all_ops);
    outAns(all_ops);
    return 0;
}