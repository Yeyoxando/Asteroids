/*   
 * Author: Diego Ochando Torres <yeyoxando@gmail.com>    
 *                                     
 * Modified for Asteroids 
 * 
 * Lists commands & utilities                                                                           
 */

struct TObstacle{
	int shape;
	esat::Vec2 position;
	esat::Vec2 scale;
	esat::Mat3 transform;
	esat::Vec3* points;
	int numberOfPoints;
	esat::Vec2 direction;
	int size;
	BoxCollider2D boxCollider;
	TObstacle* prox;
};

void InsertObstacleInList(TObstacle **list, int newShape, esat::Vec2 newPos, esat::Vec2 newScale, esat::Mat3 newTransform, esat::Vec3* newPoints, int numOfPoints, esat::Vec2 newDirection, int newSize, BoxCollider2D newCol){
  TObstacle *p = (TObstacle*)malloc(sizeof(TObstacle));

  p->shape = newShape;
  p->position = newPos;
  p->scale = newScale;
  p->transform = newTransform;
  p->points = newPoints;
  p->numberOfPoints = numOfPoints;
  p->direction = newDirection;
  p->size = newSize;
  p->boxCollider = newCol;
  
  p->prox = *list;
  *list = p;
}

//---------------------------------------------------------------------------//

void DeleteObstacleInList(TObstacle **list, TObstacle *value) {
	if(*list == value){
		TObstacle* next = (**list).prox;
		*list=NULL;
		free(*list);
		*list = (TObstacle*) calloc(1, sizeof(TObstacle));
		*list = next;
	}
	else{
		DeleteObstacleInList(&(**list).prox, value);
	}
}

//---------------------------------------------------------------------------//

bool IsEmptyObstacleList(TObstacle *list) {
  return list == NULL;
}

//---------------------------------------------------------------------------//

void DeleteObstacleList(TObstacle **list) {
	while (!IsEmptyObstacleList(*list)) {
		TObstacle *p = *list;
		*list = p->prox;
		free(p);
	}
}