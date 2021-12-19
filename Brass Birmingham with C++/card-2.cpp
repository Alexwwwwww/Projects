#include "card.h"

#include "util-common.h"

//// TODO: Complete the member function defintion of the card classes
Card::Card(const std::string &name) : name_(name)
{
}
Card::~Card()
{
}
// this is LocationCard
//============================================================================
// Returns true if the card can be realized as a "Wild Card".
LocationCard::LocationCard(Location *location) : Card(location->name()), location_(location){}

bool LocationCard::IsWildCard() const
{
    return false;
}

// Returns true if the card's specification allows us to overbuild
// the industry `to_be_overbuilt`, i.e., assume we are going to build
// a valid industry on it, does the card permit us to do so?
bool LocationCard::CanIndustryBeOverbuilt(const PlayerNetwork *network,
                                          const Industry *to_be_overbuilt) const
{
    // same player, already build on the same place
    bool IndTypeMatch=true;
    bool LocationInNetwork=true;
    static_cast<void>(network);
    static_cast<void>(to_be_overbuilt);
    return (IndTypeMatch && LocationInNetwork);
}

// Returns the card type.
CardType LocationCard::card_type() const
{
    return CardType::kLocationCard;
}

// Returns the name of the card.
// For wild cards, it is just "Wild Location" or "Wild Industry".
// For non-wild cards, it is just the name of the location,
// or the string representation of the industry type (check the util header file).
std::string LocationCard::name() const
{
    return name_;
 
}
//=====================this is wildlocationcard============================
WildLocationCard::WildLocationCard() : Card("Wild Location"),location_(nullptr){}

bool WildLocationCard::IsWildCard() const
{
    return true;
}


// Returns true if the card's specification allows us to overbuild
// the industry `to_be_overbuilt`, i.e., assume we are going to build
// a valid industry on it, does the card permit us to do so?
bool WildLocationCard::CanIndustryBeOverbuilt(const PlayerNetwork *network,
                                              const Industry *to_be_overbuilt) const
{
    bool IndTypeMatch=true;
    bool LocationInNetwork=true;
    static_cast<void>(network);
    static_cast<void>(to_be_overbuilt);
    return (IndTypeMatch && LocationInNetwork);
}


// Returns the card type.
CardType WildLocationCard::card_type() const
{
    return CardType::kWildLocationCard;
}

// Returns the name of the card.
// For wild cards, it is just "Wild Location" or "Wild Industry".
// For non-wild cards, it is just the name of the location,
// or the string representation of the industry type (check the util header file).
std::string WildLocationCard::name() const
{
    return name_;
}
//=======================================this is industry card===============================================

IndustryCard::IndustryCard(IndustryType industry_type):Card(ToString(industry_type)), industry_type_(industry_type){
    // for(int i=0; i<6; i++)
    // {
    //     if(industry_type_==kIndustryTypes[i])
    //     {
    //         name_=ToString(industry_type_);
    //         break;
    //     }    
    // }
}
bool IndustryCard::IsWildCard()const {
    return false;
}


bool IndustryCard::CanIndustryBeOverbuilt(const PlayerNetwork *network,
                                    const Industry *to_be_overbuilt) const {   
    // same type, tech advanced, owner own
    bool IndTypeMatch=(to_be_overbuilt->industry_type()==this->industry_type_);
    bool LocationInNetwork;
   
    static_cast<void>(network);
    static_cast<void>(to_be_overbuilt);
    if(!network->HasEstablished())
        LocationInNetwork=true;
    else
        LocationInNetwork=network->IsCovering(to_be_overbuilt->location());
    
    return (IndTypeMatch && LocationInNetwork);
        
}

// Returns the card type.
CardType IndustryCard::card_type() const {
    return CardType::kIndustryCard;
}

std::string IndustryCard::name() const {
    return name_;
}

//=====================================this is wildindustry card=================================================
WildIndustryCard::WildIndustryCard():Card("Wild Industry"),industry_type_(IndustryType::kAny){
}
bool WildIndustryCard::IsWildCard() const{
    return true;
}

bool WildIndustryCard::CanIndustryBeOverbuilt(const PlayerNetwork* network,
                              const Industry* to_be_overbuilt) const
{
    bool IndTypeMatch=true;
    bool LocationInNetwork;
    static_cast<void>(network);
    static_cast<void>(to_be_overbuilt);
    if(to_be_overbuilt==nullptr) return false;
    if(!network->HasEstablished())  
    {
        return false;
    }    
    else
    {
        if(to_be_overbuilt->location()==nullptr)
            return false;
        LocationInNetwork=network->IsCovering(to_be_overbuilt->location());
        return (IndTypeMatch && LocationInNetwork );
    }
}

CardType WildIndustryCard::card_type() const{
    return CardType::kWildIndustryCard;
}

std::string WildIndustryCard::name() const{
    return name_;
}

//// TODO ends

