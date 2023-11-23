file(
	GLOB Sources
	"include/*.h"
	"source/*.cpp"
)

add_library(
	Input-Devices STATIC
	${Sources}
)

target_include_directories(
	Input-Devices PUBLIC
	include
)

target_link_libraries(
	Input-Devices PUBLIC
	Glm
	Window
)