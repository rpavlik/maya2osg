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
#include "osgfiletranslator.h"


/**
 *
 */
void *OSGFileTranslator::creator()
{
    return new OSGFileTranslator();
}


/** 
 *
 */
MStatus OSGFileTranslator::reader( const MFileObject &file, const MString &optionsString, 
                                   MPxFileTranslator::FileAccessMode mode )
{
    // TO-DO ... fixme!!! *****
    return MStatus::kSuccess;
}


/**
 *
 */
MStatus OSGFileTranslator::writer( const MFileObject &file, const MString &optionsString,
                                   MPxFileTranslator::FileAccessMode mode )
{
    // TO-DO ... fixme!!! *****
    return MStatus::kSuccess;
}


/**
 *
 */
OSGFileTranslator::MFileKind OSGFileTranslator::identifyFile (const MFileObject &file, const char *buffer, short size) const
{
    std::string filename = file.name().asChar();
    if ( filename.substr( filename.size()-4, 4 ).compare(".osg") )
        return kIsMyFileType;
    else if ( filename.substr( filename.size()-4, 4 ).compare(".ive") )
        return kIsMyFileType;
    else if ( filename.substr( filename.size()-5, 5 ).compare(".osgt") )
        return kIsMyFileType;
    else if ( filename.substr( filename.size()-5, 5 ).compare(".osgb") )
        return kIsMyFileType;
    else if ( filename.substr( filename.size()-5, 5 ).compare(".osgx") )
        return kIsMyFileType;

    return kNotMyFileType;       
}
