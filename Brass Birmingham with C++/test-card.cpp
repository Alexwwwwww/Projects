#include "card.h"

#include "industry-location.h"

#include <iostream>

// The following are some possible tests that can be performed on the Card classes.
// Although they are named as TestXXXXXX, some of them are actually the realizations of the tests,
// i.e. test cases. In other words, I just give you one test case per test.

void TestLocationCard() {
  Location* location = new IndustryLocation("A", Color::kGold);
  Card* card = new LocationCard(location);
  IndustryProxy* industry_slot = new IndustryProxy(
    IndustryType::kCoalMine, location, ResourceType::kCoal);
  PlayerNetwork* network = new PlayerNetwork();
  
  bool ok = card->CanIndustryBeOverbuilt(network, industry_slot);

  std::cout << "Description: This tests on a location card." << std::endl;
  std::cout << "* does the card allow us to build on the given industry? " << ok << std::endl;

  delete network;
  delete industry_slot;
  delete card;
  delete location;
}

void TestWildLocationCard() {
  Location* location = new IndustryLocation("A", Color::kGold);
  Card* card = new WildLocationCard;
  IndustryProxy* industry_slot = new IndustryProxy(
    IndustryType::kCoalMine, location, ResourceType::kCoal);
  PlayerNetwork* network = new PlayerNetwork();
  
  bool ok = card->CanIndustryBeOverbuilt(network, industry_slot);

  std::cout << "Description: This tests on a wild location card." << std::endl;
  std::cout << "* does the card allow us to build on the given industry? " << ok << std::endl;

  delete network;
  delete industry_slot;
  delete card;
  delete location;
}

void TestIndustryCard() {
  Location* location = new IndustryLocation("A", Color::kGold);
  Card* card = new IndustryCard(IndustryType::kManufacturer);
  IndustryProxy* industry_slot_1 = new IndustryProxy(
    IndustryType::kCoalMine, location, ResourceType::kCoal);
  IndustryProxy* industry_slot_2 = new IndustryProxy(
    IndustryType::kCoalMine, location, ResourceType::kCoal);
  PlayerNetwork* network = new PlayerNetwork();
  IndustryProperties industry_properties { 1, 1, 1, 1, 1, 1, 1, 1, 1 };
  IndustryImpl* industry = new SecondaryIndustry(
    IndustryType::kManufacturer, industry_properties);
  
  industry->Build(industry_slot_2);
  network->AddToNetwork(industry);

  bool ok = card->CanIndustryBeOverbuilt(network, industry_slot_1);

  std::cout << "Description: This tests on an industry card." << std::endl;
  std::cout << "* does the card allow us to build on the given industry? " << ok << std::endl;

  // delete industry;  // this is deleted by the network once it becomes part of the network, but not by you
  delete network;
  delete industry_slot_2;
  delete industry_slot_1;
  delete card;
  delete location;
}

void TestWildIndustryCard() {
  Location* location = new IndustryLocation("A", Color::kGold);
  Card* card = new WildIndustryCard;
  IndustryProxy* industry_slot = new IndustryProxy(
    IndustryType::kCoalMine, location, ResourceType::kCoal);
  PlayerNetwork* network = new PlayerNetwork();

  bool ok = card->CanIndustryBeOverbuilt(network, industry_slot);

  std::cout << "Description: This tests on a wild industry card." << std::endl;
  std::cout << "* does the card allow us to build on the given industry? " << ok << std::endl;

  delete network;
  delete industry_slot;
  delete card;
  delete location;
}

int main(int argc, char* argv[]) {
  std::cout << std::boolalpha;  // this let us print `true` as "true", `false` as "false"

  if (argc != 2) {
    std::cerr << "Please provide a test ID as a command-line argument to the program." << std::endl;
    return 1;
  }

  switch (std::stoi(argv[1])) {
    case 1:
      TestLocationCard();
      break;
    case 2:
      TestWildLocationCard();
      break;
    case 3:
      TestIndustryCard();
      break;
    case 4:
      TestWildIndustryCard();
      break;
    default:
      std::cerr << "Invalid option." << std::endl;
      return 1;
  }
  return 0;
}
