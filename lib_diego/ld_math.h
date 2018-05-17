/*   
 * Author: Diego Ochando Torres <yeyoxando@gmail.com>    
 *                                                   
 * Math commands and utilities
 * Drawing utilities 
 */

struct TExtrude{
	esat::Vec4* points3D;
	int steps;
	int divisions;
};

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

esat::Mat4 Mat4Transform(float posX, float posY, float posZ, float scaleX, float scaleY, float scaleZ, float rotX, float rotY, float rotZ){
	esat::Mat4 translation = esat::Mat4Translate(posX, posY, posZ);
	esat::Mat4 scale = esat::Mat4Scale(scaleX, scaleY, scaleZ);
	esat::Mat4 rotation = esat::Mat4Multiply(esat::Mat4RotateX(rotX), esat::Mat4RotateY(rotY));
	rotation = esat::Mat4Multiply(rotation, esat::Mat4RotateZ(rotZ));
	esat::Mat4 transform = esat::Mat4Multiply(translation, scale);
	transform = esat::Mat4Multiply(transform, rotation);
	return transform;
}

esat::Vec4 Vec4Homogenize(esat::Vec4 point){
	esat::Vec4 homogenizedPoint = point;
	homogenizedPoint.x /= point.w;
	homogenizedPoint.y /= point.w;
	homogenizedPoint.z /= point.w;
	homogenizedPoint.w /= point.w;
	return homogenizedPoint;
}

float Interpolate(float x, float y, float value){
	float result = 0;
	float auxiliar_value = 0;

	value = ((value) / 100);
	auxiliar_value = y - x;
	auxiliar_value *= value;

	result = auxiliar_value;
	result = result + x;

	return result;
}

void AllocMesh(TExtrude *mesh, int n_steps, int n_divisions){
	mesh->points3D = (esat::Vec4*) malloc (sizeof(esat::Vec4) * n_steps * (n_divisions + 1));
	mesh->steps = n_steps;
	mesh->divisions = n_divisions;
}

void FreeMesh(TExtrude *mesh){
	free(mesh->points3D);
	mesh->steps = 0;
	mesh->divisions = 0;
	mesh->points3D = nullptr;
}

void ExtrudeMesh(TExtrude *mesh, esat::Vec2* points2D, float z, int n_steps, int n_divisions){
	for(int i = 0; i <= n_divisions; i++){
		for(int j = 0; j < n_steps; j++){
			mesh->points3D[(i * n_steps) + j] = {points2D[j].x, points2D[j].y, ((z / n_divisions) * i), 1};
		}
	}
}

void DrawMesh(TExtrude mesh, esat::Mat4 transform){
	esat::Vec4* transformedPoints = (esat::Vec4*) malloc (sizeof(esat::Vec4) * mesh.steps * (mesh.divisions + 1));
	for(int i = 0; i < (mesh.divisions+1)*mesh.steps; i++){
		transformedPoints[i] = esat::Mat4TransformVec4(transform, mesh.points3D[i]);
		transformedPoints[i] = Vec4Homogenize(transformedPoints[i]);
	}
	
	for(int i = 0; i <= mesh.divisions; i++){
		for(int j = 0; j < mesh.steps; j++){
			esat::DrawLine(transformedPoints[j + (i * mesh.steps)].x, transformedPoints[j + (i * mesh.steps)].y, transformedPoints[((j+ 1)%mesh.steps + (i * mesh.steps))].x, transformedPoints[((j+ 1)%mesh.steps + (i * mesh.steps))].y);
			if(i < mesh.divisions)
				esat::DrawLine(transformedPoints[j + (i * mesh.steps)].x, transformedPoints[j + (i * mesh.steps)].y, transformedPoints[(j+mesh.steps) + (i * mesh.steps)].x, transformedPoints[(j+mesh.steps) + (i * mesh.steps)].y);
		}
	}
	
	free(transformedPoints);
	transformedPoints = nullptr;
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

void DrawArrow(esat::Mat3 transform, float initialRotation){
	esat::Vec3* quad = (esat::Vec3*) calloc (7, sizeof(esat::Vec3));
	quad[0] = {20, 10, 1};
	quad[1] = {15, 0, 1};
	quad[2] = {12, 0, 1};
	quad[3] = {20, 16, 1};
	quad[4] = {28, 0, 1};
	quad[5] = {25, 0, 1};
	quad[6] = {20, 10, 1};

	esat::Mat3 correction = Mat3TransformOffset(-20, -8, 1, 1, DegreesToRadian(initialRotation));
	for(int i = 0; i < 7; i++){
		quad[i] = esat::Mat3TransformVec3(correction, quad[i]);
	}
	
	float* points = (float*) calloc (14, sizeof(float));
	
	for(int i = 0; i < 7; i++){
		quad[i] = esat::Mat3TransformVec3(transform, quad[i]);
		points[(i*2)] = quad[i].x;
		points[(i*2)+1] = quad[i].y;
	}
	
	esat::DrawSolidPath(points, 7);
}