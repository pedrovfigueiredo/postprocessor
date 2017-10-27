//
//  postprocessor.h
//  postprocessor
//
//  Created by Pedro Figueirêdo on 24/10/2017.
//  Copyright © 2017 Pedro Figueirêdo. All rights reserved.
//

#ifndef postprocessor_h
#define postprocessor_h

#include <string>
#include <unordered_map>

#include "../../gap_identifier/Headers/gap.hpp"
#include "../../gap_identifier/Headers/gap_identifier.hpp"
#include "../../gap_identifier/Headers/srt_gaps.hpp"
#include "../../gap_identifier/Headers/timestamp.hpp"
#include "network_parser.hpp"
#include "speech.hpp"

class PostProcessor
{
    
public:
    // Method buildAd
    // - Goal:
    //      -> Build audio description file from network output
    // - Parameters:
    //      -> parser - varies according to the network used
    //      -> identifier -  subtitle | or silence approach (coming soon) |
    //      -> output - path to audio description file generated
    
    bool buildAd_(const NetworkParser* parser, const GapIdentifier* identifier, const std::string& output);
    
private:
    bool writeSpeeches_(std::vector<Speech*>& speeches, const std::string& output);
    
    void buildSpeeches_(std::vector<Speech*>& speeches, Speech::speech_objects_array& objArray, std::vector<Gap*>& gaps, const int pace, const int minCount);
    
    void buildSpeechObjectArray_(Speech::speech_objects_array& objArray, NetworkParser::objects_map& objects , std::vector<Gap*>::iterator currGap, const int framesPerSecond, const int speechDuration, const int minAccuracy);
    
    void eraseUnusedObjects_(NetworkParser::objects_map& objects, const std::vector<Gap*>& gaps, const int framesPerSecond);
    
    int getExpectedNSpeeches_(const std::vector<Gap*>& gaps, const int speechDuration);
    
    void deallocate_(NetworkParser::objects_map& objects, Speech::speech_objects_array& objArray, std::vector<Speech*>& speeches, std::vector<Gap*>& gaps);
};


#endif /* postprocessor_h */

