cmake_minimum_required(VERSION 3.3)

project(usbguard-notifier)


include(GNUInstallDirs)

find_package(PkgConfig)
find_package(Threads)

pkg_check_modules(USBGUARD REQUIRED libusbguard)
pkg_check_modules(RSVG REQUIRED librsvg-2.0)
pkg_check_modules(LIBNOTIFY REQUIRED libnotify)

set(SRCSDIR "${CMAKE_CURRENT_SOURCE_DIR}/src")
set(ICONS_DIR "${CMAKE_CURRENT_SOURCE_DIR}/icons")
set(TESTS_DIR "${SRCSDIR}/Tests/Unit")


add_library("${PROJECT_NAME}-serializer" "${SRCSDIR}/Serializer.hpp;${SRCSDIR}/Serializer.cpp")

add_library("lib${PROJECT_NAME}" STATIC "${SRCSDIR}/Notifier.hpp;${SRCSDIR}/Notifier.cpp")
target_link_libraries("lib${PROJECT_NAME}" PRIVATE "${PROJECT_NAME}-serializer" ${LIBNOTIFY_LIBRARIES})
target_include_directories("lib${PROJECT_NAME}" PRIVATE ${LIBNOTIFY_INCLUDE_DIRS} "${PROJECT_NAME}-serializer")
set_target_properties("lib${PROJECT_NAME}" PROPERTIES
	PREFIX ""
)


set(ICON_FILE "${ICONS_DIR}/usbguard-icon.svg")
set(ICON_SOURCE "${CMAKE_CURRENT_BINARY_DIR}/usbguard-icon.cpp")
file(READ "${ICON_FILE}" ICON_TEXT)
file(WRITE "${ICON_SOURCE}" "extern \"C\"{\nextern const char _binary_icons_usbguard_icon_svg_start[];\nextern const char _binary_icons_usbguard_icon_svg_end[];\nconst char _binary_icons_usbguard_icon_svg_start[] = R\"dsfcsdfsdf(${ICON_TEXT})dsfcsdfsdf\";\nconst char _binary_icons_usbguard_icon_svg_end[] = \"\";\n};")
add_library("${PROJECT_NAME}-icon" OBJECT "${ICON_SOURCE}")


set("NOTIFICATION_PATH" "/tmp/usbguard-notifier" CACHE PATH "Path of a temporary notification file")
file(WRITE "${SRCSDIR}/BuildConfig.h" "#define NOTIFICATION_DIR \"${NOTIFICATION_PATH}\"\n")


add_executable("${PROJECT_NAME}" "${SRCSDIR}/NotifyWrapper.hpp;${SRCSDIR}/Log.hpp;${SRCSDIR}/Main.cpp;${SRCSDIR}/NotifyWrapper.cpp;${SRCSDIR}/Log.cpp")
target_include_directories("${PROJECT_NAME}" PRIVATE ${USBGUARD_INCLUDE_DIRS} ${RSVG_INCLUDE_DIRS} ${LIBNOTIFY_INCLUDE_DIRS})
target_link_libraries("${PROJECT_NAME}" PRIVATE "${PROJECT_NAME}-serializer" "lib${PROJECT_NAME}" "${PROJECT_NAME}-icon" ${USBGUARD_LIBRARIES} ${RSVG_LIBRARIES} Threads::Threads)

add_library("lib${PROJECT_NAME}-cli" STATIC "${SRCSDIR}/NotifierCLI.cpp;")
target_include_directories("lib${PROJECT_NAME}-cli" PRIVATE ${USBGUARD_INCLUDE_DIRS} ${LIBNOTIFY_INCLUDE_DIRS})
target_link_libraries("lib${PROJECT_NAME}-cli" PRIVATE "${PROJECT_NAME}-serializer" ${LIBNOTIFY_LIBRARIES})

add_executable("${PROJECT_NAME}-cli" "${SRCSDIR}/MainCli.cpp")
target_include_directories("${PROJECT_NAME}-cli" PRIVATE ${USBGUARD_INCLUDE_DIRS} ${LIBNOTIFY_INCLUDE_DIRS})
target_link_libraries("${PROJECT_NAME}-cli" PRIVATE "${PROJECT_NAME}-serializer" ${LIBNOTIFY_LIBRARIES} "lib${PROJECT_NAME}-cli")

set(SERVICE_FILE_NAME "usbguard-notifier.service")
set(SERVICE_SOURCE_FILE "${CMAKE_CURRENT_SOURCE_DIR}/${SERVICE_FILE_NAME}.in")
set(SERVICE_FILE "${CMAKE_CURRENT_BINARY_DIR}/${SERVICE_FILE_NAME}")

file(READ "${SERVICE_SOURCE_FILE}" SERVICE_SOURCE)
string(REPLACE "%bindir%" "${CMAKE_INSTALL_FULL_BINDIR}" SERVICE_SOURCE "${SERVICE_SOURCE}")
file(WRITE "${SERVICE_FILE}" "${SERVICE_SOURCE}")

string(REPLACE "/${CMAKE_LIBRARY_ARCHITECTURE}" "" INSTALL_LIB_DIR_ARCHIND "${CMAKE_INSTALL_LIBDIR}")
set("INSTALL_SERVICE_DATADIR" "${INSTALL_LIB_DIR_ARCHIND}/systemd/user")
install(FILES "${SERVICE_FILE}"
	DESTINATION "${INSTALL_SERVICE_DATADIR}"
)

install(TARGETS "${PROJECT_NAME}-cli")
install(TARGETS "${PROJECT_NAME}")

set_target_properties("${PROJECT_NAME}-cli" PROPERTIES
	SOVERSION "${CPACK_PACKAGE_VERSION}"
)
set_target_properties("${PROJECT_NAME}" PROPERTIES
	SOVERSION "${CPACK_PACKAGE_VERSION}"
)

option(WITH_TESTS "Build tests" ON)

if(WITH_TESTS)
	find_package(Catch2)
	include(Catch)
	get_target_property(Catch2_INCLUDE_DIRS Catch2::Catch2 INTERFACE_INCLUDE_DIRECTORIES)
	message(STATUS "Catch2_INCLUDE_DIRS ${Catch2_INCLUDE_DIRS}")
	add_executable(test-unit "${TESTS_DIR}/test_notifier_cli.cpp")
	target_link_libraries(test-unit PRIVATE Catch2::Catch2WithMain Catch2::Catch2 "${PROJECT_NAME}-serializer" "lib${PROJECT_NAME}" "lib${PROJECT_NAME}-cli")
	target_include_directories(test-unit PRIVATE "${SRCSDIR}" "${Catch2_INCLUDE_DIRS}/catch2")
	catch_discover_tests(test-unit)
	enable_testing()
endif()


set(CPACK_PACKAGE_NAME "usbguard-notifier")
set(CPACK_PACKAGE_VENDOR "Attila Lakatos")
set(CPACK_PACKAGE_VERSION_MAJOR "0")
set(CPACK_PACKAGE_VERSION_MINOR "0")
set(CPACK_PACKAGE_VERSION_PATCH "6")
set("CPACK_PACKAGE_VERSION" "${CPACK_PACKAGE_VERSION_MAJOR}.${CPACK_PACKAGE_VERSION_MINOR}.${CPACK_PACKAGE_VERSION_PATCH}")


set(CPACK_PACKAGE_DESCRIPTION "A tool for detecting usbguard policy and device presence changes")
set(CPACK_PACKAGE_HOMEPAGE_URL "https://github.com/Cropi/usbguard-notifier")
set(CPACK_PACKAGE_MAINTAINER "${CPACK_PACKAGE_VENDOR}")
set(CPACK_DEBIAN_PACKAGE_MAINTAINER "${CPACK_PACKAGE_MAINTAINER}")
set(CPACK_RESOURCE_FILE_LICENSE "${CMAKE_CURRENT_SOURCE_DIR}/LICENSE")
set(CPACK_RESOURCE_FILE_README "${CMAKE_CURRENT_SOURCE_DIR}/README.md")
set(CPACK_DEB_COMPONENT_INSTALL ON)
set(CPACK_RPM_COMPONENT_INSTALL ON)
set(CPACK_NSIS_COMPONENT_INSTALL ON)
set(CPACK_DEBIAN_COMPRESSION_TYPE "xz")
set(CPACK_DEBIAN_PACKAGE_SHLIBDEPS ON)
set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS ON)
set(CPACK_DEBIAN_PACKAGE_GENERATE_SHLIBS_POLICY ">=")

set("CPACK_DEBIAN_PACKAGE_NAME" "${PROJECT_NAME}")
set("CPACK_RPM_PACKAGE_NAME" "${PROJECT_NAME}")

include(CPack)
