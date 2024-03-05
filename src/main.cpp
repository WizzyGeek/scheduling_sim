#include"tester.hpp"

int main() {
    VirtualJobManager v;
    v.read_jobs();

    Scheduler *foo = nullptr;

    while (true) {
        int ch = 0;

        std::cout <<
        "1. FCFS\n"
        "2. SJF Non-premptive\n"
        "3. Max Priority Premptive\n"
        "4. Max Priority Non-Premptive\n"
        "5. Round Robin\n"
        "6. SJF Pre-emptive\n"
        "7. Exit\nChoice: ";

        std::cin >> ch;

        delete foo;
        switch (ch)
        {
        case 1:
            foo = new FCFS;
            break;

        case 2:
            foo = new SJFNP;
            break;

        case 3:
            foo = new MPP;
            break;

        case 4:
            foo = new MPNP;
            break;

        case 5:
            foo = new RRP;
            break;

        case 6:
            foo = new SJFP;
            break;

        case 7:
            return 0;

        default:
            std::cout << "Choice not recognized.\n";
            continue;
        }

        foo->do_scheduling(v);
        std::cout << '\n';
    }
    return 0;
}