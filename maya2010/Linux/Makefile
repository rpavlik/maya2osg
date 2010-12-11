MAYA_BASE=/usr/autodesk/maya2010-x64
CXXFLAGS_MAYA= -pipe -m64 -O3 -pipe -D_BOOL -DLINUX_64 -DREQUIRE_IOSTREAM -fPIC -w -c -I$(MAYA_BASE)/include
CXXFLAGS=-Wall -pedantic -Wno-long-long $(CXXFLAGS_MAYA)
LDFLAGS=-L$(MAYA_BASE)/lib -m64 -O3 -pipe
LDLIBS=-lOpenMaya -lOpenMayaAnim -lOpenMayaFX -lOpenMayaRender -losg -losgDB -losgViewer -losgParticle -losgShadow
#VPATH=../../src

vpath %.h ../../src
vpath %.cpp ../../src

DIRS=GLSL
OBJS=maya2osg.o dagnode.o group.o transform.o mesh.o shader.o texture.o config.o lights.o directionallight.o \
	pointlight.o spotlight.o cameraanimation.o camera.o common.o osg2maya.o proxymaterials.o importervisitor.o \
	particle.o nparticle.o pointemitter.o shadows.o GLSL/texturingconfig.o GLSL/glslutils.o GLSL/lambert.o \
	GLSL/shadingnetwork.o GLSL/shadingnode.o GLSL/shadingnodefactory.o GLSL/surfaceshader.o GLSL/filetexture.o \
	GLSL/bump2d.o GLSL/bump2normal.o GLSL/flatshader.o GLSL/phong.o

TARGET=../bin/maya2osg.so
INSTDIR=$(MAYA_BASE)/bin/plug-ins

all: dirs $(TARGET)

dirs:
	mkdir $(DIRS)

$(TARGET): $(OBJS)
	$(CXX) -shared $(LDFLAGS) -o $(TARGET) $(OBJS) $(LDLIBS)

install: $(TARGET)
	cd ..
	sh INSTALL.sh

clean:
	rm -f $(TARGET) $(OBJS) *~
	rmdir $(DIRS)
