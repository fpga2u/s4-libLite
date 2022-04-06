#include <thread>
#include <iostream>
#include <vector>
#include "common/s4semaphore.h"
#define  THREAD_NUM 10
#define  WORKSPACE_NUM 200

S4::semaphore_t g_sema_p2c;

std::vector<std::vector<int>> workingspace;
std::atomic<int> workingSeq = 0;
bool g_stop = false;
void comsumor()
{
    while(true)
    {
        g_sema_p2c.setReady();
        g_sema_p2c.wait();
        if (g_stop)break;
        do{
            int seq = (workingSeq+=10);
            if (seq >= (int)workingspace.size())
                break;
            //std::cout << seq << std::endl;
            for (int i = 0; i < 1000 + 1000 *seq; ++i)
                workingspace[seq].emplace_back(i * seq);
            //std::cout << seq << "done" << std::endl;
        } while (true);
    }
}
 
void productor(bool stop = false)
{
    while (g_sema_p2c.getReady() != THREAD_NUM);
    if (stop) {
        g_stop = true;
        g_sema_p2c.notify(THREAD_NUM);
    }
    workingSeq = 0;
    g_sema_p2c.clrReady();
    g_sema_p2c.notify(THREAD_NUM);
}

int main(int argc, char** argv)
{
    std::vector<std::shared_ptr<std::thread>> threads;
    workingspace.resize(WORKSPACE_NUM);
    for (int i=0; i< THREAD_NUM; ++i){
        auto p = std::make_shared<std::thread>(comsumor);
        threads.emplace_back(p);
    }
    for (int l = 0; l < 100; ++l) {
        productor();
        productor();
    }
	productor(true);
	for (int i = 0; i < THREAD_NUM; ++i) {
        threads[i]->join();
    }
    for (int i = 0; i < WORKSPACE_NUM; ++i) {
        std::cout << i << "  " << workingspace[i].size() << std::endl;
    }

    return 0;
}
