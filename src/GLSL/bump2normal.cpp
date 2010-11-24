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
#include "bump2normal.h"

#include <osgDB/FileUtils>
#include <osgDB/FileNameUtils>
#include <osgDB/ReadFile>
#include <osgDB/WriteFile>

#ifdef WIN32
#  include <windows.h>
#  include <winbase.h>
#endif
#include <io.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>


//#define NVIDIA_TEXTURE_TOOLS


#ifdef WIN32
/**
 *
 */
std::string shortPath( const std::string &long_path )
{
    long     length = 0;
    TCHAR*   buffer = NULL;
    // First obtain the size needed by passing NULL and 0.
    length = GetShortPathName(long_path.c_str(), NULL, 0);
    if (length == 0) 
        return "";

    // Dynamically allocate the correct size 
    // (terminating null char was included in length)
    buffer = new TCHAR[length];
    // Now simply call again using same long path.
    length = GetShortPathName(long_path.c_str(), buffer, length);
    if (length == 0)
        return "";

    std::string short_path = buffer;
    delete [] buffer;

    return short_path;
}
#endif


/**
 *  Convert bump map file to normal map file
 *
 *  @return File name of the bump map
 */
std::string bump2Normal( const std::string &bump_map_file_name )
{
    std::string normal_map_file_name;

#if defined(NVIDIA_TEXTURE_TOOLS) && defined(WIN32)
    std::string texture_tools_dir = getenv( "TEXTURE_TOOLS_DIR" );
    bool nvidia_texture_tools = !texture_tools_dir.empty();
#else
    bool nvidia_texture_tools = false;
#endif
    if ( nvidia_texture_tools ) {
        normal_map_file_name = osgDB::getNameLessExtension(bump_map_file_name) + "_nmap.dds";
    }
    else {
        normal_map_file_name = osgDB::getNameLessExtension(bump_map_file_name) + "_nmap.png";
    }

    // Check whether the normal map file exists (and is more recent than bump map)
    if ( osgDB::fileExists( normal_map_file_name ) ) {
        // If it exists, check if it is newer or older than the bump map file
        struct _stat stat_bump_map;
        int fd, result;
        fd = _open( bump_map_file_name.c_str(), _O_RDONLY );
        result = _fstat( fd, &stat_bump_map );
        _close(fd);
        struct _stat stat_normal_map;
        fd = _open( normal_map_file_name.c_str(), _O_RDONLY );
        result = _fstat( fd, &stat_normal_map );
        _close(fd);
        if ( stat_normal_map.st_mtime > stat_bump_map.st_mtime ) {
            // Normal map is more recent than bump map. It is done!
            return normal_map_file_name;
        }
    }

#if defined(NVIDIA_TEXTURE_TOOLS) && defined(WIN32)

    if ( nvidia_texture_tools ) {
        std::string nvcompress_exe = osgDB::convertFileNameToNativeStyle(texture_tools_dir + "/nvcompress.exe");
        std::string filename_orig = osgDB::convertFileNameToNativeStyle( bump_map_file_name );
        std::string filename_nm = osgDB::convertFileNameToNativeStyle( normal_map_file_name );
        normal_map_file_name = filename_nm;

        // CREATE AN EMPTY NM TO BE ABLE TO GET ITS SHORT PATH
        int fd = _open( filename_nm.c_str(), _O_CREAT|_O_TRUNC, _S_IREAD|_S_IWRITE );
        if ( fd == -1 ) {
            std::cerr << "ERROR. Creating normal map file " << filename_nm << std::endl;
        }
        else {
            close(fd);
        }
        filename_orig = shortPath( filename_orig );
        filename_nm = shortPath( filename_nm );
        nvcompress_exe = shortPath( nvcompress_exe );

        std::string command = std::string( nvcompress_exe
            + " -rgb -tonormal " + filename_orig + " " + filename_nm );
        std::cout << "NVIDIA Texture tools installed. Converting bump map to DOT3 normal map" << std::endl;
        std::cout << command << std::endl;
        system( command.c_str() );

        // DDS - Flip vertical
        //osg::ref_ptr<osg::Image> img = osgDB::readImageFile( filename_nm );
        //if ( img.valid() ) {
        //    img->flipVertical();
        //    osgDB::writeImageFile( *img, filename_nm );
        //}
        //else {
        //    std::cerr << "ERROR. Generating normal map file " << filename_nm << std::endl;
        //}
    }
    else
#endif    
    {
        // Texture tools not installed (or disabled). Use built-in method (work in progress)

        std::cout << "Converting bump map \"" << bump_map_file_name << "\" to normal map \"" << normal_map_file_name << "\"" << std::endl;
        osg::ref_ptr<osg::Image> bump_map = osgDB::readImageFile( bump_map_file_name );
        osg::ref_ptr<osg::Image> normal_map = new osg::Image();
        normal_map->allocateImage( bump_map->s(), bump_map->t(), bump_map->r(), GL_RGB, GL_UNSIGNED_BYTE );

//#define THE_CG_TUTORIAL 1
#define BUMP_SCALE 10.0

#ifdef THE_CG_TUTORIAL

        // see "The Cg Tutorial" pp. 202-203

        unsigned char *buffer = (unsigned char *)normal_map->getDataPointer();
        for( int t=0 ; t < bump_map->t() ; t++ ) {
            int tp1 = (t==bump_map->t()-1)?0:(t+1);
            for( int s=0 ; s < bump_map->s() ; s++ ) {
                int sp1 = (s==bump_map->s()-1)?0:(s+1);
                //std::cout << "COLOR : " << bump_map->getColor(s,t).r() << " "
                //    << bump_map->getColor(s,t).g() << " "
                //    << bump_map->getColor(s,t).b() << " "
                //    << bump_map->getColor(s,t).a()
                //    << std::endl;
                // What component to take? R? (R+G+B)/3.0? Alpha? ...
                double hg = bump_map->getColor(s, t).r();
                double ha = bump_map->getColor(s, tp1).r();
                double hr = bump_map->getColor(sp1, t).r();
                double hg_ha = (hg - ha) * BUMP_SCALE;
                double hg_hr = (hg - hr) * BUMP_SCALE;
                osg::Vec3 normal = osg::Vec3(hg_hr, hg_ha, 1.0) / sqrt( hg_ha*hg_ha + hg_hr*hg_hr + 1.0 );
                *buffer++ = (unsigned char)(( normal.x() * 0.5 + 0.5 ) * 255.0);
                *buffer++ = (unsigned char)(( normal.y() * 0.5 + 0.5 ) * 255.0);
                *buffer++ = (unsigned char)(( normal.z() * 0.5 + 0.5 ) * 255.0);
            }
        }
#else // THE_CG_TUTORIAL

        unsigned char *buffer = (unsigned char *)normal_map->getDataPointer();
        for( int t=0 ; t < bump_map->t() ; t++ ) {
            int tp1 = (t==bump_map->t()-1)?0:(t+1);
            int tm1 = (t==0)?(bump_map->t()-1):(t-1);
            for( int s=0 ; s < bump_map->s() ; s++ ) {
                int sp1 = (s==bump_map->s()-1)?0:(s+1);
                int sm1 = (s==0)?(bump_map->s()-1):(s-1);
                //std::cout << "COLOR : " << bump_map->getColor(s,t).r() << " "
                //    << bump_map->getColor(s,t).g() << " "
                //    << bump_map->getColor(s,t).b() << " "
                //    << bump_map->getColor(s,t).a()
                //    << std::endl;
                // What component to take? R? (R+G+B)/3.0? Alpha? ...
                double ha = bump_map->getColor(s, tp1).r();
                double hb = bump_map->getColor(s, tm1).r();
                double hr = bump_map->getColor(sp1, t).r();
                double hl = bump_map->getColor(sm1, t).r();
                osg::Vec3 v1( 1.0, 0.0, (hr-hl)*BUMP_SCALE );
                osg::Vec3 v2( 0.0, 1.0, (ha-hb)*BUMP_SCALE );
                osg::Vec3 normal = v1 ^ v2;
                normal.normalize();
                *buffer++ = (unsigned char)(( normal.x() * 0.5 + 0.5 ) * 255.0);
                *buffer++ = (unsigned char)(( normal.y() * 0.5 + 0.5 ) * 255.0);
                *buffer++ = (unsigned char)(( normal.z() * 0.5 + 0.5 ) * 255.0);
            }
        }

#endif

        osgDB::writeImageFile( *normal_map, normal_map_file_name );

    }
    return normal_map_file_name;
}
