#include <functional>
#include <thread>

class IInter2 {};

class IThreadLoop {
public:
  IThreadLoop(float step);

protected:
  IThreadLoop() = default;
  float loopStep;
  std::thread _thread;
  virtual void StartWork() = 0;
  virtual void JoinThread() = 0;
};

constexpr float Wait1 = 1;
class Class1 : public IThreadLoop, IInter2 {
public:
  Class1() = default;
  Class1(float step);
  void InsertFunction(std::function<int()> func);
  void StartWork() override;
  void JoinThread() override;
  void FinishThreadLoop();

private:
  std::atomic<bool> isLoopActive;
  void ThreadWork();

private:
  void FireFunction();
  std::function<int()> GetIndexFromClass2;
};

constexpr float Wait2 = 0.1f;
class Class2 {
public:
  void StartWork();
  void JoinThread();
  int GetPassedSteps();

  std::function<void()> LoopEndedAction;

private:
  void UpdatePassedSteps();
  void ThreadWork();

private:
  std::thread _thread;
  std::atomic<int> stepsPassed;
};

constexpr int MaxTestSteps = 102;