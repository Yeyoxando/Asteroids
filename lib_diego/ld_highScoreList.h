/*   
 * Author: Diego Ochando Torres <yeyoxando@gmail.com>    
 *                                     
 * Modified for Asteroids 
 * 
 * Lists commands & utilities                                                                           
 */

struct THighScore{
	std::string username;
	std::string date;
	int score;
	THighScore* prox;
};

void InsertHighScoreInPosition(THighScore **list, int position, std::string newUsername, std::string newDate, int newScore){
  THighScore *p = (THighScore*)malloc(sizeof(THighScore));

  p->username = newUsername;
  p->date = newDate;
  p->score = newScore;
  
  p->prox = *list;
  *list = p;
}

void DeleteHighScoreInList(THighScore **list, THighScore *value) {
	if(*list == value){
		THighScore* next = (**list).prox;
		*list=NULL;
		free(*list);
		*list = (THighScore*) calloc(1, sizeof(THighScore));
		*list = next;
	}
	else{
		DeleteHighScoreInList(&(**list).prox, value);
	}
}

bool IsEmptyHighScoreList(THighScore *list) {
  return list == NULL;
}

void DeleteShootList(THighScore **list) {
	while (!IsEmptyHighScoreList(*list)) {
		THighScore *p = *list;
		*list = p->prox;
		free(p);
	}
}