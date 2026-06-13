#include "Test/Class1.h"
#include <chrono>
#include <functional>
#include <ostream>
#include <iostream>
#include <thread>

Class1::Class1(float step)
{
  loopStep = step;
}

void Class1::FireFunction()
{
  int answer = GetIndexFromClass2();
  std::cout << "Stored value is:   " << answer << std::endl;
}

void Class1::InsertFunction(std::function<int()> func)
{
  GetIndexFromClass2 = func;
}

void Class1::StartWork()
{
  isLoopActive = true;
  _thread = std::thread(&Class1::ThreadWork, this);
}

void Class1::FinishThreadLoop()
{
  isLoopActive = false;
}

void Class1::JoinThread()
{
  if (_thread.joinable()) {
    _thread.join();
  }
}

void Class1::ThreadWork()
{
  std::chrono::duration duration = std::chrono::duration<float>{Wait1};
  while (isLoopActive) {
    std::this_thread::sleep_for(duration);
    FireFunction();
  }
}

//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//
//

void Class2::StartWork()
{
  _thread = std::thread(&Class2::ThreadWork, this);
}

void Class2::JoinThread()
{
  if (_thread.joinable()) {
    _thread.join();
  }
}

void Class2::ThreadWork()
{
  while (stepsPassed < MaxTestSteps) {
    std::cout << "Thread Class 2 Work Start" << std::endl;
    std::chrono::duration duration = std::chrono::duration<float>{Wait2};
    std::this_thread::sleep_for(duration);

    UpdatePassedSteps();

    std::cout << "Thread Work End. Index is:   " << GetPassedSteps() << std::endl;
  }

  LoopEndedAction();
}

int Class2::GetPassedSteps()
{
  return stepsPassed;
}

void Class2::UpdatePassedSteps()
{
  ++stepsPassed;
}