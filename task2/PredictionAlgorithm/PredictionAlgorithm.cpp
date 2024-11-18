// This file contains a template for the implementation of Robo prediction
// algorithm

#include "PredictionAlgorithm.hpp"

#include <unordered_map>
#include <vector>
#include <cstdint>

struct RoboPredictor::RoboMemory {
  // Structure for group-level tracking
  struct GroupHistory {
    int day_count = 0;
    int night_count = 0;
    
    void record(bool timeOfDay) {
      if (timeOfDay)
        ++day_count;
      else
        ++night_count;
    }

    bool predict() const {
      return day_count >= night_count; // Majority voting
    }

    double confidence() const {
      int total = day_count + night_count;
      if (total == 0) return 0.5; // No data yet
      return static_cast<double>(day_count) / total;
    }
  };

  GroupHistory groupHistories[1024]; // Group tag memory (0-1023)
  std::unordered_map<std::uint64_t, bool> planetMemory; // Specific planet history
};

bool RoboPredictor::predictTimeOfDayOnNextPlanet(
    std::uint64_t nextPlanetID, bool spaceshipComputerPrediction,
    int nextPlanetGroupTag) {

    auto& group = roboMemory_ptr->groupHistories[nextPlanetGroupTag];
    auto planetIt = roboMemory_ptr->planetMemory.find(nextPlanetID);

    // Retrieve predictions
    bool groupPrediction = group.predict();
    double groupConfidence = group.confidence();

    bool planetPrediction = spaceshipComputerPrediction;
    double planetConfidence = 0.5; // Default confidence for spaceship

    if (planetIt != roboMemory_ptr->planetMemory.end()) {
    planetPrediction = planetIt->second;
    planetConfidence = 1.0; // Fully confident in individual data
    }

    // Weighted blending of predictions
    double groupWeight = groupConfidence * 0.7; // Adjust this weight based on dataset
    double planetWeight = planetConfidence * 0.3; // Individual planet has a smaller impact

    // Final decision
    return (groupWeight * groupPrediction + planetWeight * planetPrediction +
      (1 - groupWeight - planetWeight) * spaceshipComputerPrediction) >= 0.5;
  // Robo can consult data structures in its memory while predicting.
  // Example: access Robo's memory with roboMemory_ptr-><your RoboMemory
  // content>

  // Robo can perform computations using any data in its memory during
  // prediction. It is important not to exceed the computation cost threshold
  // while making predictions and updating RoboMemory. The computation cost of
  // prediction and updating RoboMemory is calculated by the playground
  // automatically and printed together with accuracy at the end of the
  // evaluation (see main.cpp for more details).

  // Task2 specifics: now, Robo can use additional 10 bits from the 
  // space atlas for prediction.
  // nextPlanetGroupTag

  // Simple prediction policy: follow the spaceship computer's suggestions
  return spaceshipComputerPrediction;
}

void RoboPredictor::observeAndRecordTimeofdayOnNextPlanet(
    std::uint64_t nextPlanetID, bool timeOfDayOnNextPlanet) {
  int groupTag = nextPlanetID % 1024;
  auto& group = roboMemory_ptr->groupHistories[groupTag];

  // Update group-level data
  group.record(timeOfDayOnNextPlanet);

  // Update planet-specific data
  roboMemory_ptr->planetMemory[nextPlanetID] = timeOfDayOnNextPlanet;
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
RoboPredictor::RoboPredictor() { roboMemory_ptr = new RoboMemory; }
RoboPredictor::~RoboPredictor() { delete roboMemory_ptr; }
