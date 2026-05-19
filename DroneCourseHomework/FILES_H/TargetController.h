#pragma once
#include <vector>

#include "DataStructs.h"

class TargetController
{
	public:TargetController();
	public:TargetController(const std::vector<DataStructs::Position2D> positionsData, const float& stepTimeData);

	public:DataStructs::Position2D GetCurrentPosition();
	public:DataStructs::Position2D GetPredictedPosition(const float& time);
	public:void Update(const float& simStepTime);

	private:void UpdateTargetSpeed();
	private:void UpdateCurrentPathStep();


	// variables
	private:float stepTime;
	private:float currentStepTime;
	private:float velocityX;
	private:float velocityY;

	private:unsigned int currentPathStep;

	private:DataStructs::Position2D currentPosition;
	private:std::vector<DataStructs::Position2D> positions;
};
