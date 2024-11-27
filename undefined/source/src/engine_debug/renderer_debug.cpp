#include "engine_debug/renderer_debug.h"

#include <iostream>
#include <glad/glad.h>

#include "engine_debug/logger.h"

void RendererDebug::DebugInit()
{
    glEnable(GL_DEBUG_OUTPUT);
    glEnable(GL_DEBUG_OUTPUT_SYNCHRONOUS);
    glDebugMessageCallback([](GLenum source, GLenum type, GLuint id, GLenum severity, GLsizei, const GLchar* message, const void*)
    {
            std::string sourceMessage;
            std::string typeMessage;

            switch (source)
            {
            case GL_DEBUG_SOURCE_API:
                sourceMessage = "API";
                break;

            case GL_DEBUG_SOURCE_WINDOW_SYSTEM:
                sourceMessage = "WINDOW SYSTEM";
                break;

            case GL_DEBUG_SOURCE_SHADER_COMPILER:
                sourceMessage = "SHADER COMPILER";
                break;

            case GL_DEBUG_SOURCE_THIRD_PARTY:
                sourceMessage = "THIRD PARTY";
                break;

            case GL_DEBUG_SOURCE_APPLICATION:
                sourceMessage = "APPLICATION";
                break;

            case GL_DEBUG_SOURCE_OTHER:
                sourceMessage = "UNKNOWN";
                break;

            default:
                sourceMessage = "UNKNOWN";
                break;
            }

            switch (type) 
            {
            case GL_DEBUG_TYPE_ERROR:
                typeMessage = "ERROR";
                break;

            case GL_DEBUG_TYPE_DEPRECATED_BEHAVIOR:
                typeMessage = "DEPRECATED BEHAVIOR";
                break;

            case GL_DEBUG_TYPE_UNDEFINED_BEHAVIOR:
                typeMessage = "UDEFINED BEHAVIOR";
                break;

            case GL_DEBUG_TYPE_PORTABILITY:
                typeMessage = "PORTABILITY";
                break;

            case GL_DEBUG_TYPE_PERFORMANCE:
                typeMessage = "PERFORMANCE";
                break;

            case GL_DEBUG_TYPE_OTHER:
                typeMessage = "OTHER";
                break;

            case GL_DEBUG_TYPE_MARKER:
                typeMessage = "MARKER";
                break;

            default:
                typeMessage = "UNKNOWN";
                break;
            }

            switch (severity) 
            {
            case GL_DEBUG_SEVERITY_HIGH:
                Logger::Error("[{}] {} {} {}", sourceMessage, id, typeMessage, message);
                break;

            case GL_DEBUG_SEVERITY_MEDIUM:
                Logger::Warning("[{}] {} {} {}", sourceMessage, id, typeMessage, message);
                break;
            }
    },
    nullptr
    );

    glDebugMessageControl(GL_DONT_CARE, GL_DONT_CARE, GL_DONT_CARE, 0, nullptr, GL_TRUE);
}
