#include "Threads/BaseLoop.h"

#include <iostream>
#include <ostream>
#include <thread>

BaseLoop::BaseLoop(const float& stepT, const int& timeScale)
{
  stepTime.store(stepT);
  this->duration = std::chrono::duration<float>{stepTime / timeScale};
  isLoopActive.store(false);
}

void BaseLoop::StartLoopThread()
{
  isLoopActive.store(true);
  thread = std::thread(&BaseLoop::LoopFunction, this);
}

void BaseLoop::JoinThread()
{
  if (std::this_thread::get_id() == thread.get_id()) {
    std::cerr << "Cannot fire join for itself!" << std::endl;
    return;
  }

  if (thread.joinable()) {
    thread.join();
  }
}

void BaseLoop::LoopFunction()
{
  while (isLoopActive.load() == true) {
    if (LoopStepStartedAction) {
      LoopStepStartedAction();
    }

    OnLoopStepStart();

    std::this_thread::sleep_for(duration);

    OnLoopStepEnd();

    if (LoopStepEndedAction) {
      LoopStepEndedAction();
    }

    OnAfterStepEndAction();
  }
}

void BaseLoop::FinishLoopThread()
{
  isLoopActive.store(false);

  if (LoopEndedAction) {
    LoopEndedAction();
  }
}

void BaseLoop::OnLoopStepStart() {};
void BaseLoop::OnLoopStepEnd() {};
void BaseLoop::OnAfterStepEndAction() {};