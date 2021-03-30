cmake_minimum_required(VERSION 3.10)
set(COMPONENT_DIRS "main" "../../components" "$ENV{IDF_PATH}/components")
include($ENV{IDF_PATH}/tools/cmake/project.cmake)
project(sleep-hibernate-wakeup)
