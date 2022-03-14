#pragma once

#include "GL/displayable.hpp"
#include "GL/dynamic_object.hpp"
#include "aircraft.hpp"

#include <memory>
#include <string_view>

class AircraftManager : public GL::DynamicObject
{
private:
    std::unordered_map<std::string_view, std::unique_ptr<Aircraft>> aircrafts;

public:
    AircraftManager() = default;

    bool move() override
    {

        for (auto it = aircrafts.begin(); it != aircrafts.end();)
        {
            if ((it->second)->move())
            {
                it = aircrafts.erase(it);
            }
            else
            {
                it++;
            }
        }
        return false;
    };
};
