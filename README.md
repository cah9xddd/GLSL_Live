# GLSL_Live
[![Build Status](https://github.com/cah9xddd/GLSL_Live/actions/workflows/server.yml/badge.svg)]([https://github.com//cah9xddd/GLSL_Live/actions/workflows/cmake-single-platform_windows.yml])

This project enables live editing and rendering of GLSL fragment shaders, inspired by [ShaderToy](https://www.shadertoy.com).

It was created as a learning tool to explore [The Book of Shaders](https://thebookofshaders.com).

This application allows real-time interaction with GLSL shaders, providing an easy-to-use interface for testing and modifying shader code on the fly.

## Features
- Real-time fragment shader editing.
- Interactive GLSL shader rendering.
- Uniforms passed with the same name as on ShaderToy, so you can easily copy and paste to learn.

## Requirements
- CMake
- vcpkg (for managing dependencies, ensure it is installed and added to your PATH)

## Building the Project

1. **Clone the repository:**
   ```bash
   git clone https://github.com/cah9xddd/GLSL_Live.git
   cd GLSL_Live
   ```
2. **Build the project (ensure you have vcpkg and its added to the PATH).**   
   ```bash   
   cmake -B build
   cmake --build build --config Release
   ```
3. **Go the bin/platform/build_type/project_name/ and run exe. Example:**
  ```bash
  cd ./build/bin/Win64/Release/GLSL_Live.v.1.0.0/
  ./GLSL_Live.exe
  ```


