//
//  yolo_parser.cpp
//  postprocessor
//
//  Created by Pedro Figueirêdo on 24/10/2017.
//  Copyright © 2017 Pedro Figueirêdo. All rights reserved.
//

#include "yolo_parser.hpp"

YoloParser::YoloParser(const std::string& yoloOut):
NetworkParser(yoloOut){}

YoloParser::~YoloParser(){}

bool YoloParser::parseObjects_(objects_map& objects) const{
    std::ifstream yoloOut(this->inputFilePath_);
    std::string line, objectName;
    std::size_t found, currFrame = 0;
    int accuracy;
    
    if (yoloOut.is_open()) {
        while (getline(yoloOut, line)) {
            found = line.find(".jpg");
            // Check line type
            if (found != std::string::npos)
                currFrame = stoi(line.substr(8,found - 8));
            else{
                found = line.find(':');
                objectName = line.substr(0, found);
                accuracy = stoi(line.substr(found + 2, 2));
                
                if (objects.find(currFrame) == objects.end())
                    objects[currFrame] = {new Object(objectName,accuracy)};
                else
                    (objects[currFrame]).insert(new Object(objectName,accuracy));
                
            }
        }
    }else return false;
    
    return true;
}
