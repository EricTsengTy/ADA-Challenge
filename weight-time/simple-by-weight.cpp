#include <bits/stdc++.h>
#include "utils.h"

using namespace std;
using pii = pair<int, int>;

/* 用來按照 weight 排序 Job */
bool cmp_weight(const Job &a, const Job &b){
    return a.w > b.w;
}

/* 按照 job 的 weight 由大到小去做排程 */
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
    double max_v = weight_first(max_t);             // 將這份複製的 jobs 丟到某個排程 function 裡 （這邊是 weight_first)，function 會回傳 score（可以藉由呼叫 score() 計算）
    cerr << max_v << '\n';                          // 只是為了看一下 score 好用來判斷這個排程結果好不好

    /* 一些輸出 */
    vector<OGen> all_ops;                           // 令一個 vector<OGen> all_ops
    genAns(max_t, all_ops);                         // 將想要輸出的排程 vector<Job> max_t 和 all_ops 一起丟到 genAns 裡，它會算好東西存在 all_ops
    outAns(all_ops);                                // 輸出 all_ops
    return 0;
}