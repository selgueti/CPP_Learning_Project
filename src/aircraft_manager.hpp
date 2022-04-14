#pragma once

#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"
#include "aircraft_manager.hpp"

#include <memory>

class AircraftManager : public GL::DynamicObject
{

private:
    std::vector<std::unique_ptr<Aircraft>> aircrafts;
    int crashed_aircraft = 0;

public:
    AircraftManager() { GL::move_queue.insert(this); };

    bool move() override;
    void add_aircraft(std::unique_ptr<Aircraft> avion); // std::move

    int get_required_fuel() const;
    int get_crashed_aircraft() const { return crashed_aircraft; };
};