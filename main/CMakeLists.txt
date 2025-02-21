idf_component_register(
					SRCS "src/udp_server.c" "src/dns.c" "src/server.c" "src/list.c"
                    INCLUDE_DIRS "include")

idf_build_set_property(COMPILE_OPTIONS "-Wall" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Wextra" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Werror" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-O1" APPEND)
