CXX = g++
CXXFLAGS = -mcmodel=medium -Ofast
TARGET = solver
SOURCE = main.cpp

all: $(TARGET)

$(TARGET): $(SOURCE)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SOURCE)


run: $(TARGET)
	./$(TARGET)


clean:
	rm -f $(TARGET)
