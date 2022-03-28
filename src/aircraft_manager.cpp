#include "aircraft_manager.hpp"

#include "aircraft_factory.hpp"

#include <memory>

// const std::string airlines[8] = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };

bool AircraftManager::move()
{
    auto to_remove = std::remove_if(aircrafts.begin(), aircrafts.end(),
                                    [](const auto& aircraft) { return aircraft->move(); });
    aircrafts.erase(to_remove, aircrafts.end());

    return true;
}

void AircraftManager::add_aircraft(std::unique_ptr<Aircraft> avion)
{
    aircrafts.emplace_back(std::move(avion));
}