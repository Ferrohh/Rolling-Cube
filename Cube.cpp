#include "Cube.hpp"

Cube::Cube(int cubeWidth, int scaling, int windowWidth, int windowHeight, float _speed)
:w_width(windowWidth), w_height(windowHeight), c_width(cubeWidth), scaling(scaling), speed(_speed)
{
    zBuffer.resize(w_width * w_height);
    buffer.resize(w_width * w_height);
}

void Cube::clear() {
#if defined _WIN32
    system("cls");
    //clrscr(); // including header file : conio.h
#elif defined (__LINUX__) || defined(__gnu_linux__) || defined(__linux__)
    system("clear");
    //std::cout<< u8"\033[2J\033[1;1H"; //Using ANSI Escape Sequences 
#elif defined (__APPLE__)
    system("clear");
#endif
}

float Cube::calculateX(int i, int j, int k) {
    return j*sin(A)*sin(B)*cos(C) - k*cos(A)*sin(B)*cos(C) + j*cos(A)*sin(C) + k*sin(A)*sin(C) + i*cos(B)*cos(C);
}


float Cube::calculateY(int i, int j, int k) {
    return j*cos(A)*cos(C) + k*sin(A)*cos(C) - j*sin(A)*sin(B)*sin(C) + k*cos(A)*sin(B)*sin(C) - i*cos(B)*sin(C);
}


float Cube::calculateZ(int i, int j, int k) {
    return k*cos(A)*cos(B) - j*sin(A)*cos(B) + i*sin(B);
}


void Cube::calculateForSurface(float c_x, float c_y, float c_z, char ch) {
	// find x y and z in 3D
	float x = Cube::calculateX(c_x, c_y, c_z);
	float y = Cube::calculateY(c_x, c_y, c_z);
	float z = Cube::calculateZ(c_x, c_y, c_z) + 100;

	float z_rec = 1/z;

	// find x and y in 2D
	int x_plane = (w_width/2 + scaling * z_rec * x * 2);
	int y_plane = (w_height/2 + scaling * z_rec * y);

	// find index from x and y 2D
	int i = x_plane + y_plane * w_width;

	if ((i >= 0 && i < w_width*w_height) && // control is not out of bound
	(z_rec > zBuffer[i])) { // control that the scalar is more depth
		
		zBuffer[i] = z_rec; // put the dept into the z buffer
		buffer[i] = ch; // put the char into the buffer
	}
}	

void Cube::run() {
	Cube::clear();
	
	while(true) {
		std::fill(buffer.begin(), buffer.end(), bg);
		std::fill(zBuffer.begin(), zBuffer.end(), 0.0f);

		h_offset = -2*c_width;

		// go through every coords
		// we start from negative to center it in (0,0)
		for(float c_x = -c_width; c_x < c_width; c_x+=speed) {
			for(float c_y = -c_width; c_y < c_width; c_y+=speed) {
				calculateForSurface(c_x, c_y, -c_width, '@'); 	// side 1
				calculateForSurface(c_width, c_y, c_x, '$'); 	// side 2
				calculateForSurface(-c_width, c_y, -c_x, '~'); 	// side 3
				calculateForSurface(-c_x, c_y, c_width, '#'); 	// side 4
				calculateForSurface(c_x, -c_width, -c_y, ';'); 	// side 5
				calculateForSurface(c_x, c_width, c_y, '+'); 	// side 6
			}
		}

		// printing
		for(int i = 0; i < w_width * w_height; i++) {
			std::cout << (i%w_width ? buffer[i] : '\n');
		}
		
		A+=0.05;
		B+=0.05;
		C+=0.05;
		
		usleep(8000);
	}
}
