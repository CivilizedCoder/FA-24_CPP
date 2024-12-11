#include <vector>
#include "raylib.h"
#include "world.h"
#include "pubsub.h"
#include "entity_view.h"
#include "player_entity.h"
#include <iostream>

class Controller : public Subscriber
{
private:
    World world;
    vector<EntityView *> views;

public:
    void gameLoop();
    void receiveMessage(string channel, string message, void* data);
};

int main(void)
{
    Controller controller;
    controller.gameLoop();

    return 0;
}

int viewportWidth = 320;
int viewportHeight = 160;
int viewportX = 160;
int viewportY = 100;
int viewportDrawX = 0;
int viewportDrawY = 0;

void Controller::gameLoop()
{
    const int screenWidth = 500;
    const int screenHeight = 500;

    map<KeyboardKey, PlayerAction> keyMapping;

    keyMapping[KEY_W] = PlayerUp;
    keyMapping[KEY_A] = PlayerLeft;
    keyMapping[KEY_S] = PlayerDown;
    keyMapping[KEY_D] = PlayerRight;
    keyMapping[KEY_SPACE] = PlayerJump;
    keyMapping[KEY_R] = PlayerReset;

    PubSub::subscribe("entity", this);
    PubSub::subscribe("player", this);

    InitWindow(screenWidth, screenHeight, "Escape the Ship");
    SetTargetFPS(60);

    // First Hall.

    float x = 320;
    float y = 280;
    world.addPlayer(x, y, 32, 32, Cat);
    world.addEntity(x + 32, y + 32, 32, 32, Fuel);
    int tempY = 0;
    int tempX = 0;
    

    //leftWall
    x = 160;
    y = 352 - 64 - 32;
    world.addEntity(x, y, 32, 32, Obstacle);
    y += 32;
    world.addEntity(x, y, 32, 32, Obstacle);
    y += 32;
    world.addEntity(x, y, 32, 32, Obstacle);


    //starter platform
    y = 352;
    for (tempX = 0; tempX <= 32 * 18; tempX += 32)
        if(tempX != 32 * 18)
            world.addEntity(x + tempX, y, 32, 32, Obstacle);
 
    //wall of fire
    for (tempY = 32*50; tempY >= 32 * -18; tempY -= 32)
        world.addEntity(x + 32*17, y + tempY, 32, 32, Obstacle);  
    tempX = 0;
    for (tempY = 32*50; tempY >= 32 * -18; tempY -= 32)
        world.addEntity(x + 32 * 19, y + tempY, 32, 32, Obstacle);

    

    //tip
    world.addEntity(x + 32 * 17, y + tempY, 32, 32, Hurt);
    world.addEntity(x + 32 * 16, y + tempY, 32, 32, Hurt);

    world.addEntity(x + 32 * 19, y + tempY, 32, 32, Bounce);
    world.addEntity(x + 32 * 20, y + tempY, 32, 32, Bounce);

    tempY = 0;
    tempX = 0;

    //staircase left
    for (tempX = 0; tempX >= 32 * -10; tempX -= 32) {
        tempY += 32;
        //world.addEntity(x + tempX, y + tempY, 32, 32, Obstacle);
    }

    world.addEntity(x + 32 * 18, y + 32 * 55, 32, 32, Bounce);

    x += 32 * 19;
    y += 32 * 54;

    //staircase of hell
    for (int i = 0; i < 10; ++i) {
        world.addEntity(x + 32*i, y - 32*(i -1), 32, 32, Obstacle);
    }
    for (int i = 0; i < 10; ++i) {
        world.addEntity(x + 32 * i, y - 32 * i, 32, 32, Hurt);
    }

    for (int i = 12; i < 22; ++i) {
        world.addEntity(x + 32 * i, y - 32 * 2, 32, 32, Hurt);
    }

    for (int i = 12; i < 22; ++i) {
        world.addEntity(x + 32 * i, y - 32*3, 32, 32, Obstacle);
    }

    world.addEntity(x + 32 * 22, y - 32 *5, 32, 32, Fuel);

    for (int i = 0; i < 5; ++i) {
        world.addEntity(x + 32 * 22, y + 64 * i, 32, 32, Bounce);
    }

    for (int i = 22; i > 10; --i) {
        world.addEntity(x + 32 * i, y + 64 * 5, 32, 32, Bounce);
    }
    world.addEntity(x + 32 * 11, y + 64 * 7, 32, 32, Goal);
    std::cout << x + 32 * 11 << "            " << y + 64 * 7 << "\n" << endl;

 









    // Go into the main loop
    while (!WindowShouldClose())
    {
        // Handle user input
        for (auto& i : keyMapping)
        {
            if (i.first == KEY_SPACE)
            {
                // Only trigger SPACE action on initial press
                if (IsKeyPressed(i.first))
                {
                    PlayerAction action = i.second;
                    PubSub::publish("action", "player", &action);
                }
            }
            else
            {
                // Trigger other actions while the keys are held down
                if (IsKeyDown(i.first))
                {
                    PlayerAction action = i.second;
                    PubSub::publish("action", "player", &action);
                }
            }
        }

        // Give the world a chance to progress
        world.tick();

        BeginDrawing();

        ClearBackground(RAYWHITE);

        // Draw the views
        for (EntityView *view : views)
            view->draw(viewportX, viewportY, viewportDrawX, viewportDrawY, viewportWidth, viewportHeight);
        string data = "refresh";
        PubSub::publish("action", "player", &data);

        EndDrawing();
    }

    CloseWindow();
}

void Controller::receiveMessage(string channel, string message, void* data)
{
    // On the "entity" channel there can be "new" and "delete" messages.
    
    if (channel == "entity" && message == "new")
    {
        // Create a view for the entity
        EntityView* view = new EntityView((Entity *) data);
        views.push_back(view);
    }

    if (channel == "entity" && message == "delete")
    {
        // Try to find the view for the entity that's being deleted
        for (int i = 0; i < views.size(); i++)
        {
            if (views[i]->isViewFor((Entity*) data))
            {
                // Delete this view's memory
                delete views[i];
                // Delete it from the vector
                views.erase(views.begin() + i);
                // and stop looking, since we found the right view
                break;
            }
        }
    }

    if (channel == "player" && message == "location")
    {
        Vector2* position = (Vector2*)data;
        viewportX = position->x - viewportWidth / 2;
        viewportY = position->y - viewportHeight * 1.25;
        
    }
}
