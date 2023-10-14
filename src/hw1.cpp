#include "hw1.h"
#include "hw1_scenes.h"

using namespace hw1;

Image3 hw_1_1(const std::vector<std::string> &params) {
    // Homework 1.1: render a circle at the specified
    // position, with the specified radius and color.

    Image3 img(640 /* width */, 480 /* height */);

    Vector2 center = Vector2{img.width / 2 + Real(0.5), img.height / 2 + Real(0.5)};
    Real radius = 100.0;
    Vector3 color = Vector3{1.0, 0.5, 0.5};
    for (int i = 0; i < (int)params.size(); i++) {
        if (params[i] == "-center") {
            Real x = std::stof(params[++i]);
            Real y = std::stof(params[++i]);
            center = Vector2{x, y};
        } else if (params[i] == "-radius") {
            radius = std::stof(params[++i]);
        } else if (params[i] == "-color") {
            Real r = std::stof(params[++i]);
            Real g = std::stof(params[++i]);
            Real b = std::stof(params[++i]);
            color = Vector3{r, g, b};
        }
    }

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
			
			// calculate distance of pixel to circle center
			Real dist 	= (x - center.x)*(x - center.x)
						+ (y - center.y)*(y - center.y);
			dist = sqrt(dist);
			
			// if distance < radius => pixel inside circle
			if (dist < radius) {
				img(x, y) = color;
			} else {
				img(x, y) = Vector3{0.5, 0.5, 0.5};
			}
        }
    }
    return img;
}

Image3 hw_1_2(const std::vector<std::string> &params) {
    // Homework 1.2: render multiple circles
    if (params.size() == 0) {
        return Image3(0, 0);
    }
	
	int boundingBox = 1;

    int scene_id = std::stoi(params[0]);
    const CircleScene &scene = hw1_2_scenes[scene_id];

    Image3 img(scene.resolution.x, scene.resolution.y);

	if (boundingBox) {
		for (int y = 0; y < img.height; y++) {
			for (int x = 0; x < img.width; x++) {
				img(x, y) = scene.background;
				
				auto object = scene.objects.rbegin();
				
				// check for bounding box
				while (object != scene.objects.rend()) {
					
					Circle circle = *object;
					
					if (abs(x - circle.center.x) < circle.radius &&
						abs(y - circle.center.y) < circle.radius) {
						
						Real dist 	= (x - circle.center.x)*(x - circle.center.x)
									+ (y - circle.center.y)*(y - circle.center.y);
						dist = sqrt(dist);
						
						// if distance < radius => pixel inside circle
						if (dist < circle.radius) {
							img(x, y) = circle.color;
							break;
						}
					// otherwise check objects further back
					}
					object++;
				}
			}
		}
	} else {
		for (int y = 0; y < img.height; y++) {
			for (int x = 0; x < img.width; x++) {
				img(x, y) = scene.background;
				
				// start with furthest back circle
				auto object = scene.objects.begin();
				
				// draw each circle by checking for pixel-center distance
				while (object != scene.objects.end()) {
					Circle circle = *object;
					
					Real dist 	= (x - circle.center.x)*(x - circle.center.x)
								+ (y - circle.center.y)*(y - circle.center.y);
					dist = sqrt(dist);
					
					// if distance < radius => pixel inside circle
					if (dist < circle.radius) {
						img(x, y) = circle.color;
					}
					object++;
				}
			}
		}
	}
    return img;
}

struct BBox {
	Vector2 p_min, p_max;
};

int min3(int a, int b, int c) {
	if (a <= b && a <= c) return a;
	else if (b <= a && b <= c) return b;
	else return c;
}

int max3(int a, int b, int c) {
	if (a >= b && a >= c) return a;
	else if (b >= a && b >= c) return b;
	else return c;
}

int min2(int a, int b) {
	if (a <= b) return a;
	return b;
}

int max2(int a, int b) {
	if (a >= b) return a;
	return b;
}

BBox findBBox(Shape shape) {
	BBox bbox;
	if (auto *circle = std::get_if<Circle>(&shape)) {
		bbox.p_min.x = circle->center.x - circle->radius;
		bbox.p_min.y = circle->center.y - circle->radius;
		bbox.p_max.x = circle->center.x + circle->radius;
		bbox.p_max.y = circle->center.y + circle->radius;
	} else if (auto *rectangle = std::get_if<Rectangle>(&shape)) {
		bbox.p_min = rectangle->p_min;
		bbox.p_max = rectangle->p_max;
	} else if (auto *triangle = std::get_if<Triangle>(&shape)) {
		bbox.p_min.x = min3(triangle->p0.x, triangle->p1.x, triangle->p2.x);
		bbox.p_min.y = min3(triangle->p0.y, triangle->p1.y, triangle->p2.y);
		bbox.p_max.x = max3(triangle->p0.x, triangle->p1.x, triangle->p2.x);
		bbox.p_max.y = max3(triangle->p0.y, triangle->p1.y, triangle->p2.y);
	}
	return bbox;
}

BBox findBBox(Circle circle) {
	BBox bbox;
	bbox.p_min.x = circle.center.x - circle.radius;
	bbox.p_min.y = circle.center.y - circle.radius;
	bbox.p_max.x = circle.center.x + circle.radius;
	bbox.p_max.y = circle.center.y + circle.radius;
	return bbox;
}

BBox findBBox(Triangle triangle) {
	BBox bbox;
	bbox.p_min.x = min3(triangle.p0.x, triangle.p1.x, triangle.p2.x);
	bbox.p_min.y = min3(triangle.p0.y, triangle.p1.y, triangle.p2.y);
	bbox.p_max.x = max3(triangle.p0.x, triangle.p1.x, triangle.p2.x);
	bbox.p_max.y = max3(triangle.p0.y, triangle.p1.y, triangle.p2.y);
	return bbox;
}

bool inCircle(Vector2 center, Real radius, Real x, Real y) {
	Real dist 	= (x - center.x)*(x - center.x)
				+ (y - center.y)*(y - center.y);
	dist = sqrt(dist);
	
	// if distance < radius => pixel inside circle
	if (dist < radius) {
		return true;
	}
	return false;
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

bool inRectangle (Vector2 p_min, Vector2 p_max, Real x, Real y) {
	if (x > p_min.x && x < p_max.x && y > p_min.y && y < p_max.y) return true;
	return false;
}

Image3 hw_1_3(const std::vector<std::string> &params) {
    // Homework 1.3: render multiple shapes
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = scene.background;
        }
    }
	
	auto shape = scene.shapes.begin();
	
	while (shape != scene.shapes.end()) {
		if (auto *circ = std::get_if<Circle>(&*shape)) {
			Circle circle = *circ;
			BBox bbox = findBBox(circle);
			int xmin = max2(bbox.p_min.x, 0);
			int ymin = max2(bbox.p_min.y, 0);
			int xmax = min2(bbox.p_max.x, img.width);
			int ymax = min2(bbox.p_max.y, img.height);
			for (int x = xmin; x < xmax; x++) {
				for (int y = ymin; y < ymax; y++) {
					if (inCircle(circle.center, circle.radius, (Real) x, (Real) y)) {
						img(x, y) = circle.color;
					}
				}
			}
		} else if (auto *rect = std::get_if<Rectangle>(&*shape)) {
			Rectangle rectangle = *rect;
			int xmin = max2(rectangle.p_min.x, 0);
			int ymin = max2(rectangle.p_min.y, 0);
			int xmax = min2(rectangle.p_max.x, img.width);
			int ymax = min2(rectangle.p_max.y, img.height);
			
			for (int x = xmin; x < xmax; x++) {
				for (int y = ymin; y < ymax; y++) {
					img(x, y) = rectangle.color;
				}
			}
		} else if (auto *tri = std::get_if<Triangle>(&*shape)) {
			Triangle triangle = *tri;
			BBox bbox = findBBox(triangle);
			int xmin = max2(bbox.p_min.x, 0);
			int ymin = max2(bbox.p_min.y, 0);
			int xmax = min2(bbox.p_max.x, img.width);
			int ymax = min2(bbox.p_max.y, img.height);
			
			for (int x = xmin; x < xmax; x++) {
				for (int y = ymin; y < ymax; y++) {
					if (inTriangle(triangle.p0, triangle.p1, triangle.p2, x, y)) {
						img(x, y) = triangle.color;
					}
				}
			}
		}
		shape++;
	}
	
    return img;
}

Real progress = 1.0;

void hw_1_4_animate(const std::vector<std::string> &parameters) {
	progress = 0.1;
	Image3 img = hw_1_4(parameters);
	imwrite("animate/hw_1_4(0).png", img);
	
	progress = 0.2;
	img = hw_1_4(parameters);
	imwrite("animate/hw_1_4(1).png", img);
	
	progress = 0.3;
	img = hw_1_4(parameters);
	imwrite("animate/hw_1_4(2).png", img);
	
	progress = 0.4;
	img = hw_1_4(parameters);
	imwrite("animate/hw_1_4(3).png", img);
	
	progress = 0.5;
	img = hw_1_4(parameters);
	imwrite("animate/hw_1_4(4).png", img);
	
	progress = 0.6;
	img = hw_1_4(parameters);
	imwrite("animate/hw_1_4(5).png", img);
	
	progress = 0.7;
	img = hw_1_4(parameters);
	imwrite("animate/hw_1_4(6).png", img);
	
	progress = 0.8;
	img = hw_1_4(parameters);
	imwrite("animate/hw_1_4(7).png", img);
	
	progress = 0.9;
	img = hw_1_4(parameters);
	imwrite("animate/hw_1_4(8).png", img);
	
	progress = 1.0;
	img = hw_1_4(parameters);
	imwrite("animate/hw_1_4(9).png", img);
}

Image3 hw_1_4(const std::vector<std::string> &params) {
    // Homework 1.4: render transformed shapes
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
			img(x, y) = scene.background;
        }
    }
	
	auto shape = scene.shapes.begin();
	
	while (shape != scene.shapes.end()) {
		if (auto *circ = std::get_if<Circle>(&*shape)) {
			Circle circle = *circ;
			
			// compute tranformation and inverse transform matrices
			Matrix3x3 F = circle.transform;
			Matrix3x3 Fp = inverse(F);
			
			for (int x = 0; x < img.width; x++) {
				for (int y = 0; y < img.height; y++) {
					
					// screen coordinates → object coordinates
					Vector3 scr = Vector3(Real(x), Real(y), Real(1));
					Vector3 obj = Fp * scr;
					
					// test if in shape
					if (inCircle(circle.center, circle.radius, obj.x, obj.y)) {
						img(x, y) = circle.color;
					}
				}
			}
		} else if (auto *rect = std::get_if<Rectangle>(&*shape)) {
			Rectangle rectangle = *rect;
			
			// compute tranformation and inverse transform matrices
			Matrix3x3 F = rectangle.transform;
			Matrix3x3 Fp = inverse(F);
			
			
			for (int x = 0; x < img.width; x++) {
				for (int y = 0; y < img.height; y++) {
					
					// screen coordinates → object coordinates
					Vector3 scr = Vector3(Real(x), Real(y), Real(1));
					Vector3 obj = Fp * scr;
					if (inRectangle(rectangle.p_min, rectangle.p_max, obj.x, obj.y)) {
						img(x, y) = rectangle.color;
					}
				}
			}
		} else if (auto *tri = std::get_if<Triangle>(&*shape)) {
			Triangle triangle = *tri;
			
			// compute tranformation and inverse transform matrices
			Matrix3x3 F = triangle.transform;
			Matrix3x3 Fp = inverse(F);
			
			for (int x = 0; x < img.width; x++) {
				for (int y = 0; y < img.height; y++) {
					
					// screen coordinates → object coordinates
					Vector3 scr = Vector3(Real(x), Real(y), Real(1));
					Vector3 obj = Fp * scr;
					
					// test if in shape
					if (inTriangle(triangle.p0, triangle.p1, triangle.p2, obj.x, obj.y)) {
						img(x, y) = triangle.color;
					}
				}
			}
		}
		shape++;
	}
	
    return img;
}

Image3 hw_1_5(const std::vector<std::string> &params) {
    // Homework 1.5: antialiasing
    if (params.size() == 0) {
        return Image3(0, 0);
    }
	
	int ss = 4; // define how much super-sampling to do

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x * ss, scene.resolution.y * ss);

	for (int y = 0; y < img.height; y++) {
		for (int x = 0; x < img.width; x++) {
			img(x, y) = scene.background;
		}
	}
	
	auto shape = scene.shapes.begin();
	
	while (shape != scene.shapes.end()) {
		if (auto *circ = std::get_if<Circle>(&*shape)) {
			Circle circle = *circ;
			
			// compute tranformation and inverse transform matrices
			Matrix3x3 F = circle.transform;
			Matrix3x3 Fp = inverse(F);
			
			for (int x = 0; x < img.width; x++) {
				for (int y = 0; y < img.height; y++) {
					
					// screen coordinates → object coordinates
					Vector3 scr = Vector3(Real(x)/ss, Real(y)/ss, Real(1));
					Vector3 obj = Fp * scr;
					
					// test if in shape
					if (inCircle(circle.center, circle.radius, obj.x, obj.y)) {
						img(x, y) = circle.color;
					}
				}
			}
		} else if (auto *rect = std::get_if<Rectangle>(&*shape)) {
			Rectangle rectangle = *rect;
			
			// compute tranformation and inverse transform matrices
			Matrix3x3 F = rectangle.transform;
			Matrix3x3 Fp = inverse(F);
			
			
			for (int x = 0; x < img.width; x++) {
				for (int y = 0; y < img.height; y++) {
					
					// screen coordinates → object coordinates
					Vector3 scr = Vector3(Real(x)/ss, Real(y)/ss, Real(1));
					Vector3 obj = Fp * scr;
					if (inRectangle(rectangle.p_min, rectangle.p_max, obj.x, obj.y)) {
						img(x, y) = rectangle.color;
					}
				}
			}
		} else if (auto *tri = std::get_if<Triangle>(&*shape)) {
			Triangle triangle = *tri;
			
			// compute tranformation and inverse transform matrices
			Matrix3x3 F = triangle.transform;
			Matrix3x3 Fp = inverse(F);
			
			for (int x = 0; x < img.width; x++) {
				for (int y = 0; y < img.height; y++) {
					
					// screen coordinates → object coordinates
					Vector3 scr = Vector3(Real(x)/ss, Real(y)/ss, Real(1));
					Vector3 obj = Fp * scr;
					
					// test if in shape
					if (inTriangle(triangle.p0, triangle.p1, triangle.p2, obj.x, obj.y)) {
						img(x, y) = triangle.color;
					}
				}
			}
		}
		shape++;
	}
	
	Image3 ssimg(scene.resolution.x, scene.resolution.y);
	
	for (int y = 0; y < ssimg.height; y++) {
		for (int x = 0; x < ssimg.width; x++) {
			
			Vector3 sum;
			
			sum.x = Real(0);
			sum.y = Real(0);
			sum.z = Real(0);
			
			for (int i = 0; i < ss; i++) {
				for (int j = 0; j < ss; j++) {
					sum = sum + img(ss*x + i, ss*y + j);
				}
			}
			//sum = img(ss*x, ss*y  ) + img(ss*x+1, ss*y)
			//+ img(ss*x, ss*y+1) + img(ss*x+1, ss*y+1);
			
			ssimg(x, y) = sum * (Real(1) / Real(ss * ss));
		}
	}
	
    return ssimg;
}

Image3 hw_1_6(const std::vector<std::string> &params) {
    // Homework 1.6: alpha blending
    if (params.size() == 0) {
        return Image3(0, 0);
    }

	int ss = 4; // define how much super-sampling to do

	Scene scene = parse_scene(params[0]);
	std::cout << scene << std::endl;

	Image3 img(scene.resolution.x * ss, scene.resolution.y * ss);

	for (int y = 0; y < img.height; y++) {
		for (int x = 0; x < img.width; x++) {
			img(x, y) = scene.background;
		}
	}
	
	auto shape = scene.shapes.begin();
	
	while (shape != scene.shapes.end()) {
		if (auto *circ = std::get_if<Circle>(&*shape)) {
			Circle circle = *circ;
			
			// compute tranformation and inverse transform matrices
			Matrix3x3 F = circle.transform;
			Matrix3x3 Fp = inverse(F);
			
			for (int x = 0; x < img.width; x++) {
				for (int y = 0; y < img.height; y++) {
					
					// screen coordinates → object coordinates
					Vector3 scr = Vector3(Real(x)/ss, Real(y)/ss, Real(1));
					Vector3 obj = Fp * scr;
					
					// test if in shape
					if (inCircle(circle.center, circle.radius, obj.x, obj.y)) {
						img(x, y) = circle.color * circle.alpha
								  + img(x, y) * (1.0 - circle.alpha);
					}
				}
			}
		} else if (auto *rect = std::get_if<Rectangle>(&*shape)) {
			Rectangle rectangle = *rect;
			
			// compute tranformation and inverse transform matrices
			Matrix3x3 F = rectangle.transform;
			Matrix3x3 Fp = inverse(F);
			
			
			for (int x = 0; x < img.width; x++) {
				for (int y = 0; y < img.height; y++) {
					
					// screen coordinates → object coordinates
					Vector3 scr = Vector3(Real(x)/ss, Real(y)/ss, Real(1));
					Vector3 obj = Fp * scr;
					if (inRectangle(rectangle.p_min, rectangle.p_max, obj.x, obj.y)) {
						img(x, y) = rectangle.color * rectangle.alpha
								  + img(x,y) * (1 - rectangle.alpha);
					}
				}
			}
		} else if (auto *tri = std::get_if<Triangle>(&*shape)) {
			Triangle triangle = *tri;
			
			// compute tranformation and inverse transform matrices
			Matrix3x3 F = triangle.transform;
			Matrix3x3 Fp = inverse(F);
			
			for (int x = 0; x < img.width; x++) {
				for (int y = 0; y < img.height; y++) {
					
					// screen coordinates → object coordinates
					Vector3 scr = Vector3(Real(x)/ss, Real(y)/ss, Real(1));
					Vector3 obj = Fp * scr;
					
					// test if in shape
					if (inTriangle(triangle.p0, triangle.p1, triangle.p2, obj.x, obj.y)) {
						img(x, y) = triangle.color * triangle.alpha
								  + img(x, y) * (1 - triangle.alpha);
					}
				}
			}
		}
		shape++;
	}
	
	Image3 ssimg(scene.resolution.x, scene.resolution.y);
	
	for (int y = 0; y < ssimg.height; y++) {
		for (int x = 0; x < ssimg.width; x++) {
			
			Vector3 sum;
			
			sum.x = Real(0);
			sum.y = Real(0);
			sum.z = Real(0);
			
			for (int i = 0; i < ss; i++) {
				for (int j = 0; j < ss; j++) {
					sum = sum + img(ss*x + i, ss*y + j);
				}
			}
			//sum = img(ss*x, ss*y  ) + img(ss*x+1, ss*y)
			//+ img(ss*x, ss*y+1) + img(ss*x+1, ss*y+1);
			
			ssimg(x, y) = sum * (Real(1) / Real(ss * ss));
		}
	}
	
	return ssimg;
}
