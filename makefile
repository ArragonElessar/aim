NAME=aim
VERSION=1.0.0

CXX=g++
CXXFLAGS=-std=c++17
LIBS= -lglfw -ldl

SRC_DIR=src
BUILD_DIR=build
INCLUDES=include

SRC_FILES= 	$(SRC_DIR)/main.cpp \
			$(SRC_DIR)/window_mgr.cpp \
			$(SRC_DIR)/resource_mgr.cpp \
			$(SRC_DIR)/shader.cpp \
			$(SRC_DIR)/texture.cpp \
			$(SRC_DIR)/player.cpp \
			$(SRC_DIR)/glad.c

TARGET=$(BUILD_DIR)/$(NAME)

all: debug

debug:
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -g $(SRC_FILES) -I $(INCLUDES) -o $(TARGET) $(LIBS)

run: debug
	./$(TARGET)

clean:
	rm -rf $(BUILD_DIR)/*