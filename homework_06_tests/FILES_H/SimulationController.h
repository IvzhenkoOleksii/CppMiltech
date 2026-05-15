#pragma once

constexpr int MaxSimulationSteps = 10000;

class SimulationController
{
	public:bool IsWorking();
	public:void Update();
	public:float GetSimulationStepTime();

	SimulationController(const float& simStepTime);

	private:int currentSimulationStep;
	private:float simulationStepTime;
	private:float currentSimulationTime;
};