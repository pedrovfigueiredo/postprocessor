//
//  yolo_parser.hpp
//  postprocessor
//
//  Created by Pedro Figueirêdo on 24/10/2017.
//  Copyright © 2017 Pedro Figueirêdo. All rights reserved.
//

#ifndef yolo_parser_hpp
#define yolo_parser_hpp

#include <vector>
#include <string>
#include <iostream>

#include "network_parser.hpp"
#include "object.hpp"

class YoloParser : public NetworkParser {
    
public:
    YoloParser(const std::string& yoloOut);
    ~YoloParser();
    bool parseObjects_(objects_map& objects) const;
};

#endif /* yolo_parser_hpp */
