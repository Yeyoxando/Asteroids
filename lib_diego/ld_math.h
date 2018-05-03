/*   
 * Author: Diego Ochando Torres <yeyoxando@gmail.com>    
 *                                                   
 * Math commands and utilities
 * Drawing and math utilities 
 */

struct Mat2 { float d[4]; };

Mat2 Mat2Rotate(float angle){
	Mat2 result;
	angle *= 0.01745329252f;
	result.d[0] = cos(angle);
	result.d[1] = -sin(angle);
	result.d[2] = sin(angle);
	result.d[3] = cos(angle);
	return result;
}

Mat2 Mat2Scale(float xScale, float yScale){
	Mat2 result;
	result.d[0] = xScale;
	result.d[1] = 0;
	result.d[2] = 0;
	result.d[3] = yScale;
	return result;
}

Mat2 Mat2Identity(){
	Mat2 result;
	result.d[0] = 1;
	result.d[1] = 0;
	result.d[2] = 0;
	result.d[3] = 1;
	return result;
}

Mat2 Mat2Multiply(Mat2 matrix1, Mat2 matrix2){
	Mat2 result;
	result.d[0] = matrix1.d[0] * matrix2.d[0] + matrix1.d[1] * matrix2.d[2];
	result.d[1] = matrix1.d[0] * matrix2.d[1] + matrix1.d[1] * matrix2.d[3];
	result.d[2] = matrix1.d[2] * matrix2.d[0] + matrix1.d[3] * matrix2.d[2];
	result.d[3] = matrix1.d[2] * matrix2.d[1] + matrix1.d[3] * matrix2.d[3];
	return result;
}

Mat2 Mat2Transform(float scaleX, float scaleY, float radians){
	Mat2 rotation = Mat2Rotate(radians);
	Mat2 scale = Mat2Scale(scaleX, scaleY);
	return Mat2Multiply(rotation, scale);
}

esat::Vec2 Mat2MultiplyVec2(Mat2 matrix, esat::Vec2 vector){
	esat::Vec2 result;
	result.x = matrix.d[0] * vector.x + matrix.d[1] * vector.y;
	result.y = matrix.d[2] * vector.x + matrix.d[3] * vector.y;
	return result;
}

esat::Vec2 Vec2Normalize(esat::Vec2 v){
	esat::Vec2 result;
	float norm = sqrt(v.x * v.x + v.y * v.y);
	result = {v.x /= norm, v.y /= norm};
	return result;
}

esat::Vec2 CalculateVDirection(esat::Vec2 p, esat::Vec2 q, esat::Mat3 transform){
	esat::Vec2 result;
	q = esat::Mat3TransformVec2(transform, q);
	p = {p.x -= q.x, p.y -= q.y};
	result = {-p.x, -p.y};
	result = Vec2Normalize(result);
	return result;
}

bool LineDotDistance(esat::Vec3 line, esat::Vec3 dot1){
  float distance = 0;

  distance = (line.x * dot1.x) + \
             (line.y * dot1.y) + \
             line.z;

  if(distance < 0){
    printf("Inside\n\n");
  }else{
    printf("Outside\n\n");
  }

  return distance < 0;
}

esat::Vec3 ExplicitRect(esat::Vec3 dot1, esat::Vec3 dot2){
  esat::Vec3 aux;
  float aux_value = 0;

  aux.x = dot2.x - dot1.x;
  aux.y = dot2.y - dot1.y;

  aux_value = aux.x;

  aux.x = -(aux.y);
  aux.y = aux_value;


  aux.z = -(dot1.x * aux.x + dot1.y * aux.y);
	//a = x, b = y, z = c : ecuacion de la recta
  return aux;
}

void Mat2Print(Mat2 matrix){
	printf("\n %02f   %02f", matrix.d[0], matrix.d[1]);
	printf("\n %02f   %02f", matrix.d[2], matrix.d[3]);
}

esat::Mat3 Mat3Transform(float posX, float posY, float scaleX, float scaleY, float radians){
	esat::Mat3 translation = esat::Mat3Translate(posX, posY);
	esat::Mat3 scale = esat::Mat3Scale(scaleX, scaleY);
	esat::Mat3 rotation = esat::Mat3Rotate(radians);
	esat::Mat3 transform = esat::Mat3Multiply(translation, scale);
	transform = esat::Mat3Multiply(transform, rotation);
	return transform;
}

esat::Mat3 Mat3TransformOffset(float posX, float posY, float scaleX, float scaleY, float radians){
	esat::Mat3 translation = esat::Mat3Translate(posX, posY);
	esat::Mat3 scale = esat::Mat3Scale(scaleX, scaleY);
	esat::Mat3 rotation = esat::Mat3Rotate(radians);
	esat::Mat3 transform = esat::Mat3Multiply(translation, scale);
	transform = esat::Mat3Multiply(rotation, transform);
	return transform;
}

float DegreesToRadian(int degrees){
	return degrees * 0.01745329252f;
}

void DrawShape(esat::Vec3* points, int numberOfPoints, esat::Mat3 transform){
	esat::Vec3* newPoints = (esat::Vec3*) calloc (numberOfPoints, sizeof(esat::Vec3));
	esat::Vec2* transformedPoints = (esat::Vec2*) calloc (numberOfPoints, sizeof(esat::Vec2));
	
	for(int i = 0; i < numberOfPoints; i++){
		newPoints[i] = {points[i].x, points[i].y, points[i].z};
		newPoints[i] = esat::Mat3TransformVec3(transform, newPoints[i]);
		transformedPoints[i].x = newPoints[i].x;
		transformedPoints[i].y = newPoints[i].y;
	}
	
	esat::DrawPath(&transformedPoints[0].x, numberOfPoints);
}

void DrawQuad(esat::Mat3 transform){
	esat::Vec3* quad = (esat::Vec3*) calloc (5, sizeof(esat::Vec3));
	quad[0] = {0.5f, 0.5f, 1};
	quad[1] = {0.5f, -0.5f, 1};
	quad[2] = {-0.5f, -0.5f, 1};
	quad[3] = {-0.5f, 0.5f, 1};
	quad[4] = {0.5f, 0.5f, 1};
	
	float* points = (float*) calloc (10, sizeof(float));
	
	for(int i = 0; i < 5; i++){
		quad[i] = esat::Mat3TransformVec3(transform, quad[i]);
		points[(i*2)] = quad[i].x;
		points[(i*2)+1] = quad[i].y;
	}
	
	esat::DrawPath(points, 5);
}

void DrawArrow(esat::Mat3 transform){
	esat::Vec3* quad = (esat::Vec3*) calloc (7, sizeof(esat::Vec3));
	quad[0] = {20, 10, 1};
	quad[1] = {15, 0, 1};
	quad[2] = {12, 0, 1};
	quad[3] = {20, 16, 1};
	quad[4] = {28, 0, 1};
	quad[5] = {25, 0, 1};
	quad[6] = {20, 10, 1};
	
	float* points = (float*) calloc (10, sizeof(float));
	
	for(int i = 0; i < 7; i++){
		quad[i] = esat::Mat3TransformVec3(transform, quad[i]);
		points[(i*2)] = quad[i].x;
		points[(i*2)+1] = quad[i].y;
	}
	
	esat::DrawPath(points, 7);
}
