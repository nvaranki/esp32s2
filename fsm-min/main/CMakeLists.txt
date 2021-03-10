idf_component_register(
                    SRCS 
                        "ulp_example_main.cpp"
                        "MicroControllerUnit.cpp"
                        "PowerManagementUnit.cpp"
                        "SleepAndWakeupController.cpp"
                        "RegisterRO.cpp"
                        "RegisterRW.cpp"
                        "ValueRO.cpp"
                        "ValueRW.cpp"
                    INCLUDE_DIRS ""
                    REQUIRES soc nvs_flash ulp)

#add_subdirectory(esp32s2)
#list(APPEND EXTRA_LIBS esp32s2)
#list(APPEND EXTRA_COMPONENT_DIRS esp32s2)
#
# ULP support additions to component CMakeLists.txt.
#
# 1. The ULP app name must be unique (if multiple components use ULP).
set(ulp_app_name ulp_${COMPONENT_NAME})
#
# 2. Specify all assembly source files.
#    Files should be placed into a separate directory (in this case, ulp/),
#    which should not be added to COMPONENT_SRCS.
set(ulp_s_sources "ulp/pulse_cnt.S" "ulp/wake_up.S")
#
# 3. List all the component source files which include automatically
#    generated ULP export file, ${ulp_app_name}.h:
set(ulp_exp_dep_srcs "ulp_example_main.cpp")
#
# 4. Call function to build ULP binary and embed in project using the argument
#    values above.
ulp_embed_binary(${ulp_app_name} "${ulp_s_sources}" "${ulp_exp_dep_srcs}")
