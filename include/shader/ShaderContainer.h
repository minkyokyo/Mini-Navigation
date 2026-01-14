#pragma once
static const char *vertexShaderSource = "#version 330 core\n"
                                        "layout (location = 0) in vec3 aPos;\n"
                                        "uniform mat4 model;\n"
                                        "uniform mat4 view;\n"
                                        "uniform mat4 projection;\n"
                                        "void main()\n"
                                        "{\n"
                                        "   gl_Position = projection * view * model * vec4(aPos.x,aPos.y,aPos.z, 1.0);\n"
                                        "}\0";

static const char *fragmentShaderSource = "#version 330 core\n"
                                          "out vec4 FragColor;\n"
                                          "void main()\n"
                                          "{\n"
                                          "   FragColor = vec4(1.0f,1.0f,1.0f, 1.0f);\n"
                                          "}\n\0";

static const char *uiVertexShaderSource =
    "#version 330 core\n"
    "layout (location = 0) in vec2 aPos;\n"
    "void main() {\n"
    "    gl_Position = vec4(aPos, 0.0, 1.0);\n"
    "}\0";

static const char *uiFragmentShaderSource =
    "#version 330 core\n"
    "out vec4 FragColor;\n"
    "uniform vec4 uColor;\n"
    "void main() {\n"
    "    FragColor = uColor;\n"
    "}\n\0";

static const char *mapVertexShaderSource =
    "#version 330 core\n"
    "layout(location=0) in vec2 aPos;\n"
    "layout(location=1) in vec4 aColor;\n"
    "layout(location=2) in float aType;\n"
    "out vec4 vColor;\n"
    "out float vType;\n"
    "uniform int uMode;\n" // 0=road,1=poi
    "uniform float uPointSize;\n"
    "void main(){\n"
    "    gl_Position = vec4(aPos, 0.0, 1.0);\n"
    "    vColor = aColor;\n"
    "    vType = aType;\n"
    "    if(uMode==1) gl_PointSize = uPointSize;\n"
    "}\0";

static const char *mapFragmentShaderSource =
    "#version 330 core\n"
    "in vec4 vColor;\n"
    "in float vType;\n"
    "out vec4 FragColor;\n"
    "uniform int uMode;\n" // 0=road,1=poi
    "void main(){\n"
    "    if(uMode==1){\n"
    "        // vType: 0 red square, 1 blue square, 2 green circle\n"
    "        if(abs(vType - 2.0) < 0.5){\n"
    "            vec2 pc = gl_PointCoord * 2.0 - 1.0;\n"
    "            if(dot(pc, pc) > 1.0) discard;\n"
    "        }\n"
    "    }\n"
    "    FragColor = vColor;\n"
    "}\n\0";
