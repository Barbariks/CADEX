//
//  main.cpp
//  Test
//
//  Created by Andrew on 23.11.2023.
//

#include <iostream>
#include <random>
#include <cmath>
#include <memory>
#include <typeinfo>
#include <algorithm>

using namespace std;

constexpr double eps = 1e-6;

struct Point3D {double x{}, y{}, z{};};
ostream& operator<<(ostream& os, const Point3D& p){return os << "\nDot: " << p.x << ", " << p.y << ", " << p.z << endl;};
struct Vector3D { double x{}, y{}, z{}; };
ostream& operator<<(ostream& os, const Vector3D& v){return os << "Vector: " << v.x << ", " << v.y << ", " << v.z << endl;};

class Geometry {
protected:
    Point3D Dot{};
    Geometry() = default;
    Geometry(const Point3D& center) : Dot{ center } {};
public:
    virtual ~Geometry(){};
    virtual Point3D curve_point(double t) const = 0;
    virtual Vector3D curve_vector(double t) const = 0;
};


class Circle : public Geometry{
    double rad{};
public:
    Circle() = default;
    Circle(const Point3D& center, double radius):Geometry(center), rad{ radius } {};
    virtual ~Circle(){};
    double radius() const { return rad; }
    virtual Point3D curve_point(double t) const override
    {
        return Point3D
        {
            rad * cos(t) + Dot.x,
            rad * sin(t) + Dot.y,
            Dot.z
        };
    }
    virtual Vector3D curve_vector(double t) const override
    {
        return Vector3D
        {
            -1 * rad * sin(t),
            rad * cos(t)
        };
    }
};

class Ellipse : public Geometry{
    double rad_x{};
    double rad_y{};
public:
    Ellipse() = default;
    Ellipse(const Point3D& center, double radius_x, double radius_y):Geometry(center), rad_x{ radius_x }, rad_y{ radius_y } {};
    virtual ~Ellipse(){};
    virtual Point3D curve_point(double t) const override
    {
        return Point3D
        {
            rad_x * cos(t) + Dot.x,
            rad_y * sin(t) + Dot.y,
            Dot.z
        };
    }
    virtual Vector3D curve_vector(double t) const override
    {
        return Vector3D
        {
            -1 * rad_x * sin(t),
            rad_y * cos(t)
        };
    }
};

class Helix : public Geometry{
    double rad{};
    double step{};
public:
    Helix() = default;
    Helix(const Point3D& center, double radius, double step_z):Geometry(center), rad{ radius }, step{ step_z } {};
    virtual ~Helix(){};
    virtual Point3D curve_point(double t) const override
    {
        return Point3D
        {
            rad * cos(t) + Dot.x,
            rad * sin(t) + Dot.y,
            step * t + Dot.z
        };
    }
    virtual Vector3D curve_vector(double t) const override
    {
        return Vector3D
        {
            -1 * rad * sin(t),
            rad * cos(t),
            step
        };
    }
};

using PShape = shared_ptr<Geometry>;

class Generator {
    enum types{circle_t, ellipse_t, helix_t};
    static random_device rd;
    uniform_real_distribution<double> udd{-1, 1};
    uniform_int_distribution<> usd{ circle_t,  helix_t };
public:
    void set_New_Range(double min, double max){
        if((abs(min)+abs(max))< 2 * eps){return;}
        udd = decltype(udd){ min, max };
    };
    PShape circle(){   // метод генерирует и размещает в динамической памяти объект класса Circle3D
        // и возвращает умный указатель на базовый класс
        double radius{};
        while (radius < eps)    // проверяем, что бы радиус был не слишком маленьким
            radius = abs(udd(rd));    // и > 0

        return make_shared<Circle>(Point3D{ udd(rd), udd(rd) }, radius);
    };
    PShape ellips(){   // метод генерирует и размещает в динамической памяти объект класса Circle3D
        // и возвращает умный указатель на базовый класс
        double radius_x{}, radius_y{};
        while (radius_x < eps) radius_x = abs(udd(rd));
        while (radius_y < eps) radius_y = abs(udd(rd));
        return make_shared<Ellipse>(Point3D{ udd(rd), udd(rd) }, radius_x, radius_y);
    };
    PShape helix(){
        double radius{}, step{};
        while (radius < eps) radius = abs(udd(rd));
        while (step < eps) step = abs(udd(rd));
        return make_shared<Helix>(Point3D{ udd(rd), udd(rd) }, radius, step);
    };
    PShape random_shape(){
        switch (usd(rd)) {
            case circle_t:return circle();
            case helix_t:return helix();
            case ellipse_t:return ellips();
            default:return circle();
        }
    }
};

random_device Generator::rd;

int main()
{
    constexpr size_t sz = 10;
    const double pi = acos(-1);
    
    vector<PShape> shape_vec;

    {
        Generator s_gen;
        s_gen.set_New_Range(-10, 10);
        for (size_t i{}; i < sz; ++i)
            shape_vec.emplace_back(s_gen.random_shape());
    }
    
    using PCircle = shared_ptr<Circle>;

    vector<PCircle> circle_vec;

    double radius_sum{};

    for (auto& ps : shape_vec)
    {
        const type_info& ti{ typeid(*ps) };

        cout << ti.name() << ' '
                  << ps->curve_point(pi/4) << ' '
                  << ps->curve_vector(pi/4) << endl;

        if (ti.hash_code() == typeid(Circle).hash_code())
        {
            circle_vec.emplace_back(dynamic_pointer_cast<Circle>(ps));
            radius_sum += circle_vec.back()->radius();
            cout << "to another vector -->\n";
        }
        cout << endl;
    }
    sort(circle_vec.begin(), circle_vec.end(), [](const auto& lh, const auto& rh)
    {return lh->radius() < rh->radius();});

    for (auto& pc : circle_vec)
        cout << pc->radius() << endl;
    cout << "summ = " << radius_sum << endl;
}
