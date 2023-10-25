#include "hw2.h"
#include "hw2_scenes.h"



// return 1 if triangle is behind clipping plane
bool clipTri(Vector3 p0, Vector3 p1, Vector3 p2, Real z_near) {
	
	Real maxZ = - 1;
	
	if (p0.z > maxZ) maxZ = p0.z;
	if (p1.z > maxZ) maxZ = p1.z;
	if (p2.z > maxZ) maxZ = p2.z;
	
	return (maxZ >= z_near);
}

bool inTriangle(Vector2 p0, Vector2 p1, Vector2 p2, Real x, Real y) {
	Vector2 q;
	q.x = x;
	q.y = y;
	
	Vector2 v0 = p1 - p0;
	Vector2 v1 = p2 - p1;
	Vector2 v2 = p0 - p2;
	
	Vector2 q0, q1, q2;
	
	q0 = q - p0;
	q1 = q - p1;
	q2 = q - p2;
	
	Vector2 n0, n1, n2;
	n0.x = v0.y;
	n0.y = - v0.x;
	n1.x = v1.y;
	n1.y = - v1.x;
	n2.x = v2.y;
	n2.y = - v2.x;
	
	Real a = dot(q0, n0);
	Real b = dot(q1, n1);
	Real c = dot(q2, n2);
	
	if (a > 0 && b > 0 && c > 0) return true;
	if (a < 0 && b < 0 && c < 0) return true;
	
	return false;
}

void projTri(Real s, Real a, int w, int h,
			 Vector3* p0, Vector3* p1, Vector3* p2,
			 Vector2* q0, Vector2* q1, Vector2* q2) {
	
	// 3D space to camera space
	
	q0->x = -1 * (p0->x / p0->z);
	q0->y = -1 * (p0->y / p0->z);
	
	q1->x = -1 * (p1->x / p1->z);
	q1->y = -1 * (p1->y / p1->z);
	
	q2->x = -1 * (p2->x / p2->z);
	q2->y = -1 * (p2->y / p2->z);
	
	// camera space to image space
	
	q0->x = w * ((q0->x + (s*a))/
				 (2 * s * a));
	q0->y = h * ((q0->y + s)/
				 (2 * s));
	
	q1->x = w * ((q1->x + (s*a))/
				 (2 * s * a));
	q1->y = h * ((q1->y + s)/
				 (2 * s));
	
	q2->x = w * ((q2->x + (s*a))/
				 (2 * s * a));
	q2->y = h * ((q2->y + s)/
				 (2 * s));
}

using namespace hw2;

Image3 hw_2_1(const std::vector<std::string> &params) {
	// Homework 2.1: render a single 3D triangle

	Image3 img(640 /* width */, 480 /* height */);
	
	Image3 ssimg(img.width*4, img.height*4);
	
	// three points define triangle in 3D space
	Vector3 p0{0, 0, -1};
	Vector3 p1{1, 0, -1};
	Vector3 p2{0, 1, -1};
	
	Real s = 1; // scaling factor of the view frustrum
	Real a = Real(img.width) / Real(img.height);
	
	// color of triangle? this is unspecified
	Vector3 color = Vector3{1.0, 0.5, 0.5};
	
	
	Real z_near = 1e-6;	// distance of the near clipping plane
	
	
	// defines the single triangle to render
	for (int i = 0; i < (int)params.size(); i++) {
		if (params[i] == "-s") {
			s = std::stof(params[++i]);
		} else if (params[i] == "-p0") {
			p0.x = std::stof(params[++i]);
			p0.y = std::stof(params[++i]);
			p0.z = std::stof(params[++i]);
		} else if (params[i] == "-p1") {
			p1.x = std::stof(params[++i]);
			p1.y = std::stof(params[++i]);
			p1.z = std::stof(params[++i]);
		} else if (params[i] == "-p2") {
			p2.x = std::stof(params[++i]);
			p2.y = std::stof(params[++i]);
			p2.z = std::stof(params[++i]);
		} else if (params[i] == "-color") {
			Real r = std::stof(params[++i]);
			Real g = std::stof(params[++i]);
			Real b = std::stof(params[++i]);
			color = Vector3{r, g, b};
		} else if (params[i] == "-znear") {
			z_near = std::stof(params[++i]);
		}
	}
	
	if (clipTri(p0, p1, p2, z_near)) {
		fprintf(stderr, "clip triangle, don't render\n");
		
		for (int y = 0; y < img.height; y++) {
			for (int x = 0; x < img.width; x++) {
				img(x, y) = Vector3{0.5, 0.5, 0.5};		// only draw background color
			}
		}
	}
	
	Vector2 q0, q1, q2;
	projTri(s, a, ssimg.width, ssimg.height, &p0, &p1, &p2, &q0, &q1, &q2);
	

	for (int y = 0; y < ssimg.height; y++) {
		for (int x = 0; x < ssimg.width; x++) {
			
			if (inTriangle(q0, q1, q2, x, y)) {
				ssimg(x, y) = color;
			} else {
				ssimg(x, y) = Vector3{0.5, 0.5, 0.5};
			}
			
		}
	}
	
	ssimg (300, 200) = Vector3{1.0, 0.0, 0.0};
	return ssimg;
}

Image3 hw_2_2(const std::vector<std::string> &params) {
	// Homework 2.2: render a triangle mesh

	Image3 img(640 /* width */, 480 /* height */);

	Real s = 1; // scaling factor of the view frustrum
	Real z_near = 1e-6; // distance of the near clipping plane
	int scene_id = 0;
	for (int i = 0; i < (int)params.size(); i++) {
		if (params[i] == "-s") {
			s = std::stof(params[++i]);
		} else if (params[i] == "-znear") {
			z_near = std::stof(params[++i]);
		} else if (params[i] == "-scene_id") {
			scene_id = std::stoi(params[++i]);
		}
	}

	TriangleMesh mesh = meshes[scene_id];
	UNUSED(mesh); // silence warning, feel free to remove this

	for (int y = 0; y < img.height; y++) {
		for (int x = 0; x < img.width; x++) {
			img(x, y) = Vector3{1, 1, 1};
		}
	}
	return img;
}

Image3 hw_2_3(const std::vector<std::string> &params) {
	// Homework 2.3: render a triangle mesh with vertex colors

	Image3 img(640 /* width */, 480 /* height */);

	Real s = 1; // scaling factor of the view frustrum
	Real z_near = 1e-6; // distance of the near clipping plane
	int scene_id = 0;
	for (int i = 0; i < (int)params.size(); i++) {
		if (params[i] == "-s") {
			s = std::stof(params[++i]);
		} else if (params[i] == "-znear") {
			z_near = std::stof(params[++i]);
		} else if (params[i] == "-scene_id") {
			scene_id = std::stoi(params[++i]);
		}
	}

	TriangleMesh mesh = meshes[scene_id];
	UNUSED(mesh); // silence warning, feel free to remove this

	for (int y = 0; y < img.height; y++) {
		for (int x = 0; x < img.width; x++) {
			img(x, y) = Vector3{1, 1, 1};
		}
	}
	return img;
}

Image3 hw_2_4(const std::vector<std::string> &params) {
	// Homework 2.4: render a scene with transformation
	if (params.size() == 0) {
		return Image3(0, 0);
	}

	Scene scene = parse_scene(params[0]);
	std::cout << scene << std::endl;

	Image3 img(scene.camera.resolution.x,
			   scene.camera.resolution.y);

	for (int y = 0; y < img.height; y++) {
		for (int x = 0; x < img.width; x++) {
			img(x, y) = Vector3{1, 1, 1};
		}
	}
	return img;
}

