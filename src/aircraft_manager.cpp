#include "aircraft_manager.hpp"

#include "aircraft_factory.hpp"

#include <algorithm>
#include <memory>

bool AircraftManager::move()
{
    static int i = 0;
    std::cout << "test before" << i++ << std::endl;
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

    std::cout << "test after" << i++ << std::endl;

    auto to_remove = std::remove_if(aircrafts.begin(), aircrafts.end(),
                                    [](const auto& aircraft) { return aircraft->move(); });
    aircrafts.erase(to_remove, aircrafts.end());

    return true;
}

void AircraftManager::add_aircraft(std::unique_ptr<Aircraft> avion)
{
    aircrafts.emplace_back(std::move(avion));
}