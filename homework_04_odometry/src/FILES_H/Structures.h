#pragma once

class Structures
{
	public:struct WheelsOdometrData
	{
        long TimeStamp = 0;
		long ForwardLeftWheel = 0;
		long ForwardRightWheel = 0;
        long BackwardLeftWheel = 0;
        long BackwardRightWheel = 0;

		public:void Print();
	};

	public:struct NrkState
	{
		double X = 0.0f;
		double Y = 0.0f;
		double Theta = 0.0f;
	};
};