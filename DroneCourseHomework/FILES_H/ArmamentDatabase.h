#pragma once
#include <string>
#include <map>
#include <cstdint>

// this class represents data-table
// I made it as a private dictionary, so it will be 
// like sql file illusion or json or other data type
// Type will be like enum, so I took small type:
// 0 - free fall type
// 1 - glider type
class ArmamentDatabase
{
	public:struct Data
	{
		public:float Mass;
		public:float Drag;
		public:float Lift;
		public:uint8_t Type;

		Data(float mass, float drag, float lift, uint8_t type)
		{
			Mass = mass;
			Drag = drag;
			Lift = lift;
			Type = type;
		}
	};

	public:static Data GetArmament(std::string name);

	private:static std::map<std::string, Data> sArmaData;
};