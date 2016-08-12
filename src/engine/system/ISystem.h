#pragma once

#include "engine/Config.h"
#include "engine/message/Message.h"
#include "engine/system/script/ScriptBindingSet.h"

namespace ds
{
/**
 * Interface for all systems to be processed by the engine.
 *
 * A system is any set of related functionality that needs to be updated by the
 * engine every game tick. More specifically, systems consist of high-level
 * functionality. For lower level systems, the IFrameworkSystem interface
 * exists.
 */
class ISystem
{
public:
    /**
     * Virtual destructor. Required so that the destructor of derived classes is
     * called properly.
     */
    virtual ~ISystem()
    {
    }

    /**
     * Perform any necessary initialization of the system
     *
     * If initialization fails, method should return FALSE, otherwise TRUE.
     *
     * @param   config  const Config &, configuration loaded by engine.
     * @return          bool, TRUE if initialization succeeds, FALSE otherwise.
     */
    virtual bool Initialize(const Config &config) = 0;

    /**
     * Update the system over the given timestep.
     *
     * @param  deltaTime  float, timestep to update the system over.
     */
    virtual void Update(float deltaTime) = 0;

    /**
     * Perform any teardown of the system.
     */
    virtual void Shutdown() = 0;

    /**
     * Post messages for system to handle.
     *
     * Messages may be handled in this method but it is recommended that the
     * handling of messages is deferred until the Update() call. You could
     * achieve this by storing the events in a local MessageStream and then
     * processing them in the Update() call.
     *
     * @param  messages  const ds_msg::MessageStream &messages, messages to send
     * to this class.
     */
    virtual void PostMessages(const ds_msg::MessageStream &messages) = 0;

    /**
     * Collect messages generated by this system.
     *
     * It is assumed that once this message is called the messages are no longer
     * needed, thus you can sefely purge any generated events after this method
     * has been called.
     *
     * @return  ds_msg::MessageStream, stream of messages generated by this
     * system.
     */
    virtual ds_msg::MessageStream CollectMessages() = 0;

    /**
     * Get the name of the system. This name is used in the script system of the
     * engine, a pointer to the system is placed in a Lua global with the name
     * '__GetNameResult' (i.e. two underscores prepended to the result of this
     * function).
     *
     * @post   Returned name is unique among systems added to the engine.
     *
     * @return   const char *, name string.
     */
    virtual const char *GetName() const = 0;

    /**
     * Optionally return any required script bindings.
     *
     * @return  ScriptBindingSet, the script bindings the system wants to
     * register with the Script system.
     */
    virtual ScriptBindingSet GetScriptBindings() const
    {
        return ScriptBindingSet();
    }
};
}
