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

#ifndef VIEWER3D_H
#define VIEWER3D_H

#include "ViewerDLL.h"
#include "glwindow.qt.h"
#include "Rendering/vertexformat.h"
#include "Plane/PlaneCoefficients.h"
#include <unordered_map>
#include "Primitives/vertex.h"

class MayaCamera;


namespace Viewer
{

		class VIEWER_API Viewer3D : public GLWindow
		{
		private:

			struct PlaneData
			{
				Geometry::Plane::PlaneCoefficients coefficients;
				int renderObjectIndex;
			};

			typedef	std::unordered_map<std::string, PlaneData> PlaneMap;
			typedef PlaneMap::value_type PlaneMapPair;

		public:
			explicit Viewer3D(QWindow *parent = 0);
			~Viewer3D();

			void render(QPainter *painter);

			//void setVertexData( const vertex_v3fv3f* vertexData, const vertex_v3f* indexData, int countVertices, int countIndices);

			void setVertexData( const Geometry::Primitives::vec3* vertexData, const Geometry::Primitives::vec3* normalData, const unsigned int* indexData, int countVertices, int countIndices);


			/**
			* \brief Adds new vertex data to the viewer
			*
			* \return bool true, if successfully added, else false
			* \param  std::string name	The point cloud name
			* \param  const Viewer::Rendering::vec3 * vertexData The vertex data
			* \param  const Viewer::Rendering::vec3 * normalData The corresponding normal data
			* \param  const unsigned int * indexData Index data
			* \param  int countVertices The number of vertices
			* \param  int countIndices The number of indices
			* \param  int primitiveType The primitive type, eg. GL_POINTS
			*/
			int addVertexData( std::string name, const Geometry::Primitives::vec3* vertexData, const Geometry::Primitives::vec3* normalData, const unsigned int* indexData, int countVertices, int countIndices, int primitiveType);

			/**
			* \brief Adds new vertex data to the viewer
			*
			* \return bool true, if successfully added, else false
			* \param  std::string name	The point cloud name
			* \param  const Viewer::Rendering::vertex_v3n3 * vertexData The vertex data with normals
			* \param  const unsigned int * indexData Index data
			* \param  int countVertices The number of vertices
			* \param  int countIndices The number of indices
			* \param  int primitiveType The primitive type, eg. GL_POINTS
			*/
			int addVertexData( std::string name, const Geometry::Primitives::vertex_v3n3* vertexData, const unsigned int* indexData, int countVertices, int countIndices, int primitiveType);

		/**
		* \brief Adds new vertex data to the viewer
		*
		* \return bool true, if successfully added, else false
		* \param  std::string name	The point cloud name
		* \param  unsigned int vbo The vertex data as VBO
		* \param  int countVertices The number of vertices
		* \param  int countIndices The number of indices
		*/
		int addVertexData( std::string name, unsigned int vbo, int countVertices, int countIndices, Viewer::Rendering::VERTEX_FORMAT::Format format);



			/**
			* \brief Removes vertices from the viewer
			*
			* \return bool true, if successfully removed, else false
			* \param  std::string name The point cloud name
			*/
			bool removeVertexData(std::string name);

			
			/**
			* \brief Adds a plane with the given plane parameters to the viewer
			*
			* \return bool returns true if successfully added, else false
			* \param  std::string name The plane identifier
			* \param  const Geometry::Plane::PlaneCoefficients & coefficients The plane coefficients
			*/
			bool addPlane(std::string name, const Geometry::Plane::PlaneCoefficients& coefficients);

						
			/**
			* \brief Adds a plane at the given point to the viewer
			*
			* \return bool returns true if successfully added, else false
			* \param  std::string name The plane identifier
			* \param  Geometry::Primitives::vec3 point	A plane point
			* \param  const Geometry::Plane::PlaneCoefficients & coefficients The plane coefficients
			*/
			bool addPlane(std::string name, Geometry::Primitives::vec3 point, const Geometry::Plane::PlaneCoefficients& coefficients);

			/**
			* \brief Adds a plane at the given point to the viewer
			*
			* \return bool returns true if successfully added, else false
			* \param  std::string name The plane identifier
			* \param  const Geometry::Plane::PlaneCoefficients & coefficients The plane coefficients
			* \param bool filled determines if the plane should be filled
			* \param int planeSize The plane size
			* \param byte r The red color component
			* \param byte g The green color component
			* \param byte b The blue color component
			*/
			bool addPlane(std::string name, const Geometry::Plane::PlaneCoefficients& coefficients, bool filled, int planeSize = 1, byte r = 255, byte g = 255, byte b = 255);

			/**
			* \brief Adds a plane at the given point to the viewer
			*
			* \return bool returns true if successfully added, else false
			* \param  std::string name The plane identifier
			* \param  Geometry::Primitives::vec3 point	A plane point
			* \param  const Geometry::Plane::PlaneCoefficients & coefficients The plane coefficients
			* \param bool filled determines if the plane should be filled
			* \param int planeSize The plane size
			* \param byte r The red color component
			* \param byte g The green color component
			* \param byte b The blue color component
			*/
			bool addPlane(std::string name, Geometry::Primitives::vec3 point, const Geometry::Plane::PlaneCoefficients& coefficients, bool filled, int planeSize = 1, byte r = 255, byte g = 255, byte b = 255);


			/**
			* \brief Removes a plane from the internal list
			*
			* \return bool true if successful, else false
			* \param  std::string name The plane identifier
			*/
			bool removePlane(std::string name);


		void setTrackingBallVertex();

		protected:
			//! Mouse button event handler.
			virtual void mousePressEvent(QMouseEvent *m);

			//! Mouse double click event handler.
			virtual void mouseDoubleClickEvent(QMouseEvent *m);

			//! Mouse event handler.
			virtual void mouseMoveEvent(QMouseEvent *m);

			//! Mouse wheel event handler.
			virtual void wheelEvent(QWheelEvent *m);

			//! Key event handler
			virtual void keyPressEvent(QKeyEvent *m);


			public slots:


		protected:

			virtual void initialize();
			virtual void resizeEvent(QResizeEvent *event);
			virtual void addGrid(unsigned int lineCount);
			virtual void addTrackingBall(char trackingBallAxis);
			virtual void addOrientationArrows();

			//virtual bool initProgram();
			//virtual bool initArrayBuffer();
			//virtual bool initTexture2D();
			//virtual bool initVertexArray();
			//virtual bool initUniformBuffer();
			//virtual bool initDebugOutput();
			//virtual bool cleanUp();

		private:

			MayaCamera*		_virtMayaCamera;

			QPoint			_lastPos;						//!< last cursor position
			QPoint			_lastWheelPos;					//!< last cursor position
			bool			_lastWheelPosSet;			

			float			_moveCameraX;
			float			_moveCameraY;
			float			_moveCameraZ;

			GLdouble		_glMousePosX;
			GLdouble		_glMousePosY;
			GLdouble		_glMousePosZ;


			std::vector<Geometry::Primitives::vertex_v3> _trackingBallX;
			std::vector<Geometry::Primitives::vertex_v3> _trackingBallY;
			std::vector<Geometry::Primitives::vertex_v3> _trackingBallZ;

			PlaneMap		_planeMap;
		};

		typedef boost::shared_ptr<Viewer3D>	Viewer3DPtr;

}	// namespace Viewer


#endif	// Viewer3D_H
