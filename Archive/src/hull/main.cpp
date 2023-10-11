#include <algorithm>
#include <complex>
#include <fstream>
#include <iostream>
#include <numeric>
#include <vector>

typedef std::complex<double> Point;
typedef std::vector<Point> Polygon;

// Compute the determinant of two points
double inline det(const Point &u, const Point &v) {
    return u.real() * v.imag() - u.imag() * v.real();
}

struct Compare {
    Point p0; // Leftmost point of the poly
    bool operator ()(const Point &p1, const Point &p2) {
        return atan2(p1.imag() - p0.imag(), p1.real() - p0.real()) <
               atan2(p2.imag() - p0.imag(), p2.real() - p0.real());
    }
};

bool inline salientAngle(const Point &a, const Point &b, const Point &c) {
    return det(b - a, c - b) <= 0;  // Check for right turn or collinear
}

Polygon convex_hull(std::vector<Point> &points) {
    Compare order;
    order.p0 = *std::min_element(points.begin(), points.end(), [](const Point &a, const Point &b) {
        return a.imag() < b.imag() || (a.imag() == b.imag() && a.real() < b.real());
    });
    std::sort(points.begin(), points.end(), order);
    Polygon hull;
    for (const auto &point : points) {
        while (hull.size() > 1 && salientAngle(hull[hull.size() - 2], hull.back(), point)) {
            hull.pop_back();
        }
        if (hull.empty() || hull.back() != point) {
            hull.push_back(point);
        }
    }
    if (hull.size() > 1 && hull.front() == hull.back()) {
        hull.pop_back();
    }
    return hull;
}

std::vector<Point> load_xyz(const std::string &filename) {
    std::ifstream in(filename);
    if (!in.is_open()) {
        std::cerr << "Please open file: " << filename << std::endl;
        exit(1);
    }

    int number;
    in >> number;
    std::vector<Point> points;
    for (int i = 0; i < number; ++i) {
        double x, y, z;
        in >> x >> y >> z;
        points.push_back(Point(x, y));
    }
    return points;
}

void save_obj(const std::string &filename, Polygon &poly) {
    std::ofstream out(filename);
    if (!out.is_open()) {
        throw std::runtime_error("failed to open file " + filename);
    }
    out << std::fixed;
    for (const auto &v : poly) {
        out << "v " << v.real() << ' ' << v.imag() << " 0\n";
    }
    for (size_t i = 0; i < poly.size(); ++i) {
        out << "l " << i+1 << ' ' << 1+(i+1)%poly.size() << "\n";
    }
    out << std::endl;
}

int main(int argc, char * argv[]) {
    if (argc <= 2) {
        std::cerr << "Usage: " << argv[0] << " points.xyz output.obj" << std::endl;
        return 1;
    }
    std::vector<Point> points = load_xyz(argv[1]);
    Polygon hull = convex_hull(points);
    save_obj(argv[2], hull);
    return 0;
}
