#pragma once

#include "engine/entity/EntityManager.h"
#include "engine/system/ISystem.h"
#include "engine/system/scene/TransformComponentManager.h"
#include "engine/system/script/LuaEnvironment.h"
#include "math/Quaternion.h"
#include "math/Vector3.h"

namespace ds
{
/**
 * Manages the scripting interface in the engine.
 *
 * Scripting system calls a user specified boot script at startup. The boot
 * script must contain at least the following global functions: init(),
 * shutdown(), update(deltaTime) and render(). These functions are called in
 * appropriate places by the Scripting system. In the config file, this boot
 * script path should be found under "Script.bootScript" and be a path to a lua
 * file with the above functions defined, path relative to game executable
 * location.
 */
class Script : public ISystem
{
public:
    /**
     * Default constructor.
     */
    Script();

    /**
     * Initialize the scripting system.
     *
     * Attempts to load the boot script and calls it's init() method if
     * successfully loaded.
     *
     * @param  config  const Config &, configuration loaded by engine.
     * @return         bool, TRUE if initialization was successful, FALSE
     *                 otherwise.
     */
    virtual bool Initialize(const Config &config);

    /**
     * Update the scripting system.
     *
     * Calls the update(deltaTime) and render() methods of the boot script.
     *
     * @param deltaTime float, timestep to update over.
     */
    virtual void Update(float deltaTime);

    /**
     * Shutdown the scripting system, free any resources associated with it.
     *
     * Calls the shutdown() method of the boot script.
     */
    virtual void Shutdown();

    /**
     * Give messages to the scripting system.
     *
     * Messages are not handled in this method. Handling of events is deferred
     * to the Update method.
     *
     * @param  messages  const ds_msg::MessageStream &messages, messages to send
     * to the Message Bus.
     */
    virtual void PostMessages(const ds_msg::MessageStream &messages);

    /**
     * Collect messages generated by the scripting system.
     *
     * @return  ds_msg::MessageStream, stream of messages generated by this
     * system.
     */
    virtual ds_msg::MessageStream CollectMessages();

    /**
     * Return script bindings required by script system.
     *
     * @return  ScriptBindingSet, the script bindings the script system wants to
     * register.
     */
    virtual ScriptBindingSet GetScriptBindings() const;

    /**
     * Register another system's script bindings.
     *
     * @param  systemName  const char *, name of the registering system.
     * @param  systemPtr   ISystem *, pointer to registering system.
     */
    void RegisterScriptBindings(const char *systemName, ISystem *systemPtr);

    /**
     * Spawn a prefab in the world.
     *
     * @param   prefabFile  std::string, path to prefab, relative to the assets
     * directory.
     * @param   position    const ds_math::Vector3, spawn prefab at this
     * position with default orientation and scale.
     * @return              Entity, entity id of prefab spawned.
     */
    Entity SpawnPrefab(std::string prefabFile,
                       const ds_math::Vector3 &position);

    /**
     * Send a move entity message to all systems.
     *
     * @param  entity         Entity, entity to move.
     * @param  deltaPosition  const ds_math::Vector3 &, amount and direction to
     * move entity.
     */
    void MoveEntity(Entity entity, const ds_math::Vector3 &deltaPosition);

    /**
     * Get the world transform of an entity.
     *
     * @param   entity  Entity, entity to get world transform of.
     * @return          ds_math::Matrix4, world transform of the given
     * entity.
     */
    ds_math::Matrix4 GetWorldTransform(Entity entity) const;

    /**
     * Get the local transform of an entity.
     *
     * @param   entity  Entity, entity to get local transform of.
     * @return          ds_math::Matrix4, local transform of the given entity.
     */
    ds_math::Matrix4 GetLocalTransform(Entity entity) const;

    /**
     * Send a set local transform message.
     *
     * @param  entity     Entity, entity to set local transform of.
     * @param  transform  const ds_math::Matrix4 &, new local transform.
     */
    void SetLocalTransform(Entity entity, const ds_math::Matrix4 &transform);

    /**
     * Send a set animation index message.
     *
     * @param  entity          Entity, entity to set animation of.
     * @param  animationIndex  int, animation index to set.
     */
    void SetAnimationIndex(Entity entity, int animationIndex);

    /**
     * Is a new message available for the external script?
     *
     * Should be used by external scripts only.
     *
     * @return  bool, TRUE if a message is available, FALSE otherwise.
     */
    bool IsNextScriptMessage() const;

    /**
     * Get the next message available to the external script, returns an empty
     * message stream if no messages available.
     *
     * @return  ds_msg::MessageStream, next script message available.
     */
    ds_msg::MessageStream GetNextScriptMessage();

    /**
     * Rotate an entity to look at a target.
     *
     * @param  entity  Entity, entity to get to look at target.
     * @param  target  const ds_math::Vector3 &, target to look at.
     */
    void LookAt(Entity entity, const ds_math::Vector3 &target);

    /**
     * Set the material of the skybox.
     *
     * @param  skyboxMaterialPath  const std::string &, path to skybox material
     * to set.
     */
    void SetSkyboxMaterial(const std::string &skyboxMaterialPath);

    /**
     * Send a create GUI panel message.
     *
     * @param  startX        float, start x co-ordinate of the panel.
     * @param  startY        float, start y co-ordinate of the panel.
     * @param  endX          float, end x co-ordinate of the panel.
     * @param  endY          float, end y co-ordinate of the panel.
     * @param  materialPath  const std::string &, path to material to use to
     * render the panel.
     * @return               Entity, entity id of panel.
     */
    Entity CreateGUIPanel(float startX,
                          float startY,
                          float endX,
                          float endY,
                          const std::string &materialPath);

    /**
     * Send a create GUI button message.
     *
     * @param   startX               float, start x co-ordinate of the panel.
     * @param   startY               float, start y co-ordinate of the panel.
     * @param   endX                 float, end x co-ordinate of the panel.
     * @param   endY                 float, end y co-ordinate of the panel.
     * @param   defaultMaterialPath  const std::string &, path to material to
     * use to render the button when not pressed or hovered over.
     * @param   pressedMaterialPath  const std::string &, path to material to
     * use to render the button when pressed.
     * @param   hoverMaterialPath    const std::string &, path to material to
     * use to render the button when hovered over.
     */
    Entity CreateGUIButton(float startX,
                           float startY,
                           float endX,
                           float endY,
                           const std::string &defaultMaterialPath,
                           const std::string &pressedMaterialPath,
                           const std::string &hoverMaterialPath);

private:
    /**
     * Process messages in the given message stream.
     *
     * @param  ds_msg::MessageStream *, messages to process.
     */
    void ProcessEvents(ds_msg::MessageStream *messages);

    /**
     * Register the script bindings of a particular system.
     *
     * @param  systemName  const char *, name of the registering system.
     * @param  systemPtr  ISystem *, pointer to system to register script
     * bindings of.
     */
    void RegisterScriptBindingSet(const char *systemName, ISystem *systemPtr);

    /**
     * Build a create transfrom component message.
     *
     * @param   entity       Entity, entity to create transform component for.
     * @param   position     const ds_math::Vector3, position of transform
     * component.
     * @param   orientation  const ds_math::Quaternion &, orientation of
     * transform component.
     * @param   scale        const ds_math::Vector3, scale of transform
     * component.
     * @returen              ds_msg::CreateComponent, create component message;
     */
    ds_msg::CreateComponent
    BuildTransformComponentCreateMessage(Entity entity,
                                         const ds_math::Vector3 &position,
                                         const ds_math::Quaternion &orientation,
                                         const ds_math::Vector3 &scale);

    // Messaging
    ds_msg::MessageStream m_messagesGenerated, m_messagesReceived;

    // Did the boot script load successfully?
    bool m_bootScriptLoaded;
    // Lua environment
    ds_lua::LuaEnvironment m_lua;

    // Systems wanting their script bindings registered
    std::vector<std::pair<const char *, ISystem *>> m_registeredSystems;

    // Messages to pass to script
    ds_msg::MessageStream m_toScriptMessages;

    // Used to co-ordinate the creation of components in the system.
    EntityManager m_entityManager;

    // Keep track of transforms of entities
    TransformComponentManager m_transformManager;
};
}
