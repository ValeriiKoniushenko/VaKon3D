file(
	GLOB Sources
	"include/*.h"
	"source/*.cpp"
)

add_library(Editor-Integration STATIC ${Sources})

target_include_directories(Editor-Integration PUBLIC include)

target_link_libraries(Editor-Integration PUBLIC
	WSA
	Utils
	Core-Wrappers
	Socket
	Console
)