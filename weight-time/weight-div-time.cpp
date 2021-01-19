#include <bits/stdc++.h>
#include "utils.h"
using namespace std;
using pii = pair<int, int>;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

double delta, delta2, delta3;

bool cmp_weight_div_time_log(const Job &a, const Job &b){
    return (log(a.w + delta2) * log(double(b.min_t) + delta) > log(b.w + delta2) * log(double(a.min_t)) + delta);
}

bool cmp_weight_div_time_pow(const Job &a, const Job &b){
    return (pow(a.w, delta) + delta2) * double(b.min_t) > (pow(b.w, delta) + delta2) * double(a.min_t);
}

bool cmp_weight_div_time(const Job &a, const Job &b){
    return (a.w + delta) * double(b.min_t) > (b.w + delta) * double(a.min_t);
}

double weight_time_first(vector<Job> &jobs, bool be_random = true, int state = 1){
    if (state == 0){
        delta = double(rng() % 100);
        sort(jobs.begin() + 1, jobs.end(), cmp_weight_div_time);
    }
    else if (state == 1){
        delta = (double(rng() % 100000000) * 0.00000004 - 1) * 3;
        delta2 = (double(rng() % 100000000) * 0.00000004 - 2) * 0.25;
        sort(jobs.begin() + 1, jobs.end(), cmp_weight_div_time_pow);
    }
    else if (state == 2){
        delta = (double(rng() % 100000000) * 0.00000004 - 1 * 0) * 3;
        delta2 = (double(rng() % 100000000) * 0.00000004 - 2 * 0) * 3;
        sort(jobs.begin() + 1, jobs.end(), cmp_weight_div_time_log);
    }
    vector<int> timeline(100000, l);
    for (int i = 1; i <= n; ++i){
        vector<int> avails;
        auto &job = jobs[i];
        for (int j = 1; j <= job.m; ++j){
            if (job.lin[j] == 0)
                avails.push_back(j);    //zzzzzzzxxxxvdsdhello
        }
        while (!avails.empty()){
            int idx = (be_random) ? rng() % avails.size() : 0;
            int top = avails[idx];
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
                    avails.push_back(nxt);
            }
            avails.erase(avails.begin() + idx);
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
        jobs[i].calc_min_t(true);
    }
    vector<Job> max_t = jobs;
    double max_v = weight_time_first(max_t);
    cerr << max_v << ' ' << "start" << '\n';
    int pre_change = 0;
    for (int i = 0; i < 100000 && (i - pre_change <= 5000); ++i){
        vector<Job> test(jobs);
        double cur_v = weight_time_first(test);
        if (cur_v > max_v){
            pre_change = i;
            max_v = cur_v;
            max_t = test;
            cerr << max_v << ' ' << i << endl;
        }
    }
    cerr << max_v << '\n';
    vector<OGen> all_ops;
    genAns(max_t, all_ops);
    outAns(all_ops);
    return 0;
}