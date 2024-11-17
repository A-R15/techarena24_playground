// This file contains a template for the implementation of Robo prediction
// algorithm

#include "PredictionAlgorithm.hpp"
#include <cstdint>
#include <cstring>

struct RoboPredictor::RoboMemory {
	// Place your RoboMemory content here
	// Note that the size of this data structure can't exceed 64KiB!

	struct PlanetData{ //data holding for each planet. each is 8 Bytes
	std::uint32_t planetId = 0;
	std::uint8_t dCount = 0; 
	std::uint8_t nCount = 0;
	bool valid = false;
	};

	static const int max = 1024;
	PlanetData observedPlanets[max];

	  // Find the index for a planet using modulus
  int findIndex(std::uint64_t planetId) const {
    return planetId % max;
  }

  // Add or update planet data
  void addPlanet(std::uint64_t planetId, bool day) {
    int index = findIndex(planetId);
    PlanetData &p = observedPlanets[index];

    if (p.valid && p.planetId == planetId) {
      // Update counts if the planet is already recorded
      if (day) p.dCount++;
      else p.nCount++;
    } else {
      // Otherwise, overwrite this slot with the new planet
      p.planetId = planetId;
      p.dCount = day ? 1 : 0;
      p.nCount = day ? 0 : 1;
      p.valid = true;
    }
  }

  // Predict the time of day based on stored data
  bool predict(std::uint64_t planetId, bool spaceshipPrediction) const {
    int index = findIndex(planetId);
    const PlanetData &p = observedPlanets[index];

    if (p.valid && p.planetId == planetId) {
      // If the planet is in memory, return the more frequent observation
      return p.dCount >= p.nCount;
    }

    // If not found, fall back to spaceship prediction
    return false;
  }

	
};

bool RoboPredictor::predictTimeOfDayOnNextPlanet(
		std::uint64_t nextPlanetID, bool spaceshipComputerPrediction) {


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

	return roboMemory_ptr->predict(nextPlanetID, spaceshipComputerPrediction);
}

void RoboPredictor::observeAndRecordTimeofdayOnNextPlanet(
		std::uint64_t nextPlanetID, bool timeOfDayOnNextPlanet) {
	
	roboMemory_ptr->addPlanet(nextPlanetID, timeOfDayOnNextPlanet);

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
