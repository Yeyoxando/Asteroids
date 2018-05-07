#include <stdlib.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <time.h>
#include <math.h>
#include <esat/window.h>
#include <esat/draw.h>
#include <esat/input.h>
#include <esat/math.h>
#include <esat_extra/sqlite3.h>
#include "lib_diego/ld_math.h"
#include "lib_diego/ld_graphic.h"
#include "lib_diego/ld_ui.h"
#include "lib_diego/ld_physics.h"
#include "lib_diego/ld_obstacleList.h"//The obstacle struct (asteroids and aliens) is here to use it with lists
#include "lib_diego/ld_shootList.h"//The shoot struct is here to use it with lists

//Hacer un insert coin to guapo con la mondea en 3d dando vueltas

struct User{
	std::string username;
	std::string email;
	std::string password;
	std::string name;
	std::string surnames;
	std::string birthdate;
	std::string province;
	std::string country;
	std::string credits = "10";
};

struct Player{
	int level = 0;
	int lifes = 3;
	int score = 0;
	esat::Vec2 position;
	float rotation;
	esat::Vec2 scale;
	float speed;
	float shootSpeed = 8;
	esat::Vec2 direction;
	esat::Mat3 transform;
	esat::Vec3* points;
	BoxCollider2D boxCollider;
	TShoot* shoots;
};

enum scenes{
	login = 0,
	mainMenu,
	addUser,
	highScores,
	game
};

const float width = 960;
const float height = 720;

int activeScene = 0;
int currentPlayer = 0;
int numOfPlayers;
float obstacleSpeed = 2;

bool activeLoginMenu = false;
bool activeMainMenu = false;
bool activeAddUserMenu = false;
bool activeHighScoreMenu = false;

char* auxString = (char*) calloc (80, sizeof(char));
char* auxString2 = (char*) calloc (80, sizeof(char));

Player players[2];
TObstacle* obstacles;
Box* menuBoxes=NULL;
User newUser;
std::string* logUser = (std::string*) calloc (2, sizeof(std::string));
int userCredits = 0;
bool logged = false;
bool selectPlayers = false;
sqlite3 *db;
char* errMsg = (char*) calloc (80, sizeof(char));

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	if(logUser[0] == argv[0] && logUser[1] == argv[1]){
		userCredits = atoi(argv[2]);
		logged = true;
	}
	else{errMsg = "Wrong user or password";}
	 
	if(logUser[0].length() < 1 || logUser[1].length() < 1) errMsg = "Fill all the fields";

	return 0;
}

void CreateDB(){
   char *zErrMsg = 0;
   int rc;
   char *sql;

   /* Open database */
   rc = sqlite3_open("Asteroids_db.db", &db);
   
   if( rc ) {
      fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
   }

   /* Create SQL statement */
   sql = "CREATE TABLE IF NOT EXISTS USERS ("  \
         "Username  VARCHAR (50) PRIMARY KEY," \
         "Email     VARCHAR (50) UNIQUE," \
         "Password  VARCHAR (50) NOT NULL," \
         "Name      VARCHAR (50) NOT NULL," \
         "Surnames  VARCHAR (75) NOT NULL," \
				 "Birthdate VARCHAR (50) NOT NULL," \
				 "Province  VARCHAR (50) NOT NULL," \
				 "Country   VARCHAR (50) NOT NULL," \
				 "Credits   INT (3)      NOT NULL " \
				 ");";

   /* Execute SQL statement */
   rc = sqlite3_exec(db, sql, callback, 0, &zErrMsg);
   if( rc != SQLITE_OK ){
   fprintf(stderr, "SQL error: %s\n", zErrMsg);
      sqlite3_free(zErrMsg);
   }
   sqlite3_close(db);
}

void AddUserToDB(){
   char *zErrMsg = 0;
   int rc;
   std::string sql;
	 if(newUser.username.length() > 0 && newUser.email.length() > 0 && newUser.password.length() > 0 && newUser.name.length() > 0 &&
			newUser.surnames.length() > 0 && newUser.birthdate.length() > 0 && newUser.province.length() > 0 && newUser.country.length() > 0){
		 rc = sqlite3_open("Asteroids_db.db", &db);

		 if( rc ) {
				fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		 }
		 
		 sql = "INSERT INTO USERS (Username, Email, Password, Name, Surnames, Birthdate, Province, Country, Credits) "  \
					 "VALUES ('"+newUser.username+"', '"+newUser.email+"', '"+newUser.password+"', '"+newUser.name+"', '" \
					 +newUser.surnames+"', '"+newUser.birthdate+"', '"+newUser.province+"', '"+newUser.country+"', '"+newUser.credits+"'); ";
		 
		 char *sqlChar = new char[sql.length() + 1];
		 strcpy(sqlChar, sql.c_str());
	 
		 rc = sqlite3_exec(db, sqlChar, callback, 0, &zErrMsg);
		 
		 if( rc != SQLITE_OK ){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
				errMsg = "SQL error";
		 } else {
				errMsg="user saved";
		 }
		 sqlite3_close(db);
	}
	else{ 
		errMsg="fill all the fields";
	}
}

void Login(){
	sqlite3 *db;
  char *zErrMsg = 0;
  int rc;
  char *sql;
  const char* data = "Callback function called";

  /* Open database */
  rc = sqlite3_open("Asteroids_db.db", &db);
   
  if( rc ) {
    fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
  }

  sql = "SELECT Username, Password, Credits FROM USERS";

  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   
  if( rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
  sqlite3_close(db);
}

void UpdateUser(){
	if(logged){
		sqlite3 *db;
		char *zErrMsg = 0;
		int rc;
		std::string sql;
		const char* data = "Callback function called";

		/* Open database */
		rc = sqlite3_open("Asteroids_db.db", &db);
		 
		if( rc ) {
			fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		}
		
		std::string credits = itoa(userCredits, auxString, 10);
		
		sql = "UPDATE USERS SET Credits = '" + credits + "'" \
					"WHERE Username = '" + logUser[0] + "';";

		char *sqlChar = new char[sql.length() + 1];
		strcpy(sqlChar, sql.c_str());
					
		/* Execute SQL statement */
		rc = sqlite3_exec(db, sqlChar, callback, (void*)data, &zErrMsg);
		 
		if( rc != SQLITE_OK ) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		sqlite3_close(db);
	}
}

void LogOut(){
	esat::ResetBufferdKeyInput();
	UpdateUser();
	errMsg = "";
	logged = false;
	logUser= (std::string*) calloc (2, sizeof(std::string));
	userCredits = 0;
}

void InitializeShoot(){
	esat::Vec3 pos = esat::Mat3TransformVec3(players[currentPlayer].transform, players[currentPlayer].points[0]);
	esat::Vec2 dir = players[currentPlayer].direction;
	esat::Mat3 transform = Mat3Transform(pos.x, pos.y, 2, 2, DegreesToRadian(45));
	InsertShoot(&players[currentPlayer].shoots, pos, dir, transform, players[currentPlayer].shootSpeed);
}

void MoveShoot(TShoot** shoots){
	TShoot* shoot = *shoots;
	shoot->position.x += shoot->direction.x * players[currentPlayer].shootSpeed;
	shoot->position.y += shoot->direction.y * players[currentPlayer].shootSpeed;
	shoot->transform = Mat3Transform(shoot->position.x, shoot->position.y, 2, 2, 45);
	
	if(shoot->position.x > width) shoot->position.x = 0;
	if(shoot->position.x < 0) shoot->position.x = width;
	if(shoot->position.y > height) shoot->position.y = 0;
	if(shoot->position.y < 0) shoot->position.y = height;
	
	shoot->lifeTime++;
}

void ShootController(TShoot* shoots){
	while(shoots){
		DrawQuad(shoots->transform);
		MoveShoot(&shoots);
		if(shoots->lifeTime > 60){
			TShoot* aux = shoots;
			DeleteShootInList(&players[currentPlayer].shoots, shoots);
			shoots = (*aux).prox;
		}
		else{
			shoots = (*shoots).prox;
		}
	}
}

void InitializePlayer(Player *player){
	SpaceshipPoints(&(*player).points);
	(*player).position.x = 480;
	(*player).position.y = 360;
	(*player).rotation = 0;
	(*player).scale.x = 20;
	(*player).scale.y = 20;
	(*player).transform = Mat3Transform((*player).position.x, (*player).position.y, (*player).rotation, (*player).scale.x, (*player).scale.y);
	(*player).boxCollider = {1, 1, -1, -1};
}

void PlayerController(Player *player){
	esat::Vec2 q = {(*player).points[0].x, (*player).points[0].y};
	(*player).direction = CalculateVDirection((*player).position, q, (*player).transform);
	
	if(esat::IsSpecialKeyDown(esat::kSpecialKey_Space)){
		InitializeShoot();
	}
	if(esat::IsKeyPressed('A') || esat::IsSpecialKeyPressed(esat::kSpecialKey_Left)){
		(*player).rotation -= 0.1f;
	}
	if(esat::IsKeyPressed('D') || esat::IsSpecialKeyPressed(esat::kSpecialKey_Right)){
		(*player).rotation += 0.1f;
	}
	if(esat::IsKeyDown('G')){
		(*player).position.x = rand()%960;
		(*player).position.y = rand()%720;
	}
	if((esat::IsKeyPressed('W') || esat::IsSpecialKeyPressed(esat::kSpecialKey_Up)) && (*player).speed <= 9){
		(*player).speed += 0.4f;
	}
	else{
		if((*player).speed > 0) (*player).speed -= 0.1f;
		if((*player).speed < 0) (*player).speed = 0;
	}
	
	(*player).position.x += (*player).direction.x * (*player).speed;
	(*player).position.y += (*player).direction.y * (*player).speed;
	if((*player).position.x > width) (*player).position.x = 0;
	if((*player).position.x < 0) (*player).position.x = width;
	if((*player).position.y > height) (*player).position.y = 0;
	if((*player).position.y < 0) (*player).position.y = height;
	
	(*player).transform = Mat3Transform((*player).position.x, (*player).position.y, (*player).scale.x, (*player).scale.y, (*player).rotation);
	esat::Vec2 point1 = {1, 1};
	esat::Vec2 point2 = {-1, -1};
	esat::Mat3 colTransform = Mat3Transform((*player).position.x, (*player).position.y, (*player).scale.x, (*player).scale.y, 0);
	point1 = esat::Mat3TransformVec2(colTransform, point1); 
	point2 = esat::Mat3TransformVec2(colTransform, point2);
	(*player).boxCollider.x1 = point2.x;
	(*player).boxCollider.y1 = point2.y;
	(*player).boxCollider.x2 = point1.x;
	(*player).boxCollider.y2 = point1.y;
	
	float colliderShape[10];
	
	colliderShape[0] = (*player).boxCollider.x1;
    colliderShape[1] = (*player).boxCollider.y1;
    colliderShape[2] = (*player).boxCollider.x2;
    colliderShape[3] = (*player).boxCollider.y1;
    colliderShape[4] = (*player).boxCollider.x2;
    colliderShape[5] = (*player).boxCollider.y2;
    colliderShape[6] = (*player).boxCollider.x1;
    colliderShape[7] = (*player).boxCollider.y2;
    colliderShape[8] = (*player).boxCollider.x1;
    colliderShape[9] = (*player).boxCollider.y1;
	
	esat::DrawSetStrokeColor(255, 0, 0);
	esat::DrawPath(colliderShape, 5);
	
	esat::DrawSetStrokeColor(255, 255, 255);
	DrawShape((*player).points, 6, (*player).transform);
}

void PlayerDeath(Player *player){
	(*player).lifes--;
	(*player).position.x = 480;
	(*player).position.y = 360;
	(*player).rotation = 0;
	(*player).scale.x = 20;
	(*player).scale.y = 20;
	(*player).speed = 0;
	(*player).transform = Mat3Transform((*player).position.x, (*player).position.y, (*player).rotation, (*player).scale.x, (*player).scale.y);
}

void InsertObstacle(TObstacle** objects, int size, esat::Vec2 position, int shape = rand()%4){
	esat::Vec2 scale = {(float)10 * size, (float)10 * size};
	esat::Mat3 transform = Mat3Transform(position.x, position.y, scale.x, scale.y, 0);
	esat::Vec3* points;
	int numOfPoints;
	switch(shape){
		case 0:
			Asteroid1Points(&points);
			numOfPoints = 13;
			break;
		case 1:
			Asteroid2Points(&points);
			numOfPoints = 12;
			break;
		case 2:
			Asteroid3Points(&points);
			numOfPoints = 11;
			break;
		case 3:
			Asteroid4Points(&points);
			numOfPoints = 13;
			break;
		case 4:
			AlienPoints(&points);
			numOfPoints = 12;
			break;
	}
	esat::Vec2 direction;
	if(rand()%2 == 1){
		do{
			direction.x = (rand()%20);
		}while(direction.x == 0);
	}else{
		do{
			direction.x = -(rand()%20);
		}while(direction.x == 0);
	}
	if(rand()%2 == 1){
		do{
			direction.y = (rand()%20);
		}while(direction.y == 0);
	}else{
		do{
			direction.y = (rand()%20);
		}while(direction.y == 0);
	}
	direction = Vec2Normalize(direction);
	BoxCollider2D boxCol = {1, 1, -1, -1};
	InsertObstacleInList(&*objects, shape, position, scale, transform, points, numOfPoints, direction, size, boxCol);
}

void MoveObstacle(TObstacle** objects){
	TObstacle* node = *objects;
	node->position.x += node->direction.x * obstacleSpeed;
	node->position.y += node->direction.y * obstacleSpeed;
	node->transform = Mat3Transform(node->position.x, node->position.y, node->scale.x, node->scale.y, 0);
	
	if(node->position.x > width) node->position.x = 0;
	if(node->position.x < 0) node->position.x = width;
	if(node->position.y > height) node->position.y = 0;
	if(node->position.y < 0) node->position.y = height;
	
	esat::Vec2 point1 = {1, 1};
	esat::Vec2 point2 = {-1, -1};
	point1 = esat::Mat3TransformVec2(node->transform, point1); 
	point2 = esat::Mat3TransformVec2(node->transform, point2);
	node->boxCollider.x1 = point2.x;
	node->boxCollider.y1 = point2.y;
	node->boxCollider.x2 = point1.x;
	node->boxCollider.y2 = point1.y;
	
	float colliderShape[10];
	
	colliderShape[0] = node->boxCollider.x1;
    colliderShape[1] = node->boxCollider.y1;
    colliderShape[2] = node->boxCollider.x2;
    colliderShape[3] = node->boxCollider.y1;
    colliderShape[4] = node->boxCollider.x2;
    colliderShape[5] = node->boxCollider.y2;
    colliderShape[6] = node->boxCollider.x1;
    colliderShape[7] = node->boxCollider.y2;
    colliderShape[8] = node->boxCollider.x1;
    colliderShape[9] = node->boxCollider.y1;
	
	esat::DrawSetStrokeColor(255, 0, 0);
	esat::DrawPath(colliderShape, 5);
	
	esat::DrawSetStrokeColor(255, 255, 255);
}

void CheckCollisions(TObstacle* objects, Player player){
	TObstacle* node = objects;
	if(CheckBox2DCollision(node->boxCollider, player.boxCollider)){
		PlayerDeath(&players[currentPlayer]);
		DeleteObstacleInList(&obstacles, node);
		if(node->size > 1){
			InsertObstacle(&obstacles, node->size/2, node->position);
			InsertObstacle(&obstacles, node->size/2, node->position);
		}
	}
}

void CheckShootCollisions(TObstacle* objects){
	TObstacle* node = objects;
	TShoot* shoot = players[currentPlayer].shoots;
	while(shoot){
		esat::Vec2 shootPos = {shoot->position.x, shoot->position.y};
		if(CheckBox2DCollision(node->boxCollider, shootPos)){
			DeleteShootInList(&players[currentPlayer].shoots, shoot);
			DeleteObstacleInList(&obstacles, node);
			if(node->size > 1){
				InsertObstacle(&obstacles, node->size/2, node->position);
				InsertObstacle(&obstacles, node->size/2, node->position);
			}
			switch(node->size){
				case 4:
					players[currentPlayer].score += 20;
					break;
				case 2:
					players[currentPlayer].score += 50;
					break;
				case 1:
					players[currentPlayer].score += 100;
					break;
			}
		}
		shoot = (*shoot).prox;
	}
}

void ObstacleDrawer(TObstacle* objects){
	while(objects){
		DrawShape((*objects).points, (*objects).numberOfPoints, (*objects).transform);
		MoveObstacle(&objects);
		CheckCollisions(objects, players[currentPlayer]);
		CheckShootCollisions(objects);
		objects = (*objects).prox;
	}
}

void AlienSpawner(){
	
}

void LoadLevel(){
	esat::Vec2 nPosition = {200, 200};
	InsertObstacle(&obstacles, 4, nPosition, 3);
	InsertObstacle(&obstacles, 4, nPosition, 2);
	InsertObstacle(&obstacles, 4, nPosition, 1);
	InsertObstacle(&obstacles, 4, nPosition, 0);
}

void InitializeLoginMenu(){
	esat::DrawSetStrokeColor(255, 255, 255);
	menuBoxes = (Box*) calloc(4, sizeof(Box));
	menuBoxes[0].CreateBox(250, 220, 400, 60, &menuBoxes[0].boxPoints);
	menuBoxes[1].CreateBox(250, 380, 400, 60, &menuBoxes[1].boxPoints);
	menuBoxes[2].CreateBox(240, 590, 200, 40, &menuBoxes[2].boxPoints);
	menuBoxes[3].CreateBox(455, 590, 200, 40, &menuBoxes[3].boxPoints);
}

void InitializeAddUserMenu(){
	menuBoxes = (Box*) calloc(13, sizeof(Box));
	menuBoxes[0].CreateBox(250, 110, 400, 40, &menuBoxes[0].boxPoints);
	menuBoxes[1].CreateBox(250, 160, 400, 40, &menuBoxes[1].boxPoints);
	menuBoxes[2].CreateBox(250, 210, 400, 40, &menuBoxes[2].boxPoints);
	menuBoxes[3].CreateBox(250, 260, 400, 40, &menuBoxes[3].boxPoints);
	menuBoxes[4].CreateBox(250, 310, 400, 40, &menuBoxes[4].boxPoints);
	menuBoxes[5].CreateBox(250, 360, 400, 40, &menuBoxes[5].boxPoints);
	menuBoxes[6].CreateBox(250, 410, 400, 40, &menuBoxes[6].boxPoints);
	menuBoxes[7].CreateBox(250, 460, 400, 40, &menuBoxes[7].boxPoints);
	menuBoxes[8].CreateBox(250, 510, 400, 40, &menuBoxes[8].boxPoints);
	menuBoxes[9].CreateBox(193, 650, 200, 40, &menuBoxes[9].boxPoints);
	menuBoxes[10].CreateBox(405, 650, 200, 40, &menuBoxes[10].boxPoints);
	menuBoxes[11].CreateBox(670, 160, 40, 40, &menuBoxes[11].boxPoints);
	menuBoxes[12].CreateBox(670, 360, 40, 40, &menuBoxes[12].boxPoints);
}

void InitializeMainMenu(){
	esat::DrawSetStrokeColor(255, 255, 255);
	menuBoxes = (Box*) calloc(5, sizeof(Box));
	menuBoxes[0].CreateBox(330, 200, 300, 70, &menuBoxes[0].boxPoints);
	menuBoxes[1].CreateBox(330, 360, 300, 70, &menuBoxes[1].boxPoints);
	menuBoxes[2].CreateBox(330, 520, 300, 70, &menuBoxes[2].boxPoints);
	menuBoxes[3].CreateBox(680, 160, 70, 70, &menuBoxes[3].boxPoints);
	menuBoxes[4].CreateBox(680, 240, 70, 70, &menuBoxes[4].boxPoints);
}

void InitializeHighScoreMenu(){
	esat::DrawSetStrokeColor(255, 255, 255);
	menuBoxes = (Box*) calloc(2, sizeof(Box));
	menuBoxes[0].CreateBox(193, 650, 200, 40, &menuBoxes[0].boxPoints);
}

void DeselectTextFields(int textFields, Box *activeTextField){
	for(int i = 0; i < textFields; i++){
		menuBoxes[i].selected = false;
	}
	(*activeTextField).selected = true;
}

void LoginMenu(){
	esat::DrawSetFillColor(255, 255, 255);
	esat::DrawSetTextSize(80);
	esat::DrawText(320, 80, "Log in");
	esat::DrawSetTextSize(40);	
	esat::DrawText(355, 210, "Username");
	esat::DrawText(355, 370, "Password");
	esat::DrawText(50, 540, errMsg);
	
	TextField(&menuBoxes[0], &logUser[0]);
	if(menuBoxes[0].isMouseOver() && esat::MouseButtonDown(0)) DeselectTextFields(2, &menuBoxes[0]);
	
	TextField(&menuBoxes[1], &logUser[1], true);
	if(menuBoxes[1].isMouseOver() && esat::MouseButtonDown(0)) DeselectTextFields(2, &menuBoxes[1]);
	
	if(menuBoxes[2].isMouseOver() && esat::MouseButtonDown(0)){
		activeScene = 2;
		activeAddUserMenu = true;
		errMsg = "";
	}
	menuBoxes[2].DrawButton(menuBoxes[2].isMouseOver(), white);
	DrawTextPlus(260, 623, "Add user", black, white, menuBoxes[2]);
	
	if(menuBoxes[3].isMouseOver() && esat::MouseButtonDown(0)){
		Login();
		if(logged){
			activeScene = 1;
			activeMainMenu = true;
		}
	}
	
	menuBoxes[3].DrawButton(menuBoxes[3].isMouseOver(), white);
	DrawTextPlus(505, 623, "Login", black, white, menuBoxes[3]);
}

void AddUserMenu(){
	esat::DrawSetFillColor(255, 255, 255);
	esat::DrawSetTextSize(80);
	esat::DrawText(250, 80, "New user");
	esat::DrawSetTextSize(40);
	esat::DrawText(20, 140, "Username");
	esat::DrawText(20, 190, "Email");
	esat::DrawText(20, 240, "Password");
	esat::DrawText(20, 290, "Name");
	esat::DrawText(20, 340, "Surnames");
	esat::DrawText(20, 390, "Birthdate");
	esat::DrawText(20, 440, "Province");
	esat::DrawText(20, 490, "Country");
	esat::DrawText(20, 540, "Credits");
	esat::DrawText(50, 620, errMsg);
	
	TextField(&menuBoxes[0], &newUser.username);
	if(menuBoxes[0].isMouseOver() && esat::MouseButtonDown(0)) DeselectTextFields(9, &menuBoxes[0]);
	
	TextField(&menuBoxes[1], &newUser.email);
	if(menuBoxes[1].isMouseOver() && esat::MouseButtonDown(0)) DeselectTextFields(9, &menuBoxes[1]);

	TextField(&menuBoxes[2], &newUser.password);
	if(menuBoxes[2].isMouseOver() && esat::MouseButtonDown(0)) DeselectTextFields(9, &menuBoxes[2]);
	
	TextField(&menuBoxes[3], &newUser.name);
	if(menuBoxes[3].isMouseOver() && esat::MouseButtonDown(0)) DeselectTextFields(9, &menuBoxes[3]);
	
	TextField(&menuBoxes[4], &newUser.surnames);
	if(menuBoxes[4].isMouseOver() && esat::MouseButtonDown(0)) DeselectTextFields(9, &menuBoxes[4]);
	
	TextField(&menuBoxes[5], &newUser.birthdate);
	if(menuBoxes[5].isMouseOver() && esat::MouseButtonDown(0)) DeselectTextFields(9, &menuBoxes[5]);
	
	TextField(&menuBoxes[6], &newUser.province);
	if(menuBoxes[6].isMouseOver() && esat::MouseButtonDown(0)) DeselectTextFields(9, &menuBoxes[6]);
	
	TextField(&menuBoxes[7], &newUser.country);
	if(menuBoxes[7].isMouseOver() && esat::MouseButtonDown(0)) DeselectTextFields(9, &menuBoxes[7]);
	
	esat::DrawText(250, 540, "10");
 
	if(menuBoxes[9].isMouseOver() && esat::MouseButtonDown(0)){
		activeScene = 0;
		activeLoginMenu = true;
		errMsg = "";
	}
	menuBoxes[9].DrawButton(menuBoxes[9].isMouseOver(), white);
	DrawTextPlus(225, 680, "Return", black, white, menuBoxes[9]);
	
	if(menuBoxes[10].isMouseOver() && esat::MouseButtonDown(0)){
		AddUserToDB();
	}
	menuBoxes[10].DrawButton(menuBoxes[10].isMouseOver(), white);
	DrawTextPlus(425, 680, "Add user", black, white, menuBoxes[10]);
	
	if(menuBoxes[11].isMouseOver() && esat::MouseButtonDown(0)){
		newUser.email += '@';
	}
	menuBoxes[11].DrawButton(menuBoxes[11].isMouseOver(), white);
	DrawTextPlus(678, 195, "@", black, white, menuBoxes[11]);
	
	if(menuBoxes[12].isMouseOver() && esat::MouseButtonDown(0)){
		newUser.birthdate += '/';
	}
	menuBoxes[12].DrawButton(menuBoxes[12].isMouseOver(), white);
	DrawTextPlus(680, 395, "/", black, white, menuBoxes[12]);
}

void MainMenu(){
	esat::DrawSetFillColor(255, 255, 255);
	esat::DrawSetTextSize(80);
	esat::DrawText(300, 80, "Asteroids");
	esat::DrawSetTextSize(50);
	std::string welcome = "Welcome, " + logUser[0];
	char *welcomeChar = new char[welcome.length() + 1];
	strcpy(welcomeChar, welcome.c_str());
	esat::DrawText(30, 680, welcomeChar);
	esat::DrawText(500, 680, "Credits: ");
	esat::DrawText(700, 680, itoa(userCredits, auxString, 10));
	
	if(menuBoxes[0].isMouseOver() && esat::MouseButtonDown(0)){
		selectPlayers = true;
	}
	menuBoxes[0].DrawButton(menuBoxes[0].isMouseOver(), white);
	DrawTextPlus(425, 250, "Play", black, white, menuBoxes[0]);
	
	if(menuBoxes[1].isMouseOver() && esat::MouseButtonDown(0)){
		activeScene = 3;
		activeHighScoreMenu = true;
		selectPlayers = false;
	}
	menuBoxes[1].DrawButton(menuBoxes[1].isMouseOver(), white);
	DrawTextPlus(350, 410, "High scores", black, white, menuBoxes[1]);
	
	if(menuBoxes[2].isMouseOver() && esat::MouseButtonDown(0)){
		LogOut();
		activeScene = 0;
		activeLoginMenu = true;
		selectPlayers = false;
	}
	menuBoxes[2].DrawButton(menuBoxes[2].isMouseOver(), white);
	DrawTextPlus(385, 570, "Log out", black, white, menuBoxes[2]);
	
	if(selectPlayers){
		if(menuBoxes[3].isMouseOver() && esat::MouseButtonDown(0)){
			if(userCredits > 0){
				numOfPlayers = 1;
				activeScene = 4;
				userCredits--;
			}
		}
		menuBoxes[3].DrawButton(menuBoxes[3].isMouseOver(), white);
		DrawTextPlus(690, 210, "1P", black, white, menuBoxes[3]);
		
		if(menuBoxes[4].isMouseOver() && esat::MouseButtonDown(0)){
			if(userCredits > 1){
				numOfPlayers = 2;
				activeScene = 4;
				userCredits -= 2;
			}
		}
		menuBoxes[4].DrawButton(menuBoxes[4].isMouseOver(), white);
		DrawTextPlus(690, 290, "2P", black, white, menuBoxes[4]);
	}
}

void HighScoreMenu(){
	if(menuBoxes[0].isMouseOver() && esat::MouseButtonDown(0)){
		activeScene = 1;
		activeMainMenu = true;
		errMsg = "";
	}
	menuBoxes[0].DrawButton(menuBoxes[0].isMouseOver(), white);
	DrawTextPlus(225, 680, "Return", black, white, menuBoxes[0]);
}

void GameUI(){
	esat::DrawSetTextSize(40);
	esat::DrawSetStrokeColor(255, 255, 255);
	esat::DrawText(30, 40, itoa(players[currentPlayer].score, auxString, 10));
	
	for(int i = 0; i < players[currentPlayer].lifes; i++){
		esat::Mat3 transform = Mat3Transform(i * 15 + 30, 60, 10, 10, 0);
		DrawShape(players[currentPlayer].points, 6, transform);
	}
}

void Game(){
	ObstacleDrawer(obstacles);
	ShootController(players[currentPlayer].shoots);
	PlayerController(&players[currentPlayer]);
	GameUI();
	if(esat::IsKeyDown('M')){//Debug
		activeScene = 1;
		activeMainMenu = true;
	}
}

void SceneManager(){
	if(activeLoginMenu) {
		InitializeLoginMenu();
		activeLoginMenu = false;
	}
	if(activeScene == 0){
		LoginMenu();
	}
	if(activeMainMenu) {
		InitializeMainMenu();
		activeMainMenu = false;
	}
	if(activeScene == 1){
		MainMenu();
	}
	if(activeAddUserMenu) {
		InitializeAddUserMenu();
		activeAddUserMenu = false;
	}
	if(activeScene == 2){
		AddUserMenu();
	}
	if(activeHighScoreMenu) {
		InitializeHighScoreMenu();
		activeHighScoreMenu = false;
	}
	if(activeScene == 3){
		HighScoreMenu();
	}
	if(activeScene == 4){
		Game();
	}
}

void Init(){
	esat::DrawSetTextFont("resources/fonts/Hyperspace Bold.otf");
	InitializeLoginMenu();
	CreateDB();
}

void Exit(){
	DeleteObstacleList(&obstacles);
	DeleteShootList(&players[0].shoots);
	DeleteShootList(&players[1].shoots);
	UpdateUser();
}

int esat::main(int argc, char **argv) {
	esat::WindowInit(width, height);
	esat::WindowSetMouseVisibility(true);
	
	srand(time(NULL));
	
	Init();
	InitializePlayer(&players[0]);
	InitializePlayer(&players[1]);
	LoadLevel();
	
	while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
		esat::DrawBegin();
		esat::DrawClear(0,0,0);
		esat::DrawSetStrokeColor(255,255,255);
		
		SceneManager();
		
		esat::DrawEnd();
		esat::WindowFrame();
	}
 
	Exit();
 
	esat::WindowDestroy();
	return 0;
}