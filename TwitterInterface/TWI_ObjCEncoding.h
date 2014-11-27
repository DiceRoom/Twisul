//
//  TWI_ObjeCEncoding.h
//  TwiSUL
//
//  Created by mba on 9/18/12.
//  Copyright (c) 2012 mba. All rights reserved.
//

#ifndef __TwiSUL__TWI_ObjeCEncoding__
#define __TwiSUL__TWI_ObjeCEncoding__

#include "TWI_Status.h"
#include <iostream>
#include <list>
#endif /* defined(__TwiSUL__TWI_ObjeCEncoding__) */

std::string encodeEscapeFromUtf8(const std::string &utf8);
std::string decodeEscapeFromUtf8(const std::string &utf8);
void calcEscapeRange(const std::string &target, std::vector<std::pair<int, int> > &offsetList);