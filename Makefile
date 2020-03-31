EXE := city_plan

SRC_DIR := src
OBJ_DIR := obj

SRC := $(wildcard $(SRC_DIR)/*.cpp)
OBJ := $(SRC:$(SRC_DIR)/%.cpp=$(OBJ_DIR)/%.o)

CPPFLAGS := -Iinclude
CFLAGS   := -Wall
LDFLAGS  := -Llib
LDLIBS   := -lm

.PHONY: all clean

all: $(EXE)

$(EXE): $(OBJ)
	g++ $(LDFLAGS) $^ $(LDLIBS) -o $@ -lstdc++fs

$(OBJ_DIR)/%.o: $(SRC_DIR)/%.cpp | $(OBJ_DIR)
	g++ $(CPPFLAGS) $(CFLAGS) -c $< -o $@ -lstdc++fs

$(OBJ_DIR):
	mkdir $@

clean:
	$(RM) $(OBJ)