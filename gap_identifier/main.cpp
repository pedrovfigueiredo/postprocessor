//
//  main.cpp
//  gap_identifier
//
//  Created by Pedro Figueirêdo on 24/10/2017.
//  Copyright © 2017 Pedro Figueirêdo. All rights reserved.
//

#include <iostream>

#include "Headers/gap_identifier.hpp"
#include "Headers/srt_gaps.hpp"
#include "Headers/gap.hpp"
#include "Headers/timestamp.hpp"


int main(int argc, const char * argv[]) {
    
    const std::string videoPath = "/Users/PedroFigueiredo/Desktop/TresMinutos.mp4";
    const std::string srtPath = "/Users/PedroFigueiredo/Desktop/TresMinutos.srt";
    std::vector<Gap*> gaps;
    
    GapIdentifier* identifier = new SrtGaps(videoPath,srtPath);
    
    if (!identifier->computeGaps_(gaps, 2))
        std::cout << "Deu pau" << std::endl;
    
    return 0;
}

