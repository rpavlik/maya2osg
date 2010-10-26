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
#include "cameraanimation.h"
#include "config.h"

#include <fstream>
#include <iomanip>
#include <osg/io_utils>

/**
 *	Graba la animación a fichero
 */
void CameraAnimation::save(const osg::AnimationPath *ap, const std::string &filename)
{
	osg::AnimationPath::TimeControlPointMap::const_iterator it;

	std::ofstream output(filename.c_str());

    osg::Quat quat_yup2zup;
    quat_yup2zup.makeRotate( osg::Vec3(0.0, 0.0, 1.0), osg::Vec3(0.0, 1.0, 0.0) );
    osg::Matrix mat_yup2zup( quat_yup2zup );

	for(it=ap->getTimeControlPointMap().begin(); it!=ap->getTimeControlPointMap().end() ; it++){
		double time = it->first;
		osg::AnimationPath::ControlPoint cp = it->second;
        if ( Config::instance()->getYUp2ZUp() ) {
            osg::Vec4 pos = mat_yup2zup * osg::Vec4(cp.getPosition(), 1.0);
            osg::Vec3 pos3(pos.x(), pos.y(), pos.z());
            // We must post-multiply the quaternion with the conjugate 
            // of yup2zup quaternion, even though I don't really know exactly why... :-m
            osg::Quat rot = cp.getRotation() * quat_yup2zup.conj();
            output << std::setprecision(10) << time << " " << pos3 << " " << rot << std::endl;
        }
        else {
            output << std::setprecision(10) << time << " " << cp.getPosition() << " " << cp.getRotation() << std::endl;
        }
	}
}
