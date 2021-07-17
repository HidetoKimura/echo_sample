cmake_minimum_required(VERSION 3.1)

# define a variable of project name
set( project_name "Echo Sample")

# define a project name
project (${project_name} C)

# define executable
add_executable(echo_sample main.c)