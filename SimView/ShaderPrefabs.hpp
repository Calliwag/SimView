#pragma once

#include "SimView.hpp"

namespace SimView
{
    const char* vertexShaderFlatSource = /* vertex shader:*/ R"(

        #version 460 core
        in vec2 vertexPosArr;
        uniform mat3 transform;

        void main()
        {
            gl_Position = vec4(transform * vec3(vertexPosArr, 1.0f),1.0);
        };

)";

    const char* fragmentShaderFlatSource = /* fragment shader:*/ R"(

        #version 460 core
        uniform vec4 renderColor;
        out vec4 FragColor;
        void main()
        {
            FragColor = renderColor;
        }

)";

    const char* vertexShaderTextureSource = /* vertex shader:*/ R"(

        #version 460 core
        in vec2 vertexPosArr;
        uniform mat3 transform;
        in vec2 vertexUVArr;
        out vec2 texCoord;
        void main()
        {
            gl_Position = vec4(transform * vec3(vertexPosArr, 1.0f),1.0);
            texCoord = vertexUVArr;
        };

)";

    const char* fragmentShaderTextureSource = /* fragment shader:*/ R"(

        #version 460 core
        in vec2 texCoord;
        uniform vec4 renderColor;
        uniform sampler2D inputTexture;
        out vec4 FragColor;
        void main()
        {
            FragColor = texture(inputTexture, texCoord) * renderColor;
        }

)";

    const char* vertexShaderInstanceTextureSource = /* vertex shader:*/ R"(

        #version 460 core
        in vec2 vertexPosArr;
        uniform mat3 transform;
        in vec2 vertexUVArr;
        in vec2 offsetInstArr;
        out vec2 texCoord;
        void main()
        {
            gl_Position = vec4(transform * vec3(offsetInstArr + vertexPosArr, 1.0f), 1.0f);
            texCoord = vertexUVArr;
        };

)";

    const char* vertexShaderInstanceTextureColorSource = /* vertex shader:*/ R"(

        #version 460 core
        in vec2 vertexPosArr;
        in vec2 vertexUVArr;
        in vec2 offsetInstArr;
        in vec4 colorInstArr;
        uniform mat3 transform;
        out vec2 texCoord;
        out vec4 color;
        void main()
        {
            gl_Position = vec4(transform * vec3(offsetInstArr + vertexPosArr, 1.0f), 1.0f);
            texCoord = vertexUVArr;
            color = colorInstArr;
        };

)";

    const char* fragmentShaderInstanceTextureColorSource = /* fragment shader:*/ R"(

        #version 460 core
        in vec2 texCoord;
        in vec4 color;
        uniform sampler2D inputTexture;
        out vec4 FragColor;
        void main()
        {
            FragColor = texture(inputTexture, texCoord) * color;
        }

)";

    ShaderProgram GetFlatShader()
    {
        return ShaderProgram(vertexShaderFlatSource, fragmentShaderFlatSource, false, { "transform","renderColor" }, { "vertexPosArr" });
    }

    ShaderProgram GetTexShader()
    {
        return ShaderProgram(vertexShaderTextureSource, fragmentShaderTextureSource, false, { "transform","renderColor","inputTexture"}, {"vertexPosArr","vertexUVArr"});
    }

    ShaderProgram GetInstTexShader()
    {
        return ShaderProgram(vertexShaderInstanceTextureSource, fragmentShaderTextureSource, true, {"transform","renderColor","inputTexture"}, {"vertexPosArr", "vertexUVArr", "offsetInstArr"});
    }

    ShaderProgram GetInstTexColorShader()
    {
        return ShaderProgram(vertexShaderInstanceTextureColorSource, fragmentShaderInstanceTextureColorSource, true, {"transform", "inputTexture"}, {"vertexPosArr", "vertexUVArr", "offsetInstArr", "colorInstArr"});
    }
}