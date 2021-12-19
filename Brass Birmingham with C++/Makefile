
CXX = g++

CXXFLAGS = -std=c++11 -Wall -Wextra -pedantic -Wno-switch
# EXTRA_CXXFLAGS = -fsanitize=address,leak,undefined

SRC_DIR = .

OBJ_FILES = card.o location.o util-common.o game-observer.o link.o link-todo.o \
  infrastructure.o player.o industry.o player-network.o player-unbuilt.o player-hand.o \
	industry-location.o industry-todo.o

SRC_FILES = $(addprefix $(SRC_DIR)/, $(OBJ_FILES:.o=.cpp))

%.o: $(SRC_DIR)/%.cpp $(SRC_DIR)/%.h
	$(CXX) $(CXXFLAGS) -c -o $@ $<

test-%: $(SRC_DIR)/test-%.cpp $(OBJ_FILES)
	$(CXX) $(CXXFLAGS) -o $@ $^

depend:
	$(CXX) $(CXXFLAGS) -MM $(SRC_FILES) -c >.depend.mk

clean:
	rm -f *.o test-card test-industry test-link
