#include "TaskOperations.h"
#include <iostream>
#include <fstream>
#include <ctime>

std::string data_json_path;
std::string issued_ID_path;
std::string temp_issued_ID_path;

using json = nlohmann::json;

void create_config_paths() {
    const char* xdg_config_dir = getenv("XDG_CONFIG_DIR");
    const char* home_dir = getenv("HOME");
    if(xdg_config_dir) {
        data_json_path.append(xdg_config_dir);
        data_json_path.append("/tasky/data.json");
        issued_ID_path.append(xdg_config_dir);
        issued_ID_path.append("/tasky/issued_ID.txt");
        temp_issued_ID_path.append(xdg_config_dir);
        temp_issued_ID_path.append("/temp_issued_ID.txt");
    }
    else {
        data_json_path.append(home_dir);
        data_json_path.append("/.config/tasky/data.json");
        issued_ID_path.append(home_dir);
        issued_ID_path.append("/.config/tasky/issued_ID.txt");
        temp_issued_ID_path.append(home_dir);
        temp_issued_ID_path.append("/.config/tasky/temp_issued_ID.txt");
    }
}

bool is_file_empty(const std::string& filename) {
    std::ifstream file(filename);
    file.seekg(0, std::ios::end);
    bool empty = file.tellg() == 0;
    file.close();
    return !empty;
}

int task_id_generate() {
    std::fstream issued_ID(issued_ID_path);
    int max_id = 0;
    std::string line;
    while (std::getline(issued_ID, line)) {
        max_id = std::max(max_id, std::stoi(line));
    }
    issued_ID.close();
    
    issued_ID.open(issued_ID_path, std::ios::app);
    issued_ID << (max_id + 1) << std::endl;
    issued_ID.close();
    return max_id + 1;
}

void add_task(const char* task_description) {
    if(!task_description) {
        std::cout << "Please, enter a task name\n";
        return;
    }

    auto createdAt_time_t = time(0);
    std::string createdAt = std::ctime(&createdAt_time_t);
    createdAt.erase(createdAt.end() - 1);

    json existing_data;
    std::ifstream input(data_json_path);
    
    // Check if file is empty
    input.seekg(0, std::ios::end);
    if(input.tellg() == 0) {
        // Initialize with empty tasks array if file is empty
        existing_data["tasks"] = json::array();
    } else {
        // Reset position and parse JSON
        input.seekg(0);
        input >> existing_data;
    }
    input.close();

    int current_task_id = task_id_generate();

    json new_task = {
        {"id", current_task_id},
        {"description", task_description},
        {"created-at", createdAt},
        {"status", "in-progress"}
    };

    existing_data["tasks"].push_back(new_task);
    
    std::ofstream output(data_json_path);
    output << existing_data.dump(4);
    output.close();

    std::cout << "Task has been created (ID: " << current_task_id << " )\n";
} 