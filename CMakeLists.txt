cmake_minimum_required(VERSION 3.5)

project(Palletizing VERSION 0.1 LANGUAGES CXX)

set(CMAKE_AUTOUIC ON)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)

set(CMAKE_INCLUDE_CURRENT_DIR ON)


set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

find_package(QT NAMES Qt6 Qt5 REQUIRED COMPONENTS Widgets LinguistTools  Core Gui Sql)
find_package(Qt${QT_VERSION_MAJOR} REQUIRED COMPONENTS Widgets LinguistTools  Core Gui Sql)
find_package(Qt6 REQUIRED COMPONENTS 3DCore 3DExtras 3DInput 3DRender Core Gui Widgets AxContainer)

set(TS_FILES abc_zh_CN.ts)

set(PROJECT_SOURCES
        main.cpp
        mainwindow.cpp
        mainwindow.h
        mainwindow.ui
        ${TS_FILES}
)

if(${QT_VERSION_MAJOR} GREATER_EQUAL 6)
    qt_add_executable(Palletizing
        MANUAL_FINALIZATION
        ${PROJECT_SOURCES}
        goods.h goods.cpp goods.ui
        mygraphicrectitem.h
        mygraphicrectitem.cpp
        newgraphicsview.h
        newgraphicsview.cpp
        single_SKU.h
        single_SKU.cpp
        offline_show_dialog.cpp
        offline_show_dialog.h
        offline_show_dialog.ui
        sqlController.h
        sqlController.cpp
        sql_edit_dialog.h sql_edit_dialog.cpp sql_edit_dialog.ui
        sql_show_widget.h sql_show_widget.cpp sql_show_widget.ui
        plc_com.cpp plc_com.h single_SKU.cpp single_SKU.h
        snap7.h
        snap7.cpp
        viewer_boxes_3d.h viewer_boxes_3d.cpp viewer_boxes_3d.ui
        sql_result_show_widget.h sql_result_show_widget.cpp sql_result_show_widget.ui
        sql_box_show_widget.h sql_box_show_widget.cpp sql_box_show_widget.ui
        sql_box_edit_dialog.h sql_box_edit_dialog.cpp sql_box_edit_dialog.ui
        set_ip_dialog.h set_ip_dialog.cpp set_ip_dialog.ui
        set_db_dialog.h set_db_dialog.cpp set_db_dialog.ui
        sql_key_edit_dialog.h sql_key_edit_dialog.cpp sql_key_edit_dialog.ui



    )
# Define target properties for Android with Qt 6 as:
#    set_property(TARGET Palletizing APPEND PROPERTY QT_ANDROID_PACKAGE_SOURCE_DIR
#                 ${CMAKE_CURRENT_SOURCE_DIR}/android)
# For more information, see https://doc.qt.io/qt-6/qt-add-executable.html#target-creation

    qt_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
else()
    if(ANDROID)
        add_library(Palletizing SHARED
            ${PROJECT_SOURCES}
        )
# Define properties for Android with Qt 5 after find_package() calls as:
#    set(ANDROID_PACKAGE_SOURCE_DIR "${CMAKE_CURRENT_SOURCE_DIR}/android")
    else()
        add_executable(Palletizing
            ${PROJECT_SOURCES}
        )
    endif()

    qt5_create_translation(QM_FILES ${CMAKE_SOURCE_DIR} ${TS_FILES})
endif()


set(SNAP7_ROOT  ${CMAKE_CURRENT_SOURCE_DIR}/include)  # 指定 Snap7 根路径
# 包含 Snap7 的头文件和库目录
set(SNAP7_LIB_DIR "${SNAP7_ROOT}/lib")
set(SNAP7_DLL_DIR "${SNAP7_ROOT}/bin")  # 这里是 DLL 文件所在的目录
link_directories(${SNAP7_LIB_DIR})
target_link_libraries(Palletizing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/lib/snap7.lib)

# 将 DLL 文件复制到构建输出目录
add_custom_command(TARGET Palletizing POST_BUILD
    COMMAND ${CMAKE_COMMAND} -E copy_if_different
    "${SNAP7_DLL_DIR}/snap7.dll"
    $<TARGET_FILE_DIR:Palletizing>)

target_link_libraries(Palletizing PRIVATE Qt${QT_VERSION_MAJOR}::Widgets)
target_link_libraries(Palletizing PRIVATE Qt${QT_VERSION_MAJOR}::Core)
target_link_libraries(Palletizing PRIVATE Qt${QT_VERSION_MAJOR}::Gui)
target_link_libraries(Palletizing PRIVATE Qt${QT_VERSION_MAJOR}::Sql)
target_link_libraries(Palletizing PUBLIC
    Qt::3DCore
    Qt::3DExtras
    Qt::3DInput
    Qt::3DRender
    Qt::Core
    Qt::Gui
    Qt::Widgets
    Qt::AxContainer
)
#target_link_libraries(Palletizing PRIVATE ${CMAKE_CURRENT_SOURCE_DIR}/include/snap7.lib)
# Qt for iOS sets MACOSX_BUNDLE_GUI_IDENTIFIER automatically since Qt 6.1.
# If you are developing for iOS or macOS you should consider setting an
# explicit, fixed bundle identifier manually though.
if(${QT_VERSION} VERSION_LESS 6.1.0)
  set(BUNDLE_ID_OPTION MACOSX_BUNDLE_GUI_IDENTIFIER com.example.Palletizing)
endif()
set_target_properties(Palletizing PROPERTIES
    ${BUNDLE_ID_OPTION}
    MACOSX_BUNDLE_BUNDLE_VERSION ${PROJECT_VERSION}
    MACOSX_BUNDLE_SHORT_VERSION_STRING ${PROJECT_VERSION_MAJOR}.${PROJECT_VERSION_MINOR}
    MACOSX_BUNDLE TRUE
    WIN32_EXECUTABLE TRUE
)

include(GNUInstallDirs)
install(TARGETS Palletizing
    BUNDLE DESTINATION .
    LIBRARY DESTINATION ${CMAKE_INSTALL_LIBDIR}
    RUNTIME DESTINATION ${CMAKE_INSTALL_BINDIR}
)

if(QT_VERSION_MAJOR EQUAL 6)
    qt_finalize_executable(Palletizing)
endif()
