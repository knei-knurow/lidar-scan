ROOT := ${CURDIR}
OS=$(shell uname -s)

CXX := g++
CPPFLAGS := -std=c++17
LIBS := -lrplidar_sdk -pthread
LDFLAGS := -L$(ROOT)/rplidar_sdk/sdk/output/$(OS)/Release
INCLUDES := -I$(ROOT)/rplidar_sdk/sdk/sdk/include -I$(ROOT)/rplidar_sdk/sdk/sdk/src

all: main.o app.o cloud.o lidar.o stream.o
	$(CXX) --output lidar-scan \
	main.o \
	app.o \
	cloud.o \
	lidar.o \
	stream.o \
	$(LDFLAGS) \
	$(LIBS)

main.o: src/main.cpp
	$(CXX) $(CPPFLAGS) -c src/main.cpp $(INCLUDES)

app.o: src/app.cpp
	$(CXX) $(CPPFLAGS) -c src/app.cpp $(INCLUDES)

cloud.o: src/cloud.cpp
	$(CXX) $(CPPFLAGS) -c src/cloud.cpp $(INCLUDES)

lidar.o: src/lidar.cpp
	$(CXX) $(CPPFLAGS) -c src/lidar.cpp $(INCLUDES)

stream.o: src/stream.cpp
	$(CXX) $(CPPFLAGS) -c src/stream.cpp $(INCLUDES)

clean:
	rm -f *.o src/*.o lidar-scan
