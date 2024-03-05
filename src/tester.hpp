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
        unsigned int bt_left;
        unsigned int arrival_time;
        unsigned int compl_time = 0;
        unsigned int priority = 0;

        int get_turnaround_time() {
            return this->compl_time - this->arrival_time;
        }

        int get_waiting_time() {
            return this->get_turnaround_time() - this->burst_time;
        }

        VirtualJob(int pi, int bt, int at, int p) : pid(pi), burst_time(bt), bt_left(bt), arrival_time(at), priority(p) {};

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

    void read_jobs() {
        int n;
        std::cout << "Number of jobs: ";
        std::cin >> n;
        std::vector<VirtualJob> jobss;
        for (; curr_pid < n;) {
            std::cout << "BT AT PR | ";
            int bt, at, p;
            std::cin >> bt >> at >> p;
            jobss.push_back(VirtualJob(this->curr_pid++, bt, at, p));
        }
        std::sort(jobss.begin(), jobss.end(), [](VirtualJob &a, VirtualJob &b){
            return a.arrival_time < b.arrival_time;
        });
        jobs = std::deque<VirtualJob>(jobss.begin(), jobss.end());
    }

    void push_back_arrived_jobs(std::deque<VirtualJob> &job_q, int curr_time) {
        while (jobs.size() > 0 && curr_time >= jobs.front().arrival_time) {
            job_q.push_back(jobs.front());
            jobs.pop_front();
        }
    }

    void write_header() {
        std::cout << "pid " << std::setw(11) << "burst time" << ' ' << std::setw(11) << "arrival" << ' ';
        std::cout << std::setw(11) << "completion" << ' ';
        std::cout << std::setw(11) << "turnaround" << ' ';
        std::cout << std::setw(11) << "waiting" << '\n';
    }
};

class Scheduler {
    public:
    std::deque<VirtualJob> ready;

    virtual void do_scheduling(VirtualJobManager j) {};
};

class FCFS : public Scheduler {
    public:
    void do_scheduling(VirtualJobManager j) {
        int curr_time = 0;
        std::cout << "FCFS\n";
        j.write_header();
        int twt = 0, ttat = 0, n = 0;

        while (j.jobs.size() > 0 || this->ready.size() > 0) {
            j.push_back_arrived_jobs(this->ready, curr_time);
            if (this->ready.size() > 0) {
                VirtualJob &tmp = this->ready.front();
                tmp.compl_time = curr_time + tmp.burst_time;
                tmp.write_self_stdout();
                curr_time = tmp.compl_time;
                twt += tmp.get_waiting_time();
                ttat += tmp.get_turnaround_time();
                n++;
                this->ready.pop_front();
            }
            else curr_time++;
        }
        std::cout << "Mean waiting time: "  << (((double)twt) / n) << '\n';
        std::cout << "Mean turnaround time: "  << (((double)ttat) / n) << '\n';
    }
};

class SJFNP : public Scheduler {
    public:
    void do_scheduling(VirtualJobManager j) {
        int curr_time = 0;
        std::cout << "SJF (Non-premptive)\n";
        j.write_header();
        int twt = 0, ttat = 0, n = 0;

        while (j.jobs.size() > 0 || this->ready.size() > 0) {
            j.push_back_arrived_jobs(this->ready, curr_time);
            if (this->ready.size() > 0) {
                std::sort(this->ready.begin(), this->ready.end(), [](VirtualJob &a, VirtualJob &b){
                    return a.burst_time < b.burst_time;
                });
                VirtualJob &tmp = this->ready.front();
                tmp.compl_time = curr_time + tmp.burst_time;
                tmp.write_self_stdout();
                curr_time = tmp.compl_time;
                twt += tmp.get_waiting_time();
                ttat += tmp.get_turnaround_time();
                n++;
                this->ready.pop_front();
            }
            else curr_time++;
        }
        std::cout << "Mean waiting time: "  << (((double)twt) / n) << '\n';
        std::cout << "Mean turnaround time: "  << (((double)ttat) / n) << '\n';
    }
};


class MPNP : public Scheduler {
    public:
    void do_scheduling(VirtualJobManager j) {
        int curr_time = 0;
        std::cout << "Max Priority (Non-premptive)\n";
        j.write_header();
        int twt = 0, ttat = 0, n = 0;

        while (j.jobs.size() > 0 || this->ready.size() > 0) {
            j.push_back_arrived_jobs(this->ready, curr_time);
            if (this->ready.size() > 0) {
                std::sort(this->ready.begin(), this->ready.end(), [](VirtualJob &a, VirtualJob &b){
                    return a.priority >= b.priority;
                });
                VirtualJob &tmp = this->ready.front();
                tmp.compl_time = curr_time + tmp.burst_time;
                tmp.write_self_stdout();
                curr_time = tmp.compl_time;
                twt += tmp.get_waiting_time();
                ttat += tmp.get_turnaround_time();
                n++;
                this->ready.pop_front();
            }
            else curr_time++;
        }
        std::cout << "Mean waiting time: "  << (((double)twt) / n) << '\n';
        std::cout << "Mean turnaround time: "  << (((double)ttat) / n) << '\n';
    }
};


class RRP : public Scheduler {
    public:
    void do_scheduling(VirtualJobManager j) {
        int curr_time = 0;
        std::cout << "Round Robin (Pre-Emptive)\n";
        j.write_header();
        int twt = 0, ttat = 0, n = 0;

        while (j.jobs.size() > 0 || this->ready.size() > 0) {
            j.push_back_arrived_jobs(this->ready, curr_time);
            if (this->ready.size() > 0) {
                VirtualJob &tmp = this->ready.front();
                tmp.bt_left--;

                if (tmp.bt_left == 0) {
                    tmp.compl_time = curr_time + 1;
                    tmp.write_self_stdout();
                    twt += tmp.get_waiting_time();
                    ttat += tmp.get_turnaround_time();
                    n++;
                    this->ready.pop_front();
                }
                else {
                    this->ready.push_back(this->ready.front());
                    this->ready.pop_front();
                }
            }
            curr_time++;
        }
        std::cout << "Mean waiting time: "  << (((double)twt) / n) << '\n';
        std::cout << "Mean turnaround time: "  << (((double)ttat) / n) << '\n';
    }
};


class SJFP : public Scheduler {
    public:
    void do_scheduling(VirtualJobManager j) {
        int curr_time = 0;
        std::cout << "SJF (Pre-Emptive)\n";
        j.write_header();
        int twt = 0, ttat = 0, n = 0;

        while (j.jobs.size() > 0 || this->ready.size() > 0) {
            j.push_back_arrived_jobs(this->ready, curr_time);
            if (this->ready.size() > 0) {
                std::sort(this->ready.begin(), this->ready.end(), [](VirtualJob &a, VirtualJob &b){
                    return a.bt_left < b.bt_left;
                });
                VirtualJob &tmp = this->ready.front();
                tmp.bt_left--;

                if (tmp.bt_left == 0) {
                    tmp.compl_time = curr_time + 1;
                    tmp.write_self_stdout();
                    twt += tmp.get_waiting_time();
                    ttat += tmp.get_turnaround_time();
                    n++;
                    this->ready.pop_front();
                }
                else {
                    this->ready.push_back(this->ready.front());
                    this->ready.pop_front();
                }
            }
            curr_time++;
        }
        std::cout << "Mean waiting time: "  << (((double)twt) / n) << '\n';
        std::cout << "Mean turnaround time: "  << (((double)ttat) / n) << '\n';
    }
};


class MPP : public Scheduler {
    public:
    void do_scheduling(VirtualJobManager j) {
        int curr_time = 0;
        std::cout << "Max Priority (Pre-Emptive)\n";
        j.write_header();
        int twt = 0, ttat = 0, n = 0;

        while (j.jobs.size() > 0 || this->ready.size() > 0) {
            j.push_back_arrived_jobs(this->ready, curr_time);
            if (this->ready.size() > 0) {
                std::sort(this->ready.begin(), this->ready.end(), [](VirtualJob &a, VirtualJob &b){
                    return a.priority >= b.priority;
                });
                VirtualJob &tmp = this->ready.front();
                tmp.bt_left--;

                if (tmp.bt_left == 0) {
                    tmp.compl_time = curr_time + 1;
                    tmp.write_self_stdout();
                    twt += tmp.get_waiting_time();
                    ttat += tmp.get_turnaround_time();
                    n++;
                    this->ready.pop_front();
                }
                else {
                    this->ready.push_back(this->ready.front());
                    this->ready.pop_front();
                }
            }
            curr_time++;
        }
        std::cout << "Mean waiting time: "  << (((double)twt) / n) << '\n';
        std::cout << "Mean turnaround time: "  << (((double)ttat) / n) << '\n';
    }
};
