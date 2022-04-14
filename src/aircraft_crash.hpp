#pragma once

#include "geometry.hpp"

#include <exception>
#include <string>

class AircraftCrash : public std::runtime_error
{
public:
    AircraftCrash(std::string_view flight_number, Point3D pos, Point3D speed, std::string_view reason) :
        std::runtime_error { build_error_msg(flight_number, pos, speed, reason) },
        _flight_number { flight_number },
        _pos { pos },
        _speed { speed },
        _reason { reason }

    {}

private:
    static std::string build_error_msg(std::string_view flight_number, Point3D pos, Point3D speed,
                                       std::string_view reason)
    {
        std::string msg;
        msg += flight_number;
        msg += " pos=[";
        msg += std::to_string(pos.x());
        msg += ",";
        msg += std::to_string(pos.y());
        msg += ",";
        msg += std::to_string(pos.z());
        msg += "] speed=";
        msg += std::to_string(speed.length());
        msg += " ";
        msg += reason;
        return msg;
    }

    std::string_view _flight_number;
    Point3D _pos;
    Point3D _speed;
    std::string_view _reason;
};
