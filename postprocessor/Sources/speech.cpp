//
//  speech.cpp
//  postprocessor
//
//  Created by Pedro Figueirêdo on 25/10/2017.
//  Copyright © 2017 Pedro Figueirêdo. All rights reserved.
//

#include "speech.hpp"

Speech::Speech(speech_objects& objects, Timestamp* begin, Timestamp* end):
id_(0),
objects_(objects),
begin_(begin),
end_(end){}

Speech::Speech(Timestamp* begin, Timestamp* end):
id_(0),
begin_(begin),
end_(end){}

Speech::~Speech(){
    delete begin_;
    delete end_;
}

void Speech::setId(const int id){
    id_ = id;
}

std::ostream& operator<< (std::ostream& stream, const Speech& v){
    stream << v.id_ << std::endl;
    stream << *v.begin_ << " --> " << *v.end_ << std::endl;
    stream << v.getTextFromObjects_() << std::endl << std::endl;
    return stream;
}

const std::string Speech::getTextFromObjects_() const{
    std::stringstream phrase;
    int count = 0;
    std::random_device rd_;
    std::mt19937 rng(rd_());
    std::uniform_int_distribution<int> uni(0,2);
    auto random = uni(rng);
    
    switch (random) {
        // Cena mostra ...
        case 0:
            phrase <<  "Cena mostra ";
            for (auto it = objects_.begin(); it!=objects_.end();it++) {
                phrase << (*it).first->accuracyOrCount_ << " " << (*it).first->name_;
                if (it + 1 != objects_.end() - 1 && it + 1 != objects_.end())
                    phrase << ", ";
                else if (it + 1 != objects_.end())
                    phrase << " e ";
            }
            phrase << ".";
            break;
        
        // Há x,y e z na cena.
        case 1:
            for (auto obj : objects_)
                count += obj.first->accuracyOrCount_;
            phrase << "Há ";
            for (auto it = objects_.begin(); it!=objects_.end();it++) {
                phrase << (*it).first->accuracyOrCount_ << " " << (*it).first->name_;
                if (it + 1 != objects_.end() - 1 && it + 1 != objects_.end())
                    phrase << ", ";
                else if (it + 1 != objects_.end())
                    phrase << " e ";
            }
            phrase << " na cena.";
            break;
            
        // São exibidos / É exibido x,y e z na cena.
        case 2:
            for (auto obj : objects_)
                count += obj.first->accuracyOrCount_;
            if (count > 1)
                phrase << "São exibidos ";
            else
                phrase << "É exibido ";
            for (auto it = objects_.begin(); it!=objects_.end();it++) {
                phrase << (*it).first->accuracyOrCount_ << " " << (*it).first->name_;
                if (it + 1 != objects_.end() - 1 && it + 1 != objects_.end())
                    phrase << ", ";
                else if (it + 1 != objects_.end())
                    phrase << " e ";
            }
            phrase << " na cena.";
            break;
    }
    
    return phrase.str();
}

// Considers speechs equal if they have the same types of objects, as they generate the same text
bool Speech::operator== (const Speech& b){
    if (this->objects_.size() != b.objects_.size())
        return false;
    
    auto auxA = this->objects_;
    auto auxB = b.objects_;
    
    std::sort(auxA.begin(), auxA.end(),[](const std::pair< Object*, int>& a, std::pair< Object*, int>& b){return a.first->name_ < b.first->name_;});
    std::sort(auxB.begin(), auxB.end(),[](const std::pair< Object*, int>& a, std::pair< Object*, int>& b){return a.first->name_ < b.first->name_;});
    
    for (int i = 0; i < this->objects_.size(); i++)
        if (auxA[i].first->name_ != auxB[i].first->name_ || auxA[i].first->accuracyOrCount_ != auxB[i].first->accuracyOrCount_)
            return false;
    
    return true;
}
