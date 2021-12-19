
#include "industry.h"

#include "player.h"

//// TODO: Complete primary and secondary industries
PrimaryIndustry::PrimaryIndustry(IndustryType industry_type, IndustryProperties properties,
                                 ResourceType resource_type) : IndustryImpl(industry_type, properties, resource_type){ProduceResources();}
//PrimaryIndustry::~PrimaryIndustry(){};

// This can overbuild the `other` industry if this is more technologically advanced,
// and they are of the same industry type. Usually the owner of this industry must
// also own the industry that he overbuilds on. However, if the game board has
// exhausted all the resources, or the `other` industry is not yet built
// (i.e. representing an empty slot), any player can overbuild the given industry
// but is subject to the same constraints.
// Hint: As we alway overbuild through an industry proxy, an an industry proxy always
//       has a resource observer attached, we can assume `other` is assoicated
//       to a valid resource observer.
bool PrimaryIndustry::CanOverbuild(Industry *other) const
{
    //more technology, same industry type 
    bool hitech, sametype;
    hitech=(this->properties().tech_level > other->properties().tech_level);
    sametype=(this->industry_type() == other->industry_type());
    if(!(sametype && hitech))
        return false;
    if(other->player()==nullptr) // an open slot
        return true;
    else if(other->player()!=this->player()) // owned by another ppl
    {
        //what if
        if(hitech && sametype)
        {
            
            if(static_cast<PrimaryIndustry*>(other->impl())==nullptr)//something is wrong?
                return false;
            if(static_cast<PrimaryIndustry*>(other->impl())->resource_observer_->HasResource(other->resource_type()))//not exhausted, gg
                return false;
            else //exhausted
                return true;
        }    
        else 
            return false;// not even satisfy hitech and sametype
    }
    else
        return true;
        
}

// Returns true if all of its resource units are consumed.
bool PrimaryIndustry::IsEligibleForVp() const
{

    if (available_units_ == 0 && this->IsBuilt())
        return true;
    else
        return false;
}

// Produces the number of output units specified by its properties,
// and notifies the resource and industry observers of the change.
void PrimaryIndustry::ProduceResources()
{

    available_units_=properties_.output_units;
   if(industry_observer_==nullptr || resource_observer_==nullptr)
        return ;
    industry_observer_->UpdateIndustry(this);
    resource_observer_->UpdateResourceCount(this->resource_type(),available_units_); //add all the resource
    resource_observer_->Notify();
    industry_observer_->Notify();
}

// Removes all the resource units available and notifies the
// resource and industry observers of the change.
void PrimaryIndustry::RemoveResources(){
   
    resource_observer_->UpdateResourceCount(this->resource_type(),-available_units_); //subtract all the resource
    available_units_=0; //available_units_=available_units_-available_units_;
    industry_observer_->UpdateIndustry(this);
    //resource_observer_->UpdateResourceCount(this->resource_type(),available_units_);
    resource_observer_->Notify();
    industry_observer_->Notify();
}

// Provides the given nubmer of resource units if possible.
// Notifies the resource and industry observes of the change.
bool PrimaryIndustry::ProvideResources(int units){
    if(available_units_>units)
    {
        available_units_=available_units_-units;

        industry_observer_->UpdateIndustry(this);
        resource_observer_->UpdateResourceCount(this->resource_type(),-units); //update the units provided
        resource_observer_->Notify();
        industry_observer_->Notify();
        return true;
    }
    else if(available_units_==units)
    {
        available_units_=0;

        this->player_->IncreaseExp(properties_.exp_increase);
        industry_observer_->UpdateIndustry(this);
        resource_observer_->UpdateResourceCount(this->resource_type(),-units); //update the units provided 
        
        resource_observer_->Notify();
        industry_observer_->Notify();
        return true;
    }    
    else
    {
        industry_observer_->UpdateIndustry(this);
        resource_observer_->UpdateResourceCount(this->resource_type(),0);
        resource_observer_->Notify();
        industry_observer_->Notify();
        return false;  
    }
        
}

// Returns the inventory of the industry.
int PrimaryIndustry::available_units() const {
    return available_units_;
}

// Sets the resource observer of the industry.
void PrimaryIndustry::set_resource_observer(ResourceObserver *rsrc_observer)
{
    resource_observer_=rsrc_observer;
}
//============================Secondary Industry========================================
SecondaryIndustry::SecondaryIndustry(IndustryType industry_type, IndustryProperties properties) : IndustryImpl(industry_type, properties)
{}
//SecondaryIndustry::~SecondaryIndustry(){}

// This is similar to the function you solved for PrimaryIndustry.
// Just that we do not allow other players who do not own the `other`
// industry to build on it, if it is already built.
bool SecondaryIndustry::CanOverbuild(Industry *other) const
{
    bool hitech, sametype;
    if(other==nullptr)
        return false;
    hitech=(this->properties().tech_level > other->properties().tech_level);
    sametype=(this->industry_type() == other->industry_type());
    if(!(hitech && sametype))
        return false;
    if(other->player()==nullptr) // an open slot
        return true;
    else if(other->player()!=this->player())
        return false;
    else
    {   //owned by the same ppl
        
        return true;
    }
    
}

// Returns true if the industry is sold.
bool SecondaryIndustry::IsEligibleForVp() const
{
    return (sold() && IsBuilt());
}

// Sells the industry, and increase the player's EXP by a number
// specified in the industry's properties.
bool SecondaryIndustry::Sell()
{
    if(!sold_)
    {
        player_->IncreaseExp(properties_.exp_increase);
        sold_=true;
        industry_observer_->UpdateIndustry(this);
        industry_observer_->Notify();
        return true;
    }
    else{
        return false;
    }
}

// Returns true if the industry is already sold.
bool SecondaryIndustry::sold() const
{
    return sold_;
}

// Ignores the resource observer as a secondary industry does not produce
// consumerable resources for players.
void SecondaryIndustry::set_resource_observer(ResourceObserver *rsrc_observer)
{
    static_cast<void>(rsrc_observer);
    
}

//// TODO ends