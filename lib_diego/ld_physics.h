/*   
 * Author: Diego Ochando Torres <yeyoxando@gmail.com>    
 *                                                   
 * Collision system and physic utilities                                                                           
 */

struct BoxCollider2D{
	float x1;
	float y1;
	float x2;
	float y2;
};

struct PolyCollider2D{
	esat::Vec3* points;
};

bool CheckBox2DCollisions(BoxCollider2D obj1, BoxCollider2D obj2) {
  bool first_check = false;
  bool second_check = false;
  int collision_points = 0;

  if((obj1.x1 <= obj2.x2 && obj1.x1 >= obj2.x1) || (obj1.x2 <= obj2.x2 && obj1.x2 >= obj2.x1)) {
    collision_points++;
  }
  if((obj1.y1 <= obj2.y2 && obj1.y1 >= obj2.y1) || (obj1.y2 <= obj2.y2 && obj1.y2 >= obj2.y1))  {
    collision_points++;
  }

  first_check = collision_points == 2;
  collision_points = 0;

  if((obj2.x1 <= obj1.x2 && obj2.x1 >= obj1.x1) || (obj2.x2 <= obj1.x2 && obj2.x2 >= obj1.x1)) {
    collision_points++;
  }
  if((obj2.y1 <= obj1.y2 && obj2.y1 >= obj1.y1) || (obj2.y2 <= obj1.y2 && obj2.y2 >= obj1.y1))  {
    collision_points++;
  }

  second_check = collision_points == 2;

  return first_check || second_check;
}

bool CheckPoly2DCollisions(PolyCollider2D obj1, PolyCollider2D obj2){
	return 0;
}

bool CheckDotPoly2DCollisions(PolyCollider2D obj, esat::Vec3 dot){
	return 0;
}

/*///////////////////////////////////////////////////////////
COLISIONES
//////////////////////////////////////////////////////////*/