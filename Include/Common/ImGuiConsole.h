#pragma once
#include <vector>
#include <string>
#include <imgui.h>
#include <mutex>

class ImGuiConsole {
private:
    // Store logged messages
    std::vector<std::string> items;

    // Track if console window is open
    bool isOpen = true;

    // Maximum number of items to keep in history
    const size_t maxItems = 1000;

    // Mutex for thread safety when logging from multiple threads
    std::mutex consoleMutex;

    // Auto-scroll state
    bool autoScroll = true;
    bool scrollToBottom = false;

    // Filter functionality
    char filterBuffer[256] = "";
    bool showInfo = true;
    bool showWarning = true;
    bool showError = true;

public:
    ImGuiConsole() = default;
    ~ImGuiConsole() = default;

    // Log different types of messages
    void LogInfo(const std::string& message) {
        std::lock_guard<std::mutex> lock(consoleMutex);
        items.push_back("[INFO] " + message);
        TrimHistory();
        scrollToBottom = true;
    }

    void LogWarning(const std::string& message) {
        std::lock_guard<std::mutex> lock(consoleMutex);
        items.push_back("[WARNING] " + message);
        TrimHistory();
        scrollToBottom = true;
    }

    void LogError(const std::string& message) {
        std::lock_guard<std::mutex> lock(consoleMutex);
        items.push_back("[ERROR] " + message);
        TrimHistory();
        scrollToBottom = true;
    }

    // Clear all console entries
    void Clear() {
        std::lock_guard<std::mutex> lock(consoleMutex);
        items.clear();
    }

    // Set console visibility
    void SetOpen(bool open) {
        isOpen = open;
    }

    bool IsOpen() const {
        return isOpen;
    }

    void ToggleOpen() {
        isOpen = !isOpen;
    }

    // Render the console window
    void Render() {
        if (!isOpen) {
            return;
        }

		ImGui::SetNextWindowSize(ImVec2(600, 600), ImGuiCond_FirstUseEver);

        if (ImGui::Begin("Console", &isOpen, ImGuiWindowFlags_MenuBar)) {
            // Menu bar with options
            if (ImGui::BeginMenuBar()) {
                if (ImGui::BeginMenu("Options")) {
                    if (ImGui::MenuItem("Clear")) {
                        Clear();
                    }
                    ImGui::Separator();
                    ImGui::MenuItem("Auto-scroll", NULL, &autoScroll);
                    ImGui::EndMenu();
                }

                if (ImGui::BeginMenu("Filters")) {
                    ImGui::MenuItem("Info", NULL, &showInfo);
                    ImGui::MenuItem("Warnings", NULL, &showWarning);
                    ImGui::MenuItem("Errors", NULL, &showError);
                    ImGui::EndMenu();
                }

                ImGui::EndMenuBar();
            }

            // Filter text input
            ImGui::Text("Filter:");
            ImGui::SameLine();
            ImGui::InputText("##ConsoleFilter", filterBuffer, IM_ARRAYSIZE(filterBuffer));
            ImGui::Separator();

            // Console content
            ImGui::BeginChild("ScrollingRegion", ImVec2(0, 0), false, ImGuiWindowFlags_HorizontalScrollbar);

            // Lock mutex while accessing items
            {
                std::lock_guard<std::mutex> lock(consoleMutex);

                // Display filtered items
                for (const auto& item : items) {
                    // Check log level filters
                    bool show = true;
                    if (item.find("[INFO]") != std::string::npos && !showInfo) show = false;
                    if (item.find("[WARNING]") != std::string::npos && !showWarning) show = false;
                    if (item.find("[ERROR]") != std::string::npos && !showError) show = false;

                    // Check text filter
                    if (filterBuffer[0] != '\0' && item.find(filterBuffer) == std::string::npos) {
                        show = false;
                    }

                    // Set colors based on message type
                    if (show) {
                        if (item.find("[ERROR]") != std::string::npos) {
                            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.4f, 0.4f, 1.0f)); // Red for errors
                        }
                        else if (item.find("[WARNING]") != std::string::npos) {
                            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(1.0f, 0.8f, 0.0f, 1.0f)); // Yellow for warnings
                        }
                        else {
                            ImGui::PushStyleColor(ImGuiCol_Text, ImVec4(0.8f, 0.8f, 0.8f, 1.0f)); // Light gray for info
                        }

                        ImGui::TextUnformatted(item.c_str());
                        ImGui::PopStyleColor();
                    }
                }
            }

            // Auto-scroll to bottom
            if (scrollToBottom || (autoScroll && ImGui::GetScrollY() >= ImGui::GetScrollMaxY())) {
                ImGui::SetScrollHereY(1.0f);
                scrollToBottom = false;
            }

            ImGui::EndChild();
        }
        ImGui::End();
    }

private:
    // Keep history under max size
    void TrimHistory() {
        if (items.size() > maxItems) {
            items.erase(items.begin(), items.begin() + (items.size() - maxItems));
        }
    }
};