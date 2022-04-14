#include "geometry.hpp"
#include "tower_sim.hpp"

#include <iostream>

void test_generic_points()
{
    Point3D p1 { 6, 3, 7 };
    Point3D p2 { 4, 9, 5 };
    Point<3, Point2D> p4 { Point2D { 1.f, 2.f }, Point2D { 3.f, 4.f }, Point2D { 5.f, 6.f } };

    auto p3 = p1 + p2;
    p1 += p2;
    p2 *= 3; // ou 3.f, ou 3.0 en fonction du type de Point

    std::cout << "p1.x = " << p1.x() << std::endl;
    std::cout << "p1.y = " << p1.y() << std::endl;
    std::cout << "p1.z = " << p1.z() << std::endl;

    std::cout << p3.x() << std::endl;

    std::cout << "p4.y().x() = " << p4.y().x() << std::endl;
}

int main()
// int main(int argc, char** argv)
{
    // TowerSimulation simulation { argc, argv };
    // simulation.launch();

    test_generic_points();

    return 0;
}