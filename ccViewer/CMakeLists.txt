project( ccViewer VERSION 1.38.0 )

include( CMakePolicies NO_POLICY_SCOPE )

include_directories( ${CloudComparePlugins_SOURCE_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR} )
include_directories( ${CMAKE_CURRENT_SOURCE_DIR}/../qCC )
include_directories( ${CMAKE_CURRENT_BINARY_DIR} )

file( GLOB header_list *.h )
file( GLOB source_list *.cpp )

file( GLOB ui_list ui_templates/*.ui )
file( GLOB qrc_list ../qCC/*.qrc )
file( GLOB rc_list ../qCC/*.rc )

qt5_wrap_ui( generated_ui_list ${ui_list} )
qt5_add_resources( generated_qrc_list ${qrc_list} )

if( MSVC )
	# App icon with MSVC
	set( rc_list images/icon/cc_viewer_icon.rc )

	#to get rid of the (system) console
	add_executable( ${PROJECT_NAME} WIN32 ${header_list} ${source_list} ${generated_ui_list} ${generated_qrc_list} ${rc_list}  ${CMAKE_CURRENT_SOURCE_DIR}/../scripts/windows/qt5.natvis )
elseif( APPLE )
	add_executable( ${PROJECT_NAME} MACOSX_BUNDLE ${header_list} ${source_list} ${generated_ui_list} ${generated_qrc_list} ${rc_list} )
else()
	add_executable( ${PROJECT_NAME} ${header_list} ${source_list} ${generated_ui_list} ${generated_qrc_list} ${rc_list} )
endif()

if( CCCORELIB_USE_QT_CONCURRENT )
	# We want deployqt to handle this dependency, so instead of just copying the file
	# link to the target.
	target_link_libraries( ${PROJECT_NAME}
		Qt5::Concurrent
	)
endif()

if( WIN32 )
	DeployQt(
		TARGET
			${PROJECT_NAME}
		DEPLOY_PATH
			${CCVIEWER_DEST_FOLDER}
	)
else()
	DeployQt(
		TARGET
			${PROJECT_NAME}
		DEPLOY_PATH
			${PROJECT_NAME}
	)
endif()

target_link_libraries( ${PROJECT_NAME}
    CCAppCommon
)

# Add custom preprocessor definitions
target_compile_definitions( ${PROJECT_NAME} PUBLIC CC_VIEWER )

if (WIN32)
	if (MSVC)
		set_target_properties( ${PROJECT_NAME} PROPERTIES LINK_FLAGS " /MANIFEST:NO" )
	endif()
endif()

# App icon with Code::Blocks/MinGW
if( WIN32 )
	if( MINGW )
		add_custom_command( TARGET ${PROJECT_NAME} PRE_BUILD COMMAND ${CMAKE_RC_COMPILER} -i ${CMAKE_CURRENT_SOURCE_DIR}/images/icon/cc_viewer_icon.rc --input-format=rc -o ${CMAKE_CURRENT_BINARY_DIR}/ccViewer.res -O coff )
	endif()
endif()

if (NOT APPLE)
	# install program
	install_ext( TARGETS ccViewer ${CCVIEWER_DEST_FOLDER} "" )
endif()

# Auxiliary files
set( auxFiles bin_other/license.txt )

if( WIN32 )
	# Additional auxiliary file(s)
	list( APPEND auxFiles bin_other/start.bat )
endif()

# Install auxiliary files
if(APPLE OR WIN32) # For Linux it's already installed in by qCC
	foreach( filename ${auxFiles} )
		install_ext( FILES ${filename} ${CCVIEWER_DEST_FOLDER} "" )
	endforeach()
endif()

# Put together our Application bundle on macOS
if( APPLE )
	add_subdirectory( Mac )
endif( APPLE )

# Export common shader files to all install destinations
if( WIN32 ) # For Linux it's already installed in by qCC
	install_ext( FILES ${CMAKE_CURRENT_SOURCE_DIR}/../qCC/shaders/ColorRamp/color_ramp.frag ${CCVIEWER_DEST_FOLDER} /shaders/ColorRamp )
endif()

# Install plugins & shaders in the correct folder for each platform
if( APPLE )	
	InstallPlugins(
		TYPES "gl;io"
		DEST_PATH "${CCVIEWER_MAC_PLUGIN_DIR}"
		SHADER_DEST_PATH "${CCVIEWER_MAC_BASE_DIR}/Contents/Shaders"
	)
elseif( UNIX )
	InstallPlugins(
		TYPES "gl;io"
		DEST_PATH "${CMAKE_INSTALL_LIBDIR}/cloudcompare/plugins"
		SHADER_DEST_PATH share/cloudcompare/shaders
	)
else()
	InstallPlugins(
		TYPES "gl;io"
		DEST_PATH "${CCVIEWER_DEST_FOLDER}"
		DEST_FOLDER "plugins"
		SHADER_DEST_PATH "${CCVIEWER_DEST_FOLDER}"
		SHADER_DEST_FOLDER "shaders"
)
endif()

# Add a 'fake' custom target to force the recompilation of ccApplication.ccp in order to
# always get an up-to-date version number (based on the current date)
add_custom_target(CC_VIEWER_UPDATE_VERSION ALL COMMAND ${CMAKE_COMMAND} -E touch_nocreate ${CMAKE_CURRENT_SOURCE_DIR}/ccViewerApplication.cpp)