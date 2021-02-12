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
CXX_SOURCES := $(wildcard ./osapi/linux/*.cpp)
CXX_SOURCES += $(wildcard ./*.cpp)

BIN_SERVER = server_app

#########################################

CXX_OBJ := $(CXX_SOURCES:%.cpp=%.cpp.o)
C_OBJ := $(C_SOURCES:%.c=%.c.o)
DEP := $(CXX_OBJ:%.o=%.d)
DEP += $(C_OBJ:%.o=%.d)

CXX_FLAGS += $(addprefix -I,$(CXX_INCLUDE_DIRS))
C_FLAGS += $(addprefix -I,$(C_INCLUDE_DIRS))

execte_server: $(BIN_SERVER)
	@echo "Run program..."
	sudo ./$(BIN_SERVER) 20006

$(BIN_SERVER) : $(C_OBJ) $(CXX_OBJ)
	
	$(CXX) -pthread -o $@ $^

-include $(DEP)

%.cpp.o : %.cpp
	@echo "Compiling c..."
	$(CXX) $(CXX_FLAGS) -c $< -o $@
%.c.o : %.c
	@echo "Compiling c++..."
	$(CC) $(C_FLAGS) -c $< -o $@


.PHONY: clean

clean:
	rm -rf $(C_OBJ) $(CXX_OBJ) $(BIN_SERVER)

.SILENT:


