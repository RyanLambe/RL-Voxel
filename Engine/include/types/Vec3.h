#pragma once

#include <math.h>

namespace Engine {
	class Vec3 {
	public:

		//conponents
		float x = 0;
		float y = 0;
		float z = 0;

		//constructors
		Vec3();
		Vec3(float value);
		Vec3(float x, float y, float z);
		Vec3(Vec3* vec);

		//functions
		static float distance(Vec3 a, Vec3 b);

		//operators
		Vec3 operator+(Vec3 other);
		Vec3 operator+(float other);
		void operator+=(Vec3 other);
		void operator+=(float other);

		Vec3 operator-(Vec3 other);
		Vec3 operator-(float other);
		void operator-=(Vec3 other);
		void operator-=(float other);

		Vec3 operator*(Vec3 other);
		Vec3 operator*(float other);
		void operator*=(Vec3 other);
		void operator*=(float other);

		Vec3 operator/(Vec3 other);
		Vec3 operator/(float other);
		void operator/=(Vec3 other);
		void operator/=(float other);

	};
}