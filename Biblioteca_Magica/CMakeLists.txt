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
        AVL/AVL_Insertar.cpp
        AVL/ArbolAVL.cpp
        include/Recorridos.h
        include/ExportadorDOT.h
        AVL/BuscarAVL.cpp
        AVL/EliminacionAVL.cpp
        ArbolB/NodoB.h
        ArbolB/ArbolB.h
        ArbolB/ArbolB_Insertar.cpp
        include/ExportadorDotB.h
        ArbolB/ArbolB_Buscar.cpp
        include/Nodo.h
        ArbolB/ListaLibros.h
        ArbolB/ListaLibros.cpp
        ArbolB/ArbolB_Eliminar.cpp
        include/ListaRepetidos.h
        include/ListaRepetidos.cpp
        Modelos/Catalogo.h
        Modelos/Catalogo.cpp
        GUI/Vistas/AVL/AVLViewer.h
        GUI/Vistas/AVL/AVLViewer.cpp
        AVL/ListaEncontrados.h
        AVL_Auxiliar/NodoIndiceISBN.h
        AVL_Auxiliar/IndiceISBN.h
        AVL_Auxiliar/ArbolAVL_Auxiliar.cpp
        GUI/Vistas/B/BViewer.h
        GUI/Vistas/B/BViewer.cpp
        ArbolB+/NodoInterno.h
        ArbolB+/NodoHoja.h
        ArbolB+/NodoBPlus.h
        ArbolB+/ArbolBPlus.h
        ArbolB+/B+_Insertar.cpp
        ArbolB+/ArbolBPlus.cpp
        ArbolB+/NodoHoja.cpp
        ArbolB+/NodoInterno.cpp
        include/ExportadorDotBPlus.h
        GUI/Vistas/B+/BPlusViewer.h
        GUI/Vistas/B+/BPlusViewer.cpp
        ArbolB+/B+_Buscar.cpp
        ArbolB+/B+_Eliminar.cpp
        GUI/Vistas/Rendimiento/PruebaRendimiento.h
        GUI/Vistas/Rendimiento/PruebaRendimiento.cpp
        GUI/Vistas/OrdenAlfabetico/ListadoAlfabetico.h
        GUI/Vistas/OrdenAlfabetico/ListadoAlfabetico.cpp
        GUI/Vistas/Busqueda/BusquedaUnificada.h
        GUI/Vistas/Busqueda/BusquedaUnificada.cpp
)
target_link_libraries(Biblioteca_Magica
        Qt5::Core
        Qt5::Gui
        Qt5::Widgets
)

