#pragma once


template<typename T>
class vec2
{
public:

    T x;
    T y;

    vec2() : x(0), y(0) {}
    vec2(const T& vX, const T& vY) : x(vX), y(vY) {}
    vec2(const vec2<T>& v) : vec2(v.x, v.y) {}

    ~vec2() {}

    vec2<T> operator+ (const vec2<T>& v) const { return vec2(x + v.x, y + v.y); }
    vec2<T> operator- (const vec2<T>& v) const { return vec2(x - v.x, y - v.y); }
    vec2<T> operator* (const vec2<T>& v) const { return vec2(x * v.x, y * v.y); }
    vec2<T> operator/ (const vec2<T>& v) const { return vec2(x / v.x, y / v.y); }

    vec2<T> operator* (const T& val    ) const { return vec2(x * val, y * val); }
    vec2<T> operator/ (const T& val    ) const { return vec2(x / val, y / val); }
    
    vec2<T>&& operator+= (const vec2<T>& v) { return *this + v; }
    vec2<T>&& operator-= (const vec2<T>& v) { return *this - v; }
    vec2<T>&& operator*= (const vec2<T>& v) { return *this * v; }
    vec2<T>&& operator/= (const vec2<T>& v) { return *this / v; }

    vec2<T>&& operator*= (const T& val    ) { return *this * val; }
    vec2<T>&& operator/= (const T& val    ) { return *this / val; }

    T&       operator[] (const size_t i)       { return *((T*)this + i);  }
    const T& operator[] (const size_t i) const { return *((T*)this + i);  }

    inline void mPrint() const { std::cout << x << " " << y << "\n"; }

    vec2<T> mGetSwapped() const { return vec2<T>(y, x); }

};
