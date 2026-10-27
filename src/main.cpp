#include "raylib.h"
#include "raymath.h"
#include <cmath>

struct Tank {
    Vector3 position = {0, 0.65f, 0};
    float heading = 0;
    float turretAngle = 0;
    float reload = 0;
};

int main() {
    InitWindow(1280, 720, "Operation Ironstorm");
    SetTargetFPS(60);

    Tank tank;
    float flash = 0;
    int shots = 0;

    Camera3D camera = {};
    camera.up = {0, 1, 0};
    camera.fovy = 65;
    camera.projection = CAMERA_PERSPECTIVE;

    while (!WindowShouldClose()) {
        float dt = fminf(GetFrameTime(), 0.05f);
        int w = GetScreenWidth();
        int h = GetScreenHeight();

        Rectangle forward = {25, h-195.0f, 90, 75};
        Rectangle backward = {25, h-105.0f, 90, 75};
        Rectangle left = {125, h-105.0f, 90, 75};
        Rectangle right = {225, h-105.0f, 90, 75};
        Rectangle fire = {w-130.0f, h-130.0f, 110, 100};
        Rectangle aimL = {w-260.0f, h-105.0f, 55, 65};
        Rectangle aimR = {w-195.0f, h-105.0f, 55, 65};

        bool go=false, back=false, turnL=false;
        bool turnR=false, shoot=false, turretL=false, turretR=false;

        for (int i=0; i<GetTouchPointCount(); i++) {
            Vector2 p = GetTouchPosition(i);
            go |= CheckCollisionPointRec(p,forward);
            back |= CheckCollisionPointRec(p,backward);
            turnL |= CheckCollisionPointRec(p,left);
            turnR |= CheckCollisionPointRec(p,right);
            shoot |= CheckCollisionPointRec(p,fire);
            turretL |= CheckCollisionPointRec(p,aimL);
            turretR |= CheckCollisionPointRec(p,aimR);
        }

        go |= IsKeyDown(KEY_W);
        back |= IsKeyDown(KEY_S);
        turnL |= IsKeyDown(KEY_A);
        turnR |= IsKeyDown(KEY_D);
        turretL |= IsKeyDown(KEY_Q);
        turretR |= IsKeyDown(KEY_E);
        shoot |= IsKeyDown(KEY_SPACE);

        if (turnL) tank.heading += 1.4f*dt;
        if (turnR) tank.heading -= 1.4f*dt;
        if (turretL) tank.turretAngle += 1.1f*dt;
        if (turretR) tank.turretAngle -= 1.1f*dt;

        float speed = go ? 9.0f : (back ? -5.0f : 0);
        tank.position.x += sinf(tank.heading)*speed*dt;
        tank.position.z += cosf(tank.heading)*speed*dt;

        tank.position.x = Clamp(tank.position.x,-90,90);
        tank.position.z = Clamp(tank.position.z,-90,90);

        tank.reload -= dt;
        flash -= dt;

        if (shoot && tank.reload <= 0) {
            tank.reload = 1.5f;
            flash = 0.15f;
            shots++;
        }

        camera.target = Vector3Add(tank.position,{0,1.5f,0});
        camera.position = {
            tank.position.x-sinf(tank.heading)*13,
            8,
            tank.position.z-cosf(tank.heading)*13
        };

        BeginDrawing();
        ClearBackground(SKYBLUE);
        BeginMode3D(camera);

        DrawPlane({0,0,0},{200,200},DARKGREEN);
        DrawGrid(40,5);

        // Tank hull
        DrawCube(tank.position,3.2f,1.3f,5.5f,OLIVE);
        DrawCubeWires(tank.position,3.2f,1.3f,5.5f,BLACK);

        // Tracks
        DrawCube(Vector3Add(tank.position,{-1.7f,-0.2f,0}),
                 0.6f,1.1f,5.7f,DARKGRAY);
        DrawCube(Vector3Add(tank.position,{1.7f,-0.2f,0}),
                 0.6f,1.1f,5.7f,DARKGRAY);

        // Turret and cannon
        Vector3 turret = Vector3Add(tank.position,{0,1.1f,0});
        DrawCylinder(turret,1.2f,1.2f,0.9f,12,ARMYGREEN);

        float angle = tank.heading+tank.turretAngle;
        Vector3 muzzle = {
            turret.x+sinf(angle)*4.0f,
            turret.y+0.3f,
            turret.z+cosf(angle)*4.0f
        };

        DrawCylinderEx(
            Vector3Add(turret,{0,0.3f,0}),
            muzzle,0.22f,0.18f,10,DARKGRAY
        );

        if (flash > 0)
            DrawSphere(muzzle,0.8f,YELLOW);

        EndMode3D();

        DrawText("OPERATION IRONSTORM",15,15,24,WHITE);
        DrawText(TextFormat("SHELLS FIRED: %d",shots),15,45,18,WHITE);

        DrawRectangleRec(forward,Fade(BLACK,0.5f));
        DrawRectangleRec(backward,Fade(BLACK,0.5f));
        DrawRectangleRec(left,Fade(BLACK,0.5f));
        DrawRectangleRec(right,Fade(BLACK,0.5f));
        DrawRectangleRec(fire,Fade(RED,0.65f));
        DrawRectangleRec(aimL,Fade(BLACK,0.5f));
        DrawRectangleRec(aimR,Fade(BLACK,0.5f));

        DrawText("UP",45,h-165,25,WHITE);
        DrawText("DN",45,h-80,25,WHITE);
        DrawText("L",155,h-80,25,WHITE);
        DrawText("R",255,h-80,25,WHITE);
        DrawText("FIRE",w-113,h-92,24,WHITE);
        DrawText("<",w-245,h-85,25,WHITE);
        DrawText(">",w-180,h-85,25,WHITE);

        EndDrawing();
    }

    CloseWindow();
    return 0;
}
