idf_component_register(SRCS "timestamp.cpp" "timestamp_format.cpp"
                       INCLUDE_DIRS "."
                       REQUIRES message
                       mqtt_client)