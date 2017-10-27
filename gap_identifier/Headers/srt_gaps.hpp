//
//  srt_gaps.hpp
//  gap_identifier
//
//  Created by Pedro Figueirêdo on 24/10/2017.
//  Copyright © 2017 Pedro Figueirêdo. All rights reserved.
//

#ifndef srt_gaps_hpp
#define srt_gaps_hpp

#include <string>
#include <sstream>
#include <fstream>
#include <cstdio>
#include <iostream>
#include <memory>
#include <stdexcept>
#include <array>

#include "gap_identifier.hpp"
#include "gap.hpp"
#include "timestamp.hpp"

class SrtGaps : public GapIdentifier
{
    
public:
    SrtGaps(const std::string& videoPath ,const std::string& srtPath);
    ~SrtGaps();
    
    bool computeGaps_(std::vector<Gap*>& gaps, int minimumDuration) const;
    
private:
    
    std::string exec_(const std::string cmd) const;
    
    const std::string srtPath_;
};

#endif /* srt_gaps_hpp */
