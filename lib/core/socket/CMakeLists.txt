file(
	GLOB Sources
	"include/*.h"
	"source/*.cpp"
)

add_library(Socket STATIC ${Sources})

target_include_directories(Socket PUBLIC include)

target_link_libraries(Socket PUBLIC
	WSA
	Utils
)