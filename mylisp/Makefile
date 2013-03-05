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
	CPP_STD_FLAG = -std=c++11
	CPP_STD_LIB_FLAG = -stdlib=libc++
else
	CPP_STD_FLAG = 
	CPP_STD_LIB_FLAG = 
endif

CCFLAGS := -pipe -g -Wall $(CCFLAGS)
CPPFLAGS := $(CPP_STD_FLAG) $(CPP_STD_LIB_FLAG) $(CCFLAGS) -I$(BOOST_HOME)
#CXXFLAGS := $(CPPFLAGS)
LDFLAGS := $(CPP_STD_FLAG) $(CPP_STD_LIB_FLAG) $(LD_FLAGS) -L$(BOOST_HOME)/stage/lib

OBJECTS = $(SOURCES:.cpp=.o)

$(TARGET): $(OBJECTS)
	$(LD) $(LDFLAGS) -o $@ $(OBJECTS)

%.d: %.cpp
	@set -e; rm -f $@; \
         $(CC) -MM $(CPPFLAGS) $< > $@.$$$$; \
         sed 's,\($*\)\.o[ :]*,\1.o $@ : ,g' < $@.$$$$ > $@; \
		 rm -f $@.$$$$

include $(SOURCES:.cpp=.d)

#%.o: %.cpp
	#$(CC) $(CPPFLAGS) -o $@ $<

run:
	./$(TARGET)

clean:
	-@rm *.o *.d $(TARGET)
