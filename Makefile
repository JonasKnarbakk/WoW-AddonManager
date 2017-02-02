NAME=Addon-Manager
CXX=g++
CXXFLAGS=-std=c++11 -Wall -Wextra -g -pthread
LDFLAGS=-lcurl `pkg-config --libs --cflags gtk+-3.0`
SOURCES=main.cpp Connection.cpp Addon.cpp HTMLParser.cpp GUI.cpp
SRC_FILES=$(addprefix src/,$(SOURCES))
TESTS=$(filter-out main.cpp,$(SOURCES))
TST_FILES=$(addprefix src/,$(TESTS))
TESTLIB=tests.cpp ReturnValue.cpp
TST_LIB=$(addprefix tests/,$(TESTLIB))

default: $(SRC_FILES)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ -o bin/$(NAME)
test: $(TST_FILES)
	$(CXX) $(CXXFLAGS) $(LDFLAGS) $^ $(TST_LIB) -o bin/$(NAME) && ./bin/$(NAME)
run:
	./bin/$(NAME)
clean:
	rm bin/$(NAME)
