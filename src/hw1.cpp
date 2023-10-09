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
					if (abs(x - object->center.x) < object->radius &&
						abs(y - object->center.y) < object->radius) {
						
						Circle circle = *object;
						
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

bool inCircle(Vector2 center, Real radius, int x, int y) {
	Real dist 	= (x - center.x)*(x - center.x)
				+ (y - center.y)*(y - center.y);
	dist = sqrt(dist);
	
	// if distance < radius => pixel inside circle
	if (dist < radius) {
		return true;
	}
	return false;
}

bool inTriangle(Vector2 p0, Vector2 p1, Vector2 p2, int x, int y) {
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
					if (inCircle(circle.center, circle.radius, x, y)) {
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
            img(x, y) = Vector3{1, 1, 1};
        }
    }
    return img;
}

Image3 hw_1_5(const std::vector<std::string> &params) {
    // Homework 1.5: antialiasing
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{1, 1, 1};
        }
    }
    return img;
}

Image3 hw_1_6(const std::vector<std::string> &params) {
    // Homework 1.6: alpha blending
    if (params.size() == 0) {
        return Image3(0, 0);
    }

    Scene scene = parse_scene(params[0]);
    std::cout << scene << std::endl;

    Image3 img(scene.resolution.x, scene.resolution.y);

    for (int y = 0; y < img.height; y++) {
        for (int x = 0; x < img.width; x++) {
            img(x, y) = Vector3{1, 1, 1};
        }
    }
    return img;
}
