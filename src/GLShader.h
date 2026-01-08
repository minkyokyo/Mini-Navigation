static const char VertexShader[] =
    "#version 330 core\n"
    "\n"
    "layout (location = 0)\n"
    "\n"
    "in vec3 aPos\n"
    "\n"
    "void main()\n"
    "{\n"
    "    gl_Position = vec4(aPos.x, aPos.y, aPos.z, 1.0);\n"
    "};\n";

static const char FragmentShader[] =
    "#version 330 core\n"
    "\n"
    "out vec4 FragColor;\n"
    "\n"
    "void main()\n"
    "{\n"
    "    FragColor = vec4(1.0f, 0.0f, 0.0f, 1.0f);\n"
    "}";
