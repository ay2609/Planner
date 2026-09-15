#ifndef PLANNER_FUNCTIONS_H
#define PLANNER_FUNCTIONS_H



#include "../imgui/imgui.h"
#include "../imgui/imguidatechooser.h"
#include "../imgui/backends/imgui_impl_glfw.h"
#include "../imgui/backends/imgui_impl_opengl3.h"

//#include "../fmt/include/fmt/core.h"

#include <iostream>
#include <fstream>
#include <vector>
#include <filesystem>
#include <writer.h>
#include <stringbuffer.h>
#define FMT_HEADER_ONLY
#include <fmt/format.h>
#include <GLFW/glfw3.h>
#include <OpenGL/gl3.h>



class assignment {
public:
    int number = 0;
    tm date_val = {};

    std::vector<std::string> assignment_details() {
        std::vector<std::string> temp = {std::to_string(number),
                                         fmt::format("{}-{}-{}", date_val.tm_mon+1, date_val.tm_mday, date_val.tm_year+1900)
        };
        return temp;

    }
};

//class track {
//public:
//    tm date_val = {};
//
//private:
//
//};

void PlannerWindow(bool* p_open);

#endif // PLANNER_FUNCTIONS_H
