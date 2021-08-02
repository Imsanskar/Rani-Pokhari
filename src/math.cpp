#include "Math/matrix.h"
#include "Math/vec.h"

namespace MathLib {
	//mat3 constructor
	Mat3::Mat3(Mat4 mat4){
		for (int i = 0; i < 3; i++){
			for(int j = 0; j < 3; j++){
				mat[i][j] = mat4.mat[i][j];
			}
		}
		
	}



	//matrix multiplication
	Mat4 operator *(Mat4 a, Mat4 b) {
		Mat4 result;
		for (int i = 0; i < 4; i++) {
			for (int j = 0; j < 4; j++) {
				for (int k = 0; k < 4; k++) {
					result.mat[i][j] += a.mat[i][k] * b.mat[k][j];
				}
			}
		}
		return result;
	}


	Mat4 translate(const Mat4& mat, float tx, float ty, float tz) {
		Mat4 result = mat;
		result.mat[0][3] = tx;
		result.mat[1][3] = ty;
		result.mat[1][3] = tz;
		return result;
	}



	Mat4 translate(const Mat4& mat, Vec3 vec) {
		Mat4 result = mat;
		result.mat[0][3] = vec.x;
		result.mat[1][3] = vec.y;
		result.mat[2][3] = vec.z;

		return result;
	}

	Mat4 rotateX(const Mat4& matrix, float angle) {
		Mat4 result(1.0f);

		result.mat[1][1] = cos(angle);
		result.mat[1][2] = -sin(angle);
		result.mat[2][1] = sin(angle);
		result.mat[2][2] = cos(angle);

		return result * matrix;
	}


	Mat4 rotateY(const Mat4& matrix, float angle) {
		Mat4 result(1.0f);

		result.mat[0][0] = cos(angle);
		result.mat[0][2] = sin(angle);
		result.mat[2][0] = -sin(angle);
		result.mat[2][2] = result.mat[0][0];


		return result * matrix;
	}


	Mat4 rotateZ(const Mat4& matrix, float angle) {
		Mat4 result(1.0f);

		result.mat[0][0] = cos(angle);
		result.mat[0][1] = -sin(angle);
		result.mat[1][0] = sin(angle);
		result.mat[1][1] = cos(angle);

		return result * matrix;
	}


	// Mat4 rotate(const Mat4& matrix, const Vec3& vec, const Vec3& point, const float angle) {
	// 	Mat4 result(1.0f);
	// 	Vec3 axis = vec.unitVector();

	// 	result = translate(result, -point);
	// 	result = rotate(matrix, vec, angle);
	// 	result = translate(result, point);
	// 	return result * matrix;
	// }

	Mat4 rotate(const Mat4& matrix, const Vec3& vec, const float angle) {
		Mat4 result(1.0f);
		Vec3 axis = vec.unitVector();

		const float sinVal = sin(angle);
		const float cosVal = cos(angle);

		const float x = axis.x;
		const float y = axis.y;
		const float z = axis.z;

		result.mat[0][0] = cosVal + x * x * (1 - cosVal);
		result.mat[0][1] = x * y * (1 - cosVal) - z * sinVal;
		result.mat[0][2] = x * z * (1 - cosVal) + y * sinVal;

		result.mat[1][0] = x * y * (1 - cosVal) + z * sinVal;
		result.mat[1][1] = cosVal + y * y * (1 - cosVal);
		result.mat[1][2] = z * y * (1 - cosVal) - x * sinVal;

		result.mat[2][0] = x * z * (1 - cosVal) - y * sinVal;
		result.mat[2][1] = z * y * (1 - cosVal) + x * sinVal;
		result.mat[2][2] = cosVal + z * z * (1 - cosVal);

		return result * matrix;
	}

	Mat4 lookAt(const Vec3& eye, const Vec3& center, const Vec3& up) {
		Mat4 result(1.0f);

		auto f = (center - eye).unitVector();
		auto s = cross(f, up).unitVector();
		auto u = cross(s, f).unitVector();

		result.mat[0][0] = s.x;
		result.mat[1][0] = s.y;
		result.mat[2][0] = s.z;


		result.mat[0][1] = u.x;
		result.mat[1][1] = u.y;
		result.mat[2][1] = u.z;


		result.mat[0][2] = f.x;
		result.mat[1][2] = f.y;
		result.mat[2][2] = f.z;


		result.mat[3][0] = -s.dot(eye);
		result.mat[3][1] = -u.dot(eye);
		result.mat[3][2] = -f.dot(eye);

		return result;
	}
}