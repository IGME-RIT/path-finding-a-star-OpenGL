/*
Title: SAT-2D (OBB)
File Name: Model.h
Copyright © 2015
Original authors: Brockton Roth
Written under the supervision of David I. Schwartz, Ph.D., and
supported by a professional development seed grant from the B. Thomas
Golisano College of Computing & Information Sciences
(https://www.rit.edu/gccis) at the Rochester Institute of Technology.
This program is free software: you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.
This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the GNU
General Public License for more details.
You should have received a copy of the GNU General Public License
along with this program.  If not, see <http://www.gnu.org/licenses/>.
Description:
This is a Separating Axis Theorem test. (Sometimes just called Separating Axis Test.) This is in 2D with multiple polygons.
*/
#ifndef _MODEL_H
#define _MODEL_H

#include "GLIncludes.h"
class Model
{
private:
	int numVertices;
	VertexFormat* vertices;

	int numIndices;
	GLuint* indices;

	GLuint vbo;
	GLuint ebo;


public:
	Model(int numVerts = 0, VertexFormat* verts = nullptr, int numInds = 0, GLuint* inds = nullptr);
	~Model();

	
	GLuint AddVertex(VertexFormat*);
	void AddIndex(GLuint);

	void InitBuffer();
	void UpdateBuffer();

	void Draw();

	// Our get variables.
	int NumVertices()
	{
		return numVertices;
	}
	int NumIndices()
	{
		return numIndices;
	}
	VertexFormat* Vertices()
	{
		return vertices;
	}
	GLuint* Indices()
	{
		return indices;
	}

	
};

#endif _MODEL_H
