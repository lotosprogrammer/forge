# Compiler settings - Can be customized.
CC = g++
CXXFLAGS = -std=c++23 -Wall -Wpedantic #-O3 
LDFLAGS = -lglfw -lvulkan 
# Makefile settings - Can be customized.
APPNAME = a
EXT = .cpp
SRCDIR = src
OBJDIR = obj

# List of source files (replace with your actual source files)
SRC := $(wildcard $(SRCDIR)/*.cpp) $(wildcard $(SRCDIR)/**/*.cpp)
OBJ := $(patsubst $(SRCDIR)/%.cpp,$(OBJDIR)/%.o,$(SRC))
DEP := $(OBJ:.o=.d)

############## Do not change anything from here downwards! #############

# Default target (all)
.DEFAULT_GOAL := all

.PHONY: all debug clean shaders

# Default target (all)
all: CXXFLAGS += 
all: $(APPNAME)

# Debug target
debug: CXXFLAGS += -g -D'_DEBUG'
debug: $(APPNAME)

# Builds the app
$(APPNAME): $(OBJ)
	$(CC) $(CXXFLAGS) -o $@ $^ $(LDFLAGS)

# Creates the dependency rules
%.d: $(SRCDIR)/%.cpp
	@$(CC) $(CXXFLAGS) -MM -MT $(@:.d=.o) $< > $@

# Includes all .h files
-include $(DEP)

# Building rule for .o files and its .cpp in combination with all .h
$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	@mkdir -p $(@D)
	$(CC) $(CXXFLAGS) -o $@ -c $<

# Cleaning rule
clean:
	rm -rf $(OBJDIR) $(APPNAME)

shaders:
	glslc shader.vert -o vert.spv
	glslc shader.frag -o frag.spv
