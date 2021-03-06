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

#include <osg/TexEnv>
#include <osg/BlendFunc>
#include <osg/Vec3>

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

	/// Supported shadow techniques
	typedef enum {
		SHADOW_MAP,
		SOFT_SHADOW_MAP,
		SHADOW_VOLUME,
		SHADOW_TEXTURE,
		PARALLEL_SPLIT_SHADOW_MAP
	} ShadowTechnique;

	/// export Transform Animation as motionPath or osgAnimation
	typedef enum {
		ANIMATION_PATH,
		OSG_ANIMATION
	} AnimTransformType;
	
    /// Scene file path (where 3D contents are exported into)
    class SceneFilePath {
    private:
        std::string _path;
    public:
        SceneFilePath( const std::string & path );
        std::string getFullPath() const;
        std::string getFileBaseName() const;
        std::string getDirectory() const;
        std::string getExtension() const;
    };

private:
	static Config *_instance;

	/// Scene file path
	SceneFilePath _sceneFilePath;

	/// TexEnv
	osg::TexEnv::Mode _mode;

	/// Surface mode
	SurfaceMode _surfaceMode;

	/// Flip Normals
	bool _flipNormalsIfOpposite;

	/// Texture clamp mode
	TextureClampMode _texClampMode;

	/// Blend Function (source)
	osg::BlendFunc::BlendFuncMode _blendFuncSrc;
	/// Blend Function (destination)
	osg::BlendFunc::BlendFuncMode _blendFuncDst;

	/// Export default cameras
	bool _exportDefaultCameras;

	/// Export orthographic cameras
	bool _exportOrthographicCameras;

	/// Convert from Y-Up to Z-Up
	bool _YUp2ZUp;

	/// Export animations
	bool _exportAnimations;

	/// export Transform Animation as motionPath or osgAnimation
	AnimTransformType _animTransformType;

	/// Animation export "sample by"
	int _animSampleBy;

	/// Emissive particles
	bool _particlesEmissive;

	/// Particles lighting
	bool _particlesLighting;

	/// Compute shadows
	bool _computeShadows;

	/// Shadow technique
	ShadowTechnique _shadowTechnique;

	/// Global ambient color
	osg::Vec3 _globalAmbient;

	/// Local viewer (for lighting model)
	bool _localViewer;

	/// Use GLSL shaders to emulate Maya shaders
	bool _useGLSL;

	/// Export geometry normals
	bool _exportNormals;

	/// Export texture coordinates
	bool _exportTexCoords;

	/// Maximum number of samples for anisotropic filtering
	int _maxAnisotropy;

	/// Enable the bump mapping in the exported GLSL shader
	bool _enableBumpMapping;

	/// Apply bump depth in run-time (vs baked into the texture file)
	bool _runTimeBumpDepth;

	/// Regenerate textures (even when they are up-to-date)
	bool _regenerateTextures;

	/// Required option, as fileTranslator exports also selected even if "Export All" is used
	bool _exportSelection;

	/// Constructor
	Config();

public:
	static inline Config *instance()
	{ return _instance; }

	/// Get the scene file base name
	inline const SceneFilePath &getSceneFilePath()
    { return _sceneFilePath; }

	/// Set the scene file base name
	inline void setSceneFilePath( const std::string &name )
    { _sceneFilePath = SceneFilePath(name); }

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

	inline void setFlipNormalsIfOpposite( bool v )
	{ _flipNormalsIfOpposite = v; }

	inline bool getFlipNormalsIfOpposite() const
	{ return _flipNormalsIfOpposite; }

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

	inline void setAnimTransformType( AnimTransformType m )
	{ _animTransformType = m; }

	inline AnimTransformType getAnimTransformType() const
	{ return _animTransformType; }

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

	inline void setComputeShadows( bool v )
	{ _computeShadows = v; }

	inline bool getComputeShadows() const
	{ return _computeShadows; }

	inline void setShadowTechnique( ShadowTechnique st )
	{ _shadowTechnique = st; }

	inline ShadowTechnique getShadowTechnique() const
	{ return _shadowTechnique; }

	inline void setGlobalAmbient( osg::Vec3 color )
	{ _globalAmbient = color; }

	inline osg::Vec3 getGlobalAmbient() const
	{ return _globalAmbient; }

	inline void setLocalViewer( bool lv )
	{ _localViewer = lv; }

	inline bool getLocalViewer() const
	{ return _localViewer; }

	inline void setUseGLSL( bool v )
	{ _useGLSL = v; }

	inline bool getUseGLSL() const
	{ return _useGLSL; }

	inline void setExportNormals( bool v )
	{ _exportNormals = v; }

	inline bool getExportNormals() const
	{ return _exportNormals; }

	inline void setExportTexCoords( bool v )
	{ _exportTexCoords = v; }

	inline bool getExportTexCoords() const
	{ return _exportTexCoords; }

	inline void setMaxAnisotropy( int v )
	{ _maxAnisotropy = v; }

	inline int getMaxAnisotropy() const
	{ return _maxAnisotropy; }

	inline void setEnableBumpMapping( bool v )
	{ _enableBumpMapping= v; }

	inline bool getEnableBumpMapping() const
	{ return _enableBumpMapping; }

    inline void setRunTimeBumpDepth( bool v )
    { _runTimeBumpDepth = v; }

    inline bool getRunTimeBumpDepth() const
    { return _runTimeBumpDepth; }

    inline void setRegenerateTextures( bool v )
    { _regenerateTextures = v; }

    inline bool getRegenerateTextures() const
    { return _regenerateTextures; }

	 inline void setExportSelection( bool v )
	 { _exportSelection = v; }

	 inline bool getExportSelection() const
	 { return _exportSelection; }

};

#endif //_CONFIG_H_
