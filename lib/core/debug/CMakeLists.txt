file(
	GLOB Sources
	"include/*.h"
	"source/*.cpp"
)

add_library(
	Debug STATIC
	${Sources}
)

target_include_directories(
	Debug PUBLIC
	include
)

target_link_libraries(
	Debug PUBLIC
	Utils
	Logger
)