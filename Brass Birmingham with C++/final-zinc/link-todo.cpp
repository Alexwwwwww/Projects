
#include "link.h"

//// TODO: complete the member function defintions of LinkImpl and LinkProxy
//===============================   impl begin ======================================
LinkImpl::LinkImpl(LinkType link_type, LinkProperties properties):Link(link_type),properties_(properties)
{}
//LinkImpl::~LinkImpl(){}

bool LinkImpl::IsBuilt() const
{
    if(adj_==nullptr)
        return false;
    else
        return true;
       // no adj, not built
}
bool LinkImpl::Build(Link* link)
{
        
    if (CanOverbuild(link)) {
        adj_ = link->adjacency();
        link->BeOverbuilt(this);
        //static_cast<LinkProxy*>(link)->BeOverbuilt(this);
        return true;
    }
    return false;
}
bool LinkImpl::CanOverbuild(Link* link) const
{
    if(link->IsBuilt())
        return false;
    else
        return true;
}
int LinkImpl::vp() const
{
    return (adj_==nullptr)?0:adj_->vp_for_link(); 
}
LinkProperties LinkImpl::properties() const
{
    return properties_;
}
Player* LinkImpl::player() const
{
    return player_;
}
std::string LinkImpl::player_name() const
{
    if(player_==nullptr)
        return kEmptyPlayerName;
    else
        return player_->name();
}
void LinkImpl::set_player(Player* player)
{
    player_=player;
}
void LinkImpl::set_observer(LinkObserver* observer)
{
    static_cast<void>(observer);
}
//===============================   impl ends   ======================================
//===============================   proxy begin ======================================
LinkProxy::LinkProxy(LinkType link_type, Adjacency* adj):Link(link_type,adj){}
//LinkProxy::~LinkProxy(){}

bool LinkProxy::IsBuilt() const
{
    if(impl_==nullptr)      // check nullptr
        return false;       //adj is null, not built false
    else
        return (impl_->IsBuilt());   // not null ptr-> has adjacency->is built->true
}
bool LinkProxy::Build(Link* link)   //build on a link
{

    static_cast<void>(link);
    return false;
}

bool LinkProxy::CanOverbuild(Link* link) const
{
    if(link==nullptr)
        return false;
    return !(link->IsBuilt());  // is built==cannot be over built
}
void LinkProxy::BeOverbuilt(Link* link)
{
   //static_cast<void>(link);
    impl_=static_cast<LinkImpl*>(link);
    link->set_observer(observer_);  
    //link not valid
    if(impl_)
        impl_->set_adjacency(link->adjacency());
    if(observer_)
        observer_->UpdateLink(this);
    
}
int LinkProxy::vp() const
{
    if(impl_!=nullptr)
        return adj_->vp_for_link(); //only after this is built can we calculate vp?
    else
        return 0;
}
LinkProperties LinkProxy::properties() const
{
    return (impl_!=nullptr)?impl_->properties():LinkProperties();
    //return LinkProperties::LinkProperties(); //???
    
   
}

Player* LinkProxy::player() const
{
    return (impl_==nullptr)?nullptr:impl_->player();
}
std::string LinkProxy::player_name() const// returns kEmptyPlayerName if not available
{
    return (impl_==nullptr)?kEmptyPlayerName:impl_->player_name();
}
void LinkProxy::set_player(Player* player) 
{
    static_cast<void>(player);
    //(impl_==nullptr)?nullptr:impl_->player();
}
void LinkProxy::set_observer(LinkObserver* observer)
{
    observer_=observer;
    if(observer_!=nullptr)
        observer_->UpdateLink(this);
}

//======================== proxy ends ============================

//// TODO ends
