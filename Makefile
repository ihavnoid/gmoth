default:
	$(MAKE) CC=gcc CXX=g++ \
		CXXFLAGS='$(CXXFLAGS) -Wall -Wextra -pipe -O3 -g -ffast-math -flto -std=c++14'  \
		LDFLAGS='$(LDFLAGS) -flto -g' \
		gmoth

debug:
	$(MAKE) CC=gcc CXX=g++ \
		CXXFLAGS='$(CXXFLAGS) -Wall -Wextra -pipe -Og -g -std=c++14' \
		LDFLAGS='$(LDFLAGS) -g' \
		gmoth

CXXFLAGS += -Wno-deprecated-copy
CPPFLAGS += -MD -MP

sources_cpp = main.cpp

objects = $(sources_cpp:.cpp=.o)
deps = $(sources_cpp:%.cpp=%.d)

-include $(deps)

%.o: %.cpp
	$(CXX) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

%.o: %.c
	$(CC) $(CXXFLAGS) $(CPPFLAGS) -c -o $@ $<

gmoth: $(objects) $(LIBS)
	$(CXX) $(LDFLAGS) -o $@ $^ $(DYNAMIC_LIBS)

clean:
	-$(RM) gmoth $(objects) $(deps)

.PHONY: clean default debug 
