#include "engine/common/StringIntern.h"
#include "engine/system/Console.h"
#include "engine/message/MessageHelper.h"

namespace ds
{
bool Console::Initialize(const Config &config)
{
    bool result = true;

    m_isConsoleOpen = false;

    // Send system init message
    ds_msg::SystemInit initMsg;
    initMsg.systemName = "Console";

    ds_msg::AppendMessage(&m_messagesGenerated, ds_msg::MessageType::SystemInit,
                          sizeof(ds_msg::SystemInit), &initMsg);

    return result;
}

void Console::Update(float deltaTime)
{
    ProcessEvents(&m_messagesReceived);

    // Flush console output
    Flush();

    // TESTING ONLY
    // ds_msg::MessageHeader header;
    // header.type = ds_msg::MessageType::ScriptInterpret;
    // header.size = sizeof(ds_msg::ScriptInterpret);

    // ds_msg::ScriptInterpret scriptMsg;
    // scriptMsg.string = "print(\"Hello World\")";
    // m_messagesGenerated << header << scriptMsg;
}

void Console::Shutdown()
{
}

void Console::PostMessages(const ds_msg::MessageStream &messages)
{
    AppendStreamBuffer(m_messagesReceived, messages);
}

ds_msg::MessageStream Console::CollectMessages()
{
    ds_msg::MessageStream tmp = m_messagesGenerated;

    m_messagesGenerated.Clear();

    return tmp;
}

void Console::Flush()
{
    std::cout << m_buffer.str();

    // Clean buffer
    m_buffer.str(std::string());
}

void Console::ProcessEvents(ds_msg::MessageStream *messages)
{
    while (messages->AvailableBytes() != 0)
    {
        // Extract header
        ds_msg::MessageHeader header;
        (*messages) >> header;

        switch (header.type)
        {
        case ds_msg::MessageType::SystemInit:
            ds_msg::SystemInit initMsg;
            (*messages) >> initMsg;

            m_buffer << "Console out: " << initMsg.systemName
                     << " system initialized." << std::endl;
            break;
        case ds_msg::MessageType::ConfigLoad:
            ds_msg::ConfigLoad configLoadMsg;
            (*messages) >> configLoadMsg;

            if (configLoadMsg.didLoad)
            {
                m_buffer << "Console out: "
                         << "Loaded config file: \""
                         << configLoadMsg.configFilePath << "\"" << std::endl;
            }
            else
            {
                m_buffer << "Console out: "
                         << "Failed to load config file: \""
                         << configLoadMsg.configFilePath << "\"" << std::endl;
            }
            break;
        case ds_msg::MessageType::ScriptInterpret:
            ds_msg::ScriptInterpret scriptMsg;
            (*messages) >> scriptMsg;

            m_buffer << "Console out: "
                     << "Executed script command: "
                     << StringIntern::Instance().GetString(scriptMsg.stringId)
                     << std::endl;
            break;
        case ds_msg::MessageType::KeyboardEvent:
            ds_msg::KeyboardEvent keyEvent;
            (*messages) >> keyEvent;

            if (keyEvent.state == ds_platform::Keyboard::State::Key_Pressed)
            {
                m_buffer << "Console out: "
                         << "Key pressed: '"
                         << ds_platform::Keyboard::PrintKey(keyEvent.key)
                         << "'." << std::endl;

                if (keyEvent.key == ds_platform::Keyboard::Key::Key_Backspace &&
                    m_inputText.length() > 0)
                {
                    m_inputText.pop_back();
                }
                else if (keyEvent.key ==
                             ds_platform::Keyboard::Key::Key_Return &&
                         m_isConsoleOpen)
                {
                    // Send message to Script system to execute Lua code
                    // inputted into console.
                    ds_msg::ScriptInterpret scriptMsg;
                    scriptMsg.stringId =
                        StringIntern::Instance().Intern(m_inputText);

                    ds_msg::AppendMessage(&m_messagesGenerated,
                                          ds_msg::MessageType::ScriptInterpret,
                                          sizeof(ds_msg::ScriptInterpret),
                                          &scriptMsg);

                    // Clear input text
                    m_inputText.clear();
                }
            }
            else if (keyEvent.state ==
                     ds_platform::Keyboard::State::Key_Released)
            {
                m_buffer << "Console out: "
                         << "Key released: '"
                         << ds_platform::Keyboard::PrintKey(keyEvent.key)
                         << "'." << std::endl;
            }
            break;
        case ds_msg::MessageType::QuitEvent:
            ds_msg::QuitEvent quitEvent;
            (*messages) >> quitEvent;

            m_buffer << "Console out: "
                     << "Quit event." << std::endl;
            break;
        case ds_msg::MessageType::ConsoleToggle:
            ds_msg::ConsoleToggle consoleToggle;
            (*messages) >> consoleToggle;

            m_buffer << "Console out:"
                     << "Console toggled." << std::endl;

            if (m_inputText.length() > 0)
            {
                // Lop off console toggle key character from input text
                m_inputText.pop_back();
            }

            // Open or close console
            m_isConsoleOpen = !m_isConsoleOpen;
            break;
        case ds_msg::MessageType::TextInput:
            ds_msg::TextInput textInput;
            (*messages) >> textInput;

            m_buffer << "Console out: "
                     << "Text input: "
                     << StringIntern::Instance().GetString(textInput.stringId)
                     << std::endl;

            // If console is open, accept text input.
            if (m_isConsoleOpen == true)
            {
                m_inputText +=
                    StringIntern::Instance().GetString(textInput.stringId);
                std::cout << m_inputText << std::endl;
            }
            break;
        case ds_msg::MessageType::GraphicsContextCreated:
            ds_msg::GraphicsContextCreated gfxContext;
            (*messages) >> gfxContext;

            if (gfxContext.contextInfo.type ==
                ds_platform::GraphicsContext::ContextType::OpenGL)
            {
                m_buffer << "Console out: ";
                m_buffer << "OpenGL context created version "
                         << gfxContext.contextInfo.openGL.majorVersion << "."
                         << gfxContext.contextInfo.openGL.minorVersion;
                if (gfxContext.contextInfo.openGL.profile ==
                    ds_platform::GraphicsContext::ContextProfileOpenGL::
                        Compatability)
                {
                    m_buffer << " (compatability profile)." << std::endl;
                }
                else
                {
                    m_buffer << " (core profile)." << std::endl;
                }
            }
            break;
        case ds_msg::MessageType::CreateComponent:
            ds_msg::CreateComponent createComponentMsg;
            (*messages) >> createComponentMsg;

            // Print console msg
            m_buffer << "Console out: Component created: Entity: "
                     << createComponentMsg.entity.id << " ComponentType: "
                     << StringIntern::Instance().GetString(
                            createComponentMsg.componentType)
                     << std::endl;
            break;
        case ds_msg::MessageType::MoveEntity:
        {
            ds_msg::MoveEntity entityMoveMsg;
            (*messages) >> entityMoveMsg;

            // Print console msg
            m_buffer << "Console out: Entity moved: Entity: "
                     << entityMoveMsg.entity.id
                     << " deltaPosition: " << entityMoveMsg.deltaPosition
                     << std::endl;
            break;
        }
        case ds_msg::MessageType::MoveForward:
        {
            ds_msg::MoveForward moveForwardMsg;
            (*messages) >> moveForwardMsg;

            // Print move forward msg
            m_buffer << "Console out: Move forward message." << std::endl;

            break;
        }
        case ds_msg::MessageType::MoveBackward:
        {
            ds_msg::MoveBackward moveBackwardMsg;
            (*messages) >> moveBackwardMsg;

            // Print move backward msg
            m_buffer << "Console out: Move backward message." << std::endl;

            break;
        }
        case ds_msg::MessageType::StrafeLeft:
        {
            ds_msg::StrafeLeft strafeLeftMsg;
            (*messages) >> strafeLeftMsg;

            // Print strafe left msg
            m_buffer << "Console out: Strafe left message." << std::endl;
            break;
        }
        case ds_msg::MessageType::StrafeRight:
        {
            ds_msg::StrafeRight strafeRightMsg;
            (*messages) >> strafeRightMsg;

            // Print strafe right msg
            m_buffer << "Console out: Strafe right message." << std::endl;
            break;
        }
        case ds_msg::MessageType::SetLocalTransform:
        {
            ds_msg::SetLocalTransform setLocalMsg;
            (*messages) >> setLocalMsg;

            // m_buffer << "Set local transform of " << setLocalMsg.entity.id
            //          << " to: " << setLocalMsg.localTransform << std::endl;
            break;
        }
        case ds_msg::MessageType::PhysicsMove:
        {
            ds_msg::PhysicsMove physMove;
            (*messages) >> physMove;

            m_buffer << "Entity moved: " << physMove.entity.id
                     << " position: " << physMove.position
                     << " velocity: " << physMove.velocity
                     << " ellipsoidRadius: " << physMove.ellipsoidRadius
                     << std::endl;
            break;
        }
        default:
            // Always extract the payload
            messages->Extract(header.size);

            // Console should handle all message types, so crash if not handled.
            assert(false && "Console::ProcessEvents: Unhandled message type!");
            break;
        }
    }
}
}
