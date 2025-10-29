/*******************************************************************************************
*
*   Example licensed under an unmodified zlib/libpng license, which is an OSI-certified,
*   BSD-like license that allows static linking with closed source software
*
*   Copyright (c) 2014-2025 Ramon Santamaria (@raysan5)
*
********************************************************************************************/

#include "raylib.h"

#include "fstream"
#include <iostream>
#include <string>
#include <array>
#include <map>

using namespace std;

string tmp;
string title;

float version_cur = 0.5;
float version_min = 0.3;
float version_file = -1.0;

int level_w_min = 2;
int level_h_min = 2;
int level_w;
int level_h;

float cubeIniX = 0.0;
float cubeIniZ = 0.0;

int textures_min = 1;
int textures_num;

map <char, string> textures_file;
map <char, Model> models;

char** level_floor;
char** level_stage;
char** level_collisions;
char** level_objects;

float cubeSize = 1.0f;
Vector3 playerPosition = { 0.0f, 1.0f, 0.0f };
const float playerRadius = 0.375f;
bool useTexturedPlayer = false;
Model playerModel;

int coffinCounter;



bool check_collision(float x, float z, bool isPlayer) {

    int pos_x = x - cubeIniX;
    int pos_y = z - cubeIniZ;
    if (isPlayer) {
    if (level_collisions[pos_y][pos_x] != '0') {

        return true;
    }
    else {

    return false;
    }
    }
    else {
        if (level_collisions[pos_y][pos_x] == 'S') {
            return true;
        }
        else {
            return false;
        }
    }

};

bool check_coffin(float x, float z) {

    int pos_x = x - cubeIniX;
    int pos_z = z - cubeIniZ;

    if (level_objects[pos_z][pos_x] == 'C')
        return true;

    return false;

}

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
 

    // Camara 3D
    Camera3D camera = { 0 };
    camera.position = Vector3{ 0.0f, 10.0f, 10.0f };
    camera.target = Vector3{ 0.0f, 0.0f, 0.0f };
    camera.up = Vector3{ 0.0f, 1.0f, 0.0f };
    camera.fovy = 45.0f;
    camera.projection = CAMERA_PERSPECTIVE;

    float cubeOffsetX = 0.0f;
    if (level_w % 2 == 0) cubeOffsetX = 0.5f;

    float cubeOffsetZ = 0.0f;
    if (level_h % 2 == 0) cubeOffsetZ = 0.5f;

    cubeIniX = -(level_w / 2.0f) + cubeOffsetX;
    cubeIniZ = -(level_h / 2.0f) + cubeOffsetZ;

    for (auto it = textures_file.begin(); it != textures_file.end(); ++it)
    {
        Texture2D texture = LoadTexture(it->second.c_str());

        Mesh mesh = GenMeshCube(cubeSize, cubeSize, cubeSize);
        Model model = LoadModelFromMesh(mesh);

        model.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = texture;

        models.insert({ it->first, model });
    }

    if (FileExists("player.png"))
    {
        Mesh pm = GenMeshSphere(playerRadius, 16, 16);
        playerModel = LoadModelFromMesh(pm);
        Texture2D ptex = LoadTexture("player.png");
        playerModel.materials[0].maps[MATERIAL_MAP_DIFFUSE].texture = ptex;
        useTexturedPlayer = true;
    }

    SetTargetFPS(60);
    float timer;
    //--------------------------------------------------------------------------------------
    // Main game loop
    while (!WindowShouldClose())
    {
        if (coffinCounter != 0) {
        timer = GetTime();

        }
        // Update: movimiento por pasos (WASD y cursores)
        //----------------------------------------------------------------------------------
        float temp_x = playerPosition.x;
        float temp_z = playerPosition.z;
        char player_direction = 'X';

        if (IsKeyPressed(KEY_W) || IsKeyPressed(KEY_UP)) {
            temp_z -= 1.0f;

            player_direction = 'U';

        }

        if (IsKeyPressed(KEY_S) || IsKeyPressed(KEY_DOWN)) {
            temp_z += 1.0f;
            player_direction = 'D';

        }

        if (IsKeyPressed(KEY_A) || IsKeyPressed(KEY_LEFT)) {
            temp_x -= 1.0f;
            player_direction = 'L';

        }

        if (IsKeyPressed(KEY_D) || IsKeyPressed(KEY_RIGHT)) {
            temp_x += 1.0f;
            player_direction = 'R';

        }

        if (!check_collision(temp_x, temp_z, true)) {
            if (check_coffin(temp_x, temp_z)) {
                float coffin_in_x = temp_x;
                float coffin_in_z = temp_z;

                switch (player_direction) {

                case 'U':
                    coffin_in_z = temp_z - 1.0;
                    break;

                case 'D':
                    coffin_in_z = temp_z + 1.0;
                    break;

                case 'L':
                    coffin_in_x = temp_x - 1.0;
                    break;

                case 'R':
                    coffin_in_x = temp_x + 1.0;
                    break;
                }

                if (!check_collision(coffin_in_x, coffin_in_z, false)) {
                    
                    int pos_x = temp_x - cubeIniX;
                    int pos_z = temp_z - cubeIniZ;
                    
                    level_objects[pos_z][pos_x] = '0';

                    pos_x = coffin_in_x - cubeIniX;
                    pos_z = coffin_in_z - cubeIniZ;
                    
                    level_objects[pos_z][pos_x] = 'C';

                    playerPosition.x = temp_x;
                    playerPosition.z = temp_z;
                }
                else {
                    int pos_x = temp_x - cubeIniX;
                    int pos_z = temp_z - cubeIniZ;

                    level_objects[pos_z][pos_x] = '0';

                    pos_x = coffin_in_x - cubeIniX;
                    pos_z = coffin_in_z - cubeIniZ;

                    level_floor[pos_z][pos_x] = 'C';
                    level_collisions[pos_z][pos_x] = '0';
                    playerPosition.x = temp_x;
                    playerPosition.z = temp_z;

                    coffinCounter -= 1;

                }

            
            }
            else {
            playerPosition.x = temp_x;
            playerPosition.z = temp_z;

            }

        }

        //----------------------------------------------------------------------------------

        // Draw
        //----------------------------------------------------------------------------------
        BeginDrawing();
        ClearBackground(BLACK);
        BeginMode3D(camera);

        // 1) FLOOR
        {
            Vector3 pos = { cubeIniX, 0.0f, cubeIniZ };
            for (int h = 0; h < level_h; h++)
            {
                for (int w = 0; w < level_w; w++)
                {
                    char id = level_floor[h][w];
                    if (id != '0')
                    {
                        DrawModel(models[id], pos, 1.0f, WHITE);
                    }
                    pos.x += cubeSize;
                }
                pos.x = cubeIniX;
                pos.z += cubeSize;
            }
        }

        // 2) STAGE
        {
            Vector3 pos = { cubeIniX, 1.0f, cubeIniZ };
            for (int h = 0; h < level_h; h++)
            {
                for (int w = 0; w < level_w; w++)
                {
                    char id = level_stage[h][w];
                    if (id != '0')
                    {
                        DrawModel(models[id], pos, 1.0f, WHITE);
                    }
                    pos.x += cubeSize;
                }
                pos.x = cubeIniX;
                pos.z += cubeSize;
            }
        }

        // 3) OBJECTS
        {
            Vector3 pos = { cubeIniX, 1.0f, cubeIniZ };
            for (int h = 0; h < level_h; h++)
            {
                for (int w = 0; w < level_w; w++)
                {
                    char id = level_objects[h][w];

                    if (id != '0' && id != '@')
                    {
                        DrawModel(models[id], pos, 1.0f, WHITE);
                    }
                    pos.x += cubeSize;
                }
                pos.x = cubeIniX;
                pos.z += cubeSize;
            }

            if (useTexturedPlayer)
            {
                DrawModel(playerModel, playerPosition, 1.0f, WHITE);
            }
            else
            {
                DrawSphere(playerPosition, playerRadius, RED);
            }
        }


        DrawGrid(8, 1.0f);
        EndMode3D();
        if (coffinCounter == 0) {

        DrawText("LAS ENTERRASTE BIEN", 10, 40, 20, DARKGRAY);
        DrawText("HAS TARDADO ->", 10, 60, 20, DARKGRAY);
        DrawText(TextFormat("%.2f", timer), 10, 80, 20, DARKGRAY);

        }
        else {
        DrawText(TextFormat("%.2f", timer), 10, 60, 20, DARKGRAY);
        DrawText("ENTIERRALAS", 10, 40, 20, DARKGRAY);

        }
        DrawFPS(10, 10);
        EndDrawing();
        //----------------------------------------------------------------------------------
    }

    // De-Initialization
    //--------------------------------------------------------------------------------------
    // Unload de modelos cargados
    for (auto& kv : models) UnloadModel(kv.second);
    if (useTexturedPlayer) UnloadModel(playerModel);

    CloseWindow();
    //--------------------------------------------------------------------------------------

    return 0;
}

int main()
{
    //------0. FILE------
    ifstream level_file("first_level.erro");

    if (!level_file.is_open()) {
        cout << "ERROR 1: FILE DOES NOT EXIST." << endl;
        return 1;
    }

    //------1. HEADER------
    getline(level_file, tmp, ';');

    if (tmp != "ERRO") {
        cout << "ERROR 2: FILE DOES NOT MATCH FORMAT." << endl;
        return 2;
    }

    //------1. VERSION------
    getline(level_file, tmp, ';');

    version_file = stof(tmp);

    if (version_file > version_cur || version_file < version_min) {
        cout << "ERROR 3: FILE DOES NOT MATCH MIN. VERSION." << endl;
        return 3;
    }

    //------1.2. READ TILL "ENDL"------
    getline(level_file, tmp, '\n');

    //------2. TITLE------
    getline(level_file, tmp, ';');

    if (tmp != "TITLE") {
        cout << "ERROR 4: FILE DOES NOT HAVE TITLE." << endl;
        return 4;
    }

    //------2. TITLE NAME------
    getline(level_file, tmp, ';');

    if (tmp == "") {
        cout << "ERROR 5: FILE DOES NOT HAVE A TITLE NAME." << endl;
        return 5;
    }
    else {
        title = tmp;
    }

    //------2.2. READ TILL "ENDL"------
    getline(level_file, tmp, '\n');

    //------3. SIZE------
    getline(level_file, tmp, ';');

    if (tmp != "SIZE") {
        cout << "ERROR 6: FILE DOES NOT HAVE A SIZE." << endl;
        return 6;
    }

    //------3. WIDTH------
    getline(level_file, tmp, ';');

    level_w = stoi(tmp);

    if (level_w < level_w_min) {
        cout << "ERROR 7.1: FILE DOES NOT HAVE MIN. SIZE." << endl;
        return 7;
    }

    //------3. HEIGTH------
    getline(level_file, tmp, ';');

    level_h = stoi(tmp);

    if (level_h < level_h_min) {
        cout << "ERROR 7.2: FILE DOES NOT HAVE MIN. SIZE." << endl;
        return 7;
    }

    //------3.2. READ TILL "ENDL"------
    getline(level_file, tmp, '\n');

    //------4. TEXTURES------
    getline(level_file, tmp, ';');

    if (tmp != "TEXTURES") {
        cout << "ERROR 8.1: FILE DOES NOT HAVE TEXTURES." << endl;
        return 8;
    }

    //------4. TEXTURES NUM.------
    getline(level_file, tmp, ';');

    textures_num = stoi(tmp);

    if (textures_num < textures_min) {
        cout << "ERROR 8.2: FILE DOES NOT HAVE MIN. ONE TEXTURES." << endl;
        return 8;
    }

    //------4.2. READ TILL "ENDL"------
    getline(level_file, tmp, '\n');

    //------5. TEXTURES READING------
    for (int i = 0; i < textures_num; i++) {
        getline(level_file, tmp, ';');
        if (tmp == "") {
            cout << "ERROR 9.1: FILE DOES NOT HAVE CORRECT CHAR. TEXTURES." << endl;
            return 9;
        }
        char character = tmp[0];

        getline(level_file, tmp, ';');
        if (tmp == "") {
            cout << "ERROR 9.2: FILE DOES NOT HAVE CORRECT NAME TEXTURES." << endl;
            return 9;
        }

        string texture_name = tmp;

        textures_file.insert(make_pair(character, texture_name));

        getline(level_file, tmp, '\n');
    }

    //------6. FLOOR TITLE------
    getline(level_file, tmp, ';');
    if (tmp != "FLOOR") {
        cout << "ERROR 10.1: FILE DOES NOT HAVE FLOOR." << endl;
        return 10;
    }

    getline(level_file, tmp, '\n');

    //------6.1. FLOOR READING------
    level_floor = new char* [level_h];

    for (int h = 0; h < level_h; h++)
    {
        level_floor[h] = new char[level_w];
    }

    for (int h = 0; h < level_h; h++)
    {
        for (int w = 0; w < level_w; w++)
        {
            getline(level_file, tmp, ';');

            if (tmp == "")
            {
                cout << "ERROR 10.2: INVALID FLOOR DATA." << endl;
                return 10;
            }

            level_floor[h][w] = tmp[0];
        }
        getline(level_file, tmp, '\n');
    }

    //------7. STAGE TITLE------
    getline(level_file, tmp, ';');
    if (tmp != "STAGE") {
        cout << "ERROR 11.1: FILE DOES NOT HAVE STAGE." << endl;
        return 11;
    }

    getline(level_file, tmp, '\n');

    //------7.1. STAGE READING------
    level_stage = new char* [level_h];

    for (int h = 0; h < level_h; h++)
    {
        level_stage[h] = new char[level_w];
    }

    for (int h = 0; h < level_h; h++)
    {
        for (int w = 0; w < level_w; w++)
        {
            getline(level_file, tmp, ';');

            if (tmp == "")
            {
                cout << "ERROR 11.1: INVALID STAGE DATA." << endl;
                return 11;
            }

            level_stage[h][w] = tmp[0];
        }
        getline(level_file, tmp, '\n');
    }

    //------8. COLLISIONS TITLE------
    getline(level_file, tmp, ';');
    if (tmp != "COLLISIONS") {
        cout << "ERROR 12.1: FILE DOES NOT HAVE COLLISIONS." << endl;
        return 12;
    }

    getline(level_file, tmp, '\n');

    //------8.1. COLLISIONS READING------
    level_collisions = new char* [level_h];

    for (int h = 0; h < level_h; h++)
    {
        level_collisions[h] = new char[level_w];
    }

    for (int h = 0; h < level_h; h++)
    {
        for (int w = 0; w < level_w; w++)
        {
            getline(level_file, tmp, ';');

            if (tmp == "")
            {
                cout << "ERROR 12.1: INVALID COLLISIONS DATA." << endl;
                return 12;
            }
            if (tmp[0] == 'S') {
                coffinCounter++;
            }
            level_collisions[h][w] = tmp[0];
        }
        getline(level_file, tmp, '\n');
    }

    //------9. OBJECTS TITLE------
    getline(level_file, tmp, ';');
    if (tmp != "OBJECTS") {
        cout << "ERROR 13.1: FILE DOES NOT HAVE OBJECTS." << endl;
        return 13;
    }

    getline(level_file, tmp, '\n');

    //------9.1. OBJECTS READING------
    level_objects = new char* [level_h];

    for (int h = 0; h < level_h; h++)
    {
        level_objects[h] = new char[level_w];
    }

    for (int h = 0; h < level_h; h++)
    {
        for (int w = 0; w < level_w; w++)
        {
            getline(level_file, tmp, ';');

            if (tmp == "")
            {
                cout << "ERROR 13.1: INVALID OBJECTS DATA." << endl;
                return 13;
            }

            level_objects[h][w] = tmp[0];
        }
        getline(level_file, tmp, '\n');
    }

    //------END------
    level_file.close();

    {
        float cubeOffsetX = (level_w % 2 == 0) ? 0.5f : 0.0f;
        float cubeOffsetZ = (level_h % 2 == 0) ? 0.5f : 0.0f;

        float cubeIniX = -(level_w / 2.0f) + cubeOffsetX;
        float cubeIniZ = -(level_h / 2.0f) + cubeOffsetZ;

        for (int h = 0; h < level_h; ++h)
        {
            for (int w = 0; w < level_w; ++w)
            {
                if (level_objects[h][w] == '@')
                {
                    float px = cubeIniX + w * cubeSize;
                    float pz = cubeIniZ + h * cubeSize;
                    playerPosition = { px, 1.0f, pz };
                }
            }
        }
    }

    entierro();

    return 0;
}
