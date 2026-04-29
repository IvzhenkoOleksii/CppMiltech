#include "TargetController.h"

#include <algorithm>
#include <vector>
#include <iostream>

TargetController::TargetController()
{

}

TargetController::TargetController(const std::vector<DataStructs::Position2D> positionsData, const float& stepTimeData)
{
	positions = positionsData;
	stepTime = stepTimeData;

	currentPathStep = 0;
	currentStepTime = 0;
	currentPosition = positions[currentPathStep];

	UpdateTargetSpeed();
}

DataStructs::Position2D TargetController::GetCurrentPosition()
{
	return currentPosition;
}

DataStructs::Position2D TargetController::GetPredictedPosition(const float& time)
{
	DataStructs::Position2D predictedPosition;
	predictedPosition.X = currentPosition.X + (velocityX * time);
	predictedPosition.Y = currentPosition.Y + (velocityY * time);
	
	return predictedPosition;
}

void TargetController::Update(const float& simStepTime)
{
	currentStepTime += simStepTime;

	if (currentStepTime >= stepTime)
	{
		currentStepTime -= stepTime;
		UpdateCurrentPathStep();
		currentPosition = positions[currentPathStep];
		UpdateTargetSpeed();
	}
	else
	{
		currentPosition.X += velocityX * simStepTime;
		currentPosition.Y += velocityY * simStepTime;
	}
}

void TargetController::UpdateCurrentPathStep()
{
	++currentPathStep;
	if (currentPathStep >= positions.size())
	{
		// drop to start of array
		currentPathStep = 0;
	}
}

void TargetController::UpdateTargetSpeed()
{
	DataStructs::Position2D pathStartPosition = positions[currentPathStep];

	int nextPathIndex = currentPathStep + 1;
	if (nextPathIndex >= positions.size())
	{
		nextPathIndex = 0;
	}

	DataStructs::Position2D pathEndPosition = positions[nextPathIndex];

	float dX = pathEndPosition.X - pathStartPosition.X;
	float dY = pathEndPosition.Y - pathStartPosition.Y;

	velocityX = dX / stepTime;
	velocityY = dY / stepTime;
}
