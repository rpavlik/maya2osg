/**
    Maya2OSG - A toolkit for exporting Maya scenes to OpenSceneGraph
    Copyright (C) 2010 Javier Taibo <javier.taibo@gmail.com>

    This file is part of Maya2OSG.

    Maya2OSG is free software: you can redistribute it and/or modify
    it under the terms of the GNU General Public License as published by
    the Free Software Foundation, either version 3 of the License, or
    (at your option) any later version.

    Maya2OSG is distributed in the hope that it will be useful,
    but WITHOUT ANY WARRANTY; without even the implied warranty of
    MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
    GNU General Public License for more details.

    You should have received a copy of the GNU General Public License
    along with Maya2OSG.  If not, see <http://www.gnu.org/licenses/>.
*/
#ifndef _OSGFILETRANSLATOR_H_
#define _OSGFILETRANSLATOR_H_

#include <osg/Version>
#include <maya/MPxFileTranslator.h>

/**
 *  OSG File Translator
 *
 *  This class define the OSG formats in Maya and provides
 *  methods to read and write from/to these formats
 */
class OSGFileTranslator : public MPxFileTranslator {

public:

    static void * creator();

    virtual MStatus reader( const MFileObject &file, const MString &optionsString, 
                            MPxFileTranslator::FileAccessMode mode );

    virtual MStatus writer( const MFileObject &file, const MString &optionsString,
                            MPxFileTranslator::FileAccessMode mode );

    virtual inline bool haveReadMethod () const
    { return false; }

    virtual inline bool haveWriteMethod () const
    { return true; }

    virtual inline MString defaultExtension() const
//#if (OPENSCENEGRAPH_MAJOR_VERSION>=2) && (OPENSCENEGRAPH_MINOR_VERSION>=9)
//    { return "osgt"; }
//#else
    { return "osg"; }
//#endif

    virtual inline MString filter() const
//#if (OPENSCENEGRAPH_MAJOR_VERSION>=2) && (OPENSCENEGRAPH_MINOR_VERSION>=9)
//    { return "*.osgt"; }
//#else
    { return "*.osg;*.ive;*.osgt;*.osgb;*.osgx"; }
//#endif
    virtual MFileKind identifyFile (const MFileObject &file, const char *buffer, short size) const;

};

#endif //_OSGFILETRANSLATOR_H_
