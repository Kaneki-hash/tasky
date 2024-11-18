#pragma once
#include <string>
#include "json-library/single_include/nlohmann/json.hpp"

extern std::string data_json_path;
extern std::string issued_ID_path;
extern std::string temp_issued_ID_path;

void create_config_paths();
bool is_file_empty(const std::string& filename);
int task_id_generate();
void add_task(const char* task_description); 