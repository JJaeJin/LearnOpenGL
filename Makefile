APP_NAME = myApp
BUILD_DIR = ./bin
CPP_FILES = ./src/*.cpp

APP_DEFINES:=
APP_INCLUDES:= -I./src/vendors/GLFW -framework Cocoa -framework OpenGL -framework IOKit \
				-I./include
APP_LINKERS:= -L./src/vendors/GLFW/lib -lglfw3

build:
	g++ $(CPP_FILES) -o $(BUILD_DIR)/$(APP_NAME) $(APP_INCLUDES) $(APP_LINKERS)