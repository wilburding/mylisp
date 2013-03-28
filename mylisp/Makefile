.PHONY: run clean

SOURCES = $(wildcard *.cpp)

TARGET = ../bin/mylisp

ifdef USE_CLANG
	CC = clang++
	CXX = clang++
	LD = clang++
else
	CC = g++-mp-4.8
	CXX = g++-mp-4.8
	LD = g++-mp-4.8
endif

CFLAGS := -pipe -g -Wall -Wextra $(CFLAGS)
CXXFLAGS := $(CFLAGS) 
ifdef USE_CLANG
	CPPFLAGS := -std=c++11 -stdlib=libc++ -I$(BOOST_HOME)
else
	CPPFLAGS := -std=c++11 -I$(BOOST_HOME)
endif
LDFLAGS := $(LD_FLAGS)

OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(LD) $(CPPFLAGS) $(CXXFLAGS) $(LDFLAGS) $^ -o $@

%.d: %.cpp
	@set -e; rm -f $@; \
         $(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		 rm -f $@.$$$$

include $(SOURCES:.cpp=.d)

run:
	./$(TARGET)

clean:
	-rm *.o *.d 
	-rm $(TARGET)
