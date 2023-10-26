#include "hw2.h"
#include "hw2_scenes.h"

// downSamples image ssimg to img by factor ss
void downSample(int ss, Image3* ssimg, Image3* img) {
	for (int y = 0; y < img->height; y++) {
		for (int x = 0; x < img->width; x++) {
			
			Vector3 sum;
			
			sum.x = Real(0);
			sum.y = Real(0);
			sum.z = Real(0);
			
			for (int i = 0; i < ss; i++) {
				for (int j = 0; j < ss; j++) {
					sum = sum + (*ssimg)(ss*x + i, ss*y + j);
				}
			}
			
			(*img)(x, y) = sum * (Real(1) / Real(ss * ss));
		}
	}
}

// return 1 if triangle is behind clipping plane
bool clipTri(Vector3 p0, Vector3 p1, Vector3 p2, Real z_near) {
	
	Real maxZ = - 1;
	
	if (p0.z > maxZ) maxZ = p0.z;
	if (p1.z > maxZ) maxZ = p1.z;
	if (p2.z > maxZ) maxZ = p2.z;
	
	return (maxZ >= z_near);
}

// projects p0, p1, p2 onto image plane (s, a, w, h) at q0, q1, q2
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
	q0->y = h * ((s - q0->y)/
				 (2 * s));
	
	q1->x = w * ((q1->x + (s*a))/
				 (2 * s * a));
	q1->y = h * ((s - q1->y)/
				 (2 * s));
	
	q2->x = w * ((q2->x + (s*a))/
				 (2 * s * a));
	q2->y = h * ((s - q2->y)/
				 (2 * s));
}

//
void Tri2cam(Vector3* p0, Vector3* p1, Vector3* p2,
			 Vector2* q0, Vector2* q1, Vector2* q2) {
	
	// 3D space to camera space
	
	q0->x = -1 * (p0->x / p0->z);
	q0->y = -1 * (p0->y / p0->z);
	
	q1->x = -1 * (p1->x / p1->z);
	q1->y = -1 * (p1->y / p1->z);
	
	q2->x = -1 * (p2->x / p2->z);
	q2->y = -1 * (p2->y / p2->z);
}

//
void cam2scr(Real s, Real a, int w, int h,
			 Vector2* q0, Vector2* q1, Vector2* q2) {
	// camera space to image space
	
	q0->x = w * ((q0->x + (s*a))/
				 (2 * s * a));
	q0->y = h * ((s - q0->y)/
				 (2 * s));
	
	q1->x = w * ((q1->x + (s*a))/
				 (2 * s * a));
	q1->y = h * ((s - q1->y)/
				 (2 * s));
	
	q2->x = w * ((q2->x + (s*a))/
				 (2 * s * a));
	q2->y = h * ((s - q2->y)/
				 (2 * s));
}

// compute the z value of a given pixel projection to a triangle
Real compZTri(Vector3* p0, Vector3* p1, Vector3* p2,
			  Vector2* q0, Vector2* q1, Vector2* q2,
			  Vector3* p_) {
	Real b0_, b1_, b2_, b0, b1, b2, area, B, z;
	
	Vector3 q0_{q0->x, q0->y, 0.0};
	Vector3 q1_{q1->x, q1->y, 0.0};
	Vector3 q2_{q2->x, q2->y, 0.0};
	
	area = length(cross((q1_ - q0_), (q2_ - q0_))) / 2;
	
	b0_ = (length(cross(q1_ - *p_, q2_ - *p_))) / 2 / area;
	b1_ = (length(cross(q0_ - *p_, q2_ - *p_))) / 2 / area;
	b2_ = (length(cross(q0_ - *p_, q1_ - *p_))) / 2 / area;
	
	B = 1 / ((b0_ / p0->z) + (b1_ / p1->z) + (b2_ / p2->z));
	
	b0 = (B * b0_) / p0->z;
	b1 = (B * b1_) / p1->z;
	b2 = (B * b2_) / p2->z;
	
	z = (b0 * p0->z) + (b1 * p1->z) + (b2 * p2->z);
	
	return z;
}

// compute the barrycentric coordinates
Vector3 compBTri(Vector3* p0, Vector3* p1, Vector3* p2,
				 Vector2* q0, Vector2* q1, Vector2* q2,
				 Vector3* p_) {
	Real b0_, b1_, b2_, b0, b1, b2, area, B;
	
	Vector3 q0_{q0->x, q0->y, 0.0};
	Vector3 q1_{q1->x, q1->y, 0.0};
	Vector3 q2_{q2->x, q2->y, 0.0};
	
	area = length(cross((q1_ - q0_), (q2_ - q0_))) / 2;
	
	b0_ = (length(cross(q1_ - *p_, q2_ - *p_))) / 2 / area;
	b1_ = (length(cross(q0_ - *p_, q2_ - *p_))) / 2 / area;
	b2_ = (length(cross(q0_ - *p_, q1_ - *p_))) / 2 / area;
	
	//std::cout << b0_ << b1_ << b2_ << " " << b0_ + b1_ + b2_ << std::endl;
	
	B = 1 / ((b0_ / p0->z) + (b1_ / p1->z) + (b2_ / p2->z));
	
	b0 = (B * b0_) / p0->z;
	b1 = (B * b1_) / p1->z;
	b2 = (B * b2_) / p2->z;
	
	//std::cout << b0 << b1 << b2 << " " << b0 + b1 + b2 << std::endl;
	
	Vector3 b_coor(b0, b1, b2);
	
	return b_coor;
}

void image2camera(Real x, Real y, Real s, Real a, int w, int h, Vector3* p_) {
	
	p_->x = (x * ((2 * s * a)/w)) - (s * a);
	p_->y = s - (y * ((2 * s)/ h));
	p_->z = 0.0;
	
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
	
	downSample(4, &ssimg, &img);
	return img;
}

Image3 hw_2_2(const std::vector<std::string> &params) {
	
	
	// Homework 2.2: render a triangle mesh
	int ss = 4;
	Image3 img(640 /* width */, 480 /* height */);
	Image3 ssimg(img.width * ss, img.height * ss);
	
	// define image parameters
	Real a = Real(img.width) / Real(img.height);
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
	
	int tris = (int) mesh.faces.size();
	
	Vector3 p0, p1, p2;
	Vector2 q0, q1, q2;
	Real z;
	
	// init z buffer & set background color
	Image1 z_buf(ssimg.width, ssimg.height);
	
	for (int i = 0; i < ssimg.width; i++) {
		for (int j = 0; j < ssimg.height; j++) {
			z_buf(i, j) = - std::numeric_limits<Real>::infinity();
			ssimg(i, j) = Vector3(0.5, 0.5, 0.5);
		}
	}
	
	// iterate over objects
	for (int t = 0; t < tris; t++) {
		
		// get triangle
		p0 = mesh.vertices[mesh.faces[t][0]];
		p1 = mesh.vertices[mesh.faces[t][1]];
		p2 = mesh.vertices[mesh.faces[t][2]];
		
		// dont draw clipped triangles
		if (clipTri(p0, p1, p2, z_near)) {
			fprintf(stderr, "clip triangle, don't render\n");
			continue;
		}
		
		projTri(s, a, ssimg.width, ssimg.height, &p0, &p1, &p2, &q0, &q1, &q2);
		
		for (int y = 0; y < ssimg.height; y++) {
			for (int x = 0; x < ssimg.width; x++) {
				
				if (!inTriangle(q0, q1, q2, x, y)) continue;
				
				Vector3 p_(Real(x), Real(y), 0.0);
				
				z = compZTri(&p0, &p1, &p2, &q0, &q1, &q2, &p_);
				
				if (z > z_buf(x, y)) {
					z_buf(x, y) = z;
					ssimg(x, y) = mesh.face_colors[t];
				}
			}
		}
		
	}

	downSample(ss, &ssimg, &img);
	
	return img;
}

Image3 hw_2_3(const std::vector<std::string> &params) {
	// Homework 2.3: render a triangle mesh with vertex colors

	int ss = 4;
	Image3 img(640 /* width */, 480 /* height */);
	Image3 ssimg(img.width * ss, img.height * ss);
	
	// define image parameters
	Real a = Real(img.width) / Real(img.height);
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
	
	int tris = (int) mesh.faces.size();
	
	
	Vector3 p0, p1, p2, color, c0, c1, c2;
	Vector2 q0, q1, q2;
	Real z;
	
	// init z buffer & set background color
	Image1 z_buf(ssimg.width, ssimg.height);
	
	for (int i = 0; i < ssimg.width; i++) {
		for (int j = 0; j < ssimg.height; j++) {
			z_buf(i, j) = - std::numeric_limits<Real>::infinity();
			ssimg(i, j) = Vector3(0.5, 0.5, 0.5);
		}
	}
	
	// iterate over objects
	for (int t = 0; t < tris; t++) {
		
		// get triangle vertices
		p0 = mesh.vertices[mesh.faces[t][0]];
		p1 = mesh.vertices[mesh.faces[t][1]];
		p2 = mesh.vertices[mesh.faces[t][2]];
		
		// get vertex colors
		c0 = mesh.vertex_colors[mesh.faces[t][0]];
		c1 = mesh.vertex_colors[mesh.faces[t][1]];
		c2 = mesh.vertex_colors[mesh.faces[t][2]];
		
		// dont draw clipped triangles
		if (clipTri(p0, p1, p2, z_near)) {
			fprintf(stderr, "clip triangle, don't render\n");
			continue;
		}
		
		projTri(s, a, ssimg.width, ssimg.height, &p0, &p1, &p2, &q0, &q1, &q2);
		
		for (int y = 0; y < ssimg.height; y++) {
			for (int x = 0; x < ssimg.width; x++) {
				
				if (!inTriangle(q0, q1, q2, x, y)) continue;
				
				Vector3 p_(Real(x), Real(y), 0.0);
				
				Vector3 b_coor = compBTri(&p0, &p1, &p2, &q0, &q1, &q2, &p_);
				
				color = (b_coor.x * c0) + (b_coor.y * c1) + (b_coor.z * c2);
				
				z = (b_coor[0] * p0.z) + (b_coor[1] * p1.z) + (b_coor[2] * p2.z);
				
				if (z > z_buf(x, y)) {
					z_buf(x, y) = z;
					ssimg(x, y) = color;
				}
			}
		}
	}
	
	downSample(ss, &ssimg, &img);
	
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

