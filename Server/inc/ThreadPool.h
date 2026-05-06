#ifndef THREAD_POOL_H
#define THREAD_POOL_H

#include <atomic>
#include <condition_variable>
#include <functional>
#include <future>
#include <mutex>
#include <queue>
#include <thread>
#include <vector>

class ThreadPool {
public:
  ThreadPool(int threadsCount) : _threadsCount{threadsCount}, _stop{false} {
    for (int i = 0; i < _threadsCount; ++i) {
      _threads.emplace_back([this]() {
        while (true) {
          std::unique_lock<std::mutex> lock(_mutex);
          _cv.wait(lock, [this]() { return (!_tasks.empty() || _stop); });

          if (_stop) {
            return;
          }

          auto task = std::move(_tasks.front());
          _tasks.pop();

          lock.unlock();

          task();
        }
      });
    }
  }

  ~ThreadPool() {
    std::unique_lock<std::mutex> lock(_mutex);
    _stop = true;

    lock.unlock();
    _cv.notify_all();

    for (auto &thread : _threads) {
      thread.join();
    }
  }

  template <typename T, typename... Args>
  auto ExecuteTask(T &&task, Args &&...args)
      -> std::future<decltype(task(args...))> {
    using ReturnType = decltype(task(args...));

    auto newTask = std::make_shared<std::packaged_task<ReturnType()>>(
        std::bind(std::forward<T>(task), std::forward<Args>(args)...));
    auto future = newTask->get_future();

    std::unique_lock<std::mutex> lock(_mutex);

    _tasks.push([newTask]() -> void { (*newTask)(); });

    lock.unlock();
    _cv.notify_one();

    return future;
  }

private:
  int _threadsCount;
  std::vector<std::thread> _threads;
  std::queue<std::function<void()>> _tasks;
  std::mutex _mutex;
  std::condition_variable _cv;
  std::atomic<bool> _stop;
};

#endif // THREAD_POOL_H
