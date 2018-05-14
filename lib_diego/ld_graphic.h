/*   
 * Author: Diego Ochando Torres <yeyoxando@gmail.com>    
 *                                                   
 * Graphic commands & utilities      
 * Shapes 
 * Colors
 */
 
struct Color{
	float r;
	float g;
	float b;
 };

Color yellow = {255, 255, 0};
Color red = {255, 0, 0};
Color lightGreen = {183, 255, 166};
Color green = {0, 255, 0};
Color darkGreen = {0, 139, 29};
Color lightGrey = {214, 214, 214};
Color grey = {153, 153, 153};
Color darkGrey = {82, 82, 82};
Color cyan = {0, 255, 255};
Color blue = {0, 0, 255};
Color darkBlue = {0, 22, 184};
Color orange = {255, 192, 0};
Color white = {255, 255, 255};
Color black = {0, 0, 0};
Color magenta = {255, 0, 255};
Color purple = {162, 0, 255};
Color brown = {155, 96, 49};

void SpaceshipPoints(esat::Vec3** pointsPtr){
	esat::Vec3* spaceship = (esat::Vec3*) calloc (6, sizeof(esat::Vec3));
	spaceship[0] = {0, -1, 1};
	spaceship[1] = {-0.6f, 1, 1};
	spaceship[2] = {-0.5f, 0.7f, 1};
	spaceship[3] = {0.5f, 0.7f, 1};
	spaceship[4] = {0.6f, 1, 1};
	spaceship[5] = {0, -1, 1};
	*pointsPtr = spaceship;
}

void AlienPoints(esat::Vec3** pointsPtr){
	esat::Vec3* alien = (esat::Vec3*) calloc (12, sizeof(esat::Vec3));
	alien[0] = {1, 0.2f, 1};
	alien[1] = {-1, 0.2f, 1};
	alien[2] = {-0.6f, 0.5f, 1};
	alien[3] = {0.6f, 0.5f, 1};
	alien[4] = {1, 0.2f, 1};
	alien[5] = {0.4f, -0.2f, 1};
	alien[6] = {0.3f, -0.5f, 1};
	alien[7] = {-0.3f, -0.5f, 1};
	alien[8] = {-0.4f, -0.2f, 1};
	alien[9] = {0.4f, -0.2f, 1};
	alien[10] = {-0.4f, -0.2f, 1};
	alien[11] = {-1, 0.2f, 1};
	*pointsPtr = alien;
}

void Asteroid1Points(esat::Vec3** pointsPtr){
	esat::Vec3* asteroid1 = (esat::Vec3*) calloc (13, sizeof(esat::Vec3));
	asteroid1[0] = {0, -0.7f, 1};
	asteroid1[1] = {0.5f, -1, 1};
	asteroid1[2] = {1, -0.5f, 1};
	asteroid1[3] = {0.5f, -0.2f, 1};
	asteroid1[4] = {1, 0.3f, 1};
	asteroid1[5] = {0.4f, 1, 1};
	asteroid1[6] = {-0.3f, 0.7f, 1};
	asteroid1[7] = {-0.5f, 1, 1};
	asteroid1[8] = {-1, 0.5f, 1};
	asteroid1[9] = {-0.7f, 0, 1};
	asteroid1[10] = {-1, -0.5f, 1};
	asteroid1[11] = {-0.4f, -1, 1};
	asteroid1[12] = {0, -0.7f, 1};
	*pointsPtr = asteroid1;
}

void Asteroid2Points(esat::Vec3** pointsPtr){
	esat::Vec3* asteroid2 = (esat::Vec3*) calloc (12, sizeof(esat::Vec3));
	asteroid2[0] = {-0.3f, -1, 1};
	asteroid2[1] = {0.5f, -1, 1};
	asteroid2[2] = {0.8f, -0.3f, 1};
	asteroid2[3] = {0.8f, 0.1f, 1};
	asteroid2[4] = {0.3f, 1, 1};
	asteroid2[5] = {-0.3f, 1, 1};
	asteroid2[6] = {-0.2f, 0.1f, 1};
	asteroid2[7] = {-0.6f, 1, 1};
	asteroid2[8] = {-0.8f, 0.3f, 1};
	asteroid2[9] = {-0.4f, 0, 1};
	asteroid2[10] = {-0.8f, -0.3f, 1};
	asteroid2[11] = {-0.3f, -1, 1};
	*pointsPtr = asteroid2;
}

void Asteroid3Points(esat::Vec3** pointsPtr){
	esat::Vec3* asteroid3 = (esat::Vec3*) calloc (11, sizeof(esat::Vec3));
	asteroid3[0] = {0, -0.6f, 1};
	asteroid3[1] = {0.4f, -1, 1};
	asteroid3[2] = {0.8f, -0.6f, 1};
	asteroid3[3] = {0.6f, -0.1f, 1};
	asteroid3[4] = {0.8f, 0.4f, 1};
	asteroid3[5] = {0.2f, 1, 1};
	asteroid3[6] = {-0.5f, 1, 1};
	asteroid3[7] = {-0.8f, 0.7f, 1};
	asteroid3[8] = {-0.8f, -0.6f, 1};
	asteroid3[9] = {-0.4f, -1, 1};
	asteroid3[10] = {0, -0.6f, 1};
	*pointsPtr = asteroid3;
}

void Asteroid4Points(esat::Vec3** pointsPtr){
	esat::Vec3* asteroid4 = (esat::Vec3*) calloc (13, sizeof(esat::Vec3));
	asteroid4[0] = {-0.4f, -1, 1};
	asteroid4[1] = {0.3f, -1, 1};
	asteroid4[2] = {0.9f, -0.5f, 1};
	asteroid4[3] = {0.9f, -0.3f, 1};
	asteroid4[4] = {0.1f, 0, 1};
	asteroid4[5] = {0.9f, 0.5f, 1};
	asteroid4[6] = {0.5f, 1, 1};
	asteroid4[7] = {0.1f, 0.7f, 1};
	asteroid4[8] = {-0.5f, 1, 1};
	asteroid4[9] = {-0.9f, 0.2f, 1};
	asteroid4[10] = {-0.9f, -0.5f, 1};
	asteroid4[11] = {-0.2f, -0.5f, 1};
	asteroid4[12] = {-0.4f, -1, 1};
	*pointsPtr = asteroid4;
}
