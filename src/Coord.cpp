/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#include <Coord.hpp>

Coor::Coord(unsigned int initial_x, unsigned int initial_y){
    
    x = initial_x;
    y = initial_y;
}

bool operator==(const Coord& a, const Coord& b)
{
    return (a.x() == b.x() && a.y() == b.y());
}

bool operator!=(const Coord& a, const Coord& b)
{
    return !(a == b);
}
