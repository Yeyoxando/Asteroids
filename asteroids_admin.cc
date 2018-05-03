#include <stdlib.h>
#include <iostream>
#include <string>
#include <stdio.h>
#include <esat/window.h>
#include <esat/draw.h>
#include <esat/input.h>
#include <esat/math.h>
#include <esat_extra/sqlite3.h>
#include "lib_diego/ld_graphic.h"
#include "lib_diego/ld_ui.h"

Box* menuBoxes=NULL;
sqlite3 *db;

int charCounter;
struct User{
	std::string username;
	std::string email;
	std::string password;
	std::string name;
	std::string surnames;
	std::string birthdate;
	std::string province;
	std::string country;
	std::string credits;
};

char* errMsg = (char*) calloc (80, sizeof(char));
User newUser;
bool activeListMenu = false;
bool activeNewUserMenu = false;
bool activeDeleteMenu = false;
bool activeModifyMenu = false;
bool activeModifyMenu2 = false;
bool activeMainMenu = false;

int scene = 0;

int callbacks = 0;
std::string userSearch;
bool foundedUser = false;
User* readUsers;

static int callback(void *NotUsed, int argc, char **argv, char **azColName) {
	callbacks++;
	readUsers = (User*) calloc (callbacks, sizeof(User));
   int i;
	 /*for(i = 0; i<argc; i++) {
      printf("% 17s    ", azColName[i]);
   }
   printf("\n");*/
   for(i = 0; i<argc; i++) {
      printf("% 12s    ", argv[i] ? argv[i] : "NULL");
   }
   printf("\n%d\n", callbacks);
   return 0;
	 
	 //Cambiar el user por un array de strings para poder controlarlo desde el bucle y luego meterselos con el for segundo
}

static int callbackSelect(void *NotUsed, int argc, char **argv, char **azColName) {
	callbacks++;
	readUsers = (User*) calloc (callbacks, sizeof(User));
   int i;
	 /*for(i = 0; i<argc; i++) {
      printf("% 17s    ", azColName[i]);
   }
   printf("\n");*/
   for(i = 0; i<argc; i++) {
      printf("% 12s    ", argv[i] ? argv[i] : "NULL");
   }
	 if(argv[0] == userSearch){ 
		foundedUser = true;
		errMsg = "User found";
	 }
   return 0;
	 
	 //Cambiar el user por un array de strings para poder controlarlo desde el bucle y luego meterselos con el for segundo
}

static int callbackModify(void *NotUsed, int argc, char **argv, char **azColName) {
	callbacks++;
	readUsers = (User*) calloc (callbacks, sizeof(User));
   int i;
	 /*for(i = 0; i<argc; i++) {
      printf("% 17s    ", azColName[i]);
   }
   printf("\n");*/
   for(i = 0; i<argc; i++) {
      printf("% 12s    ", argv[i] ? argv[i] : "NULL");
   }
	 
	 newUser.username = argv[0];
	 newUser.email = argv[1];
	 newUser.password = argv[2];
	 newUser.name = argv[3];
	 newUser.surnames = argv[4];
	 newUser.birthdate = argv[5];
	 newUser.province = argv[6];
	 newUser.country = argv[7];
	 newUser.credits = argv[8];
	 
   return 0;
	 
	 //Cambiar el user por un array de strings para poder controlarlo desde el bucle y luego meterselos con el for segundo
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
   callbacks=0;
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

void DeleteUserDB(){
   char *zErrMsg = 0;
   int rc;
   std::string sql;
	 rc = sqlite3_open("Asteroids_db.db", &db);

		 if( rc ) {
				fprintf(stderr, "Can't open database: %s\n", sqlite3_errmsg(db));
		 }
		 
		 sql = "DELETE FROM USERS WHERE Username = '" + userSearch + "';";
		 
		 char *sqlChar = new char[sql.length() + 1];
		 strcpy(sqlChar, sql.c_str());
	 
		 rc = sqlite3_exec(db, sqlChar, callback, 0, &zErrMsg);
		 
		 if( rc != SQLITE_OK ){
				fprintf(stderr, "SQL error: %s\n", zErrMsg);
				sqlite3_free(zErrMsg);
				errMsg = "SQL error";
		 } else {
				errMsg="user deleted";
		 }
		 sqlite3_close(db);
}

void SelectUserDB(){
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
		
		sql = "SELECT Username FROM USERS WHERE Username = '" + userSearch + "';";

		char *sqlChar = new char[sql.length() + 1];
		strcpy(sqlChar, sql.c_str());
					
		/* Execute SQL statement */
		rc = sqlite3_exec(db, sqlChar, callbackSelect, (void*)data, &zErrMsg);
		 
		if(!foundedUser){
			errMsg = "Can't find this user";
		}
		 
		if( rc != SQLITE_OK ) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		sqlite3_close(db);
}

void ModifyUserDB(){
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
		
		sql = "SELECT * FROM USERS WHERE Username = '" + userSearch + "';";

		char *sqlChar = new char[sql.length() + 1];
		strcpy(sqlChar, sql.c_str());
					
		/* Execute SQL statement */
		rc = sqlite3_exec(db, sqlChar, callbackModify, (void*)data, &zErrMsg);
		 
		if( rc != SQLITE_OK ) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		sqlite3_close(db);
}

void UpdateUserDB(){
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
		
		sql = "DELETE FROM USERS WHERE Username = '" + userSearch + "';";

		char *sqlChar = new char[sql.length() + 1];
		strcpy(sqlChar, sql.c_str());
				
		/* Execute SQL statement */
		rc = sqlite3_exec(db, sqlChar, callbackModify, (void*)data, &zErrMsg);
		
		sql = "INSERT INTO USERS (Username, Email, Password, Name, Surnames, Birthdate, Province, Country, Credits) "  \
					 "VALUES ('"+newUser.username+"', '"+newUser.email+"', '"+newUser.password+"', '"+newUser.name+"', '" \
					 +newUser.surnames+"', '"+newUser.birthdate+"', '"+newUser.province+"', '"+newUser.country+"', '"+newUser.credits+"'); ";
		 
		sqlChar = new char[sql.length() + 1];
		strcpy(sqlChar, sql.c_str());
				
		/* Execute SQL statement */
		rc = sqlite3_exec(db, sqlChar, callbackModify, (void*)data, &zErrMsg);
		
		if( rc != SQLITE_OK ) {
			fprintf(stderr, "SQL error: %s\n", zErrMsg);
			sqlite3_free(zErrMsg);
		}
		sqlite3_close(db);
}

void ReadDBInfo(){
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

  sql = "SELECT * from USERS";

  /* Execute SQL statement */
  rc = sqlite3_exec(db, sql, callback, (void*)data, &zErrMsg);
   
  if( rc != SQLITE_OK ) {
    fprintf(stderr, "SQL error: %s\n", zErrMsg);
    sqlite3_free(zErrMsg);
  }
  sqlite3_close(db);
}

void InitializeMainMenu(){
	esat::DrawSetStrokeColor(255, 255, 255);
	menuBoxes = (Box*) calloc(4, sizeof(Box));
	menuBoxes[0].CreateBox(200, 150, 400, 60, &menuBoxes[0].boxPoints);
	menuBoxes[1].CreateBox(200, 250, 400, 60, &menuBoxes[1].boxPoints);
	menuBoxes[2].CreateBox(200, 350, 400, 60, &menuBoxes[2].boxPoints);
	menuBoxes[3].CreateBox(200, 450, 400, 60, &menuBoxes[3].boxPoints);
}

void InitializeListMenu(){
	errMsg = "";
	menuBoxes = (Box*) calloc(1, sizeof(Box));
	menuBoxes[0].CreateBox(193, 577, 200, 40, &menuBoxes[0].boxPoints);
	ReadDBInfo();
}

void InitializeNewUserMenu(){
	errMsg = "";
	menuBoxes = (Box*) calloc(13, sizeof(Box));
	menuBoxes[0].CreateBox(220, 110, 400, 40, &menuBoxes[0].boxPoints);
	menuBoxes[1].CreateBox(220, 160, 400, 40, &menuBoxes[1].boxPoints);
	menuBoxes[2].CreateBox(220, 210, 400, 40, &menuBoxes[2].boxPoints);
	menuBoxes[3].CreateBox(220, 260, 400, 40, &menuBoxes[3].boxPoints);
	menuBoxes[4].CreateBox(220, 310, 400, 40, &menuBoxes[4].boxPoints);
	menuBoxes[5].CreateBox(220, 360, 400, 40, &menuBoxes[5].boxPoints);
	menuBoxes[6].CreateBox(220, 410, 400, 40, &menuBoxes[6].boxPoints);
	menuBoxes[7].CreateBox(220, 460, 400, 40, &menuBoxes[7].boxPoints);
	menuBoxes[8].CreateBox(220, 510, 400, 40, &menuBoxes[8].boxPoints);
	menuBoxes[9].CreateBox(193, 577, 200, 40, &menuBoxes[9].boxPoints);
	menuBoxes[10].CreateBox(405, 577, 200, 40, &menuBoxes[10].boxPoints);
	menuBoxes[11].CreateBox(640, 160, 40, 40, &menuBoxes[11].boxPoints);
	menuBoxes[12].CreateBox(640, 360, 40, 40, &menuBoxes[12].boxPoints);
}

void InitializeDeleteMenu(){
	errMsg = "";
	menuBoxes = (Box*) calloc(4, sizeof(Box));
	menuBoxes[0].CreateBox(300, 550, 200, 40, &menuBoxes[0].boxPoints);
	menuBoxes[1].CreateBox(200, 150, 400, 40, &menuBoxes[1].boxPoints);
	menuBoxes[2].CreateBox(300, 240, 200, 40, &menuBoxes[2].boxPoints);
	menuBoxes[3].CreateBox(300, 400, 200, 40, &menuBoxes[3].boxPoints);
}

void InitializeModifyMenu(){
	errMsg = "";
	menuBoxes = (Box*) calloc(4, sizeof(Box));
	menuBoxes[0].CreateBox(300, 550, 200, 40, &menuBoxes[0].boxPoints);
	menuBoxes[1].CreateBox(200, 150, 400, 40, &menuBoxes[1].boxPoints);
	menuBoxes[2].CreateBox(300, 240, 200, 40, &menuBoxes[2].boxPoints);
	menuBoxes[3].CreateBox(300, 400, 200, 40, &menuBoxes[3].boxPoints);
}

void InitializeModifyMenu2(){
	errMsg = "";
	menuBoxes = (Box*) calloc(13, sizeof(Box));
	menuBoxes[0].CreateBox(220, 110, 400, 40, &menuBoxes[0].boxPoints);
	menuBoxes[1].CreateBox(220, 160, 400, 40, &menuBoxes[1].boxPoints);
	menuBoxes[2].CreateBox(220, 210, 400, 40, &menuBoxes[2].boxPoints);
	menuBoxes[3].CreateBox(220, 260, 400, 40, &menuBoxes[3].boxPoints);
	menuBoxes[4].CreateBox(220, 310, 400, 40, &menuBoxes[4].boxPoints);
	menuBoxes[5].CreateBox(220, 360, 400, 40, &menuBoxes[5].boxPoints);
	menuBoxes[6].CreateBox(220, 410, 400, 40, &menuBoxes[6].boxPoints);
	menuBoxes[7].CreateBox(220, 460, 400, 40, &menuBoxes[7].boxPoints);
	menuBoxes[8].CreateBox(220, 510, 400, 40, &menuBoxes[8].boxPoints);
	menuBoxes[9].CreateBox(193, 577, 200, 40, &menuBoxes[9].boxPoints);
	menuBoxes[10].CreateBox(405, 577, 250, 40, &menuBoxes[10].boxPoints);
	menuBoxes[11].CreateBox(640, 160, 40, 40, &menuBoxes[11].boxPoints);
	menuBoxes[12].CreateBox(640, 360, 40, 40, &menuBoxes[12].boxPoints);
}

void MainMenu(){
	esat::DrawSetTextSize(80);
	esat::DrawSetFillColor(255, 255, 255);
	esat::DrawText(100, 100, "Asteroids admin");
	esat::DrawSetTextSize(50);
	
	//Users list button
	if(menuBoxes[0].isMouseOver() && esat::MouseButtonDown(0)){
		activeListMenu = true;
		scene = 1;
	}
	menuBoxes[0].DrawButton(menuBoxes[0].isMouseOver(), white);
	DrawTextPlus(275, 195, "Users list", black, white, menuBoxes[0]);
	
	//New user button
	if(menuBoxes[1].isMouseOver() && esat::MouseButtonDown(0)){
		activeNewUserMenu = true;
		scene = 2;
	}
	menuBoxes[1].DrawButton(menuBoxes[1].isMouseOver(), white);
	DrawTextPlus(300, 295, "New user", black, white, menuBoxes[1]);
	
	//Delete user button
	if(menuBoxes[2].isMouseOver() && esat::MouseButtonDown(0)){
		activeDeleteMenu = true;
		scene = 3;
	}
	menuBoxes[2].DrawButton(menuBoxes[2].isMouseOver(), white);
	DrawTextPlus(260, 395, "Delete user", black, white, menuBoxes[2]);
	
	//Modify user button
	if(menuBoxes[3].isMouseOver() && esat::MouseButtonDown(0)){
		activeModifyMenu = true;
		scene = 4;
	}
	menuBoxes[3].DrawButton(menuBoxes[3].isMouseOver(), white);
	DrawTextPlus(260, 495, "Modify user", black, white, menuBoxes[3]);
}

void Initialize(){
	esat::DrawSetTextFont("resources/fonts/Hyperspace Bold.otf");
	InitializeMainMenu();
	CreateDB();
}

void UsersList(){
	esat::DrawSetFillColor(255, 255, 255);
	esat::DrawSetTextSize(80);
	esat::DrawText(220, 80, "Users");
	esat::DrawSetTextSize(40);
	if(menuBoxes[0].isMouseOver() && esat::MouseButtonDown(0)){
		scene = 0;
		activeMainMenu = true;
	}
	menuBoxes[0].DrawButton(menuBoxes[0].isMouseOver(), white);
	DrawTextPlus(225, 610, "Return", black, white, menuBoxes[0]);
}

void DeselectTextFields(int textFields, Box *activeTextField){
	for(int i = 0; i < textFields; i++){
		menuBoxes[i].selected = false;
	}
	(*activeTextField).selected = true;
}

void NewUserMenu(){
	esat::DrawSetFillColor(255, 255, 255);
	esat::DrawSetTextSize(80);
	esat::DrawText(220, 80, "New user");
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
	esat::DrawText(50, 675, errMsg);
	
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
	
	TextField(&menuBoxes[8], &newUser.credits);
	if(menuBoxes[8].isMouseOver() && esat::MouseButtonDown(0)) DeselectTextFields(9, &menuBoxes[8]);

	if(menuBoxes[9].isMouseOver() && esat::MouseButtonDown(0)){
		scene = 0;
		activeMainMenu = true;
	}
	menuBoxes[9].DrawButton(menuBoxes[9].isMouseOver(), white);
	DrawTextPlus(225, 610, "Return", black, white, menuBoxes[9]);
	
	if(menuBoxes[10].isMouseOver() && esat::MouseButtonDown(0)){
		AddUserToDB();
	}
	menuBoxes[10].DrawButton(menuBoxes[10].isMouseOver(), white);
	DrawTextPlus(425, 610, "Add user", black, white, menuBoxes[10]);
	
	if(menuBoxes[11].isMouseOver() && esat::MouseButtonDown(0)){
		newUser.email += '@';
	}
	menuBoxes[11].DrawButton(menuBoxes[11].isMouseOver(), white);
	DrawTextPlus(651, 195, "@", black, white, menuBoxes[11]);
	
	if(menuBoxes[12].isMouseOver() && esat::MouseButtonDown(0)){
		newUser.birthdate += '/';
	}
	menuBoxes[12].DrawButton(menuBoxes[12].isMouseOver(), white);
	DrawTextPlus(653, 395, "/", black, white, menuBoxes[12]);
}

void DeleteUserMenu(){
	esat::DrawSetFillColor(255, 255, 255);
	esat::DrawSetTextSize(80);
	esat::DrawText(180, 80, "Delete user");
	esat::DrawSetTextSize(40);
	esat::DrawText(100, 180, "User");
	esat::DrawText(200, 350, errMsg);
	
	TextField(&menuBoxes[1], &userSearch);
	if(menuBoxes[1].isMouseOver() && esat::MouseButtonDown(0)) DeselectTextFields(1, &menuBoxes[1]);
	
	if(menuBoxes[2].isMouseOver() && esat::MouseButtonDown(0)){
		foundedUser = false;
		SelectUserDB();
	}
	menuBoxes[2].DrawButton(menuBoxes[2].isMouseOver(), white);
	DrawTextPlus(342, 272, "Search", black, white, menuBoxes[2]);
	
	if(menuBoxes[0].isMouseOver() && esat::MouseButtonDown(0)){
		scene = 0;
		activeMainMenu = true;
	}
	menuBoxes[0].DrawButton(menuBoxes[0].isMouseOver(), white);
	DrawTextPlus(340, 583, "Return", black, white, menuBoxes[0]);
	
	if(foundedUser){
		if(menuBoxes[3].isMouseOver() && esat::MouseButtonDown(0)){
			DeleteUserDB();
		}
		menuBoxes[3].DrawButton(menuBoxes[3].isMouseOver(), white);
		DrawTextPlus(340, 433, "Delete", black, white, menuBoxes[3]);
	}
}

void ModifyUserMenu(){
	esat::DrawSetFillColor(255, 255, 255);
	esat::DrawSetTextSize(80);
	esat::DrawText(180, 80, "Modify user");
	esat::DrawSetTextSize(40);
	esat::DrawText(100, 180, "User");
	esat::DrawText(200, 350, errMsg);
	
	TextField(&menuBoxes[1], &userSearch);
	if(menuBoxes[1].isMouseOver() && esat::MouseButtonDown(0)) DeselectTextFields(1, &menuBoxes[1]);
	
	if(menuBoxes[2].isMouseOver() && esat::MouseButtonDown(0)){
		foundedUser = false;
		SelectUserDB();
	}
	menuBoxes[2].DrawButton(menuBoxes[2].isMouseOver(), white);
	DrawTextPlus(342, 272, "Search", black, white, menuBoxes[2]);
	
	if(menuBoxes[0].isMouseOver() && esat::MouseButtonDown(0)){
		scene = 0;
		activeMainMenu = true;
	}
	menuBoxes[0].DrawButton(menuBoxes[0].isMouseOver(), white);
	DrawTextPlus(340, 583, "Return", black, white, menuBoxes[0]);
	
	if(foundedUser){
		if(menuBoxes[3].isMouseOver() && esat::MouseButtonDown(0)){
			ModifyUserDB();
			scene = 5;
			activeModifyMenu2 = true;
		}
		menuBoxes[3].DrawButton(menuBoxes[3].isMouseOver(), white);
		DrawTextPlus(340, 433, "Modify", black, white, menuBoxes[3]);
	}
}

void ModifyUserMenu2(){
	esat::DrawSetFillColor(255, 255, 255);
	esat::DrawSetTextSize(80);
	esat::DrawText(180, 80, "Modify user");
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
	esat::DrawText(50, 675, errMsg);
	
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
	
	TextField(&menuBoxes[8], &newUser.credits);
	if(menuBoxes[8].isMouseOver() && esat::MouseButtonDown(0)) DeselectTextFields(9, &menuBoxes[8]);

	if(menuBoxes[9].isMouseOver() && esat::MouseButtonDown(0)){
		scene = 0;
		activeMainMenu = true;
	}
	menuBoxes[9].DrawButton(menuBoxes[9].isMouseOver(), white);
	DrawTextPlus(225, 610, "Return", black, white, menuBoxes[9]);
	
	if(menuBoxes[10].isMouseOver() && esat::MouseButtonDown(0)){
		UpdateUserDB();
	}
	menuBoxes[10].DrawButton(menuBoxes[10].isMouseOver(), white);
	DrawTextPlus(425, 610, "Modify user", black, white, menuBoxes[10]);
	
	if(menuBoxes[11].isMouseOver() && esat::MouseButtonDown(0)){
		newUser.email += '@';
	}
	menuBoxes[11].DrawButton(menuBoxes[11].isMouseOver(), white);
	DrawTextPlus(651, 195, "@", black, white, menuBoxes[11]);
	
	if(menuBoxes[12].isMouseOver() && esat::MouseButtonDown(0)){
		newUser.birthdate += '/';
	}
	menuBoxes[12].DrawButton(menuBoxes[12].isMouseOver(), white);
	DrawTextPlus(653, 395, "/", black, white, menuBoxes[12]);
}

void MenuController(){
	if(activeMainMenu) {
		InitializeMainMenu();
		activeMainMenu = false;
	}
	if(scene == 0) MainMenu();
	
	if(activeListMenu) {
		InitializeListMenu();
		activeListMenu = false;
	}
	if(scene == 1) UsersList();
	
	if(activeNewUserMenu) {
		InitializeNewUserMenu();
		activeNewUserMenu = false;
	}
	if(scene == 2) NewUserMenu();
	
	if(activeDeleteMenu) {
		InitializeDeleteMenu();
		activeDeleteMenu = false;
	}
	if(scene == 3) DeleteUserMenu();
	
	if(activeModifyMenu) {
		InitializeModifyMenu();
		activeModifyMenu = false;
	}
	if(scene == 4) ModifyUserMenu();
	
	if(activeModifyMenu2) {
		InitializeModifyMenu2();
		activeModifyMenu2 = false;
	}
	if(scene == 5) ModifyUserMenu2();
}

void Exit(){
	menuBoxes=NULL;
	free(menuBoxes);
}

int esat::main(int argc, char **argv) {
	esat::WindowInit(800, 700);
	esat::WindowSetMouseVisibility(true);
	
	Initialize();
 
	while(esat::WindowIsOpened() && !esat::IsSpecialKeyDown(esat::kSpecialKey_Escape)) {
		esat::DrawBegin();
		esat::DrawClear(0,0,0);
		
		MenuController();
		
		esat::DrawEnd();
		esat::WindowFrame();
	}
 
	esat::WindowDestroy();
	Exit();
	return 0;
} 