MAYA_BASE=/usr/autodesk/maya2011-x64
CXXFLAGS_MAYA= -pipe -m64 -O3 -pipe -D_BOOL -DLINUX_64 -DREQUIRE_IOSTREAM -fPIC -w -c -I$(MAYA_BASE)/include
CXXFLAGS=-Wall -pedantic -Wno-long-long $(CXXFLAGS_MAYA)
LDFLAGS=-L$(MAYA_BASE)/lib -m64 -O3 -pipe
LDLIBS=-lOpenMaya -lOpenMayaAnim -lOpenMayaFX -losg -losgDB -losgViewer -losgParticle -losgShadow
#VPATH=../../src

vpath %.h ../../src
vpath %.cpp ../../src

OBJS=maya2osg.o dagnode.o group.o transform.o mesh.o shader.o texture.o config.o lights.o directionallight.o \
	pointlight.o spotlight.o cameraanimation.o camera.o common.o osg2maya.o proxymaterials.o importervisitor.o \
	particle.o nparticle.o pointemitter.o shaderglsl.o shadows.o texturingconfig.o glsllighting.o lambert.o \
	shadingnetwork.o shadingnode.o shadingnodefactory.o surfaceshader.o filetexture.o

TARGET=../bin/maya2osg.so
INSTDIR=$(MAYA_BASE)/bin/plug-ins

all: $(TARGET)

$(TARGET): $(OBJS)
	$(CXX) -shared $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

install: $(TARGET)
	cp $(TARGET) $(INSTDIR)

clean:
	rm -f $(TARGET) $(OBJS) *~
