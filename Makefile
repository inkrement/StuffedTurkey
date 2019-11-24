CXX = c++
CXXFLAGS = -pthread -std=c++11 -march=native
OBJS = vector.o embedding.o
INCLUDES = -I.

embagg: $(OBJS) src/main.cc
	$(CXX) $(CXXFLAGS) $(OBJS) src/main.cc -o embagg

vector.o: src/vector.cc src/vector.h
	$(CXX) $(CXXFLAGS) -c src/vector.cc

embedding.o: src/embedding.cc src/embedding.h
	$(CXX) $(CXXFLAGS) -c src/embedding.cc

clean:
	rm -rf *.o embagg

install:
	install embagg /usr/local/bin

.PHONY : clean install
