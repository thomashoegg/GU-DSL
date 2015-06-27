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

#include "Images/ToFRawImage.h"
#include <fstream>
#include <sstream>
#include <QFile>
#include <QTextStream>
#include <QDomDocument>



namespace Container
{
		namespace Images
		{

const std::string ToFRawImage::PMDRAW_FILE_EXTENSION = ".pmdraw";

ToFRawImage::ToFRawImage()
{

}

ToFRawImage::ToFRawImage( Container::Images::Frame* frame, bool copy /*= true*/ )
	: Container::Images::Image(frame, copy)
{
}

ToFRawImage::~ToFRawImage()
{
}

bool ToFRawImage::save(std::string filename)
{
	QFile file(filename.c_str());
	if(file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&file);
		out.setRealNumberNotation(QTextStream::FixedNotation);
		out.setFieldAlignment(QTextStream::AlignRight);

		out << "<frame width='" <<_frame->header[0].width << "' height='" << _frame->header[0].height << "' version='1'>" << "\n";

		QString channelA[4];
		QTextStream** channelAStream = new QTextStream*[4];

		QString channelB[4];
		QTextStream** channelBStream = new QTextStream*[4];

		for(int i = 0; i < 4; i++)
		{
			channelAStream[i] = new QTextStream(&channelA[i]);
			channelBStream[i] = new QTextStream(&channelB[i]);


			channelAStream[i]->setRealNumberNotation(QTextStream::FixedNotation);
			channelAStream[i]->setFieldAlignment(QTextStream::AlignRight);

			channelBStream[i]->setRealNumberNotation(QTextStream::FixedNotation);
			channelBStream[i]->setFieldAlignment(QTextStream::AlignRight);
		}


		int width = _frame->header[0].width/2;
		int height = _frame->header[0].height/4;

		unsigned short* buffer = (unsigned short*)this->buffer();

		for (unsigned short y = 0; y < _frame->header[0].height; y++) 
		{
			int index = y / height;
			for (unsigned short x = 0; x < _frame->header[0].width;  x++)
			{
				// A-channel
				if(x < width)
				{
					(*channelAStream[index]) << buffer[x+_frame->header[0].width*y] << " ";
				}
				else // B-channel
				{
					(*channelBStream[index]) << buffer[x+_frame->header[0].width*y] << " ";
				}
			}
		}
		out << "<raw>\n";

		for(int i = 0; i < 4; i++)
		{
			out << "<A" << i << ">\n";
			out << channelA[i];
			out << "\n</A" << i << ">\n";

			out << "<B" << i << ">\n";
			out << channelB[i];
			out << "\n</B" << i << ">\n";
		}

		out << "\n</raw>\n";

		out << "</frame>";

		for(int i = 0; i < 4; i++)
		{
			delete channelAStream[i];
			delete channelBStream[i];
		}

		delete[] channelAStream;
		delete[] channelBStream;

		file.close();
	}
	else
		return false;

	return false;
}

bool ToFRawImage::load( std::string filename )
{
	// open file
	QFile file(filename.c_str());

	if (!file.open(QFile::ReadOnly | QFile::Text)) 
	{
		// E_OUT2("PMDImage::load()", LoaderMsg::UnableToLoadImage);
		return false;
	}

	QDomDocument domDocument;
	if (!domDocument.setContent(&file)) 
	{
		// E_OUT2("PMDImage::load()", LoaderMsg::SyntaxError);
		return false;
	}

	QDomElement root = domDocument.documentElement();

	// process file  


	// attributes 
	if ( !(root.hasAttribute("width") && root.hasAttribute("height")) ) 
	{
		// E_OUT2("PMDImage::load()", LoaderMsg::MissingImageSize);
		return false;
	}

	int width = root.attribute("width").toInt();
	int height = root.attribute("height").toInt();

	// get version of file format
	int version = 0;  

	if (root.hasAttribute("ver"))
		version = root.attribute("ver").toInt();
	else if (root.hasAttribute("version"))
		version = root.attribute("version").toInt();

	if(version == 1)
	{
		QDomElement child = root.firstChildElement(QString("raw"));
		if(child.isNull())
			return false;

		unsigned short* buffer = new unsigned short[width * height];

		QString channelA[4];
		QString channelB[4];

		QDomElement aElements[4];
		QDomElement bElements[4];

		aElements[0] = child.firstChildElement("A0");
		aElements[1] = child.firstChildElement("A1");
		aElements[2] = child.firstChildElement("A2");
		aElements[3] = child.firstChildElement("A3");

		bElements[0] = child.firstChildElement("B0");
		bElements[1] = child.firstChildElement("B1");
		bElements[2] = child.firstChildElement("B2");
		bElements[3] = child.firstChildElement("B3");

		for(int i = 0; i < 4; i++)
		{
			channelA[i] = aElements[i].text();
			channelB[i] = bElements[i].text();
		}

		int channelWidth = width/2;
		int channelHeight = height/4;

		int lastIndexA = 0;
		int lastIndexB = 0;
		int lastIndex = -1;

		for (unsigned short y = 0; y < _frame->header[0].height; y++) 
		{
			int index = y / channelHeight;
			if(index != lastIndex)
			{
				lastIndexA = 0;
				lastIndexB = 0;
				lastIndex = index;
			}

			for (unsigned short x = 0; x < _frame->header[0].width;  x++)
			{
				unsigned short value = 0;

				// A-channel
				if(x < channelWidth)
				{
					int indexA = channelA[index].indexOf(' ', lastIndexA);
					value = channelA[index].mid(lastIndexA, indexA - lastIndexA).toUShort();

					lastIndexA = indexA + 1;
				}
				else // B-channel
				{
					int indexB = channelB[index].indexOf(' ', lastIndexB);
					value = channelB[index].mid(lastIndexB, indexB - lastIndexB).toUShort();

					lastIndexB = indexB + 1;
				}

				buffer[x+_frame->header[0].width*y] = value;
			}
		}

		setBuffer((char*)buffer, width * height * sizeof(unsigned short));
		setWidth(width);
		setHeight(height);

		delete[] buffer;
	}


	return true;
}




	}	// namespace Images
}	// namespace Container
