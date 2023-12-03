file(
	GLOB Sources
	"include/*.h"
	"source/*.cpp"
)

add_library(WSA STATIC ${Sources})

target_include_directories(WSA PUBLIC include)

target_link_libraries(WSA PUBLIC
	Utils
	wsock32
	ws2_32
)