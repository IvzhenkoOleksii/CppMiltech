#include "Threads/BaseLoop.h"

#include <iostream>

BaseLoop::BaseLoop(const float& stepTime)
{
  this->stepTime = stepTime;
  isLoopActive = false;
}

void BaseLoop::StartLoopThread()
{
  isLoopActive = true;
  thread = std::thread(&BaseLoop::LoopFunction, this);
}

void BaseLoop::JoinThread()
{
  if (thread.joinable()) {
    thread.join();
  }
}

void BaseLoop::FinishLoopThread()
{
  std::cout << "LOOP ENDED" << std::endl;

  if (LoopEndedAction) {
    LoopEndedAction();
  }

  isLoopActive = false;
}