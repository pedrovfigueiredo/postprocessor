//
//  object.hpp
//  postprocessor
//
//  Created by Pedro Figueirêdo on 24/10/2017.
//  Copyright © 2017 Pedro Figueirêdo. All rights reserved.
//

#ifndef object_hpp
#define object_hpp

#include <string>

class Object {
    
public:
    Object(const std::string& name, int accuracyOrCount);
    ~Object();
    
    friend bool operator==(const Object& a, const Object& b);
    
    const std::string name_;
    int accuracyOrCount_;
};

#endif /* object_hpp */

