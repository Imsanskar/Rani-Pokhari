#pragma once
#include <vector>
#include "vec.h"
#include <iostream>


namespace MathLib {

	struct Mat4;

	struct Mat3{
		float mat[3][3];

		Mat3() {
			
		}

		Mat3(float x){
			for (int i = 0; i < 3; i++) {
				mat[i][i] = x;
			}
		}

		Mat3(Mat4 mat4);

		float& operator [](const std::pair<int, int>& index) {
			return mat[index.first][index.second];
		}
 
		Mat3 operator +(Mat3 m) {
			Mat3 result;

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					result.mat[i][j] = mat[i][j] + m.mat[i][j];
				}
			}
			return result.transpose();
		}



		Mat3 operator -(Mat3 m) {
			Mat3 result;

			for (int i = 0; i < 3; i++) {
				for (int j = 0; j < 3; j++) {
					result.mat[i][j] = mat[i][j] - m.mat[i][j];
				}
			}
			return result.transpose();
		}

		bool operator ==(Mat3 m) {
			// bool flag = true;
			// for (int i = 0; i < 3; i++) {
			// 	flag = this->mat[i] == m[i];
			// 	if (!flag)
			// 		return flag;
			// }
			// return flag;
			return false;
		}

		float* value_ptr() {
			return &mat[0][0];
		}


		Mat3 transpose(){
			Mat3 result;

			for(int i = 0; i < 3; i++){
				for(int j = 0; j < 3; j++){
					result.mat[i][j] = mat[j][i];
				}
			}
			return result.transpose();
		}
	};

	 inline std::ostream& operator<<(std::ostream& os, Mat3 matrix){
    	std::cout << "\nOutput in Row major order -> \n";
    	for (int i = 0; i < 3; ++i)
    	{
    	  for (int j = 0; j < 3; ++j)
    	  {
	os 	<< '[' << i << ']' << '[' << j << ']' <<" -> " << matrix.mat[j][i] << "   "; 
    	  }
    	  os << '\n';
    	}
    	return os << std::endl;
	};


	struct Mat4 {
		float mat[4][4] =  {0.0f};

		Mat4() {
			
		}
		

		Mat4(Mat3 mat3){
			for (int i = 0; i < 3; i++){
				for(int j = 0; j < 3; j++){
					mat[i][j] = mat3.mat[i][j];
				}
			}
			mat[3][3] = 1.0f;
		}

		Mat4(float x){
			for (int i = 0; i < 4; i++) {
				mat[i][i] = x;
			}
		}

		float* operator [](const int index){
			return mat[index];
		}

		float& operator [](const std::pair<int, int>& index) {
			return mat[index.first][index.second];
		}
 
		Mat4 operator +(Mat4 m) {
			Mat4 result;

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					result.mat[i][j] = mat[i][j] + m.mat[i][j];
				}
			}
			return result.transpose();
		}



		Mat4 operator -(Mat4 m) {
			Mat4 result;

			for (int i = 0; i < 4; i++) {
				for (int j = 0; j < 4; j++) {
					result.mat[i][j] = mat[i][j] - m.mat[i][j];
				}
			}
			return result.transpose();
		}

		bool operator ==(Mat4 m) {
			// bool flag = true;
			// for (int i = 0; i < 4; i++) {
			// 	flag = this->mat[i] == m[i];
			// 	if (!flag)
			// 		return flag;
			// }
			// return flag;
			return false;
		}

		float* value_ptr() {
			return &mat[0][0];
		}


		Mat4 transpose(){
			Mat4 result;

			for(int i = 0; i < 4; i++){
				for(int j = 0; j < 4; j++){
					result.mat[i][j] = mat[j][i];
				}
			}
			return result.transpose();
		}
	};

	 inline std::ostream& operator<<(std::ostream& os, Mat4 matrix){
    	std::cout << "\nOutput in Row major order -> \n";
    	for (int i = 0; i < 4; ++i)
    	{
    	  for (int j = 0; j < 4; ++j)
    	  {
	os 	<< '[' << i << ']' << '[' << j << ']' <<" -> " << matrix.mat[j][i] << "   "; 
    	  }
    	  os << '\n';
    	}
    	return os << std::endl;
  	}

	Mat4 operator *(Mat4 a, Mat4 b);

	Mat4 translate(const Mat4& mat, float tx, float ty, float tz);

	Mat4 translate(const Mat4& mat, Vec3 vec);

	Mat4 rotateX(const Mat4& matrix, float angle);

	Mat4 rotateY(const Mat4& matrix, float angle);

	Mat4 rotateZ(const Mat4& matrix, float angle);

	Mat4 rotate(const Mat4& matrix, const Vec3& vec, const float angle);

	Mat4 lookAt(const Vec3& cameraPos, const Vec3& target, const Vec3& up);
}