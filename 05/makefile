# Source and target
SRCS = main.cpp

TARGET = raylib_app

# OS detection (macOS = Darwin, Windows via MinGW = MINGW*)
UNAME_S := $(shell uname -s)

# Add the CS3113 library if it exists
ifeq ($(wildcard CS3113/cs3113.cpp),CS3113/cs3113.cpp)
    SRCS += CS3113/cs3113.cpp
endif

# Add the Entity library if it exists
ifeq ($(wildcard CS3113/Entity.cpp),CS3113/Entity.cpp)
    SRCS += CS3113/Entity.cpp
endif

# Add the Projectile library if it exists
ifeq ($(wildcard CS3113/Projectile.cpp),CS3113/Projectile.cpp)
    SRCS += CS3113/Projectile.cpp
endif

# Add the Player library if it exists
ifeq ($(wildcard CS3113/Player.cpp),CS3113/Player.cpp)
    SRCS += CS3113/Player.cpp
endif

# Add the Boss1 library if it exists
ifeq ($(wildcard CS3113/Boss1.cpp),CS3113/Boss1.cpp)
    SRCS += CS3113/Boss1.cpp
endif

# Add the Boss2 library if it exists
ifeq ($(wildcard CS3113/Boss2.cpp),CS3113/Boss2.cpp)
    SRCS += CS3113/Boss2.cpp
endif

# Add the Boss3 library if it exists
ifeq ($(wildcard CS3113/Boss3.cpp),CS3113/Boss3.cpp)
    SRCS += CS3113/Boss3.cpp
endif

# Add the Map library if it exists
ifeq ($(wildcard CS3113/Map.cpp),CS3113/Map.cpp)
    SRCS += CS3113/Map.cpp
endif

# Add the Scene library if it exists
ifeq ($(wildcard CS3113/Scene.cpp),CS3113/Scene.cpp)
    SRCS += CS3113/Scene.cpp
endif

# Add the MainMenu library if it exists
ifeq ($(wildcard CS3113/MainMenu.cpp),CS3113/MainMenu.cpp)
    SRCS += CS3113/MainMenu.cpp
endif

# Add the Phase1 library if it exists
ifeq ($(wildcard CS3113/Phase1.cpp),CS3113/Phase1.cpp)
    SRCS += CS3113/Phase1.cpp
endif

# Add the Phase2 library if it exists
ifeq ($(wildcard CS3113/Phase2.cpp),CS3113/Phase2.cpp)
    SRCS += CS3113/Phase2.cpp
endif

# Add the Phase3 library if it exists
ifeq ($(wildcard CS3113/Phase3.cpp),CS3113/Phase3.cpp)
    SRCS += CS3113/Phase3.cpp
endif

# Add the winScreen library if it exists
ifeq ($(wildcard CS3113/winScreen.cpp),CS3113/winScreen.cpp)
    SRCS += CS3113/winScreen.cpp
endif

# Add the loseScreen library if it exists
ifeq ($(wildcard CS3113/loseScreen.cpp),CS3113/loseScreen.cpp)
    SRCS += CS3113/loseScreen.cpp
endif


# Add the Effects library if it exists
ifeq ($(wildcard CS3113/Effects.cpp),CS3113/Effects.cpp)
    SRCS += CS3113/Effects.cpp
endif

# Add the ShaderProgram library if it exists
ifeq ($(wildcard CS3113/ShaderProgram.cpp),CS3113/ShaderProgram.cpp)
    SRCS += CS3113/ShaderProgram.cpp
endif

# Default values
CXX = g++
CXXFLAGS = -std=c++11

# Raylib configuration using pkg-config
RAYLIB_CFLAGS = $(shell pkg-config --cflags raylib)
RAYLIB_LIBS = $(shell pkg-config --libs raylib)

ifeq ($(UNAME_S), Darwin)
    # macOS configuration
    CXXFLAGS += -arch arm64 $(RAYLIB_CFLAGS)
    LIBS = $(RAYLIB_LIBS) -framework OpenGL -framework Cocoa -framework IOKit -framework CoreVideo
    EXEC = ./$(TARGET)
else ifneq (,$(findstring MINGW,$(UNAME_S)))
    # Windows configuration (assumes raylib in C:/raylib)
    CXXFLAGS += -IC:/raylib/include
    LIBS = -LC:/raylib/lib -lraylib -lopengl32 -lgdi32 -lwinmm
    TARGET := $(TARGET).exe
    EXEC = ./$(TARGET)
else
    # Linux/WSL fallback
    CXXFLAGS +=
    LIBS = -lraylib -lGL -lm -lpthread -ldl -lrt -lX11
    EXEC = ./$(TARGET)
endif

# Build rule
$(TARGET): $(SRCS)
	$(CXX) $(CXXFLAGS) -o $(TARGET) $(SRCS) $(LIBS)

# Clean rule
clean:
	@if [ -f "$(TARGET)" ]; then rm -f $(TARGET); fi
	@if [ -f "$(TARGET).exe" ]; then rm -f $(TARGET).exe; fi

# Run rule
run: $(TARGET)
	$(EXEC)
