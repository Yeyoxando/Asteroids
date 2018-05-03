/*   
 * Author: Diego Ochando Torres <yeyoxando@gmail.com>    
 *                                     
 * Modified for Asteroids 
 * 
 * Lists commands & utilities                                                                           
 */

struct TShoot{
	esat::Vec3 position;
	esat::Vec2 direction;
	esat::Mat3 transform;
	float speed;
	float lifeTime;
	TShoot* prox;
};

void InsertShoot(TShoot **list, esat::Vec3 newPos, esat::Vec2 newDirection, esat::Mat3 newTransform, float newSpeed){
  TShoot *p = (TShoot*)malloc(sizeof(TShoot));

  p->position = newPos;
  p->direction = newDirection;
  p->transform = newTransform;
  p->speed = newSpeed;
	p->lifeTime = 0;
  
  p->prox = *list;
  *list = p;
}

//---------------------------------------------------------------------------//

void DeleteShootInList(TShoot **list, TShoot *value) {
	if(*list == value){
		TShoot* next = (**list).prox;
		*list=NULL;
		free(*list);
		*list = (TShoot*) calloc(1, sizeof(TShoot));
		*list = next;
	}
	else{
		DeleteShootInList(&(**list).prox, value);
	}
}

//---------------------------------------------------------------------------//

bool IsEmptyShootList(TShoot *list) {
  return list == NULL;
}

//---------------------------------------------------------------------------//

void DeleteShootList(TShoot **list) {
	while (!IsEmptyShootList(*list)) {
		TShoot *p = *list;
		*list = p->prox;
		free(p);
	}
}