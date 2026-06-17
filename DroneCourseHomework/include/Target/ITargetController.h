#pragma once

#include "DataStructs.h"

class ITargetController {
public:
  virtual void OnStepStart(const float& simStep) = 0;
  virtual void OnStepEnd() = 0;
  virtual DataStructs::Coord2D GetCurrentPosition() = 0;
  virtual DataStructs::Coord2D GetPredictedPosition(const float& time) = 0;
  virtual float GetVelocity() = 0;
  //   virtual ~IArmamentSolver() = default;
};