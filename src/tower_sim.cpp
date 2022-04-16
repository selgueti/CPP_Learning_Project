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

TowerSimulation::TowerSimulation(int argc, char** argv) :
    context_initializer { argc, argv },
    help { (argc > 1) && (std::string { argv[1] } == "--help"s || std::string { argv[1] } == "-h"s) }
{
    create_keystrokes();
}

TowerSimulation::~TowerSimulation()
{
    delete airport;
}

void TowerSimulation::create_random_aircraft()
{
    assert(airport && "make sure the airport is initialized before creating aircraft");
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

    GL::keystrokes.emplace(
        'm', [this]()
        { std::cout << "Aircraft crashed : " << aircraft_manager.get_crashed_aircraft() << std::endl; });

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
}

void TowerSimulation::init_airport()
{
    airport = new Airport { aircraft_manager, one_lane_airport, Point3D { 0, 0, 0 },
                            new img::Image { one_lane_airport_sprite_path.get_full_path() } };
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
    GL::loop();
}
