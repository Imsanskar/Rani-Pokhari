#include <math.h>
#include <vector>
#include "vec.h"

namespace MathLib{
	struct Mat4{
		float mat[4][4] = {0};

		Mat4(){
		
		}

		Mat4(float x){
			for(int i = 0; i < 4; i++){
				mat[i][i] = x;
			}
		}

		Mat4 operator +(Mat4 m){
			Mat4 result;

			for(int i = 0; i < 4; i++){
				for(int j = 0; j < 4; j++){
					result.mat[i][j] = mat[i][j] + m.mat[i][j];
				}
			}
			return result;
		}



		Mat4 operator -(Mat4 m){
			Mat4 result;

			for(int i = 0; i < 4; i++){
				for(int j = 0; j < 4; j++){
					result.mat[i][j] = mat[i][j] - m.mat[i][j];
				}
			}
			return result;
		}

		float *value_ptr(){
			return &mat[0][0];
		}
	};



	//matrix multiplication
	Mat4 operator *(Mat4 a, Mat4 b){
		Mat4 result;
		for(int i = 0; i < 4; i++){
			for(int j = 0; j < 4; j++){
				for(int k = 0; k < 4; k++){
					result.mat[i][j] += a.mat[i][k] * b.mat[k][j];
				}
			}
		}
		return result;
	}


	Mat4 tranlationMatrix(const Mat4& mat, float tx, float ty, float tz){
		Mat4 result = mat;
		result.mat[0][3] = tx;
		result.mat[1][3] = ty;
		result.mat[1][3] = tz;

		return result;
	}



	Mat4 tranlationMatrix(const Mat4& mat, Vec3 vec){
		Mat4 result = mat;
		result.mat[0][3] = vec.x;
		result.mat[1][3] = vec.y;
		result.mat[1][3] = vec.z;
	}

	Mat4 rotateX(Mat4 mat, float angle){
		Mat4 result(1.0f);

		result.mat[0][0] = mat.mat[0][0] * cos(angle);
		result.mat[0][2] = mat.mat[0][2] * sin(angle);
		result.mat[2][0] = - mat.mat[2][0] * sin(angle);
		result.mat[2][2] = mat.mat[2][2] * cos(angle);

		return result;
	}


	Mat4 rotateY(Mat4 mat, float angle){
		Mat4 result(1.0f);

		result.mat[1][1] = mat.mat[1][1] * cos(angle);
		result.mat[1][2] = - mat.mat[1][2] * sin(angle);
		result.mat[2][1] = mat.mat[2][1] * sin(angle);
		result.mat[2][2] = mat.mat[2][2] * cos(angle);

		return result;
	}

	
	Mat4 rotateZ(Mat4 mat, float angle){
		Mat4 result(1.0f);

		result.mat[0][0] = mat.mat[0][0] * cos(angle);
		result.mat[0][1] = - mat.mat[0][1] * sin(angle);
		result.mat[1][0] = mat.mat[1][0] * sin(angle);
		result.mat[1][1] = mat.mat[1][1] * cos(angle);

		return result;
	}
}