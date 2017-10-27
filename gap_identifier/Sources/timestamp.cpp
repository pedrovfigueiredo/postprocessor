//
//  timestamp.cpp
//  gap_identifier
//
//  Created by Pedro Figueirêdo on 24/10/2017.
//  Copyright © 2017 Pedro Figueirêdo. All rights reserved.
//

#include "../Headers/timestamp.hpp"

Timestamp::Timestamp():
hours_{0},
minutes_{0},
seconds_{0},
miliseconds_{0}{}

Timestamp::~Timestamp(){}

Timestamp::Timestamp(int hours, int minutes, int seconds, int miliseconds):
hours_{hours},
minutes_{minutes},
seconds_{seconds},
miliseconds_{miliseconds}{}

Timestamp::Timestamp(const Timestamp& t):
hours_(t.hours_),
minutes_(t.minutes_),
seconds_(t.seconds_),
miliseconds_(t.miliseconds_){}

float Timestamp::toSeconds_() const{
    return hours_ * 3600 + minutes_ * 60 + seconds_ + miliseconds_ / 1000.0f;
}

std::ostream& operator<< (std::ostream& stream, const Timestamp& t){
    return stream << std::setw(2) << std::setfill('0') << t.hours_ << ":"
                  << std::setw(2) << std::setfill('0') << t.minutes_ << ":"
                  << std::setw(2) << std::setfill('0') << t.seconds_ << ","
                  << std::setw(3) << std::setfill('0') << t.miliseconds_;
}

Timestamp Timestamp::operator-( const Timestamp& b){
    int hours, minutes, seconds, miliseconds;
    
    hours = this->hours_ - b.hours_;
    minutes = this->minutes_ - b.minutes_;
    seconds = this->seconds_ - b.seconds_;
    miliseconds = this->miliseconds_ - b.miliseconds_;
    
    if (minutes < 0){
        minutes += 60;
        hours--;
    }
    
    if (seconds < 0){
        seconds += 60;
        minutes--;
    }
    
    if (miliseconds < 0){
        miliseconds += 1000;
        seconds--;
    }
    
    return Timestamp(hours, minutes, seconds, miliseconds);
}
