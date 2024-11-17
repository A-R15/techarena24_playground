// This file contains a template for the implementation of Robo prediction
// algorithm

#include "PredictionAlgorithm.hpp"
#include <algorithm>

struct RoboPredictor::RoboMemory {
	// Place your RoboMemory content here
	// Note that the size of this data structure can't exceed 64KiB!

	// Circular buffer to store recent planet IDs and their time-of-day
    constexpr static int CIRCULAR_BUFFER_SIZE = 64;
    constexpr static int HASH_TABLE_SIZE = 2048;

    bool time_of_day_buffer[CIRCULAR_BUFFER_SIZE];
    int circular_buffer_index = 0;

    struct PlanetData{
    	std::uint64_t planetID;
    	double rollingProbability;  // Rolling probability of being in the day phase
    	int visits;			// num times planet visited
    	int correctPredictions;
    };

    PlanetData hash_table[HASH_TABLE_SIZE];

    uint64_t hash(uint64_t id) {
        return id % HASH_TABLE_SIZE;
    }
};

bool RoboPredictor::predictTimeOfDayOnNextPlanet(
		std::uint64_t nextPlanetID, bool spaceshipComputerPrediction) {

	// Check the hash table for a direct match
    // Hash table lookup
    int hash_index = roboMemory_ptr->hash(nextPlanetID);
    RoboMemory::PlanetData& planet = roboMemory_ptr->hash_table[hash_index];

    // Rolling probability (calculated from the planet's historical predictions)
    double rollingProbability = (planet.planetID == nextPlanetID) 
        ? planet.rollingProbability 
        : 0;  // Default to 50% if planet is unseen

    // Local trend improvement: Compute separate day/night counts
    int dayCount = 0, nightCount = 0;
    int window_size = std::min(10, roboMemory_ptr->circular_buffer_index + 1);

    for (int i = 0; i < window_size; ++i) {
        bool isDay = roboMemory_ptr->time_of_day_buffer[
            (roboMemory_ptr->circular_buffer_index - i + RoboMemory::CIRCULAR_BUFFER_SIZE) % RoboMemory::CIRCULAR_BUFFER_SIZE];
        if (isDay) dayCount++;
        else nightCount++;
    }

    double localTrend = (double)dayCount / window_size;

    // Dynamic weight computation based on accuracy
    double rollingAccuracy = (planet.visits > 0) 
        ? (double)planet.correctPredictions / planet.visits 
        : 0.5;  // Default accuracy

    double spaceshipAccuracy = 0.8;  // Assume spaceship is 80% accurate (adjust dynamically if possible)
    double totalAccuracy = rollingAccuracy + spaceshipAccuracy;

    double rollingWeight = rollingAccuracy / totalAccuracy;
    double spaceshipWeight = spaceshipAccuracy / totalAccuracy;
    double localTrendWeight = 1.0 - rollingWeight - spaceshipWeight;

    // Enhanced fallback for less frequent planets (low visits or sparse data)
    if (planet.visits < 5) {
        rollingWeight = 0.4;
        spaceshipWeight = 0.4;
        localTrendWeight = 0.2;
    }

    // Blend predictions based on weights
    double finalPrediction = 
        (rollingProbability * rollingWeight) + 
        (spaceshipComputerPrediction * spaceshipWeight) +
        (localTrend * localTrendWeight);

    return finalPrediction > 0.5;
    
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

    roboMemory_ptr->time_of_day_buffer[roboMemory_ptr->circular_buffer_index] = timeOfDayOnNextPlanet;
    roboMemory_ptr->circular_buffer_index = (roboMemory_ptr->circular_buffer_index + 1) % RoboMemory::CIRCULAR_BUFFER_SIZE;

    // Update the hash table
    int hash_index = roboMemory_ptr->hash(nextPlanetID);
    RoboMemory::PlanetData& planet = roboMemory_ptr->hash_table[hash_index];

    if (planet.planetID == nextPlanetID) {
        // Update rolling probability with exponential smoothing
        double alpha = 0.3;  // Smoothing factor
        planet.rollingProbability = 
            (alpha * timeOfDayOnNextPlanet) + ((1.0 - alpha) * planet.rollingProbability);

        // Track correct predictions
        bool wasPredictionCorrect = 
            (planet.rollingProbability > 0.5) == timeOfDayOnNextPlanet;
        if (wasPredictionCorrect) {
            planet.correctPredictions++;
        }
        planet.visits++;
    } else {
        // Initialize new planet entry
        planet.planetID = nextPlanetID;
        planet.rollingProbability = timeOfDayOnNextPlanet ? 1.0 : 0.0;
        planet.correctPredictions = 0;
        planet.visits = 1;
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
