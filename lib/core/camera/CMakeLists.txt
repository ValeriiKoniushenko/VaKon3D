file(
	GLOB Sources
	"include/*.h"
	"source/*.cpp"
)

add_library(
	Camera STATIC
	${Sources}
)

target_include_directories(
	Camera PUBLIC
	include
)

target_link_libraries(
	Camera PUBLIC
	Utils
	glfw
	Glad
	Logger
	Delegate
	Misc
	Window
	FreeType-Wrapper
	Scene-Objects
)