CXX = g++
CXXFLAGS = -Wall -Wextra -std=c++17
LDFLAGS = -lssl -lcrypto

TARGET = xoer
SRC = xoer.cpp

all: $(TARGET)

$(TARGET): $(SRC)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRC) $(LDFLAGS)

clean:
	rm -f $(TARGET)