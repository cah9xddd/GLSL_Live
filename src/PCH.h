#pragma once
// C++ Standard Library headers
#include <string>
#include <string_view>
#include <vector>
#include <map>
#include <unordered_map>
#include <set>
#include <array>
#include <unordered_set>
#include <stack>
#include <deque>
#include <queue>
#include <bitset>
#include <functional>
#include <thread>
#include <memory>
#include <chrono>
#include <thread>
#include <mutex>
#include <filesystem>
#include <random>
#include <optional>
#include <fstream>
#include <sstream>
#include <iostream>
#include <cassert>
#include <stdexcept>
#include <algorithm>
#include <numeric>

// Third-party library headers
#include <glad/glad.h>  // GLAD MUST BE FIRST
#include <glfw/glfw3.h>
// imgui headers
#include <imgui.h>
#include <imgui_impl_glfw.h>
#include <imgui_impl_opengl3.h>
#include <imgui_stdlib.h>
// glm headers
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include <glm/gtc/quaternion.hpp>
#include <glm/gtc/type_ptr.hpp>

#include <glm/gtc/type_ptr.hpp>
// spdlog headers
#include <spdlog/spdlog.h>
#include <spdlog/async.h>
#include <spdlog/sinks/stdout_color_sinks.h>
#include <spdlog/sinks/basic_file_sink.h>
// Project usefully definitions for spdlog
#define LOG_TRACE(...)    SPDLOG_TRACE(__VA_ARGS__)  // Lowest level
#define LOG_DEBUG(...)    SPDLOG_DEBUG(__VA_ARGS__)
#define LOG_INFO(...)     SPDLOG_INFO(__VA_ARGS__)
#define LOG_WARN(...)     SPDLOG_WARN(__VA_ARGS__)
#define LOG_ERROR(...)    SPDLOG_ERROR(__VA_ARGS__)
#define LOG_CRITICAL(...) SPDLOG_CRITICAL(__VA_ARGS__)  // Highest level

// Magic Enum library
#include <magic_enum/magic_enum.hpp>
