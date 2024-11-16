// This file contains a template for the implementation of Robo prediction
// algorithm

#include "PredictionAlgorithm.hpp"
#include <unordered_map>
#include <iostream>

struct RoboPredictor::RoboMemory {
	// Place your RoboMemory content here
	// Note that the size of this data structure can't exceed 64KiB!
	struct PlanetData{ //data holding for each planet. each is 8 Bytes
	int dCount; 
	int nCount;
	};

	//using hash map - pairing ID to PlanetData - O(1)
	std::unordered_map<std::uint64_t, PlanetData> observedPlanets; // key - 8B ; PlanetData - 8B
};

//	~40B per record.

bool RoboPredictor::predictTimeOfDayOnNextPlanet(
		std::uint64_t nextPlanetID, bool spaceshipComputerPrediction) {
	auto p = roboMemory_ptr->observedPlanets.find(nextPlanetID);	//search for planet with ID

	if (p != roboMemory_ptr->observedPlanets.end()){ //if a record exits
		const auto& planet = p-> second;	//gets planetdata 
		return planet.dCount >= planet.nCount; //then predicts day if days is >= nights
	}

	// Robo can consult data structures in its memory while predicting.
	// Example: access Robo's memory with roboMemory_ptr-><your RoboMemory
	// content>

	// Robo can perform computations using any data in its memory during
	// prediction. It is important not to exceed the computation cost threshold
	// while making predictions and updating RoboMemory. The computation cost of
	// prediction and updating RoboMemory is calculated by the playground
	// automatically and printed together with accuracy at the end of the
	// evaluation (see main.cpp for more details).

	// Simple prediction policy: follow the spaceship computer's suggestions
	return false;
}

void RoboPredictor::observeAndRecordTimeofdayOnNextPlanet(
		std::uint64_t nextPlanetID, bool timeOfDayOnNextPlanet) {

	auto& planet = roboMemory_ptr->observedPlanets[nextPlanetID]; //loads a planets data using it's ID
	
	if (timeOfDayOnNextPlanet)	//increment counters
		planet.dCount++;
	else
		planet.nCount++;	

	// Robo can consult/update data structures in its memory
	// Example: access Robo's memory with roboMemory_ptr-><your RoboMemory
	// content>

	// It is important not to exceed the computation cost threshold while making
	// predictions and updating RoboMemory. The computation cost of prediction and
	// updating RoboMemory is calculated by the playground automatically and
	// printed together with accuracy at the end of the evaluation (see main.cpp
	// for more details).

	// Simple prediction policy: do nothing
}


//------------------------------------------------------------------------------
// Please don't modify this file below
//
// Check if RoboMemory does not exceed 64KiB
static_assert(
		sizeof(RoboPredictor::RoboMemory) <= 65536,
		"Robo's memory exceeds 65536 bytes (64KiB) in your implementation. "
		"Prediction algorithms using so much "
		"memory are ineligible. Please reduce the size of your RoboMemory struct.");

// Declare constructor/destructor for RoboPredictor
RoboPredictor::RoboPredictor() {
	roboMemory_ptr = new RoboMemory;
}
RoboPredictor::~RoboPredictor() {
	delete roboMemory_ptr;
	std::cout << "Size of RoboMemory: " << sizeof(RoboPredictor::RoboMemory) << "B\n";
}
