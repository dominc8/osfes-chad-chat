CXX := g++
CXX_FLAGS := -O0 -g3 -Wall -fmessage-length=0 -Wno-unused-variable -pthread

CXX_INCLUDE_DIRS += ../osapi
CXX_INCLUDE_DIRS += osapi/linux

CXX_SOURCES := $(wildcard ../osapi/linux/*.cpp)
CXX_SOURCES += $(wildcard ./*.cpp)

BIN_CLIENT = client_app

#########################################

CXX_OBJ := $(CXX_SOURCES:%.cpp=%.cpp.o)
DEP := $(CXX_OBJ:%.o=%.d)
DEP += $(C_OBJ:%.o=%.d)

CXX_FLAGS += $(addprefix -I,$(CXX_INCLUDE_DIRS))

execte_client: $(BIN_CLIENT)
	@echo "Run program..."
	sudo ./$(BIN_CLIENT)

$(BIN_CLIENT) : $(CXX_OBJ)
	$(CXX) -pthread -o $@ $^

-include $(DEP)

%.cpp.o : %.cpp
	$(CXX) $(CXX_FLAGS) -c $< -o $@

.PHONY: clean

clean:
	rm -rf $(CXX_OBJ) $(BIN_CLIENT)

.SILENT:


