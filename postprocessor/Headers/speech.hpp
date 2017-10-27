//
//  speech.hpp
//  postprocessor
//
//  Created by Pedro Figueirêdo on 25/10/2017.
//  Copyright © 2017 Pedro Figueirêdo. All rights reserved.
//

#ifndef speech_hpp
#define speech_hpp

#include <string>
#include <vector>

#include "../../gap_identifier/Headers/timestamp.hpp"
#include "object.hpp"

class Speech {
    
public:
    typedef std::vector< std::pair< Object*, int> > speech_objects;
    typedef std::vector< speech_objects > speech_objects_array;
    
    Speech(speech_objects& objects, Timestamp* begin, Timestamp* end);
    Speech(Timestamp* begin, Timestamp* end);
    ~Speech();
    friend std::ostream& operator<< (std::ostream& stream, const Speech& t);
    bool operator== (const Speech& b);
    
    void setId(const int id);
    
    speech_objects objects_;
    const Timestamp* begin_;
    const Timestamp* end_;
private:
    const std::string getTextFromObjects_() const;
    
    int id_;
};

#endif /* speech_hpp */

