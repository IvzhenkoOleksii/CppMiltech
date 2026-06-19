#include "Threads/BaseLoop.h"

#include <iostream>
#include <ostream>
#include <thread>

BaseLoop::BaseLoop(const float& stepTime, const int& timeScale)
{
  this->stepTime = stepTime;
  this->duration = std::chrono::duration<float>{stepTime / timeScale};
  isLoopActive = false;
}

void BaseLoop::StartLoopThread()
{
  isLoopActive = true;
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

void BaseLoop::FinishLoopThread()
{
  isLoopActive = false;

  JoinThread();

  if (LoopEndedAction) {
    LoopEndedAction();
  }
}