#include <vector>
#include "raylib.h"
#include "world.h"
#include "pubsub.h"
#include "entity_view.h"
#include "player_entity.h"

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

    PubSub::subscribe("entity", this);
    PubSub::subscribe("player", this);

    InitWindow(screenWidth, screenHeight, "Cat Moving");
    SetTargetFPS(60);

    // Load the level

    float x = 320;
    float y = 280;
    world.addPlayer(x, y, 32, 32, Cat);

    x = 160;
    world.addEntity(x, y, 32, 32, Obstacle);
    y += 32;
    world.addEntity(x, y, 32, 32, Obstacle);

    y = 352;
    for (x=160; x <= 640; x += 32)
        world.addEntity(x, y, 32, 32, Obstacle);

    world.addEntity(x-64, y-32, 32, 32, Bounce);



    x = 160;
    y = 352 - 32 * 4;
    for (x = 160; x <= 640; x += 32)
        world.addEntity(x, y, 32, 32, Obstacle);


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
    }
}
