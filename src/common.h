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
#ifndef _COMMON_H_
#define _COMMON_H_

#define VENDOR "Maya2OSG"
#define VERSION "0.4.2-SVN"

#include <string>
#include <maya/MStatus.h>

/**
 *	Check the status, return if there is an error
 *	and print an informative error message if there is.
 */
bool MCheckStatus(MStatus &st, const std::string &msg);

#endif // _COMMON_H_	
