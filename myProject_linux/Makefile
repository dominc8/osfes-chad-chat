CXX := g++
CXX_FLAGS := -O0 -g3 -Wall -fmessage-length=0 -Wno-unused-variable -pthread

CC := gcc
C_FLAGS := -O0 -g3 -Wall -fmessage-length=0 -Wno-unused-variable -pthread

C_INCLUDE_DIRS := cunit

CXX_INCLUDE_DIRS := cunit
CXX_INCLUDE_DIRS += osapi
CXX_INCLUDE_DIRS += osapi/linux
CXX_INCLUDE_DIRS += osapi_test

C_SOURCES := $(wildcard ./cunit/Sources/**/*.c)
CXX_SOURCES := $(wildcard ./osapi_test/*.cpp)
CXX_SOURCES += $(wildcard ./osapi/linux/*.cpp)

BIN = myProject_linux

#########################################

CXX_OBJ := $(CXX_SOURCES:%.cpp=%.cpp.o)
C_OBJ := $(C_SOURCES:%.c=%.c.o)
DEP := $(CXX_OBJ:%.o=%.d)
DEP += $(C_OBJ:%.o=%.d)

CXX_FLAGS += $(addprefix -I,$(CXX_INCLUDE_DIRS))
C_FLAGS += $(addprefix -I,$(C_INCLUDE_DIRS))

test: $(BIN)
	sudo ./$(BIN)

$(BIN) : $(C_OBJ) $(CXX_OBJ)
	$(CXX) -pthread -o $@ $^

-include $(DEP)

%.cpp.o : %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@
%.c.o : %.c
	$(CC) $(C_FLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf $(C_OBJ) $(CXX_OBJ) $(BIN)


