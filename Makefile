CXX := g++
CXXFLAGS := --std=c++11 -g -Wall
INC := -I include
SFML_INCLUDE := -DSFML_STATIC -I "D:/Program Files (x86)/SFML/include"
SFML_LIB := -L "D:/Program Files (x86)/SFML/lib" \
	-l sfml-graphics-s-d \
	-l sfml-window-s-d \
	-l sfml-system-s-d \
	-l opengl32 \
	-l winmm \
	-l gdi32 \
	-l freetype \
	-l jpeg \

backtrack: bin/backtrack

bin/backtrack: build/backtrack.o build/mazeHelper.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_LIB)

build/backtrack.o: src/backtrack.cpp include/mazeHelper.h
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INC) $(SFML_INCLUDE)

build/mazeHelper.o: src/mazeHelper.cpp include/mazeHelper.h
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INC) $(SFML_INCLUDE)

kruskall: bin/kruskall

bin/kruskall: build/kruskall.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_LIB)

build/kruskall.o: src/kruskall.cpp include/mazeHelper.h
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INC) $(SFML_INCLUDE)

wallFollowing: bin/wallFollowing

bin/wallFollowing: build/wallFollowing.o
	$(CXX) $(CXXFLAGS) -o $@ $^ $(SFML_LIB)

build/wallFollowing.o: src/wallFollowing.cpp include/mazeHelper.h
	$(CXX) $(CXXFLAGS) -o $@ -c $< $(INC) $(SFML_INCLUDE)

clean:
	rm build/*.*
