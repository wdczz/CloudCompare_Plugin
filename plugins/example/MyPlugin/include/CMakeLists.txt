
target_sources( ${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_LIST_DIR}/ActionA.h
		${CMAKE_CURRENT_LIST_DIR}/MyPlugin.h
		${CMAKE_CURRENT_LIST_DIR}/samdialog.h
)

target_include_directories( ${PROJECT_NAME}
	PRIVATE
		${CMAKE_CURRENT_SOURCE_DIR}
)
