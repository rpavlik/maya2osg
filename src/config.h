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
#ifndef _CONFIG_H_
#define _CONFIG_H_

#if defined(_WIN32) || defined(_WINDLL)
#	include <windows.h>
#endif

#include <osg/TexEnv>
#include <osg/BlendFunc>

/**
*	Exporter Global Configuration
*
*	(Singleton design pattern)
*/
class Config {
public:

	/// Surface modes
	typedef enum {
		SINGLE,	///< Force single sided surfaces
		DOUBLE,	///< Force double sided surfaces
		KEEP	///< Each surface as defined in Maya
	} SurfaceMode;

	/// Texture clamping mode
	typedef enum {
		EDGE,	///< Clamp to edge
		COLOR	///< Maya default color
	} TextureClampMode;

private:
	static Config *_instance;

	/// TexEnv
	osg::TexEnv::Mode _mode;

	/// Surface mode
	SurfaceMode _surfaceMode;

	/// Texture clamp mode
	TextureClampMode _texClampMode;

	/// Blend Function (source)
	osg::BlendFunc::BlendFuncMode _blendFuncSrc;
	/// Blend Function (destination)
	osg::BlendFunc::BlendFuncMode _blendFuncDst;

	/// Export default cameras
	bool _exportDefaultCameras;

	/// Export orthograhpic cameras
	bool _exportOrthographicCameras;

	/// Convert from Y-Up to Z-Up
	bool _YUp2ZUp;

	/// Export animations
	bool _exportAnimations;

	/// Animation export "sample by"
	int _animSampleBy;

	/// Emissive particles
	bool _particlesEmissive;

	/// Particles lighting
	bool _particlesLighting;

	/// Constructor
	Config();

public:
	static inline Config *instance()
	{ return _instance; }

	inline void setMode(osg::TexEnv::Mode mode)
	{ _mode = mode; }

	inline osg::TexEnv::Mode getMode() const
	{ return _mode; }

	inline void setBlendFunc( osg::BlendFunc::BlendFuncMode src, osg::BlendFunc::BlendFuncMode dst )
	{ _blendFuncSrc = src; _blendFuncDst = dst; }

	inline osg::BlendFunc::BlendFuncMode getBlendFuncSrc() const
	{ return _blendFuncSrc; }

	inline osg::BlendFunc::BlendFuncMode getBlendFuncDst() const
	{ return _blendFuncDst; }

	inline void setExportDefaultCameras( bool v )
	{ _exportDefaultCameras = v; }

	inline bool getExportDefaultCameras() const
	{ return _exportDefaultCameras; }

	inline void setExportOrthographicCameras( bool v )
	{ _exportOrthographicCameras = v; }

	inline bool getExportOrthographicCameras() const
	{ return _exportOrthographicCameras; }

	inline void setSurfaceMode( SurfaceMode m )
	{ _surfaceMode = m; }

	inline SurfaceMode getSurfaceMode() const
	{ return _surfaceMode; }

	inline void setTexClampMode( TextureClampMode m )
	{ _texClampMode = m; }

	inline TextureClampMode getTexClampMode() const
	{ return _texClampMode; }

	inline void setYUp2ZUp( bool v )
	{ _YUp2ZUp = v; }

	inline bool getYUp2ZUp() const
	{ return _YUp2ZUp; }

	inline void setExportAnimations( bool v )
	{ _exportAnimations = v; }

	inline bool getExportAnimations() const
	{ return _exportAnimations; }

	inline void setAnimSampleBy( int s )
	{ _animSampleBy = s; }

	inline int getAnimSampleBy() const
	{ return _animSampleBy; }

	inline void setParticlesEmissive( bool v )
	{ _particlesEmissive = v; }

	inline bool getParticlesEmissive() const
	{ return _particlesEmissive; }

	inline void setParticlesLighting( bool v )
	{ _particlesLighting = v; }

	inline bool getParticlesLighting() const
	{ return _particlesLighting; }

};

#endif //_CONFIG_H_
