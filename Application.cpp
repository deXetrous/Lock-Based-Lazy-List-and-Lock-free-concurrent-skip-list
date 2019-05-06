#include"LazySkipList.hpp"
#include"LockFreeSkipList.hpp"
FILE *out_L = fopen("logs_test1_lazy.txt", "w");
FILE *out_L1 = fopen("logs_test2_lazy.txt", "w");
FILE *out_L2 = fopen("logs_test3_lazy.txt", "w");
// FILE *out_L3 = fopen("logs_test4_lazy.txt", "w");
// FILE *out_L4 = fopen("logs_test5_lazy.txt", "w");
FILE *out_LF = fopen("logs_test1_lf.txt", "w");
FILE *out_LF1 = fopen("logs_test2_lf.txt", "w");
FILE *out_LF2 = fopen("logs_test3_lf.txt", "w");
// FILE *out_LF3 = fopen("logs_test4_lf.txt", "w");
// FILE *out_LF4 = fopen("logs_test5_lf.txt", "w");
FILE *times = fopen("times1.txt", "a");
FILE *times1 = fopen("times2.txt", "a");
FILE *times2 = fopen("times3.txt", "a");
// FILE *times3 = fopen("times4.txt", "a");
// FILE *times4 = fopen("times5.txt", "a");

int N;
int k;
int lambda1;
LazySkipList LSL;
LazySkipList LSL1;
LazySkipList LSL2;
// LazySkipList LSL3;
// LazySkipList LSL4;
LockFreeSkipList LFL;
LockFreeSkipList LFL1;
LockFreeSkipList LFL2;
// LockFreeSkipList LFL3;
// LockFreeSkipList LFL4;
long double * wait1;
long double * wait2;
long double * wait3;
// long double * wait4;
// long double * wait5;
double give_exp_dist(float lam)
{
    default_random_engine gen;
    exponential_distribution <double> distribution(1.0/lam);
    return distribution(gen);
}
void test1(int id, string listtype) {
    if(listtype == "LSL") {
        vector<int> vec;
        auto start = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < k; i++) {
            int add_this = rand()%100;
            vec.push_back(add_this);
            bool ret = LSL.add(add_this, id, out_L);
            if(ret == false) {
                timespec ts;
                clock_gettime(CLOCK_REALTIME, &ts);
                fprintf(out_L,"%dth thread add : %d failed at time : %lld:%ld\n",id, add_this, (long long)ts.tv_sec, ts.tv_nsec);
            }
            this_thread::sleep_for(std::chrono::milliseconds((int)(give_exp_dist(lambda1))));
        }
        for(int i = 0; i < k/3; i++) {
            int len = vec.size();
            int remove_this_ind = rand()%len;
            int remove_this = vec[remove_this_ind];
            bool ret = LSL.remove(remove_this, id, out_L);
            if(ret == false) {
                timespec ts;
                clock_gettime(CLOCK_REALTIME, &ts);
                fprintf(out_L,"%dth thread remove : %d failed at time : %lld:%ld\n",id, remove_this, (long long)ts.tv_sec, ts.tv_nsec);
            }
            this_thread::sleep_for(std::chrono::milliseconds((int)(give_exp_dist(lambda1))));
        }
        for(int i = 0; i < vec.size(); i++) {
            int len = vec.size();
            int find_ind = rand()%len;
            int ret = LSL.contains(vec[find_ind]);
            if(ret == false) {
                timespec ts;
                clock_gettime(CLOCK_REALTIME, &ts);
                fprintf(out_L,"%dth thread remove : %d failed at time : %lld:%ld\n",id, vec[find_ind], (long long)ts.tv_sec, ts.tv_nsec);
            } else {
                timespec ts;
                clock_gettime(CLOCK_REALTIME, &ts);
                fprintf(out_L,"%dth thread contains : %d passed at time : %lld:%ld\n",id, vec[find_ind], (long long)ts.tv_sec, ts.tv_nsec);
            }
            this_thread::sleep_for(std::chrono::milliseconds((int)(give_exp_dist(lambda1))));
        }
        auto finish = std::chrono::high_resolution_clock::now();
        auto m = std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count();
        wait1[id] += m;
    }
    else {
        vector<int> vec;
        auto start = std::chrono::high_resolution_clock::now();
        for(int i = 0; i < k; i++) {
            int add_this = rand()%100;
            vec.push_back(add_this);
            bool ret = LFL.add(add_this, id, out_LF);
            if(ret == false) {
                timespec ts;
                clock_gettime(CLOCK_REALTIME, &ts);
                fprintf(out_LF,"%dth thread add : %d failed at time : %lld:%ld\n",id, add_this, (long long)ts.tv_sec, ts.tv_nsec);
            }
            this_thread::sleep_for(std::chrono::milliseconds((int)(give_exp_dist(lambda1))));
        }
        for(int i = 0; i < k/3; i++) {
            int len = vec.size();
            int remove_this_ind = rand()%len;
            int remove_this = vec[remove_this_ind];
            bool ret = LFL.remove(remove_this, id, out_LF);
            if(ret == false) {
                timespec ts;
                clock_gettime(CLOCK_REALTIME, &ts);
                fprintf(out_LF,"%dth thread remove : %d failed at time : %lld:%ld\n",id, remove_this, (long long)ts.tv_sec, ts.tv_nsec);
            }
            this_thread::sleep_for(std::chrono::milliseconds((int)(give_exp_dist(lambda1))));
        }
        for(int i = 0; i < vec.size(); i++) {
            int len = vec.size();
            int find_ind = rand()%len;
            int ret = LFL.contains(vec[find_ind]);
            if(ret == false) {
                timespec ts;
                clock_gettime(CLOCK_REALTIME, &ts);
                fprintf(out_LF,"%dth thread remove : %d failed at time : %lld:%ld\n",id, vec[find_ind], (long long)ts.tv_sec, ts.tv_nsec);
            } else {
                timespec ts;
                clock_gettime(CLOCK_REALTIME, &ts);
                fprintf(out_LF,"%dth thread contains : %d passed at time : %lld:%ld\n",id, vec[find_ind], (long long)ts.tv_sec, ts.tv_nsec);
            }
            this_thread::sleep_for(std::chrono::milliseconds((int)(give_exp_dist(lambda1))));
        }
        auto finish = std::chrono::high_resolution_clock::now();
        auto m = std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count();
        wait1[id] += m;
    }
}
void test2(int id, string listtype) {
    if(listtype == "LSL") {
        auto start = std::chrono::high_resolution_clock::now();
        if(id <= N/2) {
            for(int i = 0; i < 2*k; i++) {
                int add_this = rand()%15;
                bool ret = LSL1.add(add_this, id, out_L1);
                if(ret == false) {
                    timespec ts;
                    clock_gettime(CLOCK_REALTIME, &ts);
                    fprintf(out_L1,"%dth thread add : %d failed at time : %lld:%ld\n",id, add_this, (long long)ts.tv_sec, ts.tv_nsec);
                }
                this_thread::sleep_for(std::chrono::milliseconds((int)(give_exp_dist(lambda1))));
            }
        }
        else 
        {
            for(int i = 0; i < 2*k; i++) {
                int remove_this = rand()%15;
                bool ret = LSL1.remove(remove_this, id, out_L1);
                if(ret == false) {
                    timespec ts;
                    clock_gettime(CLOCK_REALTIME, &ts);
                    fprintf(out_L1,"%dth thread remove : %d failed at time : %lld:%ld\n",id, remove_this, (long long)ts.tv_sec, ts.tv_nsec);
                }
                this_thread::sleep_for(std::chrono::milliseconds((int)(give_exp_dist(lambda1))));
            }
            
        }
        auto finish = std::chrono::high_resolution_clock::now();
        auto m = std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count();
        wait2[id] += m;
    }
    else {
        auto start = std::chrono::high_resolution_clock::now();
        if(id <= N/2) {
            for(int i = 0; i < 2*k; i++) {
                int add_this = rand()%15;
                bool ret = LFL1.add(add_this, id, out_LF1);
                if(ret == false) {
                    timespec ts;
                    clock_gettime(CLOCK_REALTIME, &ts);
                    fprintf(out_LF1,"%dth thread add : %d failed at time : %lld:%ld\n",id, add_this, (long long)ts.tv_sec, ts.tv_nsec);
                }
                this_thread::sleep_for(std::chrono::milliseconds((int)(give_exp_dist(lambda1))));
            }
        }
        else 
        {
            for(int i = 0; i < 2*k; i++) {
                int remove_this = rand()%15;
                bool ret = LFL1.remove(remove_this, id, out_LF1);
                if(ret == false) {
                    timespec ts;
                    clock_gettime(CLOCK_REALTIME, &ts);
                    fprintf(out_LF1,"%dth thread remove : %d failed at time : %lld:%ld\n",id, remove_this, (long long)ts.tv_sec, ts.tv_nsec);
                }
                this_thread::sleep_for(std::chrono::milliseconds((int)(give_exp_dist(lambda1))));
            }
            
        }
        auto finish = std::chrono::high_resolution_clock::now();
        auto m = std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count();
        wait2[id] += m;
    }
}
void test3(int id, string listtype) {
    if(listtype == "LSL") {
        auto start = std::chrono::high_resolution_clock::now();
        if(id <= N/2) {
            for(int i = 0; i < k; i++) {
                int add_this = rand()%30;
                bool ret = LSL2.add(add_this, id, out_L2);
                if(ret == false) {
                    timespec ts;
                    clock_gettime(CLOCK_REALTIME, &ts);
                    fprintf(out_L2,"%dth thread add : %d failed at time : %lld:%ld\n",id, add_this, (long long)ts.tv_sec, ts.tv_nsec);
                }
                this_thread::sleep_for(std::chrono::milliseconds((int)(give_exp_dist(lambda1))));
            }
        }
        else 
        {
            for(int i = 0; i < 3*k; i++) {
                int find_ind = rand()%30;
                int ret = LSL2.contains(find_ind);
                if(ret == false) {
                    timespec ts;
                    clock_gettime(CLOCK_REALTIME, &ts);
                    fprintf(out_L2,"%dth thread remove : %d failed at time : %lld:%ld\n",id, find_ind, (long long)ts.tv_sec, ts.tv_nsec);
                } else {
                    timespec ts;
                    clock_gettime(CLOCK_REALTIME, &ts);
                    fprintf(out_L2,"%dth thread contains : %d passed at time : %lld:%ld\n",id, find_ind, (long long)ts.tv_sec, ts.tv_nsec);
                }
                this_thread::sleep_for(std::chrono::milliseconds((int)(give_exp_dist(lambda1))));
            }
            
        }
        auto finish = std::chrono::high_resolution_clock::now();
        auto m = std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count();
        wait3[id] += m;
    }
    else {
        auto start = std::chrono::high_resolution_clock::now();
        if(id <= N/2) {
            for(int i = 0; i < k; i++) {
                int add_this = rand()%30;
                bool ret = LFL2.add(add_this, id, out_LF2);
                if(ret == false) {
                    timespec ts;
                    clock_gettime(CLOCK_REALTIME, &ts);
                    fprintf(out_LF2,"%dth thread add : %d failed at time : %lld:%ld\n",id, add_this, (long long)ts.tv_sec, ts.tv_nsec);
                }
                this_thread::sleep_for(std::chrono::milliseconds((int)(give_exp_dist(lambda1))));
            }
        }
        else 
        {
            for(int i = 0; i < 3*k; i++) {
                int find_ind = rand()%30;
                int ret = LFL2.contains(find_ind);
                if(ret == false) {
                    timespec ts;
                    clock_gettime(CLOCK_REALTIME, &ts);
                    fprintf(out_LF2,"%dth thread remove : %d failed at time : %lld:%ld\n",id, find_ind, (long long)ts.tv_sec, ts.tv_nsec);
                } else {
                    timespec ts;
                    clock_gettime(CLOCK_REALTIME, &ts);
                    fprintf(out_LF2,"%dth thread contains : %d passed at time : %lld:%ld\n",id, find_ind, (long long)ts.tv_sec, ts.tv_nsec);
                }
                this_thread::sleep_for(std::chrono::milliseconds((int)(give_exp_dist(lambda1))));
            }
            
        }
        auto finish = std::chrono::high_resolution_clock::now();
        auto m = std::chrono::duration_cast<std::chrono::milliseconds>(finish-start).count();
        wait3[id] += m;
    }
}
int main(int argc , char * argv[]) {
    N = atoi(argv[1]);
    k = atoi(argv[2]);
    lambda1 = atoi(argv[3]);
    {
        wait1 = new long double[N];
        for(int i = 0; i < N; i++) {
            wait1[i] = 0;
        }
        thread th_w[N];
        cout << "Creating " << N << " threads" << endl;
        for(int i = 0; i < N; i++) {
            th_w[i] = thread(test1,i, string("LFL"));
        }
        for(int i = 0; i < N; i++) {
            th_w[i].join();
        }
        cout << "Joined " << N << " threads" << endl;
        cout << "Test 1 completed for LF skip list"<< endl;
        long double sum = 0;
        for(int i = 0; i < N; i++) {
            sum += wait1[i];
        }
        fprintf(times,"%Le ",sum/(float)N);
        for(int i = 0; i < N; i++) {
            wait1[i] = 0;
        }
        thread th_wlf[N];
        cout << "Creating " << N << " threads" << endl;
        for(int i = 0; i < N; i++) {
            th_wlf[i] = thread(test1,i, string("LSL"));
        }
        for(int i = 0; i < N; i++) {
            th_wlf[i].join();
        }
        cout << "Joined " << N << " threads" << endl;
        cout << "Test 1 completed for Lazy skip list"<< endl;
        sum = 0;
        for(int i = 0; i < N; i++) {
            sum += wait1[i];
        }
        fprintf(times,"%Le\n",sum/(float)N);
        fclose(out_L);
        fclose(out_LF);
        fclose(times);
    }
    {
        wait2 = new long double[N];
        for(int i = 0; i < N; i++) {
            wait2[i] = 0;
        }
        thread th_w[N];
        cout << "Creating " << N << " threads" << endl;
        for(int i = 0; i < N; i++) {
            th_w[i] = thread(test2,i, string("LFL"));
        }
        for(int i = 0; i < N; i++) {
            th_w[i].join();
        }
        cout << "Joined " << N << " threads" << endl;
        cout << "Test 2 completed for LF skip list"<< endl;
        long double sum = 0;
        for(int i = 0; i < N; i++) {
            sum += wait2[i];
        }
        fprintf(times1,"%Le ",sum/(float)N);
        for(int i = 0; i < N; i++) {
            wait2[i] = 0;
        }
        thread th_wlf[N];
        cout << "Creating " << N << " threads" << endl;
        for(int i = 0; i < N; i++) {
            th_wlf[i] = thread(test2,i, string("LSL"));
        }
        for(int i = 0; i < N; i++) {
            th_wlf[i].join();
        }
        cout << "Joined " << N << " threads" << endl;
        cout << "Test 2 completed for Lazy skip list"<< endl;
        sum = 0;
        for(int i = 0; i < N; i++) {
            sum += wait2[i];
        }
        fprintf(times1,"%Le\n",sum/(float)N);
        fclose(out_L1);
        fclose(out_LF1);
        fclose(times1);
    }
    {
        wait3 = new long double[N];
        for(int i = 0; i < N; i++) {
            wait3[i] = 0;
        }
        thread th_w[N];
        cout << "Creating " << N << " threads" << endl;
        for(int i = 0; i < N; i++) {
            th_w[i] = thread(test3,i, string("LFL"));
        }
        for(int i = 0; i < N; i++) {
            th_w[i].join();
        }
        cout << "Joined " << N << " threads" << endl;
        cout << "Test 3 completed for LF list"<< endl;
        long double sum = 0;
        for(int i = 0; i < N; i++) {
            sum += wait3[i];
        }
        fprintf(times2,"%Le ",sum/(float)N);
        for(int i = 0; i < N; i++) {
            wait3[i] = 0;
        }
        thread th_wlf[N];
        cout << "Creating " << N << " threads" << endl;
        for(int i = 0; i < N; i++) {
            th_wlf[i] = thread(test3,i, string("LSL"));
        }
        for(int i = 0; i < N; i++) {
            th_wlf[i].join();
        }
        cout << "Joined " << N << " threads" << endl;
        cout << "Test 3 completed for Lazy skip list"<< endl;
        sum = 0;
        for(int i = 0; i < N; i++) {
            sum += wait3[i];
        }
        fprintf(times2,"%Le\n",sum/(float)N);
        fclose(out_L2);
        fclose(out_LF2);
        fclose(times2);
    }
    return 0;
}