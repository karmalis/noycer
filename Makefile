##
# Noycer generator
#
# A highly specific random generator
#
# @file
# @version 0.1

CC = gcc

SRCDIR = src
OBJDIR = obj
BINDIR = bin

TARGET = noycercli

INCLUDE_DIR = ./include/
IMGUI_INCLUDE_DIR = ./vendor/cimgui/
IMGUI_LIB_DIR = ./dgui/build

GLFW_INCLUDE_DIR = ./vendor/glfw/include
GLFW_LIB_DIR = ./vendor/build/glfw/src

GLM_INCLUDE_DIR = ./vendor/cglm/include

STB_INCLUDE_DIR = ./vendor/stb

CFLAGS = -Wall -Wextra -O0 -DDEBUG -g -std=gnu11 -I$(GLFW_INCLUDE_DIR) -I$(INCLUDE_DIR) -I$(IMGUI_INCLUDE_DIR) -I$(GLM_INCLUDE_DIR) -I$(STB_INCLUDE_DIR)

LDFLAGS = -L$(GLFW_LIB_DIR) -L$(IMGUI_LIB_DIR) -lglfw -lcimgui -Wl,-rpath,../dgui/build

LDFLAGS += -lwayland-client -lwayland-cursor -lxkbcommon
LDFLAGS += -lpthread -ldl -lrt -lm

SOURCES = $(shell find $(SRCDIR) -name "*.c")
OBJECTS = $(patsubst $(SRCDIR)/%.c,$(OBJDIR)/%.o,$(SOURCES))

.PHONY: all clean run

all: $(BINDIR) $(OBJDIR) $(BINDIR)/$(TARGET) copy_shaders

copy_shaders:
	cp -r shaders ./bin/
	cp -r res ./bin/

$(BINDIR)/$(TARGET): $(OBJECTS)
	$(CC) $(OBJECTS) -o $@ $(LDFLAGS)

$(OBJDIR)/%.o: $(SRCDIR)/%.c
	@mkdir -p $(dir $@)
	$(CC) $(CFLAGS) -c $< -o $@

$(BINDIR):
	mkdir -p $(BINDIR)

$(OBJDIR):
	mkdir -p $(OBJDIR)

clean:
	rm -rf $(OBJDIR)/*.o $(BINDIR)/$(TARGET)

run: all
	$(BINDIR)/$(TARGET)


# end
