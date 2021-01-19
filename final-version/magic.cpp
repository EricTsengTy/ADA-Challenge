#include <bits/stdc++.h>
#include "utils.h"
using namespace std;
using pii = pair<int, int>;

mt19937 rng(chrono::steady_clock::now().time_since_epoch().count());

double delta, delta2, delta3;

/* 用來按照 weight 排序 Job */
bool cmp_weight(const Job &a, const Job &b){
    return a.w > b.w;
}

bool cmp_weight_div_time_log(const Job &a, const Job &b){
    return (log(a.w + delta2) * log(double(b.min_t) + delta) > log(b.w + delta2) * log(double(a.min_t)) + delta);
}

bool cmp_weight_div_time_pow(const Job &a, const Job &b){
    return (pow(a.w, delta) + delta2) * double(b.min_t) > (pow(b.w, delta) + delta2) * double(a.min_t);
}

bool cmp_weight_div_time(const Job &a, const Job &b){
    return (a.w + delta) * double(b.min_t) > (b.w + delta) * double(a.min_t);
}

double just_rand(vector<Job> &jobs){
    // pair.first : job_id, pair.second : op_id
    vector<pii> avails;
    for (int i = 1; i <= n; ++i){
        for (int j = 1; j <= jobs[i].m; ++j)
            if (jobs[i].lin[j] == 0)
                avails.emplace_back(i, j);
    }
    // a random generator
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

double weight_first(vector<Job> &jobs){
    sort(jobs.begin() + 1, jobs.end(), cmp_weight);
    vector<int> timeline(100000, l);                // 用來紀錄每段時間還剩下多少 slice
    for (int i = 1; i <= n; ++i){
        queue<int> avails;                          // 紀錄目前有哪些 operation 是可以被做的（因為有些需要別的做完才可以做）
        auto &job = jobs[i];
        for (int j = 1; j <= job.m; ++j){           // 先將可以直接做的 operation 加到 avails 裡
            if (job.lin[j] == 0)
                avails.push(j);                     // zzzzzzzxxxxvdsdhello
        }
        while (!avails.empty()){                    // 持續從 avails 中隨便選一個（這邊是選第一個）開始排程
            int top = avails.front();
            avails.pop();
            auto &op = job.ops[top];
            int t = -1;                             // 用來紀錄 operation 的開始時間
            for (int j = op.low_t; ; ++j){          // 從該 operation 的 lower bound 當作開始時間開始找哪一段時間是有足夠的 slice，一找到就跳出
                bool flag = true;
                for (int k = 0; k != op.d && flag; ++k)
                    if (timeline[j + k] < op.s) flag = false;
                if (flag){
                    t = j;
                    break;
                }
            }
            for (int j = 0; j != op.d; ++j)         // 將選擇到的那段時間的slice扣除該operation需要消耗的slice
                timeline[t + j] -= op.s;
            op.st = t;                              // 設置此 operation 的 start time
            for (auto &nxt : job.adj[top]){         // 扣掉相關的 operation 的 lin，當該個 operation 的 lin = 0，代表在它之前的 operation 都做完了
                job.ops[nxt].low_t = max(job.ops[nxt].low_t, t + op.d);
                if (--job.lin[nxt] == 0)
                    avails.push(nxt);
            }
        }
    }
    sort(jobs.begin() + 1, jobs.end());             // 將 vector<Job> jobs 變回去按照輸入順序排序，為了等等方便輸出
    return score(jobs);
}

double weight_time_first(vector<Job> &jobs, int state = 1, bool be_random = true){
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
    cin >> l >> n;
    vector<Job> jobs(n + 1);
    for (int i = 1; i <= n; ++i){
        int m;
        double w;
        cin >> m >> w;
        jobs[i] = Job(m, i, w);
        jobs[i].calc_min_t(true);
    }
    /* Simple by weight */
    vector<Job> max_t = jobs;
    double max_v = weight_first(max_t);
    cerr << "Start from weight_first : " << max_v << ' ' << '\n';

    /* By random */
    int pre_change = 0;
    for (int i = 0; i < 100000 && (i - pre_change <= 5000); ++i){
        vector<Job> test(jobs);
        double cur_v = just_rand(test);
        if (cur_v > max_v){
            pre_change = i;
            max_v = cur_v;
            max_t = test;
            cerr << "just_random : " << max_v << ' ' << i << endl;
        }
    }

    /* op_weight_time_first */
    pre_change = 0;
    for (int i = 0; i < 100000 && (i - pre_change <= 5000); ++i){
        vector<Job> test(jobs);
        double cur_v = op_weight_time_first(test);
        if (cur_v > max_v){
            pre_change = i;
            max_v = cur_v;
            max_t = test;
            cerr << "op_weight_time_first : " << max_v << ' ' << i << endl;
        }
    }

    /* weight_time_first stage from 0 to 2 */
    pre_change = 0;
    for (int stage = 0; stage != 3; ++stage){
        for (int i = 0; i < 100000 && (i - pre_change <= 5000); ++i){
            vector<Job> test(jobs);
            double cur_v = weight_time_first(test, stage);
            if (cur_v > max_v){
                pre_change = i;
                max_v = cur_v;
                max_t = test;
                cerr << "weight_time_first (stage " << stage << ") : " << max_v << ' ' << i << endl;
            }
        }
    }

    cerr << "Final : " << max_v << '\n';
    vector<OGen> all_ops;
    genAns(max_t, all_ops);
    outAns(all_ops);
    return 0;
}