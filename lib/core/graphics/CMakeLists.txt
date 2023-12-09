file(
	GLOB Sources
	"include/*.h"
	"source/*.cpp"
)

add_library(
	Graphics STATIC
	${Sources}
)

target_include_directories(
	Graphics PUBLIC
	include
)

target_link_libraries(
	Graphics PUBLIC
	Utils
	glfw
	Glad
	Logger
	Delegate
	Misc
	Window
	FreeType-Wrapper
	Camera
)