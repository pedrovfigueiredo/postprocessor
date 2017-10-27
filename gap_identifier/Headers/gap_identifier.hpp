//
//  gap_identifier.hpp
//  gap_identifier
//
//  Created by Pedro Figueirêdo on 24/10/2017.
//  Copyright © 2017 Pedro Figueirêdo. All rights reserved.
//

#ifndef gap_identifier_hpp
#define gap_identifier_hpp
#include <vector>
#include <string>

#include "gap.hpp"

class GapIdentifier
{
    
public:
    GapIdentifier(const std::string& videoPath);
    virtual ~GapIdentifier() = 0;
    
    virtual bool computeGaps_(std::vector<Gap*>& gaps, int minimumDuration) const = 0;
    
protected:
    const std::string videoPath_;
};

#endif /* gap_identifier_hpp */
