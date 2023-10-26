#pragma once

#include "image.h"
#include <vector>
#include <string>

Image3 hw_2_1(const std::vector<std::string> &params);
Image3 hw_2_2(const std::vector<std::string> &params);
Image3 hw_2_3(const std::vector<std::string> &params);
Image3 hw_2_4(const std::vector<std::string> &params);

// hw 1
// return 1 if point (x, y) is within triangle defined by p0, p1, p2
bool inTriangle(Vector2 p0, Vector2 p1, Vector2 p2, Real x, Real y);


// hw 2
void downSample(int ss, Image3* ssimg, Image3* img);
bool clipTri(Vector3 p0, Vector3 p1, Vector3 p2, Real z_near);
void projTri(Real s, Real a, int w, int h,
			 Vector3* p0, Vector3* p1, Vector3* p2,
			 Vector2* q0, Vector2* q1, Vector2* q2);
void compZTri(Vector3* p0, Vector3* p1, Vector3* p2,
			  Vector2* q0, Vector2* q1, Vector2* q2,
			  Real z);
