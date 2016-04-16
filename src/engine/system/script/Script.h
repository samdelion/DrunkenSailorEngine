#pragma once

#include "engine/entity/EntityManager.h"
#include "engine/system/ISystem.h"
#include "engine/system/script/LuaEnvironment.h"

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
     * @param  prefabFile  std::string, path to prefab, relative to the assets
     * directory.
     */
    void SpawnPrefab(std::string prefabFile);

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
};
}
