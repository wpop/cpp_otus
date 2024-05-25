#include <tbb/blocked_range.h>
#include <tbb/parallel_for.h>
#include <tbb/task_scheduler_init.h>

#include <iostream>

class worker
{
public:
    void operator()(const tbb::blocked_range<int>& range) const
    {
        std::cout << "split by " << range.begin() << " and " << range.end() << std::endl;
    }
};

int main(int, char *[])
{
    tbb::task_scheduler_init init;

    tbb::parallel_for(tbb::blocked_range<int>(0, 1600), worker());

    return 0;
}
