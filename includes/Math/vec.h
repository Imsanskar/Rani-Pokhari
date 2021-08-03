#pragma once
#include <math.h>

#define PI 3.141519

#define to_radians(deg) ((deg) * (PI / 180.0f))
#define to_degrees(rad) ((rad) * (180.0f / PI))

namespace MathLib {
	union Vec2 {
		float m[2];

		struct {
			float x, y;
		};

		inline Vec2() {
			x = 0;
			y = 0;
		}
		inline Vec2(float a) { x = y = a; }
		inline Vec2(float a, float b) { x = a; y = b; }
	};


	struct Vec3 {
		struct{
			float x, y, z;
		};
		Vec3(float val = 0) {
			x = y = z = val;
		}

		Vec3(float _x, float _y, float _z) {
			x = _x;
			y = _y;
			z = _z;
		}

		inline float* value_ptr() {
			return &(this->x);
		}

		inline Vec3 operator +(const Vec3& v) const{
			return Vec3(x + v.x, y + v.y, z + v.z);
		}

		inline Vec3 operator +(float v) const{
			return Vec3(x + v, y + v, z + v);
		}

		inline Vec3 operator -(const Vec3& v) const{
			return Vec3(x - v.x, y - v.y, z - v.z);
		}

		inline Vec3 operator -(float v) const{
			return Vec3(x - v, y - v, z - v);
		}


		inline Vec3 operator *(const Vec3& v) const{
			return Vec3(x * v.x, y * v.y, z * v.z);
		}

		inline Vec3 operator *(float v) const{
			return Vec3(x * v, y * v, z * v);
		}

		inline Vec3 operator /(float v) const{
			return Vec3(x / v, y / v, z / v);
		}

		inline float norm() const{
			return sqrt(x * x + y * y + z * z);
		}

		inline Vec3 unitVector() const{
			float magnitude = norm();

			if(magnitude > 0)
				return Vec3(x / magnitude, y / magnitude, z / magnitude);
			return Vec3(0.0f);
		}

		inline Vec3 operator +=(Vec3 vec){
			x += vec.x;
			y += vec.y;
			z += vec.z;

			return *this; 
		}

		inline Vec3 operator -=(Vec3 vec){
			x -= vec.x;
			y -= vec.y;
			z -= vec.z;

			return *this; 
		}

		inline Vec3 cross(Vec3 v) {
			Vec3 result;

			result.x = y * v.z - v.y * z;
			result.y = - (x * v.z - v.x * z);
			result.z = x * v.z - z * v.x;

			return result;
		}


		inline float dot(Vec3 vec) const{
			return x * vec.x + y * vec.y + z * vec.z;
		}
	};
	
	inline Vec3 cross(Vec3 vec1, Vec3 v){
		Vec3 result;

		float x = vec1.x;
		float y = vec1.y;
		float z = vec1.z;

		result.x = y * v.z - v.y * z;
		result.y = - (x * v.z - v.x * z);
		result.z = x * v.y - y * v.x;

		return result;
	}

	inline Vec3 normalize(Vec3 vec){
		float magnitude = sqrt(vec.x * vec.x + vec.y * vec.y + vec.z * vec.z);

		if(magnitude > 0)
			return Vec3(vec.x / magnitude, vec.y / magnitude, vec.z / magnitude);
		return Vec3(0.0f);
	}


	inline Vec3 operator -(const Vec3& vec) {
		return Vec3(-vec.x, -vec.y, -vec.z);
	}


	inline Vec3 operator * (float val, const Vec3& vec){
		return Vec3(vec.x * val, vec.y * val, vec.z * val);
	}
}