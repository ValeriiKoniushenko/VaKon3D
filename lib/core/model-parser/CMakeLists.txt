file(
	GLOB Sources
	"include/*.h"
	"source/*.cpp"
)

add_library(
	Model-Parser STATIC
	${Sources}
)

target_include_directories(
	Model-Parser PUBLIC
	include
)

target_link_libraries(
	Model-Parser PUBLIC
	Utils
	glfw
	Glad
	Logger
	Delegate
	Misc
	Window
	FreeType-Wrapper
	Camera
	World
)