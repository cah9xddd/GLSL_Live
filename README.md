# GLSL_Live

This project enables live editing and rendering of GLSL fragment shaders, inspired by [ShaderToy](https://www.shadertoy.com)

It was created as a learning tool to explore [The book of Shaders](https://thebookofshaders.com).  

This application allows real-time interaction with GLSL shaders, providing an easy-to-use interface for testing and modifying shader code on the fly.

## Features
- Real-time fragment shader editing.
- Interactive GLSL shader rendering.

## Requirements
- CMake
- vcpkg (for dependencies,ensure it's installed and added to the PATH)

## Building the Project

1. **Clone the repository:**
   ```bash
   git clone https://github.com/cah9xddd/GLSL_Live.git
   cd GLSL_Live
   ```
2. **Build the project (ensure u have vcpkg and its added to the PATH).**   
   ```bash   
   cmake -B build
   cmake --build build --config Release
   ```
3. **Go the bin/platform/build_type/project_name/ and run exe. Example:**
  ```bash
  cd ./build/bin/Win64/Release/GLSL_Live.v.1.0.0/
  ./GLSL_Live.exe
  ```


