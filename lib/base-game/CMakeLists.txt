file(
	GLOB Sources
	"include/*.h"
	"source/*.cpp"
)

add_library(
	Base-Game STATIC
	${Sources}
)

target_include_directories(Base-Game PUBLIC include/)

target_link_libraries(Base-Game PUBLIC
	Utils
)