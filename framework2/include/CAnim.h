#ifndef CANIM_H
#define CANIM_H

/*
 *  CAnim.h
 *  Defines an animation sequence
 *
 *  Created by Marcelo Cohen on 08/13.
 *  Copyright 2013 PUCRS. All rights reserved.
 *
 */

#include <string>

struct CAnim
{
    std::string name;   // animation name
    int frameStart;     // starting frame
    int frameEnd;       // ending frame
    bool loop;          // true = animation loops at the end
};

#endif // CANIM_H
