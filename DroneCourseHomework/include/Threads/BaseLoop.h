#include <thread>
#include <functional>

class IBaseLoop {
public:
  IBaseLoop(const float& stepTime);
  virtual void StartLoopThread() = 0;
  virtual void JoinThread() = 0;
  virtual void FinishLoopThread() = 0;

protected:
  // functions
  IBaseLoop() = default;
  virtual void LoopFunction() = 0;

protected:
  // properties and variables
  float loopStep;
  std::thread thread;
  std::atomic<bool> isLoopActive;
};

class BaseLoop : public IBaseLoop {
public:
  BaseLoop(const float& stepTime);
  void StartLoopThread() override;
  void JoinThread() override;
  void FinishLoopThread() override;

  std::function<void()> LoopStepStartedAction;
  std::function<void()> LoopStepEndedAction;
  std::function<void()> LoopEndedAction;
};