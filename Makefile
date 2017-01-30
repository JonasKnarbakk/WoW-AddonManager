NAME=Addon-Manager
CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -g -pthread
LDFLAGS=-lcurl
SOURCES=main.cpp Connection.cpp Addon.cpp HTMLParser.cpp
SRC_FILES=$(addprefix src/,$(SOURCES))
TESTS=$(filter-out main.cpp,$(SOURCES))
TST_FILES=$(addprefix src/,$(TESTS))

default: $(SRC_FILES)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o bin/$(NAME)
test: $(TST_FILES)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ tests/tests.cpp -o bin/$(NAME) && ./bin/$(NAME)
run:
	./bin/$(NAME)
clean:
	rm bin/$(NAME)
