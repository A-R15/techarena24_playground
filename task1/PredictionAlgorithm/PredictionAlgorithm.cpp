// This file contains a template for the implementation of Robo prediction
// algorithm

#include "PredictionAlgorithm.hpp"
#include <algorithm>

struct RoboPredictor::RoboMemory {
	// Place your RoboMemory content here
	// Note that the size of this data structure can't exceed 64KiB!

	// Circular buffer to store recent planet IDs and their time-of-day
    constexpr static int CIRCULAR_BUFFER_SIZE = 1024;
    constexpr static int HASH_TABLE_SIZE = 3500;

    uint64_t circular_buffer[CIRCULAR_BUFFER_SIZE];
    bool time_of_day_buffer[CIRCULAR_BUFFER_SIZE];
    int circular_buffer_index = 0;

    struct PlanetData{
    	std::uint64_t planetID;
    	int dCount;
    	int nCount;
    };

    PlanetData hash_table[HASH_TABLE_SIZE];

    uint64_t hash(uint64_t id) {
        return id % HASH_TABLE_SIZE;
    }
};

bool RoboPredictor::predictTimeOfDayOnNextPlanet(
		std::uint64_t nextPlanetID, bool spaceshipComputerPrediction) {

	// Check the hash table for a direct match
    int hash_index = roboMemory_ptr->hash(nextPlanetID);

    RoboMemory::PlanetData& planet = roboMemory_ptr->hash_table[hash_index];

    if (planet.planetID == nextPlanetID)
		return planet.dCount>=planet.nCount;

    int total_days = 0;
    int window_size = std::min(10, roboMemory_ptr->circular_buffer_index + 1);

    for (int i = 0; i < window_size; ++i) {
        total_days += roboMemory_ptr->time_of_day_buffer[(roboMemory_ptr->circular_buffer_index - i + RoboMemory::CIRCULAR_BUFFER_SIZE) % RoboMemory::CIRCULAR_BUFFER_SIZE];
    }
    double moving_average = (double)total_days / window_size;

    // Combine moving average and spaceship prediction

    double combined_prediction = moving_average * 0.8 + spaceshipComputerPrediction * 0.2;

    return combined_prediction > 0.5;
    
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

	roboMemory_ptr->circular_buffer[roboMemory_ptr->circular_buffer_index] = nextPlanetID;
    roboMemory_ptr->time_of_day_buffer[roboMemory_ptr->circular_buffer_index] = timeOfDayOnNextPlanet;
    roboMemory_ptr->circular_buffer_index = (roboMemory_ptr->circular_buffer_index + 1) % RoboMemory::CIRCULAR_BUFFER_SIZE;

    // Update the hash table
    int hash_index = roboMemory_ptr->hash(nextPlanetID);
    uint64_t previous_planet_id = roboMemory_ptr->circular_buffer[(roboMemory_ptr->circular_buffer_index - 1 + RoboMemory::CIRCULAR_BUFFER_SIZE) % RoboMemory::CIRCULAR_BUFFER_SIZE];
    RoboMemory::PlanetData& planet = roboMemory_ptr->hash_table[hash_index];
    planet.planetID = nextPlanetID;
    planet.dCount += timeOfDayOnNextPlanet;
    planet.nCount += 1 - timeOfDayOnNextPlanet;

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
