// This file contains a template for the implementation of Robo prediction
// algorithm

#include "PredictionAlgorithm.hpp"

struct RoboPredictor::RoboMemory {
	// Place your RoboMemory content here
	// Note that the size of this data structure can't exceed 64KiB!

	//data structure sizes
    constexpr static int quSize = 32;
    constexpr static int hashSize = 4000;

	// Circular queue stores recent TODs
    bool TODqueue[quSize];
    int quIndex = 0;

    struct PlanetData{
    	std::uint64_t planetID;
    	int dCount;
    	int nCount;
    };

    PlanetData hash_table[hashSize];

    uint64_t hash(uint64_t id) {
        return id % hashSize;
    }
};

bool RoboPredictor::predictTimeOfDayOnNextPlanet(
		std::uint64_t nextPlanetID, bool spaceshipComputerPrediction) {

	// Check the hash table for a direct match
    int hashIndex = roboMemory_ptr->hash(nextPlanetID);

    RoboMemory::PlanetData& p = roboMemory_ptr->hash_table[hashIndex];

    if (p.planetID == nextPlanetID){
    	if (p.dCount == 5 && p.dCount >= p.nCount && spaceshipComputerPrediction == true){
    		return true;
    	}else{
			return p.dCount>=p.nCount;
    	}
    }
    return false;

    
}


void RoboPredictor::observeAndRecordTimeofdayOnNextPlanet(
		std::uint64_t nextPlanetID, bool timeOfDayOnNextPlanet) {

    roboMemory_ptr->TODqueue[roboMemory_ptr->quIndex] = timeOfDayOnNextPlanet;
    roboMemory_ptr->quIndex = (roboMemory_ptr->quIndex + 1) % RoboMemory::quSize;

    // Update the hash table
    int hashIndex = roboMemory_ptr->hash(nextPlanetID);

    RoboMemory::PlanetData& p = roboMemory_ptr->hash_table[hashIndex];

    p.planetID = nextPlanetID;
    p.dCount  += timeOfDayOnNextPlanet;
    p.nCount  += 1 - timeOfDayOnNextPlanet;
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