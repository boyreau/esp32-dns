idf_component_register(
					SRCS
						"src/main.c"

						"src/dns_server/server.c"
						"src/dns_server/handler.c"

						"src/web_server/server.c"
						"src/web_server/get_index.c"
						"src/web_server/post_index.c"
                    INCLUDE_DIRS
						"include"
					REQUIRES
						"mdns"
						"esp_http_server"
						"esp_event"
						"esp_netif"
						"nvs_flash"
						"dns"
						"pstring"
						"trie"
					EMBED_TXTFILES
						"index.html"
					)

idf_build_set_property(COMPILE_OPTIONS "-Wall" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Wextra" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Werror" APPEND)
