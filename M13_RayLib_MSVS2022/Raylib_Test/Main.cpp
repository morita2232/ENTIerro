#include <iostream>
#include "fstream"
#include <string>
#include <map>
#include <array>
#include "raylib.h"


using namespace std;

float currVers = 0.5;
float minVers = 0.3;

float versFile = -1.0;

string title;

int minHeight = 2;
int fileHeight = -1;
int minWidth = 2;
int fileWidth = -1;

int minTextures = 1;
int fileTextures = -1;

map<char, string> textureFiles;

map<char, Model> models;



char** lista;
char** level_s;
char** collisions;
char** objects;

char textChar;
string textName;

float cubeSize = 1.0f;

Vector3 playerPosition = { 0.0f, 1.0f, 0.0f };


//------------------------------------------------------------------------------------
// Program main entry point
//------------------------------------------------------------------------------------
int entierro(void)
{

    // Initialization
    //--------------------------------------------------------------------------------------
    const int screenWidth = 800;
    const int screenHeight = 450;

    InitWindow(screenWidth, screenHeight, "raylib [core] example - 3d camera mode");



    // Define the camera to look into our 3d world
    Camera3D camera = { 0 };
    camera.position = Vector3{ 0.0f, 10.0f, 10.0f };  // Camera position
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };      // Camera looking at point
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };          // Camera up vector (rotation towards target)
    camera.fovy = 45.0f;                                // Camera field-of-view Y
    camera.projection = CAMERA_PERSPECTIVE;             // Camera mode type

    float cubeOffsetX = 0.0f;
    float cubeOffsetZ = 0.0f;


    if (fileWidth % 2 == 0) {

        cubeOffsetX = 0.5;

    }

    if (fileHeight % 2 == 0) {

        cubeOffsetZ = 0.5;

    }
    float cubeInX = -(fileWidth / 2) + cubeOffsetX;
    float cubeInZ = -(fileHeight / 2) + cubeOffsetZ;

    Vector3 cubePosition = { cubeInX, 0.0f, cubeInZ };

    for (map<char, string>::iterator it = textureFiles.begin(); it != textureFiles.end(); ++it) {
        Texture2D texture = LoadTexture(it->second.c_str());

        Mesh mesh = GenMeshCube(cubeSize, cubeSize, cubeSize);
        Model model = LoadModelFromMesh(mesh);
        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;
        models.insert({ it->first, model });
    }
	float playerSize = 0.375f;

    Mesh playerMesh = GenMeshSphere(playerSize, 16, 16);
	Model playerModel = LoadModelFromMesh(playerMesh);

    Texture2D playerTexture = LoadTexture("player.png");

    playerModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = playerTexture;

    SetTargetFPS(60);               // Set our game to run at 60 frames-per-second
    //--------------------------------------------------------------------------------------

    // Main game loop
    while (!WindowShouldClose())    // Detect window close button or ESC key
    {
        // Update
        //----------------------------------------------------------------------------------
        // TODO: Update your variables here
        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();

        ClearBackground(BLACK);

        BeginMode3D(camera);

        cubePosition = { cubeInX, 0.0f, cubeInZ };

        for (int h = 0; h < fileHeight; h++) {

            for (int w = 0; w < fileWidth; w++) {

                if (lista[h][w] != '0') {
                    DrawModel(models[lista[h][w]], cubePosition, cubeSize, WHITE);
                }
                cubePosition.x += cubeSize;
            }
            cubePosition.x = cubeInX;
            cubePosition.z += cubeSize;
        }

        cubePosition = { cubeInX, 0.0f, cubeInZ };

        for (int h = 0; h < fileHeight; h++) {

            for (int w = 0; w < fileWidth; w++) {

                if (level_s[h][w] != '0') {
                    DrawModel(models[level_s[h][w]], cubePosition = { cubePosition.x, 1.0f , cubePosition.z }, cubeSize, WHITE);
                }
                cubePosition.x += cubeSize;
            }
            cubePosition.x = cubeInX;
            cubePosition.z += cubeSize;
        }

        cubePosition = { cubeInX, 0.0f, cubeInZ };

        for (int h = 0; h < fileHeight; h++) {

            for (int w = 0; w < fileWidth; w++) {

                if (objects[h][w] == '@') {
                    DrawModel(playerModel, playerPosition, cubeSize, WHITE);

                }

                if (objects[h][w] != '0') {
                    DrawModel(models[objects[h][w]], cubePosition = { cubePosition.x, 1.0f , cubePosition.z }, cubeSize, WHITE);
                }
                cubePosition.x += cubeSize;
            }
            cubePosition.x = cubeInX;
            cubePosition.z += cubeSize;
        }

        switch (GetKeyPressed())
        {
        case KEY_W:
            
            DrawModel(playerModel, playerPosition = {playerPosition.x, 1.0f, playerPosition.z - 1.0f}, cubeSize, WHITE);
            playerPosition = playerPosition;
			break;

        case KEY_A:
			DrawModel(playerModel, playerPosition = { playerPosition.x - 1.0f, 1.0f, playerPosition.z }, cubeSize, WHITE);

			playerPosition = playerPosition;
			break;

		case KEY_S:
			DrawModel(playerModel, playerPosition = { playerPosition.x, 1.0f, playerPosition.z + 1.0f }, cubeSize, WHITE);
			playerPosition = playerPosition;
			break;

		case KEY_D:
			DrawModel(playerModel, playerPosition = { playerPosition.x + 1.0f, 1.0f, playerPosition.z }, cubeSize, WHITE);
			playerPosition = playerPosition;
			break;
        }


        DrawGrid(8, 1.0f);

        EndMode3D();

        DrawText("Welcome to the third dimension!", 10, 40, 20, DARKGRAY);

        DrawFPS(10, 10);

        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    CloseWindow();        // Close window and OpenGL context
    //--------------------------------------------------------------------------------------

    return 0;
}


int main()
{
    ifstream levelFile("first_level.erro");

    string tmp;

    if (levelFile.is_open()) {

        //HEADER

        getline(levelFile, tmp, ';');

        if (tmp != "ERRO") {
            cout << "ERROR 2: El archivo no es de formato 'Entierro'" << endl;
            return 2;
        }

        //VERSION

        getline(levelFile, tmp, ';');

        versFile = stof(tmp);

        if (versFile > currVers || versFile < minVers) {
            cout << "ERROR 3: No es la version correcta" << endl;
            return 3;
        }

        //PASAR A LA SIGUIENTE LINEA

        getline(levelFile, tmp, '\n');

        //CABECERA TITULO

        getline(levelFile, tmp, ';');

        if (tmp != "TITLE") {
            cout << "ERROR 4: El titulo esta mal configurado" << endl;
            return 4;
        }

        //NOMBRE DEL NIVEL

        getline(levelFile, tmp, ';');

        if (tmp == "") {
            cout << "ERROR 5: No hay titulo de nivel" << endl;
            return 5;
        }

        title = tmp;

        //PASAR A LA SIGUIENTE LINEA

        getline(levelFile, tmp, '\n');

        //SIZE

        getline(levelFile, tmp, ';');

        if (tmp != "SIZE") {
            cout << "ERROR 6: El size esta mal configurado" << endl;
            return 6;
        }

        //WIDTH Y HEIGHT

        getline(levelFile, tmp, ';');

        fileWidth = stoi(tmp);

        if (fileWidth < minWidth) {
            cout << "ERROR 7.1: El width es menor que el minimo." << endl;
            return 7;
        }

        getline(levelFile, tmp, ';');

        fileHeight = stoi(tmp);

        if (fileHeight < minHeight) {
            cout << "ERROR 7.2: El height es menor que el minimo." << endl;
            return 7;
        }

        //PASAR A LA SIGUIENTE LINEA

        getline(levelFile, tmp, '\n');

        //TEXTURES

        getline(levelFile, tmp, ';');

        if (tmp != "TEXTURES") {
            cout << "ERROR 8.1: Las textureFiles estan mal encabezadas" << endl;
            return 8;
        }

        //CANTIDAD DE textureFiles
        getline(levelFile, tmp, ';');

        fileTextures = stoi(tmp);

        if (fileTextures < minTextures) {
            cout << "ERROR 8.2: No hay suficientes textureFiles." << endl;
            return 8;
        }

        //PASAR A LA SIGUIENTE LINEA

        getline(levelFile, tmp, '\n');

        //textureFiles

        for (int i = 0; i < fileTextures; i++) {

            getline(levelFile, tmp, ';');

            if (tmp == "") {
                cout << "ERROR 9.1: Caracter de textura erronea." << endl;
                return 9;
            }
            textChar = tmp[0];

            getline(levelFile, tmp, ';');

            if (tmp == "") {
                cout << "ERROR 9.2: Nombre de textura erronea." << endl;
                return 9;
            }

            textName = tmp;

            //cout << textChar << " -> " << textName << endl;

            //textureFiles.insert(make_pair(textChar, textName));
            textureFiles.insert({ textChar, textName });

            //PASAR A LA SIGUIENTE LINEA

            getline(levelFile, tmp, '\n');

        }

        //FLOOR

        getline(levelFile, tmp, ';');

        if (tmp != "FLOOR") {
            cout << "ERROR 10: El suelo esta mal encabezado" << endl;
            return 10;
        }

        //PASAR A LA SIGUIENTE LINEA

        getline(levelFile, tmp, '\n');

        //CREACION DE ARRAY PARA GUARDAR MAPAS
        //** signicia que habra una cantidad de * a una cantida que no sabemos de caracteres
        lista = new char* [fileHeight];

        for (int h = 0; h < fileHeight; h++) {

            lista[h] = new char[fileWidth];

        }

        //METER MAPA FLOOR EN EL ARRAY

        for (int h = 0; h < fileHeight; h++) {

            for (int w = 0; w < fileWidth; w++) {

                getline(levelFile, tmp, ';');

                lista[h][w] = tmp[0];

                if (tmp == "") {
                    cout << "ERROR 11: Caracter de textura erronea." << endl;
                    return 11;
                }

                // cout << lista[h][w];
            }
            // cout << endl;
            getline(levelFile, tmp, '\n');
        }


        //STAGE

        getline(levelFile, tmp, ';');

        if (tmp != "STAGE") {
            cout << "ERROR 12: El stage esta mal encabezado" << endl;
            return 12;
        }

        getline(levelFile, tmp, '\n');

        level_s = new char* [fileHeight];

        for (int h = 0; h < fileHeight; h++) {

            level_s[h] = new char[fileWidth];

        }

        //METER MAPA STAGE EN EL ARRAY

        for (int h = 0; h < fileHeight; h++) {

            for (int w = 0; w < fileWidth; w++) {

                getline(levelFile, tmp, ';');

                level_s[h][w] = tmp[0];

                if (tmp == "") {
                    cout << "ERROR 13: Caracter de textura erronea." << endl;
                    return 13;
                }

                // cout << lista[h][w];
            }
            //cout << endl;
            getline(levelFile, tmp, '\n');
        }

        //COLLISIONS

        getline(levelFile, tmp, ';');

        if (tmp != "COLLISIONS") {
            cout << "ERROR 14: Los collisions estan mal encabezados" << endl;
            return 14;
        }

        getline(levelFile, tmp, '\n');

        collisions = new char* [fileHeight];

        for (int h = 0; h < fileHeight; h++) {

            collisions[h] = new char[fileWidth];

        }

        //METER MAPA COLLISIONS EN EL ARRAY

        for (int h = 0; h < fileHeight; h++) {

            for (int w = 0; w < fileWidth; w++) {

                getline(levelFile, tmp, ';');

                collisions[h][w] = tmp[0];

                if (tmp == "") {
                    cout << "ERROR 15: Caracter de colision erroneo." << endl;
                    return 15;
                }

                // cout << lista[h][w];
            }
            //cout << endl;
            getline(levelFile, tmp, '\n');
        }

        //OBJECTS

        getline(levelFile, tmp, ';');

        if (tmp != "OBJECTS") {
            cout << "ERROR 16: Los objects estan mal encabezados" << endl;
            return 16;
        }

        getline(levelFile, tmp, '\n');

        objects = new char* [fileHeight];

        for (int h = 0; h < fileHeight; h++) {

            objects[h] = new char[fileWidth];

        }

        //METER MAPA OBJECTS EN EL ARRAY

        for (int h = 0; h < fileHeight; h++) {

            for (int w = 0; w < fileWidth; w++) {

                getline(levelFile, tmp, ';');

                objects[h][w] = tmp[0];

                if (tmp == "") {
                    cout << "ERROR 17: Caracter de object erroneo." << endl;
                    return 17;
                }

                //cout << lista[h][w];
            }
            //cout << endl;
            getline(levelFile, tmp, '\n');
        }

        float cubeOffsetX = (fileWidth % 2 == 0) ? 0.5f : 0.0f;
        float cubeOffsetZ = (fileHeight % 2 == 0) ? 0.5f : 0.0f;

        float cubeInX = -(fileWidth / 2.0f) + cubeOffsetX;
        float cubeInZ = -(fileHeight / 2.0f) + cubeOffsetZ;

        for (int h = 0; h < fileHeight; h++) {
            for (int w = 0; w < fileWidth; w++) {
                if (objects[h][w] == '@') {

                    float playerX = cubeInX + w * cubeSize;
                    float playerZ = cubeInZ + h * cubeSize;


                    playerPosition = { playerX, 1.0f, playerZ };
                }
            }
        }

        levelFile.close();

        entierro();

        return 0;

    }
    else {
        cout << "ERROR 1: El archivo no existe" << endl;

        return 1;
    }

}