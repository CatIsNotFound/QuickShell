cmake_minimum_required(VERSION 3.19)
project(QuickShell LANGUAGES CXX)

find_package(Qt6 6.5 REQUIRED COMPONENTS Core Widgets LinguistTools)

qt_standard_project_setup()

set(CMAKE_AUTORCC ON)

qt_add_executable(QuickShell
    WIN32 MACOSX_BUNDLE
    main.cpp
    GUI/mainwindow.cpp
    GUI/mainwindow.h
    GUI/mainwindow.ui
    GUI/preference.h GUI/preference.cpp GUI/preference.ui
    GUI/aboutapp.h GUI/aboutapp.cpp GUI/aboutapp.ui

    Libs/settings.h Libs/settings.cpp
    Libs/Version.h
    Libs/scriptscollection.h Libs/scriptscollection.cpp
    GUI/findterminal.h GUI/findterminal.cpp GUI/findterminal.ui
    assets.qrc
    Libs/termslist.h Libs/termslist.cpp

)

set(I18N_FILES
    i18n/QuickShell_zh_CN.ts
    i18n/QuickShell_zh_TW.ts
    i18n/QuickShell_zh_HK.ts
    i18n/QuickShell_en_US.ts
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets/scripts $<TARGET_FILE_DIR:${PROJECT_NAME}>/scripts
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets/themes $<TARGET_FILE_DIR:${PROJECT_NAME}>/themes
)

add_custom_command(TARGET ${PROJECT_NAME} POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_directory
    ${CMAKE_SOURCE_DIR}/assets/conf $<TARGET_FILE_DIR:${PROJECT_NAME}>/conf
)

qt_add_translations(QuickShell TS_FILES ${I18N_FILES})

target_link_libraries(QuickShell
    PRIVATE
        Qt::Core
        Qt::Widgets
)

include(GNUInstallDirs)

install(TARGETS QuickShell
    BUNDLE  DESTINATION .
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
)

qt_generate_deploy_app_script(
    TARGET QuickShell
    OUTPUT_SCRIPT deploy_script
    NO_UNSUPPORTED_PLATFORM_ERROR
)
install(SCRIPT ${deploy_script})
