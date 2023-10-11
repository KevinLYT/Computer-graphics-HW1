////////////////////////////////////////////////////////////////////////////////
#include <algorithm>
#include <complex>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>
////////////////////////////////////////////////////////////////////////////////

typedef std::complex<double> Point;
typedef std::vector<Point> Polygon;

double inline det(const Point &u, const Point &v) {
	// TODO
    return u.real() * v.imag() - v.real() * u.imag();
}

// Return true iff [a,b] intersects [c,d], and store the intersection in ans
bool intersect_segment(const Point &a, const Point &b, const Point &c, const Point &d, Point &ans) {
	// TODO
    Point ab = b - a;
    Point ac = c - a;
    Point ad = d - a;
    Point ca = a - c;
    Point cb = b - c;
    Point cd = d - c;
    
    double s, t;
    s = det(ac, ad)/det(ab, ad);
    t = det(ca, cb)/det(cd, cb);
    
    if(s >= 0 && s <= 1 && t >= 0 && t <= 1){
        ans = a + s * ab;
        return true;
    }
    return false;
	
}

////////////////////////////////////////////////////////////////////////////////

bool is_inside(const Polygon &poly, const Point &query) {
	// 1. Compute bounding box and set coordinate of a point outside the polygon
	// TODO
	Point outside(1e9, 1e9);
	// 2. Cast a ray from the query point to the 'outside' point, count number of intersections
	// TODO
    int count = 0;
    Point intersection;
    for (size_t i = 0; i < poly.size(); ++i) {
        if (intersect_segment(query, outside, poly[i], poly[(i + 1) % poly.size()], intersection)) {
            count++;
        }
    }
    return count % 2 == 1;
}

////////////////////////////////////////////////////////////////////////////////

std::vector<Point> load_xyz(const std::string &filename) {
	std::vector<Point> points;
	std::ifstream in(filename);
	// TODO
    double x, y;
    while (in >> x>> y){
        points.push_back(Point(x,y));
    }
	return points;
}

Polygon load_obj(const std::string &filename) {
	std::ifstream in(filename);
	// TODO
    Polygon poly;
    std::string line;
    
    while (getline(in,line)){
        if(line[0] == 'v'){
            std::istringstream iss(line.substr(2));
            double x,y;
            iss >>x >>y;
            poly.push_back(Point(x,y));
        }
    }
	return poly;
}

void save_xyz(const std::string &filename, const std::vector<Point> &points) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        std::cerr << "Cannot open file for writing: " << filename << std::endl;
        return;
    }

    for (const Point &p : points) {
        out << p.real() << " " << p.imag() << " 0" << std::endl; // 设置第三维度为0
    }
}


////////////////////////////////////////////////////////////////////////////////

int main(int argc, char * argv[]) {

	if (argc <= 3) {
		std::cerr << "Usage: " << argv[0] << " points.xyz poly.obj result.xyz" << std::endl;
	}
	std::vector<Point> points = load_xyz(argv[1]);
	Polygon poly = load_obj(argv[2]);
	std::vector<Point> result;
	for (size_t i = 0; i < points.size(); ++i) {
		if (is_inside(poly, points[i])) {
			result.push_back(points[i]);
		}
	}
	save_xyz(argv[3], result);
	return 0;
    
    std::cout << "Detected " << result.size() << " points inside the polygon." << std::endl;

    
}
