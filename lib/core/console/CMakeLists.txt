file(
	GLOB Sources
	"include/*.h"
	"source/*.cpp"
)

add_library(
	Console STATIC
	${Sources}
)

target_include_directories(
	Console PUBLIC
	include
)

target_link_libraries(
	Console PUBLIC
	Utils
	glfw
	Glad
	Logger
	Delegate
	Misc
	Window
	Boost
	freetype
	Editor-Integration
	Scene-Objects
)