#include "engine/system/physics_world/PhysicsWorld.h"

namespace ds_lua
{
static int l_PhysicsMove(lua_State *L)
{
    // Get number of arguments provided
    int n = lua_gettop(L);
    int expected = 4;
    if (n != expected)
    {
        return luaL_error(L, "Got %d arguments, expected %d.", n, expected);
    }

    // Push physics system pointer onto stack
    lua_getglobal(L, "__PhysicsWorld");

    // If first item on stack isn't user data (out physics system)
    if (!lua_isuserdata(L, -1))
    {
        // Error
        luaL_argerror(L, 1, "lightuserdata");
    }
    else
    {
        ds::PhysicsWorld *physPtr = (ds::PhysicsWorld *)lua_touserdata(L, -1);
        assert(physPtr != NULL);

        ds::Entity *entity = NULL;
        ds_math::Vector3 *position = NULL;
        ds_math::Vector3 *velocity = NULL;
        ds_math::Vector3 *ellipsoidRadius = NULL;

        entity = (ds::Entity *)lua_touserdata(L, 1);
        position = (ds_math::Vector3 *)luaL_checkudata(L, 2, "Vector3");
        velocity = (ds_math::Vector3 *)luaL_checkudata(L, 3, "Vector3");
        ellipsoidRadius = (ds_math::Vector3 *)luaL_checkudata(L, 4, "Vector3");

        if (entity != NULL && position != NULL && velocity != NULL &&
            ellipsoidRadius != NULL)
        {
            physPtr->CollideAndSlide(position, velocity, *ellipsoidRadius);

            // Pop physics world ptr
            lua_pop(L, 1);
        }
    }

    // Entity, position, velocity and ellipsoid radius
    assert(lua_gettop(L) == 4);

    return 0;
}

ds::ScriptBindingSet LoadPhysicsWorldBindings()
{
    ds::ScriptBindingSet scriptBindings;
    scriptBindings.AddFunction("physics_move", l_PhysicsMove);

    return scriptBindings;
}
}
