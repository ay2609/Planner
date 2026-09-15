#include "functions.h"


static void HelpMarker(const char* desc)
{
    ImGui::TextDisabled("(?)");
    if (ImGui::BeginItemTooltip())
    {
        ImGui::PushTextWrapPos(ImGui::GetFontSize() * 35.0f);
        ImGui::TextUnformatted(desc);
        ImGui::PopTextWrapPos();
        ImGui::EndTooltip();
    }
}


void PlannerWindow(bool* p_open)
{
    static std::vector<assignment> table_info;
    static assignment temp_val;

    std::filesystem::path asnmt_path = "data/assignments.json";
    rapidjson::StringBuffer asnmt_s;
    rapidjson::Writer<rapidjson::StringBuffer> asnmt_writer(s);
    writer.StartObject();               // Between StartObject()/EndObject(),
    writer.Key("hello");                // output a key,
    writer.String("world");             // follow by a value.
    writer.Key("t");
    writer.Bool(true);
    writer.Key("f");
    writer.Bool(false);
    writer.Key("n");
    writer.Null();
    writer.Key("i");
    writer.Uint(123);
    writer.Key("pi");
    writer.Double(3.1416);
    writer.Key("a");
    writer.EndObject();


    if (std::filesystem::exists(planner_path))
    {

    }
    else
    {
        std::ofstream file(planner_path);
        if (file.is_open()) {
            // Write data to the file

        }
        else {
            // Handle the case if any error occured
            cerr << "Failed to create file: " << filepath
                 << endl;
        }
    }

    // json stuff here

    // check if file exists, create new or load





    // json stuff end here






    // Exceptionally add an extra assert here for people confused about initial Dear ImGui setup
    // Most functions would normally just assert/crash if the context is missing.
    IM_ASSERT(ImGui::GetCurrentContext() != NULL && "Missing Dear ImGui context. Refer to examples app!");

    // Examples Apps (accessible from the "Examples" menu)

    // Dear ImGui Tools (accessible from the "Tools" menu)
    static bool show_tool_metrics = false;
    static bool show_tool_debug_log = false;
    static bool show_tool_id_stack_tool = false;
    static bool show_tool_style_editor = false;
    static bool show_tool_about = false;

    if (show_tool_metrics)
        ImGui::ShowMetricsWindow(&show_tool_metrics);
    if (show_tool_debug_log)
        ImGui::ShowDebugLogWindow(&show_tool_debug_log);
    if (show_tool_id_stack_tool)
        ImGui::ShowIDStackToolWindow(&show_tool_id_stack_tool);
    if (show_tool_style_editor)
    {
        ImGui::Begin("Dear ImGui Style Editor", &show_tool_style_editor);
        ImGui::ShowStyleEditor();
        ImGui::End();
    }
    if (show_tool_about)
        ImGui::ShowAboutWindow(&show_tool_about);

    // Demonstrate the various window flags. Typically you would just use the default!
    static bool no_titlebar = false;
    static bool no_scrollbar = false;
    static bool no_menu = false;
    static bool no_move = false;
    static bool no_resize = false;
    static bool no_collapse = false;
    static bool no_close = false;
    static bool no_nav = false;
    static bool no_background = false;
    static bool no_bring_to_front = false;
    static bool unsaved_document = false;

    ImGuiWindowFlags window_flags = 0;
    if (no_titlebar)        window_flags |= ImGuiWindowFlags_NoTitleBar;
    if (no_scrollbar)       window_flags |= ImGuiWindowFlags_NoScrollbar;
    if (!no_menu)           window_flags |= ImGuiWindowFlags_MenuBar;
    if (no_move)            window_flags |= ImGuiWindowFlags_NoMove;
    if (no_resize)          window_flags |= ImGuiWindowFlags_NoResize;
    if (no_collapse)        window_flags |= ImGuiWindowFlags_NoCollapse;
    if (no_nav)             window_flags |= ImGuiWindowFlags_NoNav;
    if (no_background)      window_flags |= ImGuiWindowFlags_NoBackground;
    if (no_bring_to_front)  window_flags |= ImGuiWindowFlags_NoBringToFrontOnFocus;
    if (unsaved_document)   window_flags |= ImGuiWindowFlags_UnsavedDocument;
    if (no_close)           p_open = NULL; // Don't pass our bool* to Begin

    no_close = true;

    // We specify a default position/size in case there's no data in the .ini file.
    // We only do it to make the demo applications a little more welcoming, but typically this isn't required.
    const ImGuiViewport* main_viewport = ImGui::GetMainViewport();
    ImGui::SetNextWindowPos(ImVec2(main_viewport->WorkPos.x + 650, main_viewport->WorkPos.y + 20), ImGuiCond_FirstUseEver);
    ImGui::SetNextWindowSize(ImVec2(550, 680), ImGuiCond_FirstUseEver);

    // Main body of the Demo window starts here.
    if (!ImGui::Begin("Planner", p_open, window_flags))
    {
        // Early out if the window is collapsed, as an optimization.
        ImGui::End();
        return;
    }

    // Most "big" widgets share a common width settings by default. See 'Demo->Layout->Widgets Width' for details.
    // e.g. Use 2/3 of the space for widgets and 1/3 for labels (right align)
    //ImGui::PushItemWidth(-ImGui::GetWindowWidth() * 0.35f);
    // e.g. Leave a fixed amount of width for labels (by passing a negative value), the rest goes to widgets.
    ImGui::PushItemWidth(ImGui::GetFontSize() * -12);

    // Menu Bar
    if (ImGui::BeginMenuBar())
    {
//        if (ImGui::BeginMenu("Menu"))
//        {
//            ImGui::EndMenu();
//        }
//        if (ImGui::BeginMenu("Examples"))
//        {
//            ImGui::EndMenu();
//        }
        //if (ImGui::MenuItem("MenuItem")) {} // You can also use MenuItem() inside a menu bar!
        if (ImGui::BeginMenu("Tools"))
        {
#ifndef IMGUI_DISABLE_DEBUG_TOOLS
            const bool has_debug_tools = true;
#else
            const bool has_debug_tools = false;
#endif
            ImGui::MenuItem("Metrics/Debugger", NULL, &show_tool_metrics, has_debug_tools);
            ImGui::MenuItem("Debug Log", NULL, &show_tool_debug_log, has_debug_tools);
            ImGui::MenuItem("ID Stack Tool", NULL, &show_tool_id_stack_tool, has_debug_tools);
            ImGui::MenuItem("Style Editor", NULL, &show_tool_style_editor);
            ImGui::MenuItem("About Dear ImGui", NULL, &show_tool_about);
            ImGui::EndMenu();
        }
        ImGui::EndMenuBar();
    }

    ImGui::Text("dear imgui says hello! (%s) (%d)", IMGUI_VERSION, IMGUI_VERSION_NUM);
    ImGui::Spacing();


    static ImGuiTableFlags flags = ImGuiTableFlags_NoHostExtendX |
//            ImGuiTableFlags_Resizable |
            ImGuiTableFlags_SizingFixedSame |
            ImGuiTableFlags_Reorderable |
            ImGuiTableFlags_Hideable |
            ImGuiTableFlags_BordersOuter |
            ImGuiTableFlags_BordersV;

    if (ImGui::CollapsingHeader("Assignments"))
    {

//        static std::vector<int> table_info;
//        static int temp_val = 0;
//        static tm myDate = {};

        ImGui::Text("hello");

        ImGui::SetNextItemWidth(60.f);
        ImGui::InputInt("##someint", &(temp_val.number), 0);

        ImGui::SameLine();
        ImGui::SetNextItemWidth(190.f);
        ImGui::DateChooser("##datechooser", (temp_val.date_val), "%m/%d/%Y");

//        ImGui::SameLine();
//        ImGui::Text("Chosen date: \"%.2d-%.2d-%.4d\"",myDate.tm_mday,myDate.tm_mon+1,myDate.tm_year+1900);

        ImGui::SameLine();
        static int clicked = 0;
        if (ImGui::Button("Save"))
            clicked++;
        if (clicked & 1)
        {
            table_info.insert(table_info.begin(), temp_val);
            clicked %= 1;
        }
//        ImGui::Text("temp_val: %d", *temp_val);


        if (ImGui::BeginTable("table1", 3, flags))
        {

            ImGui::TableSetupColumn("Nothing");
            ImGui::TableSetupColumn("Due Date");
            ImGui::TableSetupColumn("Delete");
            ImGui::TableHeadersRow();

//            for (int i = 0; i < (*table_info).size(); i++)
            for (int i = 0; i < table_info.size(); i++)
            {
                assignment current_assignment = table_info[i];
                ImGui::TableNextRow();
//                ImGui::TableSetColumnIndex(0);
                for (std::string elem : current_assignment.assignment_details())
                {
                    ImGui::TableNextColumn();
                    ImGui::Text(elem.c_str());
                }

                std::string name = "x##";
                name.append(std::to_string(i));

                ImGui::TableNextColumn();
                if (ImGui::Button(name.c_str())) {
                    table_info.erase(table_info.begin()+i);
                    i--;
                }



//                ImGui::TableSetColumnIndex(0);
//                ImGui::Text("%d", table_info[i].number);
//                ImGui::TableNextColumn();
//                ImGui::
//                ImGui::Text("%.2d-%.2d-%.4d",myDate.tm_mday,myDate.tm_mon+1,myDate.tm_year+1900);
            }
        }
        ImGui::EndTable();
    }

    if (ImGui::CollapsingHeader("Tracker"))
    {
//        if (ImGui::BeginTable("table1", 3, flags))
//        {
//
//            ImGui::TableSetupColumn("Nothing");
//            ImGui::TableSetupColumn("Due Date");
//            ImGui::TableSetupColumn("Delete");
//            ImGui::TableHeadersRow();
//
////            for (int i = 0; i < (*table_info).size(); i++)
//            for (int i = 0; i < table_info.size(); i++)
//            {
//                assignment current_assignment = table_info[i];
//                ImGui::TableNextRow();
////                ImGui::TableSetColumnIndex(0);
//                for (std::string elem : current_assignment.assignment_details())
//                {
//                    ImGui::TableNextColumn();
//                    ImGui::Text(elem.c_str());
//                }
//
//                std::string name = "x##";
//                name.append(std::to_string(i));
//
//                ImGui::TableNextColumn();
//                if (ImGui::Button(name.c_str())) {
//                    table_info.erase(table_info.begin()+i);
//                    i--;
//                }
//
//
//
////                ImGui::TableSetColumnIndex(0);
////                ImGui::Text("%d", table_info[i].number);
////                ImGui::TableNextColumn();
////                ImGui::
////                ImGui::Text("%.2d-%.2d-%.4d",myDate.tm_mday,myDate.tm_mon+1,myDate.tm_year+1900);
//            }
//        }
//        ImGui::EndTable();
    }

//    if (ImGui::CollapsingHeader("Window options"))
//    {
//        if (ImGui::BeginTable("split", 3))
//        {
//            ImGui::TableNextColumn(); ImGui::Checkbox("No titlebar", &no_titlebar);
//            ImGui::TableNextColumn(); ImGui::Checkbox("No scrollbar", &no_scrollbar);
//            ImGui::TableNextColumn(); ImGui::Checkbox("No menu", &no_menu);
//            ImGui::TableNextColumn(); ImGui::Checkbox("No move", &no_move);
//            ImGui::TableNextColumn(); ImGui::Checkbox("No resize", &no_resize);
//            ImGui::TableNextColumn(); ImGui::Checkbox("No collapse", &no_collapse);
//            ImGui::TableNextColumn(); ImGui::Checkbox("No close", &no_close);
//            ImGui::TableNextColumn(); ImGui::Checkbox("No nav", &no_nav);
//            ImGui::TableNextColumn(); ImGui::Checkbox("No background", &no_background);
//            ImGui::TableNextColumn(); ImGui::Checkbox("No bring to front", &no_bring_to_front);
//            ImGui::TableNextColumn(); ImGui::Checkbox("Unsaved document", &unsaved_document);
//            ImGui::EndTable();
//        }
//    }

    // All demo contents
//    ShowDemoWindowWidgets();
//    ShowDemoWindowLayout();
//    ShowDemoWindowPopups();
//    ShowDemoWindowTables();
//    ShowDemoWindowInputs();

    // End of ShowDemoWindow()
    ImGui::PopItemWidth();
    ImGui::End();
}