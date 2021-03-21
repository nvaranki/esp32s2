cmake_minimum_required(VERSION 3.10)

idf_component_register(
	SRC_DIRS "src" "src/bits"
	INCLUDE_DIRS "include" "src"
	REQUIRES ulp
	REQUIRED_IDF_TARGETS esp32s2)

target_link_libraries(${COMPONENT_LIB})
