programs = adding1 adding2 adding3 adding4 colors1 colors2 colors3 colors4
CXX = clang++
CXXFLAGS = -std=c++14 -pthread
LDFLAGS = -pthread
SFML = -lsfml-graphics -lsfml-window -lsfml-system


all: $(programs)

colors1: LDFLAGS += $(SFML)
colors2: LDFLAGS += $(SFML)
colors3: LDFLAGS += $(SFML)
colors4: LDFLAGS += $(SFML)

clean:
	rm -f $(programs)
