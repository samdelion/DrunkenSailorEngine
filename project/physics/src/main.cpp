#include <iostream>

#include "engine/Engine.h"
#include "engine/system/Console.h"
#include "engine/system/input/Input.h"
#include "engine/system/physics_world/PhysicsWorld.h"
#include "engine/system/platform/Platform.h"
#include "engine/system/render/Render.h"
#include "engine/system/script/Script.h"

int main(int argc, char **argv)
{
    ds::Engine engine;

    // Create script system
    ds::Script *scriptSystem = new ds::Script();

    // Create other systems
    ds::ISystem *inputSystem = new ds::Input(); 
    ds::ISystem *consoleSystem = new ds::Console(); 
    ds::ISystem *renderSystem = new ds::Render();
    ds::ISystem *physicsWorldSystem = new ds::PhysicsWorld();
    // Register script bindings of other systems
    scriptSystem->RegisterScriptBindings("Input", inputSystem);
    scriptSystem->RegisterScriptBindings("PhysicsWorld", physicsWorldSystem);

    // Add all systems to engine
    engine.AddSystem(std::unique_ptr<ds::ISystem>(inputSystem));
    engine.AddSystem(std::unique_ptr<ds::ISystem>(consoleSystem));
    engine.AddSystem(std::unique_ptr<ds::ISystem>(renderSystem));
    engine.AddSystem(std::unique_ptr<ds::ISystem>(physicsWorldSystem));
    engine.AddSystem(std::unique_ptr<ds::ISystem>(scriptSystem));

    engine.Start();

    return 0;
}
