.PHONY: run clean

SOURCES = object.cpp \
		  main.cpp \
		  symbols.cpp \
		  environment.cpp \
		  eval.cpp \
		  exception.cpp \
		  list.cpp \

TARGET = ../bin/mylisp

USE_CPP11 = 1

CC = clang++
CXX = clang++
LD = clang++

ifeq ($(USE_CPP11), 1)
	CXX_STD_FLAG = -std=c++11
	CXX_STD_LIB_FLAG = -stdlib=libc++
else
	CXX_STD_FLAG = 
	CXX_STD_LIB_FLAG = 
endif

CFLAGS := -pipe -g -Wall $(CFLAGS)
CXXFLAGS := $(CFLAGS) 
CPPFLAGS := $(CXX_STD_FLAG) $(CXX_STD_LIB_FLAG) -I$(BOOST_HOME)
LDFLAGS := $(LD_FLAGS) -L$(BOOST_HOME)/stage/lib

OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)

%.d: %.cpp
	@set -e; rm -f $@; \
         $(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		 rm -f $@.$$$$

include $(SOURCES:.cpp=.d)

run:
	./$(TARGET)

clean:
	-@rm *.o *.d *.d.* $(TARGET)
