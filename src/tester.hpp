#pragma once
#include<iostream>
#include<iomanip>
#include<deque>
#include<vector>
#include<algorithm>

class VirtualJob {
    public:
        int pid;
        unsigned int burst_time;
        unsigned int arrival_time;
        unsigned int compl_time = 0;
        unsigned int priority = 0;

        int get_turnaround_time() {
            return this->compl_time - this->arrival_time;
        }

        int get_waiting_time() {
            return this->get_turnaround_time() - this->burst_time;
        }

        VirtualJob(int pi, int bt, int at, int p) : pid(pi), burst_time(bt), arrival_time(at), priority(p) {};
        
        void write_self_stdout() {
            std::cout << pid << ' ' << std::setw(11) << burst_time << ' ' << std::setw(11) << arrival_time << ' ';
            std::cout << std::setw(11) << compl_time << ' ';
            std::cout << std::setw(11) << this->get_turnaround_time() << ' ';
            std::cout << std::setw(11) << this->get_waiting_time() << '\n'; 
        }
};

class VirtualJobManager {
    public:
    std::deque<VirtualJob> jobs;
    int curr_pid = 0;

    // VirtualJobManager(int n) : jobs(n) {}

    void read_jobs(int n) {
        std::vector<VirtualJob> jobss;
        for (; curr_pid < n;) {
            std::cout << "BT AT PR | ";
            int bt, at, p;
            jobss[curr_pid] = VirtualJob(this->curr_pid++, bt, at, p);
        }
        std::sort(jobss.begin(), jobss.end(), [](VirtualJob &a, VirtualJob &b){
            return a.arrival_time < b.arrival_time; 
        });
        jobs = std::deque<VirtualJob>(jobss.begin(), jobss.end());
    }

    void push_back_arrived_jobs(std::vector<VirtualJob> &job_q, int curr_time) {
        
    }

    // VirtualJob& max_priority_now() {
    //     int mp = -1;
    //     int midx = 0;
    //     int idx = 0;
    //     for (auto &e : jobs) {
    //         if (e.arrival_time <= this->curr_time) {
    //             if (e.priority > mp && e.compl_time == 0) {
    //                 mp = e.priority;
    //                 midx = idx;
    //             }
    //         }
    //         idx++;
    //     }
    //     return jobs[midx];
    // }
};

