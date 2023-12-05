#include <gtest/gtest.h>

#include "kc/async/TaskScheduler.hpp"

using namespace testing;
using namespace kc;

TEST(
  TaskSchedulerBaseTests,
  givenDefaultTaskScheduler_whenAskingForWorkerCount_shouldReturnHardwareConcurrency
) {
    EXPECT_EQ(
      async::TaskScheduler().getWorkerCount(), std::thread::hardware_concurrency()
    );
}

TEST(
  TaskSchedulerBaseTests,
  givenTaskSchedulerWithGivenWorkerCount_whenAskingForWorkerCount_shouldReturnCorrectValue
) {
    static constexpr int workerCount = 11;
    EXPECT_EQ(async::TaskScheduler(workerCount).getWorkerCount(), workerCount);
}

struct TaskSchedulerTests : Test {
    async::TaskScheduler ts;
};

TEST_F(TaskSchedulerTests, givenTaskScheduler_whenSchedulingTask_shouldExecute) {
    bool executed = false;

    ts.call([&executed]() { executed = true; }).wait();

    EXPECT_TRUE(executed);
};

TEST_F(
  TaskSchedulerTests, givenTaskScheduler_whenSchedulingTaskWithCallback_shouldExecute
) {
    bool executed         = false;
    bool callbackExecuted = false;
    int value             = 5;

    ts.call([&executed, value]() {
          executed = true;
          return value;
      })
      .then([&callbackExecuted, value](int argument) {
          EXPECT_EQ(value, argument);
          callbackExecuted = true;
      })
      .wait();

    EXPECT_TRUE(executed);
    EXPECT_TRUE(callbackExecuted);
};

TEST_F(TaskSchedulerTests, givenTaskScheduler_whenSchedulingTwoTasks_shouldExecute) {
    bool executed  = false;
    bool executed2 = false;

    auto t1 = ts.call([&executed]() { executed = true; });
    auto t2 = ts.call([&executed2]() {
        executed2 = true;
        return 1;
    });

    async::wait(t1, t2);

    EXPECT_TRUE(executed);
    EXPECT_TRUE(executed2);
};
