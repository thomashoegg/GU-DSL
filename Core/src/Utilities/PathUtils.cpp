/***********************************************************************************
 * Copyright (c) 2013-2015 "Thomas Hoegg et al."
 * [http://gu-dsl.org]; information about commercial use mail at info@gu-dsl.org
 *
 * This file is part of GU-DSL.
 *
 * GU-DSLis free software: you can redistribute it and/or modify
 * it under the terms of the GNU General Public License as published by
 * the Free Software Foundation, either version 3 of the License, or
 * (at your option) any later version.
 * 
 * This program is distributed in the hope that it will be useful,
 * but WITHOUT ANY WARRANTY; without even the implied warranty of
 * MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 * GNU General Public License for more details.
 * 
 * You should have received a copy of the GNU General Public License
 * along with this program.  If not, see <http://www.gnu.org/licenses/>.
 **********************************************************************************/

#include "Utilities/PathUtils.h"

#include <QString>
#include <QStringList>
#include <QFileInfo>

std::string Core::Utilities::PathUtils::makeRelativePath( const std::string filename, const std::string pathRelativeTo, bool onlyIfRootFolderMatches)
{
	QString absolutePath(filename.c_str());
	QString relativeTo(pathRelativeTo.c_str());

	QFileInfo fileInfo(absolutePath);
	QFileInfo fileInfoRelativeTo(relativeTo);

	fileInfo.makeAbsolute();
	absolutePath = fileInfo.absoluteFilePath();

	fileInfoRelativeTo.makeAbsolute();
	relativeTo = fileInfoRelativeTo.absoluteFilePath();

	bool isFile = fileInfo.isFile();

	// If required: Check if we have a common base
	if(onlyIfRootFolderMatches)
	{
		// We don't have a common base path!!!
		if(!absolutePath.startsWith(relativeTo))
			return filename;
	}
	
	QStringList absoluteDirectories = absolutePath.split( '/', QString::SkipEmptyParts );
	QStringList relativeDirectories = relativeTo.split( '/', QString::SkipEmptyParts );

	//Get the shortest of the two paths
	int length = absoluteDirectories.count() < relativeDirectories.count() ? absoluteDirectories.count() : relativeDirectories.count();

	//Use to determine where in the loop we exited
	int lastCommonRoot = -1;
	int index;

	//Find common root
	for (index = 0; index < length; index++)
		if (absoluteDirectories[index] == relativeDirectories[index])
			lastCommonRoot = index;
		else
			break;

	//If we didn't find a common prefix then throw
	if (lastCommonRoot == -1)
		throw std::string("Paths do not have a common base");

	//Build up the relative path
	QString relativePath;
	relativePath.append(".");

	for (index = lastCommonRoot + 1; index < absoluteDirectories.count(); index++)
	{
		if (absoluteDirectories[index].length() > 0)
		{
			relativePath.append("/");
			relativePath.append(absoluteDirectories[index]);
		}
	}

	////Add on the ..
	//for (index = lastCommonRoot + 1; index < absoluteDirectories.count() - (isFile?1:0); index++)
	//	if (absoluteDirectories[index].length() > 0)
	//		relativePath.append("../");

	////Add on the folders
	//for (index = lastCommonRoot + 1; index < relativeDirectories.count() - 1; index++)
	//	relativePath.append( relativeDirectories[index] ).append( "/" );

	//relativePath.append(relativeDirectories[relativeDirectories.count() - 1]);

	return relativePath.toStdString();
}
