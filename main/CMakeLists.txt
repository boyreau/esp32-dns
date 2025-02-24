idf_component_register(
					SRCS
						"src/main.c"

						"src/server.c"
						"src/handler.c"

						"src/dns/dns.c"
						"src/dns/dns_read_labels.c"

						"src/list.c"

						"src/trie/trie.c"

						"src/pstring/pstr8_cat.c"
						"src/pstring/pstr8_join.c"
						"src/pstring/pstr8_len.c"
						"src/pstring/pstr8_rev.c"
                    INCLUDE_DIRS
						"include")

idf_build_set_property(COMPILE_OPTIONS "-Wall" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Wextra" APPEND)
idf_build_set_property(COMPILE_OPTIONS "-Werror" APPEND)
