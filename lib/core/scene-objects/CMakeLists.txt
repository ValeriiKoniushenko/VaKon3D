file(
	GLOB Sources
	"include/*.h"
	"source/*.cpp"
)

add_library(
	Scene-Objects STATIC
	${Sources}
)

target_include_directories(
	Scene-Objects PUBLIC
	include
)

target_link_libraries(
	Scene-Objects PUBLIC
	Utils
	glfw
	Glad
	Logger
	Delegate
	Misc
	Window
	Boost
	FreeType-Wrapper
	Camera
)