#include "aircraft_manager.hpp"

#include "aircraft_crash.hpp"
#include "aircraft_factory.hpp"

#include <algorithm>
#include <cassert>
#include <memory>

bool AircraftManager::move()
{
    std::sort(aircrafts.begin(), aircrafts.end(),
              [](const std::unique_ptr<Aircraft>& a, const std::unique_ptr<Aircraft>& b)
              {
                  if (&a == &b)
                      return false;
                  if (a->has_terminal() && b->has_terminal())
                  {
                      return a->fuel_level() < b->fuel_level();
                  }
                  if (a->has_terminal())
                      return true;
                  if (b->has_terminal())
                      return false;
                  return a->fuel_level() < b->fuel_level();
              });

    auto to_remove = std::remove_if(aircrafts.begin(), aircrafts.end(),
                                    [this](const auto& aircraft)
                                    {
                                        try
                                        {
                                            return aircraft->move();
                                        } catch (const AircraftCrash& crash)
                                        {
                                            std::cerr << crash.what() << std::endl;
                                            crashed_aircraft++;
                                            return true;
                                        }
                                    });
    aircrafts.erase(to_remove, aircrafts.end());

    return true;
}

void AircraftManager::add_aircraft(std::unique_ptr<Aircraft> aircraft)
{
    assert(aircraft && "aircraft shouldn't be nullptr");
    aircrafts.emplace_back(std::move(aircraft));
}

int AircraftManager::get_required_fuel() const
{
    return std::accumulate(aircrafts.begin(), aircrafts.end(), 0u,
                           [](unsigned int acc, const std::unique_ptr<Aircraft>& cur)
                           {
                               if (cur->has_served() || !cur->is_low_on_fuel())
                               {
                                   return acc;
                               }
                               return acc + 3000 - cur->fuel_level();
                           });
}