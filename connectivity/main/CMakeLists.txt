set(MAIN_SRCS
    "main.c"
    "test_shared_memory.c"
    "test_token_ring.c"
)

set(MAIN_INCLUDE_DIRS
    "include"
)

set(MAIN_REQUIRES
    access_point
    device
    routing
    server
    shared_memory
    spi
    station
    tcp_client
    token_ring
)

idf_component_register(
    SRCS
        ${MAIN_SRCS}
    INCLUDE_DIRS
        ${MAIN_INCLUDE_DIRS}
    REQUIRES
        ${MAIN_REQUIRES}
)
