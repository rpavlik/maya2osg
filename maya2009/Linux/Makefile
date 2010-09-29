MAYA_BASE=/usr/autodesk/maya2009-x64
CXXFLAGS_MAYA= -pipe -D_BOOL -DLINUX -w -c -I$(MAYA_BASE)/include
CXXFLAGS=-D_DEBUG -Wall -pedantic -Wno-long-long $(CXXFLAGS_MAYA)
LDFLAGS=-L$(MAYA_BASE)/lib
LDLIBS=-lOpenMaya -losg -losgDB -losgViewer
#VPATH=../../src

vpath %.h ../../src
vpath %.cpp ../../src

OBJS=maya2osg.o dagnode.o group.o transform.o mesh.o shader.o texture.o config.o lights.o directionallight.o \
	pointlight.o spotlight.o cameraanimation.o camera.o common.o osg2maya.o proxymaterials.o importervisitor.o \
	particle.o nparticle.o pointemitter.o

TARGET=../bin/maya2osg.so
INSTDIR=$(MAYA_BASE)/bin/plug-ins

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -shared -o $(TARGET) $(OBJS) $(LDFLAGS) $(LDLIBS)

install: $(TARGET)
	cp $(TARGET) $(INSTDIR)

clean:
	rm -f $(TARGET) $(OBJS) *~
