//
//  postprocessor.cpp
//  postprocessor
//
//  Created by Pedro Figueirêdo on 24/10/2017.
//  Copyright © 2017 Pedro Figueirêdo. All rights reserved.
//

#include "../Headers/postprocessor.h"

bool PostProcessor::buildAd_(const NetworkParser* parser, const GapIdentifier* identifier, const std::string& output){
    const int framesPerSecond = 25, speechDuration = 2, minAccuracy = 60, minCount = 10, repetitionTime = 10;
    std::vector<Gap*> gaps;
    NetworkParser::objects_map objects;
    // Contains objects used in speechs, quantified by frequency.
    Speech::speech_objects_array speechObjectsArray;
    std::vector<Speech*> speeches;
    
    
    // Computing gaps
    if (!identifier->computeGaps_(gaps, speechDuration)){
        std::clog << " [POSTPROCESSOR] Failed to open original subtitle.\n";
        return false;
    }
    
    // Parsing yolo output
    if (!parser->parseObjects_(objects)){
        std::clog << " [POSTPROCESSOR] Failed to open yolo output.\n";
        return false;
    }
    
    // Erasing unused objects
    eraseUnusedObjects_(objects, gaps, framesPerSecond);
    
    // Allocating space for speechObjectsArray
    speechObjectsArray.resize(getExpectedNSpeeches_(gaps,speechDuration));
    
    // Building speechObjectsArray
    buildSpeechObjectArray_(speechObjectsArray, objects, gaps.begin(), framesPerSecond, speechDuration, minAccuracy);
    
    // Building speeches vector
    buildSpeeches_(speeches, speechObjectsArray, gaps, speechDuration, minCount, repetitionTime);
    
    // Writing Speeches to srtFile
    if (!writeSpeeches_(speeches, output)){
        std::clog << " [POSTPROCESSOR] Failed to open output AD subtitle.\n";
        return false;
    }
    
    // Deallocating dynamically allocated memory
    deallocate_(objects, speechObjectsArray, speeches, gaps);
    
    return true;
}

bool PostProcessor::writeSpeeches_(std::vector<Speech*>& speeches, const std::string& output){
    std::fstream file(output, std::fstream::out);
    
    if (file.is_open())
        for (auto speech : speeches)
            file << *speech;
    else
        return false;
    
    file.close();
    return true;
}

void PostProcessor::buildSpeeches_(std::vector<Speech*>& speeches, Speech::speech_objects_array& objArray, std::vector<Gap*>& gaps, const int pace, const int minCount, const int repetitionTime){
    int count = 0;
    Speech* currSpeech;
    
    // Building speeches from objects_array
    for (auto gap : gaps)
        for (int i = (gap->getBegin_()).toSeconds_(); i < (int)(gap->getEnd_()).toSeconds_() - 1; i += pace, count++)
            if (objArray[count].size()){
                currSpeech = new Speech( new Timestamp(i/3600,i/60, i % 60, 0), new Timestamp((i+pace)/3600, (i+pace)/60, (i+pace)%60, 0));
                for (auto obj : objArray[count])
                    // Only considers objects with at least minCount
                    if (obj.second >= minCount)
                        currSpeech->objects_.push_back(obj);
                
                // Only pushes to speeches array if at least one object was added to current speech
                if (currSpeech->objects_.size())
                    speeches.push_back(currSpeech);
                else
                    // Destructor deletes both Timestamps aswell
                    delete currSpeech;
            }
    
     // Choosing between multiple instances of same object and different count
     for (auto& speech : speeches)
         if (speech->objects_.size() > 1){
             // Sorts to place the best objects matches first
             std::sort(speech->objects_.begin(), speech->objects_.end(), [](const std::pair< Object*, int>& a, const std::pair< Object*, int>& b) {return a.second > b.second;});
             for (auto it = speech->objects_.begin(); it != speech->objects_.end(); it++){
                 auto duplicate = std::find_if(it + 1, speech->objects_.end(), [it] (const std::pair< Object*, int>& a) {return (*it).first->name_ == a.first->name_;});
                 // As long as objects duplicates exist, erase them, since they are not the best match
                 while (duplicate != speech->objects_.end()){
                     speech->objects_.erase(duplicate);
                     duplicate = std::find_if(it + 1, speech->objects_.end(), [it] (const std::pair< Object*, int>& a) {return (*it).first->name_ == a.first->name_;});
                 }
             }
         }
    
    // Erasing speech duplicates, if neighbours
    for (auto it = speeches.begin() + 1; it != speeches.end();)
        // If Speeches generate the same text AND there is a gap in between whose duration lasts no longer than repetitionTime seconds, the duplicate gets removed
        if (*(*(it-1)) == *(*it) && ((*it)->begin_->toSeconds_() - (*(it-1))->end_->toSeconds_()) <= repetitionTime)
            it = speeches.erase(it);
        else
            it++;
    
    // Setting the id for all speeches
    int id = 0;
    for (auto speech : speeches)
        speech->setId(++id);
}

void PostProcessor::buildSpeechObjectArray_(Speech::speech_objects_array& objArray, NetworkParser::objects_map& objects , std::vector<Gap*>::iterator currGap, const int framesPerSecond, const int speechDuration, const int minAccuracy){
    int offset = ((*currGap)->getBegin_()).toSeconds_() * framesPerSecond;
    int reference, speechId;
    std::vector< Object* > relevantObjects;
    
    for (NetworkParser::objects_map::iterator it = objects.begin(); it != objects.end(); it++){
        // Changed Gap
        if (it->first > ((*currGap)->getEnd_()).toSeconds_() * framesPerSecond){
            offset += ((*currGap)->getDuration_()).toSeconds_() * framesPerSecond;
            currGap++;
        }
        reference = ((*currGap)->getBegin_()).toSeconds_() * framesPerSecond;
        
        // Controls the index to insert elements into objArray
        speechId = (int) (it->first - reference + offset) / (framesPerSecond * speechDuration);
        
        // Builds the relevantObjects structure, which ignores objects under minAccuracy and sets their frequency in the pair.
        for (auto obj : it->second)
            if (obj->accuracyOrCount_ >= minAccuracy){
                auto position = std::find_if(relevantObjects.begin(), relevantObjects.end(),[obj](Object* currObj){return obj->name_ == currObj->name_;});
                if ( position == relevantObjects.end())
                    relevantObjects.push_back(new Object(obj->name_, 1));
                else
                    (*position)->accuracyOrCount_++;
                
            }
        
        // Uses the relevantObjects structure to account and label each relevantObject, tying up with its frequency and populating objArray.
        for (auto obj : relevantObjects){
            auto position = std::find_if(objArray[speechId].begin(), objArray[speechId].end(),[obj](const std::pair< Object*, int>& currObj){return obj->name_ == currObj.first->name_ && obj->accuracyOrCount_ == currObj.first->accuracyOrCount_;});
            if ( position == objArray[speechId].end())
                objArray[speechId].push_back(std::make_pair(obj,1));
            else
                position->second++;
        }
        
        relevantObjects.clear();
    }
}

void PostProcessor::eraseUnusedObjects_(NetworkParser::objects_map& objects, const std::vector<Gap*>& gaps, const int framesPerSecond){
    bool valid = false;
    std::vector< std::pair<std::size_t, std::size_t> > validIntervals;
    
    for (auto gap : gaps)
        validIntervals.push_back({gap->getBegin_().toSeconds_() * framesPerSecond, gap->getEnd_().toSeconds_() * framesPerSecond});
    
    for (auto it = objects.begin(); it != objects.end();){
        for (auto interval : validIntervals)
            if (it->first >= interval.first && it->first <= interval.second){
                valid = true;
                break;
            }
        if (!valid)
            it = objects.erase(it);
        else
            it++;
        valid = false;
    }
}

int PostProcessor::getExpectedNSpeeches_(const std::vector<Gap*>& gaps, const int speechDuration){
    int s = 0;
    for (auto gap : gaps)
        s += (gap->getDuration_()).toSeconds_();
    return s / speechDuration;
}

void PostProcessor::deallocate_(NetworkParser::objects_map& objects, Speech::speech_objects_array& objArray, std::vector<Speech*>& speeches, std::vector<Gap*>& gaps){
    // Deallocating objects from objects map
    for (auto object_set : objects)
        for (auto object : object_set.second)
            delete object;
    
    // Deallocating objects from speechObjectsArray
    for (auto speechObjects : objArray)
        for (auto pair : speechObjects)
            delete pair.first;
    
    // Deallocating speeches
    for (auto speech : speeches)
        delete speech;
    
    // Deallocating gaps
    for (auto gap : gaps)
        delete gap;
}
