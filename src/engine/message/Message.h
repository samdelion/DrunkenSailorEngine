#pragma once

#include "engine/entity/Entity.h"
#include "engine/common/StreamBuffer.h"
#include "engine/common/StringIntern.h"
#include "engine/system/platform/GraphicsContext.h"
#include "engine/system/platform/Keyboard.h"
#include "math/Matrix4.h"
#include "math/Vector3.h"

namespace ds_msg
{
/**
 * A message stream is a buffer full of messages of different types and sizes.
 * A message should always be inserted into the stream such that the message
 * header is found before the message data. To properly extract the messages
 * from the stream, extract a message header, get the type of the message and
 * the size of the message payload. Extract the given number of bytes into the
 * correct message structure.
 */
typedef ds_com::StreamBuffer MessageStream;

/**
 * Type of the message.
 */
enum class MessageType
{
    // On system init
    SystemInit,
    // On config load attempt
    ConfigLoad,
    // When you want to output a string to the console
    // ConsoleOut,
    // When you want the scripting system to interpret a string
    ScriptInterpret,
    // On Keyboard event
    KeyboardEvent,
    // On Quit event
    QuitEvent,
    // Toggle console on/off
    ConsoleToggle,
    // On text input
    TextInput,
    // On graphics context creation
    GraphicsContextCreated,
    // On component creation
    CreateComponent,
    // Move an entity
    MoveEntity,
    // Move forward
    MoveForward,
    // Move backward
    MoveBackward,
    // Strafe left
    StrafeLeft,
    // Strafe Right
    StrafeRight,
    // Set an entity's local transform
    SetLocalTransform,
    // Ask to move in the physics world
    PhysicsMove,
};

/**
 * Message header, used to extract the message payload correctly.
 */
struct MessageHeader
{
    MessageType type;
    size_t size;
};

// Message payloads
struct SystemInit
{
    const char *systemName;
};

struct ConfigLoad
{
    const char *configFilePath;
    bool didLoad;
};

struct ScriptInterpret
{
    ds::StringIntern::StringId
        stringId; // Interned id of the string to interpret
};

struct KeyboardEvent
{
    ds_platform::Keyboard::Key key;     // Key that was pressed/released
    ds_platform::Keyboard::State state; // State of the key (pressed/released)
    bool repeat;        // TRUE if this is a key repeat, FALSE otherwise.
    uint32_t timeStamp; // Time stamp of key event
    uint32_t windowID;  // ID of window with focus (if any)
};

struct QuitEvent
{
};

struct ConsoleToggle
{
};

struct TextInput
{
    ds::StringIntern::StringId stringId; // Interned id of the string produced
                                         // by this text input event.
    uint32_t timeStamp;                  // Time stamp of text input event
    uint32_t windowID;                   // ID of window with focus (if any)
};

struct GraphicsContextCreated
{
    ds_platform::GraphicsContext::ContextInfo
        contextInfo; // Information on the graphics context created
};

struct CreateComponent
{
    ds::Entity entity; // Entity to create component for
    ds::StringIntern::StringId
        componentType; // Type of component to create  as a string
    ds::StringIntern::StringId componentData; // Component config string.
};

struct MoveEntity
{
    ds::Entity entity;              // Entity to move
    ds_math::Vector3 deltaPosition; // Amount and direction to move
};

struct MoveForward
{
};

struct MoveBackward
{
};

struct StrafeLeft
{
};

struct StrafeRight
{
};

struct SetLocalTransform
{
    ds::Entity entity;               // Entity to set local transform of
    ds_math::Matrix4 localTransform; // New local transform
};

struct PhysicsMove
{
    ds::Entity entity;                // Entity to move
    ds_math::Vector3 position;        // Position of entity
    ds_math::Vector3 velocity;        // Velocity of entity
    ds_math::Vector3 ellipsoidRadius; // Ellipsoid radius
};
}
