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
			$(SRC_DIR)/object_model.cpp \
			$(SRC_DIR)/wall_model.cpp \
			$(SRC_DIR)/glad.c

TARGET=$(BUILD_DIR)/$(NAME)

all: debug

debug:
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -g $(SRC_FILES) -I $(INCLUDES) -o $(TARGET) $(LIBS)

run: debug
	./$(TARGET)

json_parse:
	@mkdir -p $(BUILD_DIR)
	$(CXX) $(CXXFLAGS) -g $(SRC_DIR)/json_parse.cpp -I $(INCLUDES) -o $(BUILD_DIR)/json_parse.exe $(LIBS)
	$(BUILD_DIR)/json_parse.exe

clean:
	rm -rf $(BUILD_DIR)/*