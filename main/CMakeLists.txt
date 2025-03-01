idf_component_register(
					SRCS
						"src/main.c"

						"src/server.c"
						"src/handler.c"

						"src/list.c"

						"src/trie/trie.c"

                    INCLUDE_DIRS
						"include"
					REQUIRES
						"esp_event"
						"esp_netif"
						"nvs_flash"
						"dns"
					)

idf_build_set_property(COMPILE_OPTIONS "-Wall" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Wextra" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Werror" APPEND)

