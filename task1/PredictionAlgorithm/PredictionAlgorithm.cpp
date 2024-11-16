// This file contains a template for the implementation of Robo prediction
// algorithm

#include "PredictionAlgorithm.hpp"

struct RoboPredictor::RoboMemory {
	// Place your RoboMemory content here
	// Note that the size of this data structure can't exceed 64KiB!
	static const int maxPlanets = 1024; //each planet is 32 bits ttl 8KiB
	int planetCount;    				//The num of planets stred in mem
	std::uint64_t visits;
	
	struct PlanetData{
	std::uint64_t planetId;
	bool time;
	std::uint64_t prevVisit;
	};

	PlanetData observedPlanets[maxPlanets];
	
	RoboMemory() : planetCount(0), visits(0) {} //set to 0
};

bool RoboPredictor::predictTimeOfDayOnNextPlanet(
		std::uint64_t nextPlanetID, bool spaceshipComputerPrediction) {
	for(int i =0; i < roboMemory_ptr -> planetCount;++i){
		if (roboMemory_ptr-> observedPlanets[i ].planetId == nextPlanetID){ //if nxt planet already in mem
			std::uint64_t timeDif = roboMemory_ptr-> visits - roboMemory_ptr->observedPlanets[i].prevVisit;

			if(timeDif < 6000 && roboMemory_ptr-> observedPlanets[i ].time == 0 && spaceshipComputerPrediction == 1){
				return !roboMemory_ptr-> observedPlanets[ i].time;
			}
		
			return roboMemory_ptr-> observedPlanets[ i].time;
		}
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
	return spaceshipComputerPrediction;
}

void RoboPredictor::observeAndRecordTimeofdayOnNextPlanet(
		std::uint64_t nextPlanetID, bool timeOfDayOnNextPlanet) {

	roboMemory_ptr->visits++;

		for(int i =0; i < roboMemory_ptr -> planetCount;++i){ //loops through all planets in mem 1024
			if (roboMemory_ptr-> observedPlanets[i ].planetId == nextPlanetID){ // if a planet in memory is = nxt planet it:
				roboMemory_ptr ->observedPlanets[i].time = timeOfDayOnNextPlanet;  // records time of day
				roboMemory_ptr-> observedPlanets[i].prevVisit = roboMemory_ptr->visits; //records the current visit 
			return;
		}
	}

	if(roboMemory_ptr->planetCount < RoboPredictor::RoboMemory::maxPlanets){
		roboMemory_ptr-> observedPlanets[ roboMemory_ptr ->planetCount++ ] ={
			nextPlanetID,
			timeOfDayOnNextPlanet,
			roboMemory_ptr->visits
		};
	}
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
}
