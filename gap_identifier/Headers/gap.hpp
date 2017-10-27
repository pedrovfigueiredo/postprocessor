//
//  gap.hpp
//  gap_identifier
//
//  Created by Pedro Figueirêdo on 24/10/2017.
//  Copyright © 2017 Pedro Figueirêdo. All rights reserved.
//

#ifndef gap_hpp
#define gap_hpp

#include "timestamp.hpp"
#include <iostream>

class Gap {
    
public:
    Gap(int id, Timestamp* begin, Timestamp* end);
    ~Gap();
    
    Timestamp getDuration_();
    Timestamp getBegin_();
    Timestamp getEnd_();
    friend std::ostream& operator<< (std::ostream& stream, const Gap& v);
    
private:
    Timestamp* begin_;
    Timestamp* end_;
    int id_;
    
};

#endif /* gap_hpp */
