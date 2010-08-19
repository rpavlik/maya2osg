MAYA_BASE=/usr/aw/maya7.0
CXXFLAGS_MAYA= -pipe -D_BOOL -DLINUX -w -c -I$(MAYA_BASE)/include
CXXFLAGS=-D_DEBUG -Wall -pedantic -Wno-long-long $(CXXFLAGS_MAYA)
LDFLAGS=-L$(MAYA_BASE)/lib
LDLIBS=-lOpenMaya -losg -losgDB

OBJS=maya2osg.o dagnode.o group.o transform.o mesh.o shader.o texture.o config.o lights.o directionallight.o poinlight.o spotlight.o cameraanimation.o
TARGET=maya2osg.so
INSTDIR=$(MAYA_BASE)/bin/plug-ins

all: install

$(TARGET): $(OBJS)
	$(CXX) -shared -o $(TARGET) $(OBJS) $(LDFLAGS) $(LDLIBS)

install: $(TARGET)
	cp $(TARGET) $(INSTDIR)

clean:
	rm -f $(TARGET) $(OBJS) *~
