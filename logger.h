#pragma once

#include "imgui\imgui.h"
#include "iHandle.h"
#include <string>

namespace En3rN::DX
{
    class Logger:unPtr(Logger)
    {
        
    public:
        enum class Level{Debug,Info,Warning,Error,Off};
        Logger()
        {
            level = Level::Debug;
            AutoScroll = true;
            Clear();
        }
        Logger(Level loglevel) : level(loglevel)
        {
            AutoScroll = true;
            Clear();
        }

        static void Err(const char* fmt, ...){
            va_list args;
            va_start(args, fmt);
            logger->msgLevel = Logger::Level::Error;
            logger->AddLog(fmt, args);
            va_end(args);
        }; 
        static void Warn(const char* fmt, ...) {
            va_list args;
            va_start(args, fmt);
            logger->msgLevel = Logger::Level::Warning;
            logger->AddLog(fmt,args);
            va_end(args);
        };
        static void Info(const char* fmt, ...){
            va_list args;
            va_start(args, fmt);
            logger->msgLevel = Logger::Level::Info;
            logger->AddLog(fmt,args);
            va_end(args);
        };
        static void Debug(const char* fmt, ...){
            va_list args;
            va_start(args, fmt);
            logger->msgLevel = Logger::Level::Debug;
            logger->AddLog(fmt,args);
            va_end(args);
        };
        static void Draw(const char* title, bool* p_open = NULL)
        {
            if (!ImGui::Begin(title, p_open))
            {
                ImGui::End();
                return;
            }

            // Options menu
            if (ImGui::BeginPopup("Options"))
            {
                const char* items("Debug\0Info\0Warning\0Error\0Off\0");
                int lvl = (int)logger->level;
                if (ImGui::Combo("LogLevel", &lvl, items, 5))
                    logger->level = (Level)lvl;
                ImGui::Checkbox("Auto-scroll", &logger->AutoScroll);
                ImGui::EndPopup();
            }

            // Main window
            if (ImGui::Button("Options"))
                ImGui::OpenPopup("Options");
            ImGui::SameLine();
            if(ImGui::Button("Clear"))
                logger->Clear();
            ImGui::SameLine();
            if(ImGui::Button("Copy"))
               ImGui::LogToClipboard();

            ImGui::SameLine();
            logger->Filter.Draw("Filter", -100.0f);

            ImGui::Separator();
            ImGui::BeginChild("scrolling", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);
                
            
               

            ImGui::PushStyleVar(ImGuiStyleVar_ItemSpacing, ImVec2(0, 0));
            const char* buf = logger->Buf.begin();
            const char* buf_end = logger->Buf.end();
            if (logger->Filter.IsActive())
            {
                // In this example we don't use the clipper when Filter is enabled.
                // This is because we don't have a random access on the result on our filter.
                // A real application processing logs with ten of thousands of entries may want to store the result of
                // search/filter.. especially if the filtering function is not trivial (e.g. reg-exp).
                for (int line_no = 0; line_no < logger->LineOffsets.Size; line_no++)
                {
                    const char* line_start = buf + logger->LineOffsets[line_no];
                    const char* line_end = (line_no + 1 < logger->LineOffsets.Size) ? (buf + logger->LineOffsets[line_no + 1] - 1) : buf_end;
                    if (logger->Filter.PassFilter(line_start, line_end))
                        ImGui::TextUnformatted(line_start, line_end);
                }
            }
            else
            {
                // The simplest and easy way to display the entire buffer:
                //   ImGui::TextUnformatted(buf_begin, buf_end);
                // And it'll just work. TextUnformatted() has specialization for large blob of text and will fast-forward
                // to skip non-visible lines. Here we instead demonstrate using the clipper to only process lines that are
                // within the visible area.
                // If you have tens of thousands of items and their processing cost is non-negligible, coarse clipping them
                // on your side is recommended. Using ImGuiListClipper requires
                // - A) random access into your data
                // - B) items all being the  same height,
                // both of which we can handle since we an array pointing to the beginning of each line of text.
                // When using the filter (in the block of code above) we don't have random access into the data to display
                // anymore, which is why we don't use the clipper. Storing or skimming through the search result would make
                // it possible (and would be recommended if you want to search through tens of thousands of entries).
                ImGuiListClipper clipper;
                clipper.Begin(logger->LineOffsets.Size);
                while (clipper.Step())
                {
                    for (int line_no = clipper.DisplayStart; line_no < clipper.DisplayEnd; line_no++)
                    {
                        const char* line_start = buf + logger->LineOffsets[line_no];
                        const char* line_end = (line_no + 1 < logger->LineOffsets.Size) ? (buf + logger->LineOffsets[line_no + 1] - 1) : buf_end;
                        ImGui::TextUnformatted(line_start, line_end);
                    }
                }
                clipper.End();
            }
            ImGui::PopStyleVar();

            if (logger->AutoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())
                ImGui::SetScrollHereY(1.0f);

            if (logger->LineOffsets.size() > 1000)
            {
                logger->Clear();
            }

            ImGui::EndChild();
            ImGui::End();
        }
        static void SetLevel(Level setloglvl){
            logger->prevLevel = logger->level;
            logger->level = setloglvl;
        }
        static void RestoreLevel(){
            logger->level = logger->prevLevel;
        }
        
    private:
        void    Clear()
        {
            Buf.clear();
            LineOffsets.clear();
            LineOffsets.push_back(0);
        }
        void    AddLog(const char* fmt, va_list args) 
        {
            if (level > msgLevel) { return; }
            std::string label = GetLabel(msgLevel);
            Buf.append(label.c_str());
            int old_size = Buf.size();
            Buf.appendfv(fmt, args);
            Buf.append("\n");
            for (int new_size = Buf.size(); old_size < new_size; old_size++)
                if (Buf[old_size] == '\n')
                    LineOffsets.push_back(old_size + 1);
            
        }
        std::string GetLabel(Level lvl)
        {
            switch (lvl)
            {
            case Logger::Level::Debug:
                return std::string("[Debug] ");
                break;
            case Logger::Level::Info:
                return std::string("[Info] ");
                break;
            case Logger::Level::Warning:
                return std::string("[Warning] ");
                break;
            case Logger::Level::Error:
                return std::string("[Error] ");
                break;
            case Logger::Level::Off:
                return std::string("[Off] ");
                break;
            default:
                return std::string("[ErrDefault] ");
                break;
            }
        }
        ImGuiTextBuffer     Buf;
        ImGuiTextFilter     Filter;
        ImVector<int>       LineOffsets; // Index to lines offset. We maintain this with AddLog() calls.
        bool                AutoScroll;  // Keep scrolling if already at the bottom.
        std::string format;
        Logger::Level prevLevel;
        Logger::Level level;
        Logger::Level msgLevel;
        static Logger::handle logger;
    };

   

    
    

    
}

