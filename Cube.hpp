#pragma once
#include <iostream>
#include <cmath>
#include "Point.hpp"
#include <vector>
#ifndef _WIN32
#include <unistd.h>
#else
#include <windows.h>
void usleep(__int64 usec)
{
  HANDLE timer;
  LARGE_INTEGER ft;

  ft.QuadPart = -(10 * usec); // Convert to 100 nanosecond interval, negative value indicates relative time

  timer = CreateWaitableTimer(NULL, TRUE, NULL);
  SetWaitableTimer(timer, &ft, 0, NULL, NULL, 0);
  WaitForSingleObject(timer, INFINITE);
  CloseHandle(timer);
}
#endif

class Cube {
    private:
        float A, B, C; // angles of the cube
        float c_width; // width of the cube
		float speed; // speed of movement
        int w_width, w_height; // window dimensions
        int scaling; // make the cube larger
		int h_offset; // horizontal offset

		std::vector<float> zBuffer; // buffer that contains depth
		std::vector<char> buffer; // buffer that contains chars of the figure

		char bg = ' ';

		float calculateX(int i, int j, int k); // find X in 3D
        float calculateY(int i, int j, int k); // find Y in 3D
        float calculateZ(int i, int j, int k); // find Z in 3D
		
        // find x and y in 2D
		void calculateForSurface(float c_x, float c_y, float c_z, char ch);
		
        // clear chat
		void clear();

    public:
        Cube(int cubeWidth = 20, int scaling = 20, int windowWidth = 160, int windowHeight = 44, float _speed = 1); // constructor
		// run the cube
        void run();
};
