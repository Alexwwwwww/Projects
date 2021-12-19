#include "industry.h"
#include<iostream>
#include "industry-location.h"
#include "player-hand.h"
#include "player-unbuilt.h"
#include "player-network.h"

#include <iostream>
#include <sstream>

// The following are some possible tests that can be performed on the Industry classes.
// Although they are named as TestXXXXXX, some of them are actually the realizations of the tests,
// i.e. test cases. In other words, I just give you one test case per test.

class IndustryInfoPrinter : public IndustrySubscriber {
 public:
  // Prints the cached information, which is earlier received from a IndustryObserver.
  void PrintCache() const;

 private:
  // Receives updates from an IndustryObserver.
  // The IndustryInfoPrinter just saves the information received
  // through the `industry_info` parameter into its cache.
  virtual void UpdateIndustryInfo(std::map<IndustryType, std::vector<IndustryInfo> > industry_info) override;

  std::map<IndustryType, std::vector<IndustryInfo> > cache_;
};

void IndustryInfoPrinter::UpdateIndustryInfo(std::map<IndustryType, std::vector<IndustryInfo> > industry_info) {
  cache_ = industry_info;
}

void IndustryInfoPrinter::PrintCache() const {
  std::cout << "The IndustryInfoPrinter wants to tell us about what it received from a IndustryObserver..." << std::endl;
  for (std::map<IndustryType, std::vector<IndustryInfo> >::const_reference p : cache_) {
    std::cout << "== " << ToString(p.first) << " ==" << std::endl;
    for (const IndustryInfo& info : p.second) {
      std::cout << "* location = " << info.location << std::endl;
      std::cout << "* tech level = " << info.tech_level << std::endl;
      std::cout << "* owner = " << info.owner << std::endl;
      std::cout << "* vp (from the tile's properties) = " << info.vp << std::endl;
      std::cout << "* eligible for vp? = " << info.eligible_for_vp << std::endl;
      std::cout << "* available units = " << info.available_units << std::endl;
    }
  }
}

class ResourceInfoPrinter : public ResourceSubscriber {
 public:
  // Prints the cached information, which is earlier received from a ResourceObserver.
  void PrintCache() const;

 private:
  // Receives updates from a ResourceObserver.
  // The ResourceInfoPrinter justs saves information received
  // through the `resouce_info` parameter into its cache.
  virtual void UpdateResourceInfo(std::map<ResourceType, ResourceInfo> resource_info) override;

  std::map<ResourceType, ResourceInfo> cache_;
};

void ResourceInfoPrinter::UpdateResourceInfo(std::map<ResourceType, ResourceInfo> resource_info) {
  cache_ = resource_info;
}

void ResourceInfoPrinter::PrintCache() const {
  std::cout << "The ResourceInfoPrinter wants to tell us about what it received from a ResourceObserver..." << std::endl;
  for (std::map<ResourceType, ResourceInfo>::const_reference p : cache_) {
    std::cout << "== " << ToString(p.first) << " ==" << std::endl;
    std::cout << "* industry supply = " << p.second.industry_supply << std::endl;
  }
}

// This puts (builds) a coal mine tile on an open slot of coal mine.
void TestBuildPrimaryIndustryOnOpenSlot() {
  // create an industry observer to observe the industry slot
  // and an industry subscriber which receives updates from the industry observer
  std::cout<<"78"<<std::endl;
  IndustryObserver* industry_observer = new IndustryObserver();
  IndustryInfoPrinter* industry_printer = new IndustryInfoPrinter();
  industry_observer->RegisterSubscriber(industry_printer);
  std::cout<<"82"<<std::endl;
  ResourceObserver* resource_observer = new ResourceObserver();
  ResourceInfoPrinter* resource_printer = new ResourceInfoPrinter();
  resource_observer->RegisterSubscriber(resource_printer);
  std::cout<<"86"<<std::endl;
  // create an industry slot on the map
  IndustryLocation* location = new IndustryLocation("A", Color::kGold);
  IndustryProxy* industry_slot = new IndustryProxy(
    IndustryType::kCoalMine, location, ResourceType::kCoal);
  industry_slot->set_industry_observer(industry_observer);
  industry_slot->set_resource_observer(resource_observer);
  location->AddIndustry(industry_slot);

  // create a player
  PlayerObserver* player_observer = new PlayerObserver();
  Player* player = new Player("Alice", new PlayerHand(), new PlayerUnbuilt(), new PlayerNetwork());
  player->set_observer(player_observer);

  // create an industry tile
  IndustryProperties industry_properties { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
  PrimaryIndustry* industry_tile = new PrimaryIndustry(
    IndustryType::kCoalMine, industry_properties, ResourceType::kCoal);
  industry_tile->set_player(player);

  // build the industry
  industry_tile->Build(industry_slot);

  // print what the subscribers got: global industry info & global resource info
  industry_printer->PrintCache();
  resource_printer->PrintCache();
  std::cout<<"ln 247"<<std::endl;
  std::cout << "Description: This tests building a primary industry on an open industry slot." << std::endl;
  std::cout << "* is built? " << industry_slot->IsBuilt()
            << " or "
            << industry_tile->IsBuilt() << " (should be the same)" << std::endl;
  std::cout << "* vp (not being 0 only if is eligible for vp) = " << industry_slot->vp()
            << " or "
            << industry_tile->vp()
            << " (should be the same)" << std::endl;
  std::cout << "* basic price = " << industry_slot->properties().basic_price
            << " or "
            << industry_tile->properties().basic_price
            << " (should be the same)" << std::endl;
  std::cout << "* coal cost = " << industry_slot->properties().coal_cost
            << " or "
            << industry_tile->properties().coal_cost
            << " (should be the same)" << std::endl;
  std::cout << "* player name = " << industry_slot->player_name()
            << " or "
            << industry_tile->player_name()
            << " (should be the same)" << std::endl;
  std::cout << "* available units = " << industry_slot->available_units()
            << " or " 
            << industry_tile->available_units()
            << " (should be the same)"
            << std::endl;

  delete industry_tile;

  delete industry_slot;
  delete location;

  delete player;
  delete player_observer;

  delete resource_printer;
  delete resource_observer;

  delete industry_printer;
  delete industry_observer;
}

// This puts (builds) a pottery tile on an open slot of pottery.
void TestBuildSecondaryIndustryOnOpenSlot() {
  // create an industry observer to observe the industry slot
  // and an industry subscriber which receives updates from the industry observer
  IndustryObserver* industry_observer = new IndustryObserver();
  IndustryInfoPrinter* industry_printer = new IndustryInfoPrinter();
  industry_observer->RegisterSubscriber(industry_printer);

  // create an industry slot
  IndustryLocation* location = new IndustryLocation("A", Color::kGold);
  IndustryProxy* industry_slot = new IndustryProxy(
    IndustryType::kPottery, location);
  industry_slot->set_industry_observer(industry_observer);
  location->AddIndustry(industry_slot);

  // create an industry tile
  IndustryProperties industry_properties { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
  SecondaryIndustry* industry_tile = new SecondaryIndustry(IndustryType::kPottery, industry_properties);
  
  // build the industry
  industry_tile->Build(industry_slot);
  
  // print what the subscriber got: global industry info
  industry_printer->PrintCache();

  std::cout << "Description: This tests building a secondary industry on an open industry slot." << std::endl;
  std::cout << "* is built? " << industry_slot->IsBuilt()
            << " or "
            << industry_tile->IsBuilt() << " (should be the same)" << std::endl;
  std::cout << "* vp (not being 0 only if is eligible for vp) = " << industry_slot->vp()
            << " or "
            << industry_tile->vp()
            << " (should be the same)" << std::endl;
  std::cout << "* basic price = " << industry_slot->properties().basic_price
            << " or "
            << industry_tile->properties().basic_price
            << " (should be the same)" << std::endl;
  std::cout << "* coal cost = " << industry_slot->properties().coal_cost
            << " or "
            << industry_tile->properties().coal_cost
            << " (should be the same)" << std::endl;
  std::cout << "* player name = " << industry_slot->player_name()
            << " or "
            << industry_tile->player_name()
            << " (should be the same)" << std::endl;
  std::cout << "* available units = " << industry_slot->available_units()
            << " or " 
            << industry_tile->available_units()
            << " (should be the same)"
            << std::endl;
  std::cout << "* sold = "
            << industry_tile->sold()
            << std::endl;

  delete industry_tile;

  delete industry_slot;
  delete location;

  delete industry_printer;
  delete industry_observer;
}

// This overbuilds a manufacturer on a built manufacturer of a lower tech level.
void TestOverbuildingSecondaryIndustryOnBuiltSecondaryndustry() {
  // create an industry observer to observe the industry slot
  // and an industry subscriber which receives updates from the industry observer
  IndustryObserver* industry_observer = new IndustryObserver();
  IndustryInfoPrinter* industry_printer = new IndustryInfoPrinter();
  industry_observer->RegisterSubscriber(industry_printer);

  // create an industry slot on the map
  IndustryLocation* location = new IndustryLocation("A", Color::kGold);
  IndustryProxy* industry_slot = new IndustryProxy(
    IndustryType::kManufacturer, location);
  industry_slot->set_industry_observer(industry_observer);
  location->AddIndustry(industry_slot);

  // create a player
  PlayerObserver* player_observer = new PlayerObserver();
  Player* player = new Player("Alice", new PlayerHand(), new PlayerUnbuilt(), new PlayerNetwork());
  player->set_observer(player_observer);

  // create an industry tile
  IndustryProperties industry_properties { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
  SecondaryIndustry* industry_tile = new SecondaryIndustry(
    IndustryType::kManufacturer, industry_properties);
  industry_tile->set_player(player);

  // build the industry
  industry_tile->Build(industry_slot);
  std::cout<<"ln 244"<<std::endl;
  // print what the subscriber got: global industry info
  industry_printer->PrintCache();
  std::cout<<"ln 247"<<std::endl;
  std::cout << "Description: This tests overbuilding a secondary industry on another." << std::endl;
  std::cout << "* is built? " << industry_slot->IsBuilt()
            << " or "
            << industry_tile->IsBuilt() << " (should be the same)" << std::endl;
  std::cout << "* vp (not being 0 only if is eligible for vp) = " << industry_slot->vp()
            << " or "
            << industry_tile->vp()
            << " (should be the same)" << std::endl;
  std::cout << "* basic price = " << industry_slot->properties().basic_price
            << " or "
            << industry_tile->properties().basic_price
            << " (should be the same)" << std::endl;
  std::cout << "* coal cost = " << industry_slot->properties().coal_cost
            << " or "
            << industry_tile->properties().coal_cost
            << " (should be the same)" << std::endl;
  std::cout << "* player name = " << industry_slot->player_name()
            << " or "
            << industry_tile->player_name()
            << " (should be the same)" << std::endl;
  std::cout << "* available units = " << industry_slot->available_units()
            << " or " 
            << industry_tile->available_units()
            << " (should be the same)"
            << std::endl;

  // create another industry tile
  industry_properties.tech_level = 2;
  SecondaryIndustry* industry_tile_2 = new SecondaryIndustry(
    IndustryType::kManufacturer, industry_properties);
  industry_tile_2->set_player(player);

  // cehck if we can build a higher-level industry
  std::cout << "Can over build by a higher-level industry held by the same player? "
            << industry_tile_2->CanOverbuild(industry_slot) << std::endl;
  // WARNING: as the skeleton code introduces a bug after fixing the deterministic ordering
  // of industries, you should not test overbuilding an existing industry directly.
  // instead, we just check if we can overbuild the existing industry.

  delete industry_tile_2;
  delete industry_tile;

  delete industry_slot;
  delete location;

  delete player;
  delete player_observer;

  delete industry_printer;
  delete industry_observer;
}

// This is almost the same as TestBuildPrimaryIndustryOnOpenSlot(), just that
// we ask the coal mine to provide its coal units upon it is built.
void TestPrimaryIndustryProvideResources() {
  // create an industry observer to observe the industry slot
  // and an industry subscriber which receives updates from the industry observer
  IndustryObserver* industry_observer = new IndustryObserver();
  IndustryInfoPrinter* industry_printer = new IndustryInfoPrinter();
  industry_observer->RegisterSubscriber(industry_printer);

  ResourceObserver* resource_observer = new ResourceObserver();
  ResourceInfoPrinter* resource_printer = new ResourceInfoPrinter();
  resource_observer->RegisterSubscriber(resource_printer);

  // create an industry slot on the map
  IndustryLocation* location = new IndustryLocation("A", Color::kGold);
  IndustryProxy* industry_slot = new IndustryProxy(
    IndustryType::kCoalMine, location, ResourceType::kCoal);
  industry_slot->set_industry_observer(industry_observer);
  industry_slot->set_resource_observer(resource_observer);
  location->AddIndustry(industry_slot);

  // create a player
  PlayerObserver* player_observer = new PlayerObserver();
  Player* player = new Player("Alice", new PlayerHand(), new PlayerUnbuilt(), new PlayerNetwork());
  player->set_observer(player_observer);

  // create an industry tile
  IndustryProperties industry_properties { 1, 1, 1, 1, 1, 1, 3, 1, 1 };    //wym
  PrimaryIndustry* industry_tile = new PrimaryIndustry(
    IndustryType::kCoalMine, industry_properties, ResourceType::kCoal);
  industry_tile->set_player(player);

  // build the industry
  industry_tile->Build(industry_slot);

  // print what the subscribers got: global industry info & global resource info
  industry_printer->PrintCache();
  resource_printer->PrintCache();
  std::cout<<"the available unit in industry tile "<<industry_tile->available_units()<<std::endl;
  std::cout<<"the available unit in industry slot "<<industry_slot->available_units()<<std::endl;
  // provide resources
  industry_slot->ProvideResources(1);  // the only additional line

  // print what the subscribers got: global industry info & global resource info
  industry_printer->PrintCache();
  resource_printer->PrintCache();
  std::cout<<"the available unit in industry tile "<<industry_tile->available_units()<<std::endl;
  std::cout<<"the available unit in industry slot "<<industry_slot->available_units()<<std::endl;

  std::cout << "Description: This tests building a primary industry on an open industry slot and asking it to provide resources." << std::endl;
  std::cout << "* is built? " << industry_slot->IsBuilt()
            << " or "
            << industry_tile->IsBuilt() << " (should be the same)" << std::endl;
  std::cout << "* vp (not being 0 only if is eligible for vp) = " << industry_slot->vp()
            << " or "
            << industry_tile->vp()
            << " (should be the same)" << std::endl;
  std::cout << "* basic price = " << industry_slot->properties().basic_price
            << " or "
            << industry_tile->properties().basic_price
            << " (should be the same)" << std::endl;
  std::cout << "* coal cost = " << industry_slot->properties().coal_cost
            << " or "
            << industry_tile->properties().coal_cost
            << " (should be the same)" << std::endl;
  std::cout << "* player name = " << industry_slot->player_name()
            << " or "
            << industry_tile->player_name()
            << " (should be the same)" << std::endl;
  std::cout << "* available units = " << industry_slot->available_units()
            << " or " 
            << industry_tile->available_units()
            << " (should be the same)"
            << std::endl;
  std::cout << "* player income level (EXP raises income level) = "
            << player->income_level()
            << std::endl;
  std::cout << "* player exp = "
            << player->exp()
            << std::endl;

  delete industry_tile;

  delete industry_slot;
  delete location;

  delete player;
  delete player_observer;

  delete resource_printer;
  delete resource_observer;

  delete industry_printer;
  delete industry_observer;
}

// This is just as TestBuildSecondaryIndustryOnOpenSlot(), but we sell the industry
// right after it is built.
void TestSellingSecondaryIndustry() {
  // create an industry observer to observe the industry slot
  // and an industry subscriber which receives updates from the industry observer
  IndustryObserver* industry_observer = new IndustryObserver();
  IndustryInfoPrinter* industry_printer = new IndustryInfoPrinter();
  industry_observer->RegisterSubscriber(industry_printer);

  // create an industry slot
  IndustryLocation* location = new IndustryLocation("A", Color::kGold);
  IndustryProxy* industry_slot = new IndustryProxy(
    IndustryType::kPottery, location);
  industry_slot->set_industry_observer(industry_observer);
  location->AddIndustry(industry_slot);

  // create a player
  PlayerObserver* player_observer = new PlayerObserver();
  Player* player = new Player("Alice", new PlayerHand(), new PlayerUnbuilt(), new PlayerNetwork());
  player->set_observer(player_observer);

  // create an industry tile
  IndustryProperties industry_properties { 1, 1, 1, 1, 1, 1, 3, 1, 1 };
  SecondaryIndustry* industry_tile = new SecondaryIndustry(IndustryType::kPottery, industry_properties);
  industry_tile->set_player(player);

  // build the industry
  industry_tile->Build(industry_slot);

  // print what the subscriber got: global industry info
  industry_printer->PrintCache();

  // sell the industry
  industry_tile->Sell();

  // print what the subscriber got: global industry info
  industry_printer->PrintCache();

  std::cout << "Description: This tests building a secondary industry on an open industry slot and selling it." << std::endl;
  std::cout << "* is built? " << industry_slot->IsBuilt()
            << " or "
            << industry_tile->IsBuilt() << " (should be the same)" << std::endl;
  std::cout << "* vp (not being 0 only if is eligible for vp) = " << industry_slot->vp()
            << " or "
            << industry_tile->vp()
            << " (should be the same)" << std::endl;
  std::cout << "* basic price = " << industry_slot->properties().basic_price
            << " or "
            << industry_tile->properties().basic_price
            << " (should be the same)" << std::endl;
  std::cout << "* coal cost = " << industry_slot->properties().coal_cost
            << " or "
            << industry_tile->properties().coal_cost
            << " (should be the same)" << std::endl;
  std::cout << "* player name = " << industry_slot->player_name()
            << " or "
            << industry_tile->player_name()
            << " (should be the same)" << std::endl;
  std::cout << "* available units = " << industry_slot->available_units()
            << " or " 
            << industry_tile->available_units()
            << " (should be the same)"
            << std::endl;
  std::cout << "* sold = "
            << industry_tile->sold()
            << std::endl;
  std::cout << "* player income level (EXP raises income level) = "
            << player->income_level()
            << std::endl;
  std::cout << "* player exp = "
            << player->exp()
            << std::endl;

  delete industry_tile;

  delete player;
  delete player_observer;

  delete industry_slot;
  delete location;

  delete industry_printer;
  delete industry_observer;
}

int main(int argc, char* argv[]) {
  std::cout << std::boolalpha;  // this let us print `true` as "true", `false` as "false"

  if (argc != 2) {
    std::cerr << "Please provide the test ID as a command-line argument to the program." << std::endl;
    return 1;
  }
  std::cout<<"main"<<std::endl;
  switch (std::stoi(argv[1])) {
    case 1:
      TestBuildPrimaryIndustryOnOpenSlot();
      break;
    case 2:
      TestBuildSecondaryIndustryOnOpenSlot();
      break;
    case 3:
      TestOverbuildingSecondaryIndustryOnBuiltSecondaryndustry();
      break;
    case 4:
      TestPrimaryIndustryProvideResources();
      break;
    case 5:
      TestSellingSecondaryIndustry();
      break;
    default:
      std::cerr << "Invalid option." << std::endl;
      return 1;
  }
  return 0;
}
