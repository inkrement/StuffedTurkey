CXX = c++
# -Wall
CXXFLAGS = -pthread -std=c++17 -march=native
OBJS = vector.o embedding.o
INCLUDES = -I.

stuffedturkey: $(OBJS) src/main.cc
	$(CXX) $(CXXFLAGS) $(OBJS) src/main.cc -o stuffedturkey

vector.o: src/vector.cc src/vector.h
	$(CXX) $(CXXFLAGS) -c src/vector.cc

embedding.o: src/embedding.cc src/embedding.h src/vector.h
	$(CXX) $(CXXFLAGS) -c src/embedding.cc

clean:
	rm -rf *.o stuffedturkey

install:
	install stuffedturkey /usr/local/bin

.PHONY : clean install
