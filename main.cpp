/*
File Name: main.cpp
Copyright © 2018
Original authors: Sanketh Bhat
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

C++ demonstration of the A* algorithm
*/


#include "GLRender.h"
#include "GameObject.h"
#include "graph.h"


#pragma region program specific Data members

float timestep = .016;

enum State{start,end,obstacle,pathing};

State current = start;
glm::vec3 mousePos;
int obscount = 0;
graph *g;

Model* baseMesh;
Model* pathMesh;
Model* obsMesh;
Model* pointMesh;
// vector of scene bodies
std::vector<GameObject*> bodies;

GameObject *goMap[S][S];

#pragma endregion


#pragma region util_functions

glm::vec3 GetMousePos()
{

	double x, y;
	glfwGetCursorPos(window, &x, &y);

	//normalized device coordinates
	float NDCX = (x / 400.0f) - 1.0f;
	float NDCY = -(y / 400.0f) + 1.0f;

	return glm::vec3(NDCX, NDCY, 0.0f);


}

//Returns the position of the unit under the mouse position
glm::vec2 getUnit(glm::vec3 mPos)
{
	
	for (int i = 0; i < S; i++)
	{

		for (int j = 0; j < S; j++)
		{
			if (glm::distance(mPos, goMap[i][j]->Position()) < 1.0f / S)
			{
				return glm::vec2(i, j);
			}	
		}
	}
	return  glm::vec2(-1, -1);
}

//CHnages the color of the clicked unit based on the current pathfind state
void changeColor(glm::vec2 pos)
{
	if (pos == glm::vec2(-1, -1))
		return;

	
	if (current == start)
	{
		g->start.x = pos.x;
		g->start.y = pos.y;
		goMap[g->start.x][g->start.y]->setModel(pointMesh);
		

		current = end;
	}
	else if (current == end)
	{
		g->end.x = pos.x;
		g->end.y = pos.y;
		goMap[g->end.x][g->end.y]->setModel(pointMesh);

		current = obstacle;

		std::cout << "Select the obstacles on the map.\n";
	}
	else if (current == obstacle)
	{
		g->map[(int)pos.x][(int)pos.y].status = "O";
		goMap[(int)pos.x][(int)pos.y]->setModel(obsMesh);

		obscount++;

		if (obscount > S*2)
		{
			current = pathing;

			g->aStarPF();

			for (Unit u : g->path)
			{
				goMap[u.pos.x][u.pos.y]->setModel(pathMesh);
			}
		}
		else
			std::cout << S * 2 - obscount << " obstacles left.\n";
	}
}

void update()
{

	for each (GameObject* body in bodies)
	{
		body->update(timestep, PV);
	}
}


// This function is used to handle key inputs.
// It is a callback funciton. i.e. glfw takes the pointer to this function (via function pointer) and calls this function every time a key is pressed in the during event polling.
void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
	//Sets the current window to a close flag when ESC is pressed
	if (key == GLFW_KEY_ESCAPE && ((action == GLFW_PRESS) || action == GLFW_REPEAT))
	{
		glfwSetWindowShouldClose(window, 1);
	}



}

void mouse_button_callback(GLFWwindow* window, int button, int action, int mods)
{
	if (button == GLFW_MOUSE_BUTTON_LEFT && action == GLFW_PRESS)
	{
		changeColor(getUnit(GetMousePos()));
	}
}


#pragma endregion

//Makes a 2D polygon based on the vertex number arguement
Model* setupModel(int n, glm::vec4 color, float size = 0.25f)
{
	std::vector<GLuint> indices;
	std::vector<VertexFormat> vertices;
	VertexFormat center(glm::vec3(0.0f, 0.0f, 0.0f), color);

	//Only add indices if you drawing a polygon with more than 3 points.
	if (n > 3)
	{
		//Indices are added in threes to form tris
		for (int i = 1; i < n + 1; i++)
		{
			indices.push_back(0); //Start at the center
			if (i == n) //If we are at the last last vertex, go back to the first  non-center vertex and add it
			{
				indices.push_back(i);
				indices.push_back(1);

			}
			else
			{	//Adds current vertex and the next one
				indices.push_back(i);
				indices.push_back(i + 1);

			}

		}

		//Only 3+ point polygons need a center vertex
		vertices.push_back(center);
	}

	float theta = 360.0f / n;


	//vertex generation
	for (int i = 0; i < n; i++)
	{
		//The point at angle theta  are fed into the buffer.
		vertices.push_back(VertexFormat(glm::vec3(size * cos(glm::radians(i*theta)), size * sin(glm::radians(i*theta)), 0.0f), color));

	}
	return new Model(vertices.size(), vertices.data(), indices.size(), indices.data());

}




void main()
{
	// Initializes most things needed before the main loop
	init();
	
	// Sends the funtion as a funtion pointer along with the window to which it should be applied to.
	glfwSetKeyCallback(window, key_callback);
	glfwSetMouseButtonCallback(window, mouse_button_callback);


	g = new graph();

	//Creating differently colored squares for the grid
	baseMesh = setupModel(4, glm::vec4(0.2, 0.4,0.4, 1),1.0f / S);
	obsMesh= setupModel(4, glm::vec4(0.8, 0, 0, 1), 1.0f / S);
	pathMesh = setupModel(4, glm::vec4(0, 1, 0, 1), 1.0f / S);
	pointMesh = setupModel(4, glm::vec4(0, 0, 0, 1), 1.0f / S);

	//Creating grid elements and placing them on screen
	float x = -1 + 1.0f / S;
	float y = 1 - 1.0f / S;

	for (int i = 0; i < S; i++)
	{
		for (int j = 0; j < S; j++)
		{
			goMap[i][j] = new GameObject(baseMesh);
			goMap[i][j]->setRotation(glm::vec3(0, 0, PI / 4));

			goMap[i][j]->Position(glm::vec3(x,y, 0));

			y -= 2.0f / S;

			bodies.push_back(goMap[i][j]);
		}

		y = 1 - 1.0f / S;
		x += 2.0f / S;

	}

	std::cout << "Select the start and end positions on the map.\n";

	

	// Enter the main loop.
	while (!glfwWindowShouldClose(window))
	{
		// Call to update() which will update the gameobjects.
		update();

		// Call the render function(s).
		renderScene();

		for each (GameObject* body in bodies)
		{
			body->render(uniMVP);
		}
		// Swaps the back buffer to the front buffer
		// Remember, you're rendering to the back buffer, then once rendering is complete, you're moving the back buffer to the front so it can be displayed.
		glfwSwapBuffers(window);

		// Checks to see if any events are pending and then processes them.
		glfwPollEvents();
	}

	for (GameObject *body : bodies)
		delete body;

	delete g;
	//Cleans shaders and the program and frees up GLFW memory
	cleanup();

	return;
}