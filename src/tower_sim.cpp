#include "tower_sim.hpp"

#include "GL/opengl_interface.hpp"
#include "aircraft.hpp"
#include "airport.hpp"
#include "config.hpp"
#include "img/image.hpp"
#include "img/media_path.hpp"

#include <cassert>
#include <cstdlib>
#include <ctime>

using namespace std::string_literals;

// const std::string airlines[8] = { "AF", "LH", "EY", "DL", "KL", "BA", "AY", "EY" };

TowerSimulation::TowerSimulation(int argc, char** argv) :
    context_initializer { argc, argv },
    help { (argc > 1) && (std::string { argv[1] } == "--help"s || std::string { argv[1] } == "-h"s) }
{
    // MediaPath::initialize(argv[0]);
    // std::srand(static_cast<unsigned int>(std::time(nullptr)));
    // GL::init_gl(argc, argv, "Airport Tower Simulation");

    create_keystrokes();
}

TowerSimulation::~TowerSimulation()
{
    delete airport;
}

/*
void TowerSimulation::create_aircraft(const AircraftType& type) const
{
    assert(airport); // make sure the airport is initialized before creating aircraft

    const std::string flight_number = airlines[std::rand() % 8] + std::to_string(1000 + (rand() % 9000));
    const float angle       = (rand() % 1000) * 2 * 3.141592f / 1000.f; // random angle between 0 and 2pi
    const Point3D start     = Point3D { std::sin(angle), std::cos(angle), 0 } * 3 + Point3D { 0, 0, 2 };
    const Point3D direction = (-start).normalize();

    Aircraft* aircraft = new Aircraft { type, flight_number, start, direction, airport->get_tower() };
    // GL::display_queue.emplace_back(aircraft);
    GL::move_queue.emplace(aircraft);
}
*/
void TowerSimulation::create_random_aircraft()
{
    // create_aircraft(*(aircraft_types[rand() % 3]));
    // assert(airport); // make sure the airport is initialized before creating aircraft
    // assert(manager); // make sure the aircraftmanager is initialized before creating aircraft

    aircraft_manager.add_aircraft(aircraft_factory.create_random_aircraft(airport->get_tower()));
}

static void up_framerate()
{
    GL::ticks_per_sec -= DEFAULT_TPS_INCREMENT;
    if (GL::ticks_per_sec == 0)
    {
        GL::ticks_per_sec = 1;
    }
    // std::cout << GL::ticks_per_sec << std::endl;
}

static void switch_paused()
{
    GL::PAUSED = !GL::PAUSED;
    std::cout << "Size display queue : " << GL::display_queue.size() << std::endl;
}

void TowerSimulation::create_keystrokes()
{
    GL::keystrokes.emplace('x', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('q', []() { GL::exit_loop(); });
    GL::keystrokes.emplace('c', [this]() { create_random_aircraft(); });
    GL::keystrokes.emplace('+', []() { GL::change_zoom(0.95f); });
    GL::keystrokes.emplace('-', []() { GL::change_zoom(1.05f); });
    GL::keystrokes.emplace('f', []() { GL::toggle_fullscreen(); });
    // additionnal keystrokes
    GL::keystrokes.emplace('u', []() { GL::ticks_per_sec += DEFAULT_TPS_INCREMENT; });
    GL::keystrokes.emplace('d', []() { up_framerate(); });
    GL::keystrokes.emplace(' ', []() { switch_paused(); });

    GL::keystrokes.emplace('0', [this]() { aircraft_factory.print_number_aircraft(0); });
    GL::keystrokes.emplace('1', [this]() { aircraft_factory.print_number_aircraft(1); });
    GL::keystrokes.emplace('2', [this]() { aircraft_factory.print_number_aircraft(2); });
    GL::keystrokes.emplace('3', [this]() { aircraft_factory.print_number_aircraft(3); });
    GL::keystrokes.emplace('4', [this]() { aircraft_factory.print_number_aircraft(4); });
    GL::keystrokes.emplace('5', [this]() { aircraft_factory.print_number_aircraft(5); });
    GL::keystrokes.emplace('6', [this]() { aircraft_factory.print_number_aircraft(6); });
    GL::keystrokes.emplace('7', [this]() { aircraft_factory.print_number_aircraft(7); });
    GL::keystrokes.emplace(
        '8',
        [this]() { std::cout << "required fuel : " << aircraft_manager.get_required_fuel() << std::endl; });
}

void TowerSimulation::display_help() const
{
    std::cout << "This is an airport tower simulator" << std::endl
              << "the following keysstrokes have meaning:" << std::endl;

    for (const auto& [key, _] : GL::keystrokes)
    {
        std::cout << key << ' ';
    }

    std::cout << std::endl;
}

void TowerSimulation::init_airport()
{
    airport = new Airport { aircraft_manager, one_lane_airport, Point3D { 0, 0, 0 },
                            new img::Image { one_lane_airport_sprite_path.get_full_path() } };

    // GL::display_queue.emplace_back(airport);
    GL::move_queue.emplace(airport);
}

void TowerSimulation::launch()
{
    if (help)
    {
        display_help();
        return;
    }

    init_airport();
    // init_aircraft_types();

    GL::loop();
}
