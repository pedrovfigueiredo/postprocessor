//
//  gap.cpp
//  gap_identifier
//
//  Created by Pedro Figueirêdo on 24/10/2017.
//  Copyright © 2017 Pedro Figueirêdo. All rights reserved.
//

#include "../Headers/gap.hpp"

Gap::Gap(int id, Timestamp* begin, Timestamp* end):
id_(id),
begin_(begin),
end_(end){}

Gap::~Gap(){
    delete this->begin_;
    delete this->end_;
}

Timestamp Gap::getDuration_(){
    return (*this->end_) - (*this->begin_);
}

Timestamp Gap::getBegin_(){
    return *this->begin_;
}

Timestamp Gap::getEnd_(){
    return *this->end_;
}

std::ostream& operator<< (std::ostream& stream, const Gap& v){
    return stream << *v.begin_ << " --> " << *v.end_;
}
