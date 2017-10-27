//
//  timestamp.hpp
//  gap_identifier
//
//  Created by Pedro Figueirêdo on 24/10/2017.
//  Copyright © 2017 Pedro Figueirêdo. All rights reserved.
//

#ifndef timestamp_hpp
#define timestamp_hpp
#include <iostream>
#include <iomanip>

class Timestamp
{
public:
    
    Timestamp();
    Timestamp(int hours, int minutes, int seconds, int miliseconds);
    Timestamp(const Timestamp& t);
    ~Timestamp();
    
    float toSeconds_() const;
    friend std::ostream& operator<< (std::ostream& stream, const Timestamp& t);
    Timestamp operator-(const Timestamp& b);
    
private:
    int miliseconds_;
    int seconds_;
    int minutes_;
    int hours_;
};

#endif /* timestamp_hpp */
