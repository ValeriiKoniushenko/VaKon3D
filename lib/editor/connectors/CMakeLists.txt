file(
	GLOB Sources
	"include/*.h"
	"source/*.cpp"
)

add_library(Connectors STATIC ${Sources})

target_include_directories(Connectors PUBLIC include)

target_link_libraries(Connectors PUBLIC
	WSA
	Utils
	Core-Wrappers
	Boost
	Socket
	Console
	Editor-Integration
)