//
//  network_parser.hpp
//  postprocessor
//
//  Created by Pedro Figueirêdo on 24/10/2017.
//  Copyright © 2017 Pedro Figueirêdo. All rights reserved.
//

#ifndef network_parser_hpp
#define network_parser_hpp

#include <map>
#include <set>
#include <string>
#include <fstream>

#include "object.hpp"

struct compareSet{
    bool operator()(const Object* a,const Object* b){
        return a->accuracyOrCount_ > b->accuracyOrCount_;
    }
};

class NetworkParser {
    
public:
    // objects_map is indexed by the frame (key) and has a set of objects as value
    typedef std::map< std::size_t, std::set<Object*, compareSet> > objects_map;
    
    NetworkParser(const std::string& inputFilePath);
    virtual ~NetworkParser();
    virtual bool parseObjects_(objects_map& objects) const  = 0;
    
protected:
    const std::string inputFilePath_;
};

#endif /* network_parser_hpp */

