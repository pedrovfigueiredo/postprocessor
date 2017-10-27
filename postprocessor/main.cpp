//
//  main.cpp
//  postprocessor
//
//  Created by Pedro Figueirêdo on 23/10/2017.
//  Copyright © 2017 Pedro Figueirêdo. All rights reserved.
//

#include <iostream>

#include "Headers/postprocessor.h"
#include "Headers/yolo_parser.hpp"

int main(int argc, const char * argv[]) {
    const std::string videoPath = "/Users/PedroFigueiredo/Desktop/TresMinutos.mp4";
    const std::string srtPath = "/Users/PedroFigueiredo/Desktop/TresMinutos.srt";
    const std::string yoloOut = "/Users/PedroFigueiredo/Desktop/Yolo.out";
    const std::string output = "/Users/PedroFigueiredo/Desktop/";
    PostProcessor* p = new PostProcessor();
    
    p->buildAd_(new YoloParser(yoloOut), new SrtGaps(videoPath,srtPath), output);
    
    return 0;
}
