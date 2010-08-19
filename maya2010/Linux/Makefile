MAYA_BASE=/usr/autodesk/maya2010-x64
CXXFLAGS_MAYA= -pipe -D_BOOL -DLINUX -w -c -I$(MAYA_BASE)/include
CXXFLAGS=-D_DEBUG -Wall -pedantic -Wno-long-long $(CXXFLAGS_MAYA)
LDFLAGS=-L$(MAYA_BASE)/lib
LDLIBS=-lOpenMaya -losg -losgDB
#VPATH=../../src

vpath %.h ../../src
vpath %.cpp ../../src

OBJS=maya2osg.o dagnode.o group.o transform.o mesh.o shader.o texture.o config.o lights.o directionallight.o pointlight.o spotlight.o cameraanimation.o
TARGET=../bin/maya2osg.so
INSTDIR=$(MAYA_BASE)/bin/plug-ins

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -shared -o $(TARGET) $(OBJS) $(LDFLAGS) $(LDLIBS)

install: $(TARGET)
	cp $(TARGET) $(INSTDIR)

clean:
	rm -f $(TARGET) $(OBJS) *~
