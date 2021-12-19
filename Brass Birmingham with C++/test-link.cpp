#include "link.h"

#include "industry-location.h"
#include "player-hand.h"
#include "player-unbuilt.h"
#include "player-network.h"

#include <iostream>

// The following are some possible tests that can be performed on the Link classes.
// Although they are named as TestXXXXXX, some of them are actually the realizations of the tests,
// i.e. test cases. In other words, I just give you one test case per test.

class LinkInfoPrinter : public LinkSubscriber {
 public:
  // Prints the cached information, which is earlier received from a LinkObserver.
  void PrintCache() const;

 private:
  // Receives updates from a LinkObserver.
  // The LinkInfoPrinter just saves the information received
  // through the `link_info` parameter into its cache.
  virtual void UpdateLinkInfo(std::vector<LinkInfo> link_info) override;

  std::vector<LinkInfo> cache_;
};

void LinkInfoPrinter::UpdateLinkInfo(std::vector<LinkInfo> link_info) {
  cache_ = link_info;
}

void LinkInfoPrinter::PrintCache() const {
  std::cout << "The LinkInfoPrinter wants to tell us about what it received from a LinkObserver..." << std::endl;
  for (const LinkInfo& info : cache_) {
    std::cout << "* link type: " << info.link_type << std::endl;
    std::cout << "* location 1: " << info.location_1 << std::endl;
    std::cout << "* location 2: " << info.location_2 << std::endl;
    std::cout << "* owner: " << info.owner << std::endl;
  }
}

// This prints the attributes of an open link slot.
void TestAttributesOfOpenLinkSlot() {
  // create a link slot dynamically
  IndustryLocation* location_1 = new IndustryLocation("A", Color::kGold);
  IndustryLocation* location_2 = new IndustryLocation("B", Color::kGold);
  Adjacency* adj = new Adjacency(location_1, location_2);
  LinkProxy* link_slot = new LinkProxy(LinkType::kCanal, adj);
  location_1->AddLink(link_slot);
  location_2->AddLink(link_slot);

  LinkProperties properties = link_slot->properties();
  std::cout << "Description: This tests the attributes of an open link slot." << std::endl;
  std::cout << "* is built? " << link_slot->IsBuilt() << std::endl;
  std::cout << "* vp = " << link_slot->vp() << std::endl;
  std::cout << "* basic price = " << properties.basic_price << std::endl;
  std::cout << "* coal cost = " << properties.coal_cost << std::endl;
  std::cout << "* player name = " << link_slot->player_name() << std::endl;

  delete link_slot;
  delete adj;
  delete location_2;
  delete location_1;
}

// This prints the attributes of an unplayed (unbuilt) link tile.
void TestAttributesOfUnbuiltLinkTile() {
  // create a player
  Player* player = new Player("Alice", nullptr, new PlayerUnbuilt(), nullptr);

  // create a link tile object dynamically
  LinkProperties properties { 10, 1 };
  LinkImpl* link_tile = new LinkImpl(LinkType::kCanal, properties);
  link_tile->set_player(player);  // the PlayerUnbuilt class does the same thing: set the player after a link tile is created
  
  //std::cout << "* player name = "<<link_tile->player_name()<< std::endl;
  std::cout << "Description: This tests the attributes of an unbuilt link tile." << std::endl;
  std::cout << "* is built? " << link_tile->IsBuilt() << std::endl;
  std::cout << "* vp = " << link_tile->vp() << std::endl;
  std::cout << "* basic price = " << properties.basic_price << std::endl;
  std::cout << "* coal cost = " << properties.coal_cost << std::endl;
  std::cout << "* player name = " << link_tile->player_name() << std::endl;

  delete link_tile;
  delete player;
}

// This puts an link tile on an open link slot.
void TestBuildingLinkOnOpenSlot() {
  // create a LinkObserver to observe `link_slot`
  LinkObserver* observer = new LinkObserver();

  // create a LinkSubscriber which receives messages from the `observer`
  LinkInfoPrinter* printer = new LinkInfoPrinter();
  observer->RegisterSubscriber(printer);

  // create a link slot
  IndustryLocation* location_1 = new IndustryLocation("A", Color::kGold);
  IndustryLocation* location_2 = new IndustryLocation("B", Color::kGold);
  Adjacency* adj = new Adjacency(location_1, location_2);
  LinkProxy* link_slot = new LinkProxy(LinkType::kCanal, adj);
  location_1->AddLink(link_slot);
  location_2->AddLink(link_slot);
  link_slot->set_observer(observer);

  // create a player to hold the link tile
  Player* player = new Player("Alice", nullptr, new PlayerUnbuilt(), nullptr);

  // create a link tile object dynamically
  LinkProperties properties { 10, 1 };
  LinkImpl* link_tile = new LinkImpl(LinkType::kCanal, properties);
  link_tile->set_player(player);  // the PlayerUnbuilt class does the same thing: set the player after a link tile is created
  
  // build the link
  link_tile->Build(link_slot);
  
 
  std::cout << "Description: This tests the result of building a link on an open slot." << std::endl;
  std::cout << "* is built? " << link_slot->IsBuilt()
            << " or "
            << link_tile->IsBuilt()
            << " (should be the same)" << std::endl;
  std::cout << "* vp = " << link_slot->vp()
            << " or "
            << link_tile->vp()
            << " (should be the same)" << std::endl;
  std::cout << "* basic price = " << link_slot->properties().basic_price
            << " or "
            << link_tile->properties().basic_price
            << " (should be the same)" << std::endl;
  std::cout << "* coal cost = " << link_slot->properties().coal_cost
            << " or "
            << link_tile->properties().coal_cost
            << " (should be the same)" << std::endl;
  std::cout << "* player name = " << link_slot->player_name()
            << " or "
            << link_tile->player_name()
            << " (should be the same)" << std::endl;

  delete link_tile;
  delete player;

  delete link_slot;
  delete adj;
  delete location_2;
  delete location_1;

  delete printer;
  delete observer;
}

// This checks if we can put a link tile over an occupied link slot, i.e. with a link tile on it.
void TestOverbuildingLinkOnBuiltLink() {
  // create a LinkObserver to observe `link_slot`
  LinkObserver* observer = new LinkObserver();

  // create a LinkSubscriber which receives messages from the `observer`
  LinkInfoPrinter* printer = new LinkInfoPrinter();
  observer->RegisterSubscriber(printer);

  // create a link slot dynamically
  Location* location_1 = new IndustryLocation("A", Color::kGold);
  Location* location_2 = new IndustryLocation("B", Color::kGold);
  Adjacency* adj = new Adjacency(location_1, location_2);
  LinkProxy* link_slot = new LinkProxy(LinkType::kCanal, adj);
  location_1->AddLink(link_slot);
  location_2->AddLink(link_slot);
  link_slot->set_observer(observer);

  // create two link tile objects dynamically
  Player* player = new Player("Alice", nullptr, new PlayerUnbuilt(), nullptr);
  LinkProperties properties { 10, 1 };
  LinkImpl* link_tile = new LinkImpl(LinkType::kCanal, properties);
  LinkImpl* link_tile_2 = new LinkImpl(LinkType::kCanal, properties);
  link_tile->set_player(player);  // the PlayerUnbuilt class does the same thing: set the player after a link tile is created
  link_tile_2->set_player(player);

  // build the link
  link_tile->Build(link_slot);
  std::cout << "Description: This tests if we can overbuild an already built link." << std::endl;
  std::cout << link_tile_2->CanOverbuild(link_tile) << std::endl;

  delete link_tile_2;
  delete link_tile;
  delete player;

  delete link_slot;
  delete adj;
  delete location_2;
  delete location_1;

  delete printer;
  delete observer;
}

// This tests if we can obtain the vp for a link from its connected locations correctly.
// We have 3 industry slots in location A.
// 1. a manufacturer built by Alice and is eligible for VP
// 2. a manufacturer built by Alice and is not eligible for VP
// 3. a manufacturer built by Bob and is eligible for VP
// Location B has no industry slot.
// This test case aims to find the vp for the link that is connected to A and B.
void TestLinkVp() {
  // create a LinkObserver to observe `link_slot`
  LinkObserver* observer = new LinkObserver();

  // create a LinkSubscriber which receives messages from the `observer`
  LinkInfoPrinter* printer = new LinkInfoPrinter();
  observer->RegisterSubscriber(printer);

  // create a player
  PlayerObserver* player_observer = new PlayerObserver();
  Player* alice = new Player("Alice", new PlayerHand(), new PlayerUnbuilt(), new PlayerNetwork());
  alice->set_observer(player_observer);
  Player* bob = new Player("Bob", new PlayerHand(), new PlayerUnbuilt(), new PlayerNetwork());
  bob->set_observer(player_observer);

  // create a link slot
  IndustryLocation* location_1 = new IndustryLocation("A", Color::kGold);
  IndustryLocation* location_2 = new IndustryLocation("B", Color::kGold);
  Adjacency* adj = new Adjacency(location_1, location_2);
  LinkProxy* link_slot = new LinkProxy(LinkType::kCanal, adj);
  location_1->AddLink(link_slot);
  location_2->AddLink(link_slot);
  link_slot->set_observer(observer);

  // create a VP-eligible industry adjacent to the link
  IndustryProxy* industry_slot_1 = new IndustryProxy(IndustryType::kManufacturer, location_1);
  location_1->AddIndustry(industry_slot_1);  // this is a must, as the location needs to know
                                             // what industries are there so that it can
                                             // sum up the "vp for adjacent links"
  IndustryProxy* industry_slot_2 = new IndustryProxy(IndustryType::kManufacturer, location_1);
  location_1->AddIndustry(industry_slot_2);

  IndustryProxy* industry_slot_3 = new IndustryProxy(IndustryType::kManufacturer, location_1);
  location_1->AddIndustry(industry_slot_3);

  // create a link tile
  LinkProperties properties { 10, 1 };
  LinkImpl* link_tile = new LinkImpl(LinkType::kCanal, properties);
  link_tile->set_player(alice);
  
  // create an industry tile
  IndustryProperties industry_properties { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
  IndustryImpl* industry_tile_1 = new SecondaryIndustry(
    IndustryType::kManufacturer, industry_properties);
  industry_tile_1->set_player(alice);
  IndustryImpl* industry_tile_2 = new SecondaryIndustry(
    IndustryType::kManufacturer, industry_properties);
  industry_tile_2->set_player(alice);
  IndustryImpl* industry_tile_3 = new SecondaryIndustry(
    IndustryType::kManufacturer, industry_properties);
  industry_tile_3->set_player(bob);

  // build the link
  link_tile->Build(link_slot);

  // build and sell the industry
  // so as to create a VP-eligible industry adjacent to the link
  industry_tile_1->Build(industry_slot_1);
  industry_tile_2->Build(industry_slot_2);
  industry_tile_3->Build(industry_slot_3);
  industry_tile_1->Sell();
  industry_tile_3->Sell();

  std::cout << "Description: This tests the correctness of Link VP obtained from adjacent industries." << std::endl;
  std::cout << "* link vp = " << link_slot->vp()
            << " or "
            << link_tile->vp()
            << " (should be the same)" << std::endl;

  delete industry_tile_1;
  delete industry_slot_1;
  delete industry_tile_2;
  delete industry_slot_2;
  delete industry_tile_3;
  delete industry_slot_3;

  delete link_tile;
  delete link_slot;

  delete adj;
  delete location_2;
  delete location_1;

  delete bob;
  delete alice;
  delete player_observer;

  delete printer;
  delete observer;
}

int main(int argc, char* argv[]) {
  std::cout << std::boolalpha;  // this let us print `true` as "true", `false` as "false"

  if (argc != 2) {
    std::cerr << "Please provide a test ID as a command-line argument to the program." << std::endl;
    return 1;
  }

  switch (std::stoi(argv[1])) {
    case 1:
      TestAttributesOfOpenLinkSlot();
      break;
    case 2:
      TestAttributesOfUnbuiltLinkTile();
      break;
    case 3:
      TestBuildingLinkOnOpenSlot();
      break;
    case 4:
      TestOverbuildingLinkOnBuiltLink();
      break;
    case 5:
      TestLinkVp();
      break;
    default:
      std::cerr << "Invalid option." << std::endl;
      return 1;
  }
  return 0;
}
