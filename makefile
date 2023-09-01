CXX = g++
CXXFLAGS = -g -Wall# -DDEBUG

INCLUDE = include/

OBJECTS = error_strings.o limits.o buffers.o file_streams.o
ENCODER_OBJECTS = encoder_main.o encode.o
DECODER_OBJECTS = decoder_main.o decode.o

ENCODER_TARGET = Encode
DECODER_TARGET = Decode

BUILD_ENCODER = $(CXX) $(CXXFLAGS) -o $(ENCODER_TARGET) $(ENCODER_OBJECTS) $(OBJECTS)
BUILD_DECODER = $(CXX) $(CXXFLAGS) -o $(DECODER_TARGET) $(DECODER_OBJECTS) $(OBJECTS)

ifeq ($(OS), Windows_NT)
	CXXFLAGS += -DWINDOWS
	CLEAN_OBJS = del *.o
	CLEAN_PRGS = del $(ENCODER_TARGET).exe $(DECODER_TARGET).exe
	CLEAN_ALL_PRGS = del $(ENCODER_TARGET) $(ENCODER_TARGET).exe $(DECODER_TARGET) $(DECODER_TARGET).exe
else
	CLEAN_OBJS = rm *.o
	CLEAN_PRGS = rm $(ENCODER_TARGET) $(DECODER_TARGET)
	CLEAN_ALL_PRGS = rm $(ENCODER_TARGET) $(ENCODER_TARGET).exe $(DECODER_TARGET) $(DECODER_TARGET).exe
endif

.PHONY: all
all: $(ENCODER_OBJECTS) $(DECODER_OBJECTS) $(OBJECTS)
	$(BUILD_ENCODER)
	$(BUILD_DECODER)

.PHONY: encoder
encoder: $(ENCODER_OBJECTS) $(OBJECTS)
	$(BUILD_ENCODER)

.PHONY: decoder
decoder: $(DECODER_OBJECTS) $(OBJECTS)
	$(BUILD_DECODER)

.PHONY: encoder_main.o
encoder_main.o: encoder_main.cpp $(INCLUDE)encode.h
	$(CXX) $(CXXFLAGS) -c encoder_main.cpp

.PHONY: encode.o
encode.o: $(INCLUDE)encode.h
	$(CXX) $(CXXFLAGS) -c encode.cpp

.PHONY: decoder_main.o
decoder_main.o: decoder_main.cpp $(INCLUDE)decode.h
	$(CXX) $(CXXFLAGS) -c decoder_main.cpp

.PHONY: decode.o
decode.o: $(INCLUDE)decode.h
	$(CXX) $(CXXFLAGS) -c decode.cpp

.PHONY: error_strings.o
error_strings.o: $(INCLUDE)error_strings.h
	$(CXX) $(CXXFLAGS) -c error_strings.cpp

.PHONY: limits.o
limits.o: $(INCLUDE)limits.h
	$(CXX) $(CXXFLAGS) -c limits.cpp

.PHONY: buffers.o
buffers.o: $(INCLUDE)buffers.h
	$(CXX) $(CXXFLAGS) -c buffers.cpp

.PHONY: file_streams.o
file_streams.o: $(INCLUDE)file_streams.h
	$(CXX) $(CXXFLAGS) -c file_streams.cpp

.PHONY: cleanobjs
cleanobjs:
	$(CLEAN_OBJS)

.PHONY: cleanprgs
cleanprgs:
	$(CLEAN_PRGS)

.PHONY: cleanallprgs
cleanallprgs:
	$(CLEAN_ALL_PRGS)

.PHONY: cleanall
cleanall: cleanobjs
	$(CLEAN_ALL_PRGS)