/*
File Name : graph.h
Copyright © 2018
Original authors : Sanketh Bhat
Written under the supervision of David I.Schwartz, Ph.D., and
supported by a professional development seed grant from the B.Thomas
Golisano College of Computing & Information Sciences
(https ://www.rit.edu/gccis) at the Rochester Institute of Technology.

This program is free software : you can redistribute it and/or modify
it under the terms of the GNU General Public License as published by
the Free Software Foundation, either version 3 of the License, or (at
your option) any later version.

This program is distributed in the hope that it will be useful, but
WITHOUT ANY WARRANTY; without even the implied warranty of
MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.See the GNU
General Public License for more details.

You should have received a copy of the GNU General Public License
along with this program.If not, see <http://www.gnu.org/licenses/>.

Description:
C++ demonstration of the A* pathfinding algorithm
*/


//https://www.geeksforgeeks.org/a-search-algorithm/
//http://code.activestate.com/recipes/577457-a-star-shortest-path-algorithm/
#ifndef GRAPH_H
#define GRAPH_H

#include <time.h>
#include <queue>
#include <functional>
#include <iostream>
#include <vector>
#include <string>

const int S = 16; //Length of a map side

const int dir = 8; //Number of possible directions we can move

//4 directions
//static int dx[dir]={1, 0, -1, 0};
//static int dy[dir]={0, 1, 0, -1};

//8 directions
static int dx[dir] = { 1, 1, 0, -1, -1, -1, 0, 1 };
static int dy[dir] = { 0, 1, 1, 1, 0, -1, -1, -1 };

//Struct to hold a x and y value(like a 2d vector)
struct Position {

	int x;
	int y;

};

//Create an operator to check for equality
static bool operator==(const Position &A, const Position &B)
{
	return (A.x == B.x && B.y == A.y);
}

//struct for a basic unit in a grid based map
struct Unit {
	Position pos;


	//A* data
	std::string status = "";
	bool open = false;
	bool closed = false;
	float f=0; //total estimated cost (prioritry, lower=better)
	float g=0; //cost so far
	float h=0; //estimate cost from this unit to the end (main heuristic)

	void estimatePriority(int xf, int yf)
	{
		int xd, yd;
		
		xd = xf - pos.x;
		yd = yf - pos.y;

		h = sqrt(xd*xd + yd*yd);

		//h=abs(xd)+abs(yd);

		//h=std::max(abs(xd), abs(yd));
		//Estimated cost = cost so fa
		f = g + h * 10;
	}

	//Updating the unit's value of g (cost to get to this unit)
	void updateCost(float _g)
	{
		g = _g + 10;
		
	}
};



static bool operator<(const Unit &A, const Unit &B)
{
	return A.f > B.f; 
}

class graph {
	
	std::priority_queue<Unit> openUnits; //We use a priority queue for the open list.
	

	int pMap[S][S]; //Map of unit priorities(value of f)

	void initMap(int oCount);

	void printGraph();
	int randIndex();
	float calcDist(Position p1, Position p2);

public:
	Unit map[S][S];
	Position start;
	Position end;
	std::vector<Unit> path;
	void aStarPF();
	graph();
	
};


#endif GRAPH_H
