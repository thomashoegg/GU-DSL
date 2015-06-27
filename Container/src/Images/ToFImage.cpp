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


#include "Images/ToFImage.h"
#include <fstream>
#include <sstream>
#include <QFile>
#include <QTextStream>



namespace Container
{
namespace Images
{

const std::string ToFImage::PMD_FILE_EXTENSION = ".pmd";

ToFImage::ToFImage()
{

}

ToFImage::ToFImage( Container::Images::Frame* frame, bool copy /*= true*/ )
	: Container::Images::Image(frame, copy)
{
}

 ToFImage::~ToFImage()
 {
 }

 bool ToFImage::save(std::string filename)
 {
	//float* frame = new float[_frame->width*_frame->height*_frame->layerCount];
	QFile file(filename.c_str());
	if(file.open(QIODevice::WriteOnly | QIODevice::Text))
	{
		QTextStream out(&file);
		out.setRealNumberNotation(QTextStream::FixedNotation);
		out.setFieldAlignment(QTextStream::AlignRight);
		
		out << "<frame width='" <<_frame->header[0].width << "' height='" << _frame->header[0].height << "' version='1'>" << "\n";


		float* distances = reinterpret_cast<float*>((void*)_frame->data[0].buffer.c_str());
		float* amplitude = (float*)(_frame->data[0].buffer.c_str()+_frame->header[0].width*_frame->header[0].height*sizeof(float));
		float* grayscale = (float*)(_frame->data[0].buffer.c_str()+2*_frame->header[0].width*_frame->header[0].height*sizeof(float));

		QString distanceString;
		QTextStream distanceStream(&distanceString);
		distanceStream.setRealNumberNotation(QTextStream::FixedNotation);
		distanceStream.setFieldAlignment(QTextStream::AlignRight);

		QString amplitudeString;
		QTextStream amplitudeStream(&amplitudeString);
		amplitudeStream.setRealNumberNotation(QTextStream::FixedNotation);
		amplitudeStream.setFieldAlignment(QTextStream::AlignRight);

		QString grayscaleString;
		QTextStream grayscaleStream(&grayscaleString);
		grayscaleStream.setRealNumberNotation(QTextStream::FixedNotation);
		grayscaleStream.setFieldAlignment(QTextStream::AlignRight);

		QString alphaString;
		QTextStream alphaStream(&alphaString);
		alphaStream.setRealNumberNotation(QTextStream::FixedNotation);
		alphaStream.setFieldAlignment(QTextStream::AlignRight);
		for (unsigned short y = 0; y < _frame->header[0].height; y++) 
		{
			for (unsigned short x = 0; x < _frame->header[0].width;  x++)
			{
				//test = ((float*)(_frame->buffer.c_str() + (y*_frame->width + x)*sizeof(float)));
				/*const float* buffer = ((float*)(_frame->buffer.c_str() + (y*_frame->width + x)*sizeof(float)));
				float test = (float)*buffer;
				out << test << " ";
				*/
				//out << distances[x+_frame->width*y] << " ";
				distanceStream << distances[x+_frame->header[0].width*y] << " ";
				amplitudeStream << amplitude[x+_frame->header[0].width*y] << " ";
				grayscaleStream << grayscale[x+_frame->header[0].width*y] << " ";
				alphaStream << 255 << " ";
				//out << *test << " ";
			}

			if (y < _frame->header[0].height - 1)
			{
				distanceStream << "\n";
				amplitudeStream << "\n";
				grayscaleStream << "\n";
				alphaStream << "\n";
			}
		}
		out << "<distance>\n";
		out << distanceString;
		out << "\n</distance>\n";

		out << "<amplitude>\n";
		//for (unsigned short y = 0; y < _frame->height; y++) 
		//{
		//	for (unsigned short x = 0; x < _frame->width;  x++)
		//	{
		//		//test = ((float*)(_frame->buffer.c_str() + (y*_frame->width + x)*sizeof(float)));
		//		/*const float* buffer = ((float*)(_frame->buffer.c_str() + (y*_frame->width + x)*sizeof(float)));
		//		float test = (float)*buffer;
		//		out << test << " ";
		//		*/
		//		out << distances[x+_frame->width*y] << " ";
		//		//out << *test << " ";
		//	}

		//	if (y < _frame->height - 1) out << "\n";
		//}
		out << amplitudeString;
		out << "\n</amplitude>\n";


		out << "<grayscale>\n";
		//for (unsigned short y = 0; y < _frame->height; y++) 
		//{
		//	for (unsigned short x = 0; x < _frame->width;  x++)
		//	{
		//		//test = ((float*)(_frame->buffer.c_str() + (y*_frame->width + x)*sizeof(float)));
		//		/*const float* buffer = ((float*)(_frame->buffer.c_str() + (y*_frame->width + x)*sizeof(float)));
		//		float test = (float)*buffer;
		//		out << test << " ";
		//		*/
		//		out << grayscale[x+_frame->width*y] << " ";
		//		//out << *test << " ";
		//	}

		//	if (y < _frame->height - 1) out << "\n";
		//}
		out << grayscaleString;
		out << "\n</grayscale>\n";
		//delete[] distances;


		out << "<alpha>\n";
		out << alphaString;
		out << "\n</alpha>\n";

		out << "</frame>";
		file.close();
	}
	else
		return false;
	
	
	//std::ofstream file;
	
	//std::ostringstream sstream;
	//sstream << filename << number << ".pmd";
	//const char* fileToOpen = sstream.str().c_str();
	//D_OUT2(__FUNCTION__, fileToOpen);
	////file.open(sstream.str().c_str(), std::ofstream::out);
	//char* test = "D:/PMDs/test.pmd";
	//std::ofstream file(fileToOpen, std::ofstream::out);
	//if (file.fail())
	//	return false;

	//file.setf(std::ofstream::right | std::ofstream::fixed);
	//file.precision(3);
	//file.width(5);
	//file.fill('0');

	//file << "<frame width='" <<_frame->width << "' height='" << _frame->height << "' version='1'>" << std::endl;

	//

	//file << "<distance>" << std::endl;
	//for (unsigned short y = 0; y < _frame->height; y++) 
	//{
	//for (unsigned short x = 0; x < _frame->width;  x++)
	//	file << *((float*)(_frame->buffer.c_str() + (y*_frame->width + x)*sizeof(float))) << " ";

	//if (y < _frame->height - 1) file << std::endl;
	//}
	//file << std::endl << "</distance>" << std::endl; 

	//file << "<grayscale>" << std::endl;
	///*for (unsigned short y = 0; y < height(); y++) 
	//{
	//	for (unsigned short x = 0; x < width();  x++)
	//		file << pixel(x, y, 1) << " ";

	//	if (y < height() - 1) file << endl;
	//}*/
	//file << std::endl << "</grayscale>" << std::endl; 

	//file << "<amplitude>" << std::endl;
	////for (unsigned short y = 0; y < height(); y++) 
	////{
	////	for (unsigned short x = 0; x < width();  x++)
	////		file << pixel(x, y, 2) << " ";

	////	if (y < height() - 1) file << endl;
	////}
	//file << std::endl << "</amplitude>" << std::endl; 

	//file << "<alpha>" << std::endl;
	////for (unsigned short y = 0; y < height(); y++) 
	////{
	////	for (unsigned short x = 0; x < width();  x++)
	////		file << (int) segmentation(x, y) << " ";

	////	if (y < height() - 1) file << endl;
	////}
	//file << std::endl << "</alpha>" << std::endl; 

	//file << "</frame>";

	//file.flush();
	////file.write();
	//file.close();

	//count++;
	return true;
 }


}	// namespace Images
}	// namespace Container
