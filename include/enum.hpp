/***********************************************************************
 * This Source Code Form is subject to the terms of the Mozilla Public *
 * License, v. 2.0. If a copy of the MPL was not distributed with this *
 * file, You can obtain one at http://mozilla.org/MPL/2.0/.            *
 ***********************************************************************/

#ifndef ENUM_HPP_INCLUDED
#define ENUM_HPP_INCLUDED

/**
 * @file enum.hpp
 * @author Barbier Julien
 * @license Mozilla Public License, v. 2.0 
 * @brief Creation of the enumeration of texture for the area.
 */

enum Area : unsigned char {

    ROCK = 0,
    GRASS,
    WATER_UR, /*!< up -> right */
    WATER_RD, /*!< right -> down */
    WATER_DL, /*!< down -> left */
    WATER_LU, /*!< left -> up */
    WATER_UD, /*!< up -> down */
    WATER_LR /*!< left-> right */
};

/**
 * @brief Creation of the direction for the duck.
 */

enum Direction : unsigned char {
    
    UP = 0,
    DOWN,
    LEFT,
    RIGHT
};

#endif /* ENUM_HPP_INCLUDED */	
