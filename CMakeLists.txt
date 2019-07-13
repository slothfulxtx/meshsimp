cmake_minimum_required(VERSION 3.2)
PROJECT(mesh_simp)

add_definitions("-O2 -g -std=c++11")

INCLUDE_DIRECTORIES(
include
)

#source directory
AUX_SOURCE_DIRECTORY(src DIR_SRCS)

#set environment variable
SET(MESH_SIMP ${DIR_SRCS})

ADD_EXECUTABLE(${PROJECT_NAME} ${MESH_SIMP})
