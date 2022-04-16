#pragma once

#include <algorithm>
#include <cassert>
#include <iostream>
#include <iterator>
#include <vector>

namespace GL {
class Displayable; //  Pour avoir accès a display_queue dans la classe
inline std::vector<const Displayable*> display_queue;

// a displayable object can be displayed and has a z-coordinate indicating who
// is displayed before whom ;]

class Displayable
{
protected:
    float z = 0;

public:
    Displayable(const float z_) : z { z_ } { display_queue.emplace_back(this); }
    virtual ~Displayable()
    {
        display_queue.erase(std::find(display_queue.begin(), display_queue.end(), this));
    }

    virtual void display() const = 0;

    float get_z() const { return z; }
};

struct disp_z_cmp
{
    bool operator()(const Displayable* a, const Displayable* b) const
    {
        assert(a && "a shoudn't be null");
        assert(b && "b shoudn't be null");
        const auto az = a->get_z();
        const auto bz = b->get_z();
        return (az == bz) ? (a > b) : (az > bz);
    }
};

} // namespace GL
