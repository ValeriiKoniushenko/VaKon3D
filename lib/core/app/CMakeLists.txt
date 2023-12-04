file(
	GLOB Sources
	"include/*.h"
	"source/*.cpp"
)

add_library(App STATIC ${Sources})

target_include_directories(App PUBLIC include)

target_link_libraries(App PUBLIC
	WSA
	Utils
	Core-Wrappers
	Boost
	Editor-Integration
)