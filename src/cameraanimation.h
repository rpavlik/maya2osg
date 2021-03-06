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
#ifndef _CAMERAANIMATION_H_
#define _CAMERAANIMATION_H_

/*
	We have to do this trick because if it is included from Windows
	file osg/AnimationPath crashes very badly. The reason is that this
	Maya plug-in is a DLL and does not define WIN32 symbol, but _WINDLL
	and AnimationPath need to have WIN32 defined
*/
#if defined(_WINDLL) && !defined(WIN32)
#	define WIN32
#	include <osg/AnimationPath>
#	undef WIN32
#else
#	include <osg/AnimationPath>
#endif

class CameraAnimation {

public:
	/// Save the animation to a file
	static void save(const osg::AnimationPath *ap, const std::string &filename);

};

#endif //_CAMERAANIMATION_H_
