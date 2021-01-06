#include "minCircle.h"

float dist2Points(const Point &p1,const Point &p2) {
    return std::sqrt((p1.x - p2.x) * (p1.x - p2.x) +
                     (p1.y - p2.y) * (p1.y - p2.y));
}
int isPointInCircle(Point &point1, Circle &c) {
    if (dist2Points(point1, c.center) <= c.radius) {
        return 1;
    } else
        return 0;
}

Circle createFrom3Points(Point &p1, Point &p2, Point &p3) {
    float slope1 = (p1.y - p2.y) / (p1.x - p2.x);
    slope1 = -1.0 / slope1;
    float cx = (p1.x + p2.x) / 2.0;
    float cy = (p1.y + p2.y) / 2.0;
    float b1 = cy - slope1 * cx;

    cx = (p2.x + p3.x) / 2.0;
    cy = (p2.y + p3.y) / 2.0;
    float slope2 = (p2.y - p3.y) / (p2.x - p3.x);
    slope2 = -1.0 / slope2;
    float b2 = cy - slope2 * cx;
    float x_circle = (b2 - b1) / (slope1 - slope2);
    float y_circle = slope2 * x_circle + b2;
    float radius = dist2Points(p1, Point(x_circle, y_circle));
    return Circle(Point(x_circle, y_circle), radius);
}

Circle createFrom2Points(Point &p1, Point &p2) {
    float xc = (p1.x + p2.x) / 2;
    float yc = (p1.y + p2.y) / 2;
    float dist = dist2Points(p1, p2) / 2;
    return Circle(Point(xc, yc), dist);
}

Circle build(vector<Point> &points) {
    int len = points.size();
    if(len == 0) {
        return Circle(Point(0,0),0);
    }
    if (len == 1) {
        Point p = points.at(0);
        return Circle(p, 0);
    }
    else if (len == 2) {
        return createFrom2Points(points[0], points[1]);
    }
    Circle minCircle = Circle(Point(0,0),99999);
    float min_radius;
    int isCircleFrom2PPoint = 0;
    for(int i = 0; i < 3; i++) {
        Circle c = createFrom2Points(points[i % 3], points[(1+i) % 3]);
        if (isPointInCircle(points[(2+i) % 3],c)) {
            if(i == 0) {
                isCircleFrom2PPoint = 1;
                minCircle = c;
            }
            else if(minCircle.radius > c.radius ) {
                minCircle = c;
            }
        }
    }
    if (isCircleFrom2PPoint) {
        return minCircle;
    }
    return createFrom3Points(points[0], points[1], points[2]);
}

Circle bestCircle(Point** &points, vector<Point> R,size_t size) {
    if (size == 0 || R.size() == 3) {
        return build(R);
    }
    Point p = *points[size -1];
    size--;
    Circle d = bestCircle(points, R,size);
    if (isPointInCircle(p, d)) {
        return d;
    }
    R.push_back(p);
    return bestCircle(points, R,size);
}
void swap(Point &p1, Point &p2){
    Point temp = p1;
    p1 = p2;
    p2 = temp;
}

void shuffle(Point** &points, size_t size) {
    for(int i = 0; i < size; i++) {
        int randindx = rand() % size;
        swap(points[i], points[randindx]);
    }
}
Circle findMinCircle(Point** &points, size_t size) {
    vector<Point> R;
    shuffle(points,size);
    return bestCircle(points, R,size);
}
