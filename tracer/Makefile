CXX = g++
CXXFLAGS = -Wall -O2 -std=c++11
LDFLAGS = -lm

TARGET = image_generator

all: $(TARGET)

$(TARGET):
	$(CXX) $(CXXFLAGS) -o $(TARGET) \
		trials/main.cpp \
		trials/Ray.cpp \
		trials/math/Matrix4x4.cpp \
		$(LDFLAGS)

clean:
	rm -f $(TARGET)

run: $(TARGET)
	./$(TARGET) > output.ppm

.PHONY: all clean run
