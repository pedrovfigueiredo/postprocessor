//
//  srt_gaps.cpp
//  gap_identifier
//
//  Created by Pedro Figueirêdo on 24/10/2017.
//  Copyright © 2017 Pedro Figueirêdo. All rights reserved.
//

#include "../Headers/srt_gaps.hpp"

SrtGaps::SrtGaps(const std::string& videoPath ,const std::string& srtPath):
GapIdentifier(videoPath),
srtPath_(srtPath)
{}

SrtGaps::~SrtGaps(){}

bool SrtGaps::computeGaps_(std::vector<Gap*>& gaps, int minimumDuration) const{
    
    std::string line, times;
    Timestamp* speechBegin = nullptr, *speechEnd = nullptr, *gapBegin = nullptr;
    int id = 0;
    std::ifstream srtfile(this->srtPath_);
    
    if (srtfile.is_open())
    {
        gapBegin = new Timestamp();
        while ( getline (srtfile,line) )
        {
            if ( ( (line.size() == 29 || line.size() == 30) && line[0] >= '0' && line[0] <= '9') ) {
                for (auto it = line.begin() ; it != line.end(); it++) {
                    if (*it >= '0' && *it <= '9') {
                        times.push_back(*it);
                    }
                }
                speechBegin = new Timestamp(stoi(times.substr(0,2)), stoi(times.substr(2,2)),
                                                 stoi(times.substr(4,2)), stoi(times.substr(6,3)));
                speechEnd = new Timestamp(stoi(times.substr(9,2)), stoi(times.substr(11,2)),
                                               stoi(times.substr(13,2)), stoi(times.substr(15,3)));

                if ((*speechBegin - *gapBegin).toSeconds_() >= minimumDuration)
                    gaps.push_back(new Gap(++id, gapBegin, speechBegin));
                else{
                    delete speechBegin;
                    delete gapBegin;
                }
                
                
                
                gapBegin = speechEnd;
                times.clear();
            }
        }
        srtfile.close();
        
        // Compute gap between last speech and end of video
        #ifdef __APPLE__
            const std::string cmd = "/usr/local/bin/ffmpeg -i " + this->videoPath_ + " 2>&1 | grep Duration | awk '{print $2}' | tr -d ,";
        #elif __linux
            const std::string cmd = "/usr/bin/ffmpeg -i " + this->videoPath_ + " 2>&1 | grep Duration | awk '{print $2}' | tr -d ,";
        #endif
        
        std::string duration = this->exec_(cmd.c_str());
        
        speechBegin = new Timestamp(stoi(duration.substr(0,2)),stoi(duration.substr(3,2)),stoi(duration.substr(6,2)),stoi(duration.substr(9,2)));
        
        gaps.push_back(new Gap(++id, gapBegin, speechBegin));
    }
    
    else return false;
    
    return true;
}

std::string SrtGaps::exec_(const std::string cmd) const{
    std::array<char, 128> buffer;
    std::string result;
    std::shared_ptr<FILE> pipe(popen(cmd.c_str(), "r"), pclose);
    if (!pipe) throw std::runtime_error("popen() failed!");
    while (!feof(pipe.get())) {
        if (fgets(buffer.data(), 128, pipe.get()) != nullptr)
            result += buffer.data();
    }
    return result;
}
