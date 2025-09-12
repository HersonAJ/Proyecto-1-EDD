cmake_minimum_required(VERSION 4.0)
project(Biblioteca_Magica)

set(CMAKE_CXX_STANDARD 17)
set(CMAKE_AUTOMOC ON)
set(CMAKE_AUTORCC ON)
set(CMAKE_AUTOUIC ON)


find_package(Qt5 COMPONENTS
        Core
        Gui
        Widgets
        REQUIRED)

add_executable(Biblioteca_Magica main.cpp
        GUI/MainWindow.h
        GUI/MainWindow.cpp
        csv/LectorCSV.h
        csv/LectorCSV.cpp
        Modelos/Libro.h
        Modelos/Libro.cpp
        AVL/NodoAVL.h
        AVL/ArbolAVL.h
        AVL/AVL_Calculo.cpp
        AVL/AVL_Insertar.cpp
        AVL/ArbolAVL.cpp
        include/Recorridos.h
        include/ExportadorDOT.h
        AVL/BuscarAVL.cpp)
target_link_libraries(Biblioteca_Magica
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

