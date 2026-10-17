CXX      := g++
CXXFLAGS := -std=c++17 -Wall
GLFW     := $(shell pkg-config --cflags --libs glfw3)
LIBS     := -lmujoco $(GLFW)

sim: main.cpp
	$(CXX) $(CXXFLAGS) main.cpp -o sim $(LIBS)

clean:
	rm -f sim