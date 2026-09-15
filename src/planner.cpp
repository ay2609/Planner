#include "../imgui/imgui.h"
#include "../imgui/backends/imgui_impl_glfw.h"
#include "../imgui/backends/imgui_impl_opengl3.h"
#include "functions.h"

#include <iostream>
#include <vector>
#include <cmath>
#include <random>
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>
#include <glm/glm.hpp>
#include <unistd.h>
#include <writer.h>
#include <stringbuffer.h>
#include <fstream>

#define GL_SILENCE_DEPRECATION

using namespace std;
using namespace ImGui;

const char *vertexShaderSource  =R"GLSL(
#version 330 core
layout (location = 0) in vec2 position;
layout (location = 1) in vec2 inTexCoord;

out vec2 texCoord;
void main(){
    texCoord = inTexCoord;
    gl_Position = vec4(position.x, position.y, 0.0f, 1.0f);
})GLSL";

const char *fragmentShaderSource  =R"GLSL(
#version 330 core
in vec2 texCoord;
uniform vec2 iResolution;
uniform float iTime;
out vec4 fragColor;
vec2 fragCoord = gl_FragCoord.xy;
#define PI 3.14159265359

float sdHyperbolicCross( in vec2 p, float k )
{
    // scale
    float s = 1.0/k - k;
    p = p*s;
    // symmetry
    p = abs(p);
    p = (p.x>p.y) ? p.yx : p.xy;
    // offset
    p += k;

    // solve quartic (for details see https://www.shadertoy.com/view/ftcyW8)
    float x2 = p.x*p.x/16.0;
    float y2 = p.y*p.y/16.0;
    float r = (p.x*p.y-4.0)/12.0;
    float q = y2-x2;
    float h = q*q-r*r*r;
    float u;
    if( h<0.0 )
    {
        float m = sqrt(r);
        u = m*cos( acos(q/(r*m) )/3.0 );
    }
    else
    {
        float m = pow(sqrt(h)+q,1.0/3.0);
        u = (m+r/m)/2.0;
    }
    float w = sqrt(u+x2);
    float x = p.x/4.0-w+sqrt(2.0*x2-u+(p.y-x2*p.x*2.0)/w/4.0);

    // clamp arm
    x = max(x,k);

    // compute distance to closest point
    float d = length( p-vec2(x,1.0/x) ) / s;

    // sign
    return p.x*p.y < 1.0 ? -d : d;
}

vec3 palette( float t ) {
    vec3 a = vec3(0.5, 0.5, 0.5);
    vec3 b = vec3(0.5, 0.5, 0.5);
    vec3 c = vec3(1.0, 1.0, 1.0);
    vec3 d = vec3(0.263,0.416,0.557);

    return a + b*cos( 6.28318*(c*t+d) );
}

//https://www.shadertoy.com/view/mtyGWy
void main(){
    float scale = 1.25;

    vec2 uv = scale*(fragCoord - iResolution.xy) / iResolution.y;
    vec2 uv0 = uv;
    vec3 finalColor = vec3(0.0);
    float time = iTime * .01;
    float angle = -time + PI;
    mat2 rot = mat2(cos(angle),-sin(angle),sin(angle),cos(angle));

    float k = 0.5 + 0.45*sin(1.234*iTime);

    for (float i = 0.0; i < 3.0; i++) {
//        uv = rot * uv;
//        uv = sin(fract(uv * 1.5) - 0.5);

        float d = sdHyperbolicCross(uv, k);
//        d *= exp(-length(uv0));

//        float d = sin(length(3.6 * uv)) * (-log(length(uv0)) - 2.);
//        float d = length(uv) * exp(-length(uv0));

        vec3 col = palette(length(uv0) + iTime*.4);

        d = sin(d*8.0 + iTime*.4) / 8.0;
//        d = abs(d);
        d = pow(0.003 / d, 2.0);
//        d = clamp(d, 0.0, 1.0);
        finalColor += col * d;
    }

    fragColor = vec4(finalColor, 1.0);
}
)GLSL";

static void glfw_error_callback(int error, const char* description)
{
    fprintf(stderr, "GLFW Error %d: %s\n", error, description);
}


int main(int, char**)
{

    int width = 1280;
    int height = 720;

    // things I don't care about
    glm::vec2 screen(width, height);

    float deltaTime = 0.0f;
    float lastFrame = 0.0f;

    glfwSetErrorCallback(glfw_error_callback);
    if (!glfwInit())
        return 1;

    // Decide GL+GLSL versions~
#if defined(IMGUI_IMPL_OPENGL_ES2)
    // GL ES 2.0 + GLSL 100
    const char* glsl_version = "#version 100";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 2);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    glfwWindowHint(GLFW_CLIENT_API, GLFW_OPENGL_ES_API);
#elif defined(__APPLE__)
    // GL 3.2 + GLSL 150
    const char* glsl_version = "#version 150";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 2);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // Required on Mac
#else
    // GL 3.0 + GLSL 130
    const char *glsl_version = "#version 330";
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 0);
    //glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);  // 3.2+ only
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);            // 3.0+ only
#endif

    // Create window with graphics context
    GLFWwindow *window = glfwCreateWindow(1280, 720, "Planner", nullptr, nullptr);
    if (window == nullptr)
        return 1;
    glfwMakeContextCurrent(window);

    int samples = 4;
    float quadVerts[] = {
            -1.0, -1.0,     0.0, 0.0,
            -1.0, 1.0,      0.0, 1.0,
            1.0, -1.0,      1.0, 0.0,

            1.0, -1.0,      1.0, 0.0,
            -1.0, 1.0,      0.0, 1.0,
            1.0, 1.0,       1.0, 1.0
    };

    GLuint VAO;
    glGenVertexArrays(1, &VAO);
    glBindVertexArray(VAO);

    GLuint VBO;
    glGenBuffers(1, &VBO);
    glBindBuffer(GL_ARRAY_BUFFER, VBO);
    glBufferData(GL_ARRAY_BUFFER, sizeof(quadVerts), quadVerts, GL_STATIC_DRAW);

    glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(0));
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 4 * sizeof(float), reinterpret_cast<void*>(2 * sizeof(float)));
    glEnableVertexAttribArray(1);

    glBindVertexArray(0);


    GLuint framebuffer;
    glGenFramebuffers(1, &framebuffer);
    glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);

    GLuint texColor;
    glGenTextures(1, &texColor);
    glBindTexture(GL_TEXTURE_2D, texColor);
    glTexImage2D(GL_TEXTURE_2D, 0, GL_RGB, width, height, 0, GL_RGB, GL_UNSIGNED_BYTE, 0);
    glBindTexture(GL_TEXTURE_2D, 0);
    glFramebufferTexture2D(GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_TEXTURE_2D, texColor, 0);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);

    //vertex shader
    unsigned int vertexShader = glCreateShader(GL_VERTEX_SHADER);
    glShaderSource(vertexShader, 1, &vertexShaderSource, NULL);
    glCompileShader(vertexShader);

    // fragment shader
    unsigned int fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    glShaderSource(fragmentShader, 1, &fragmentShaderSource, NULL);
    glCompileShader(fragmentShader);
    // check for shader compile errors

    // link shaders
    unsigned int shaderProgram = glCreateProgram();
    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);
    // check for linking errors

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);

    glUseProgram(shaderProgram);
    glUniform2fv(glGetUniformLocation(shaderProgram, "iResolution"), 1, &screen[0]);

    glBindFramebuffer(GL_FRAMEBUFFER, 0);
    glUseProgram(shaderProgram);

    // Setup Dear ImGui context
    IMGUI_CHECKVERSION();
    ImGui::CreateContext();
    ImGuiIO &io = ImGui::GetIO();
    (void) io;
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableKeyboard;     // Enable Keyboard Controls
    io.ConfigFlags |= ImGuiConfigFlags_NavEnableGamepad;      // Enable Gamepad Controls

    // Setup Dear ImGui style
    ImGui::StyleColorsDark();
    ImGui::PushStyleVar(ImGuiStyleVar_FrameRounding, 5.0f);

    // Setup Platform/Renderer backends
    ImGui_ImplGlfw_InitForOpenGL(window, true);
    ImGui_ImplOpenGL3_Init(glsl_version);

    // Our state
    bool show_planner_window = true;
    bool show_another_window = false;
    ImVec4 clear_color = ImVec4(0.0f, 0.0f, 0.0f, 0.75f);

    // non-random
//    float color_tracker[3] = {0.0f, 0.33f, 0.66f};
//    float color_speeds[3] = {0.001f, 0.001f, 0.001f};

    // random
    float color_tracker[3] = {static_cast<float>(std::rand()) / RAND_MAX,
                              static_cast<float>(std::rand()) / RAND_MAX,
                              static_cast<float>(std::rand()) / RAND_MAX};
    float color_speeds[3] = {(static_cast<float>(std::rand()) / RAND_MAX) / 300,
                             (static_cast<float>(std::rand()) / RAND_MAX) / 300,
                             (static_cast<float>(std::rand()) / RAND_MAX) / 300};

    int display_w, display_h;

    glfwMakeContextCurrent(window);
    glfwSwapInterval(1);
    while (!glfwWindowShouldClose(window))
    {
        float currentFrame = glfwGetTime();
        deltaTime = currentFrame - lastFrame;
        lastFrame = currentFrame;



        glClearColor(0.0f, 0.0f, 0.0f, 1.0f);
        glClear(GL_COLOR_BUFFER_BIT);

        glUniform1f(glGetUniformLocation(shaderProgram, "iTime"), currentFrame);
        glBindVertexArray(VAO);
        glDrawArrays(GL_TRIANGLES, 0, 6);





        // Start the Dear ImGui frame
        ImGui_ImplOpenGL3_NewFrame();
        ImGui_ImplGlfw_NewFrame();
        ImGui::NewFrame();

        // do cool things here

        ImGui::ShowDemoWindow();

        if(show_planner_window)
        {
            PlannerWindow(&show_planner_window);
        }

        ImGui::Render();
        ImGui_ImplOpenGL3_RenderDrawData(ImGui::GetDrawData());

        glfwGetFramebufferSize(window, &display_w, &display_h);
        glViewport(0, 0, display_w, display_h);
        glfwSwapBuffers(window);
        glfwPollEvents();

    }
#ifdef __EMSCRIPTEN__
    EMSCRIPTEN_MAINLOOP_END;
#endif

    // Cleanup
    ImGui_ImplOpenGL3_Shutdown();
    ImGui_ImplGlfw_Shutdown();
    ImGui::DestroyContext();

    glfwDestroyWindow(window);
    glfwTerminate();

    return 0;
}