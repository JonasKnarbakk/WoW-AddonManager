NAME=Addon-Manager
CCXX=g++
CCXXFLAGS=-std=c++11 -Wall -Wextra
LDFLAGS=-lcurl
SOURCES=main.cpp Connection.cpp test.cpp
SRC_FILES=$(addprefix src/,$(SOURCES))
TESTS=$(filter-out main.cpp,$(SOURCES))
TST_FILES=$(addprefix src/,$(TESTS))

default: $(SRC_FILES)
	$(CCXX) $(CCXXFLAGS) $(LDFLAGS) $^ -o bin/$(NAME)
test: $(TST_FILES)
	$(CCXX) $(CXXFLAGS) $(LDFLAGS) $^ tests/tests.cpp -o bin/$(NAME) && ./bin/$(NAME)
run:
	./bin/$(NAME)
clean:
	rm bin/$(NAME)
