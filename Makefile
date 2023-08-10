# The Makefile for compiling the *.so library
CXX ?= g++
AR := ar
CXXFLAGS := -g -O2 -DNDEBUG -fPIC -fno-semantic-interposition -Wall -std=gnu++17 -DNSIM2023_LOG_ENABLE
DBGFLAGS := -g
COBJFLAGS := $(CXXFLAGS) -c

# compile macros
LIB_PATH := lib
OBJ_PATH := obj
SRC_PATH := src
# The core kernel from NS3 and re-constructed into NSIM2023
TARGET_NAME := libnsim2023.so
TARGET := $(LIB_PATH)/$(TARGET_NAME)

SRC := $(foreach x, $(SRC_PATH), $(wildcard $(wildcard $(addprefix $(x)/*,.c*))))
OBJ := $(addprefix $(OBJ_PATH)/, $(addsuffix .o, $(notdir $(basename $(SRC)))))

# clean files list
DISTCLEAN_LIST := $(OBJ) 
CLEAN_LIST := $(TARGET) \
		$(DISTCLEAN_LIST)

# default rule
default: makedir all

# non-phony targets
$(TARGET): $(OBJ)
	$(CXX) -shared -o $@ $(OBJ) $(CXXFLAGS)
$(OBJ_PATH)/%.o: $(SRC_PATH)/%.c*
	$(CXX) $(COBJFLAGS) -o $@ $<

# phony rules
.PHONY: makedir
makedir:
	@mkdir -p $(OBJ_PATH) $(LIB_PATH)

.PHONY: all
all: $(TARGET)

.PHONY: clean
clean:
	@echo CLEAN $(CLEAN_LIST)
	@rm -f $(CLEAN_LIST)

.PHONY: distclean
distclean:
	@echo CLEAN $(DISTCLEAN_LIST)
	@rm -f $(DISTCLEAN_LIST)
