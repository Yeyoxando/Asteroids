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

bool CheckBox2DCollision(BoxCollider2D obj1, BoxCollider2D obj2) {
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

bool CheckBox2DCollision(BoxCollider2D obj, esat::Vec2 point) {
  bool first_check = false;
  int collision_points = 0;

  if(obj.x1 < point.x && obj.x2 > point.x) {
    collision_points++;
  }
  if(obj.y1 < point.y && obj.y2 > point.y)  {
    collision_points++;
  }

  first_check = collision_points == 2;

  return first_check;
}////////////////////////////////////////////////////////*/