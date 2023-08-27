CXX = g++
CXXFLAGS = -g -Wall# -DDEBUG

INCLUDE = include/

OBJECTS = error_strings.o limits.o buffers.o file_streams.o
ENCODER_OBJECTS = encoder_main.o encode.o
DECODER_OBJECTS = decoder_main.o decode.o

BUILD_ENCODER = $(CXX) $(CXXFLAGS) -o Encode $(ENCODER_OBJECTS) $(OBJECTS)
BUILD_DECODER = $(CXX) $(CXXFLAGS) -o Decode $(DECODER_OBJECTS) $(OBJECTS)

ifeq ($(OS), Windows_NT)
	CXXFLAGS += -DWINDOWS
endif

.PHONY: clean All Encoder Decoder $(OBJECTS) $(ENCODER_OBJECTS) $(DECODER_OBJECTS)

All: $(ENCODER_OBJECTS) $(DECODER_OBJECTS) $(OBJECTS)
	$(BUILD_ENCODER)
	$(BUILD_DECODER)

Encoder: $(ENCODER_OBJECTS) $(OBJECTS)
	$(BUILD_ENCODER)

Decoder: $(DECODER_OBJECTS) $(OBJECTS)
	$(BUILD_DECODER)

encoder_main.o: encoder_main.cpp $(INCLUDE)encode.h
	$(CXX) $(CXXFLAGS) -c encoder_main.cpp

encode.o: $(INCLUDE)encode.h
	$(CXX) $(CXXFLAGS) -c encode.cpp

decoder_main.o: decoder_main.cpp $(INCLUDE)decode.h
	$(CXX) $(CXXFLAGS) -c decoder_main.cpp

decode.o: $(INCLUDE)decode.h
	$(CXX) $(CXXFLAGS) -c decode.cpp

error_strings.o: $(INCLUDE)error_strings.h
	$(CXX) $(CXXFLAGS) -c error_strings.cpp

limits.o: $(INCLUDE)limits.h
	$(CXX) $(CXXFLAGS) -c limits.cpp

buffers.o: $(INCLUDE)buffers.h
	$(CXX) $(CXXFLAGS) -c buffers.cpp

file_streams.o: $(INCLUDE)file_streams.h
	$(CXX) $(CXXFLAGS) -c file_streams.cpp

clean:
	rm Encode Decode $(OBJECTS)