//
//  object.cpp
//  postprocessor
//
//  Created by Pedro Figueirêdo on 24/10/2017.
//  Copyright © 2017 Pedro Figueirêdo. All rights reserved.
//

#include "object.hpp"

Object::Object(const std::string& name, int accuracyOrCount):
name_(name),
accuracyOrCount_(accuracyOrCount){}

Object::~Object(){}

bool operator==(const Object& a, const Object& b){
    return a.name_ == b.name_;
}

