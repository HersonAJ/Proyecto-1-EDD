#  Biblioteca Mágica

Sistema de gestión de biblioteca implementado con múltiples estructuras de datos para optimizar las operaciones de búsqueda y gestión de libros.

##  Estructuras del Proyecto

### Estructuras de Datos Implementadas
- **Árbol AVL** (`AVL/`): Para búsqueda eficiente por título
- **Árbol B** (`ArbolB/`): Para búsqueda por rango de fechas
- **Árbol B+** (`ArbolB+/`): Para búsqueda por género literario
- **Índice ISBN** (`AVL_Auxiliar/`): Árbol AVL especializado para búsqueda por ISBN
- **Modelos** (`Models/`): Clases Libro, Listas, Catalogo
- **Interfaz** (`GUI/`): Interfaces gráficas Qt

## 📋 Prerrequisitos

### Software Requerido
- **Qt 5.15+** o **Qt 6.x**
- **CMake 3.16+**
- **Compiler C++17 compatible**
- **Graphviz** (para exportación de visualizaciones)

### Instalación en Ubuntu/Debian
```bash
sudo apt update
sudo apt install qt6-base-dev cmake graphviz g++
```

Instalación en Windows
1. Descargar Qt desde qt.io

2. Instalar CMake desde cmake.org

3. Instalar Graphviz desde graphviz.org



##  Compilación

### Requisitos de Versión (basado en tu CMakeLists.txt)
- **CMake 4.0+**
- **Qt 5** (Core, Gui, Widgets)
- **C++17**
- **Graphviz** (opcional, para exportaciones)

### Comandos de Compilación
```bash
# Crear directorio de build
mkdir build
cd build

# Configurar proyecto
cmake ..

# Compilar
make -j4

# Ejecutar
./Biblioteca_Magica
``` 

## Componentes Qt Requeridos
Qt5::Core

Qt5::Gui

Qt5::Widgets

## Características de Compilación Habilitadas
AUTOMOC - Meta-Object Compiler automático

AUTORCC - Resource compiler automático

AUTOUIC - UI compiler automático

C++17 - Standard habilitado

text


## Estructura de Directorios
```
Biblioteca_Magica/
├── CMakeLists.txt # Configuración CMake (v4.0+)
├── Main.cpp # Punto de entrada
├── README.md # Este archivo
├── AVL/ # Árbol AVL para títulos
│ ├── ArbolAVL.h/cpp
│ ├── NodoAVL.h
│ ├── AVL_Insertar.cpp
│ ├── BuscarAVL.cpp
│ ├── EliminacionAVL.cpp
│ └── ListaEncontrados.h
├── ArbolB/ # Árbol B para fechas
│ ├── ArbolB.h
│ ├── NodoB.h
│ ├── ArbolB_Insertar.cpp
│ ├── ArbolB_Buscar.cpp
│ ├── ArbolB_Eliminar.cpp
│ └── ListaLibros.h/cpp
├── ArbolB+/ # Árbol B+ para géneros
│ ├── ArbolBPlus.h/cpp
│ ├── NodoBPlus.h
│ ├── NodoInterno.h/cpp
│ ├── NodoHoja.h/cpp
│ ├── B+_Insertar.cpp
│ ├── B+_Buscar.cpp
│ └── B+_Eliminar.cpp
├── AVL_Auxiliar/ # Índice ISBN
│ ├── IndiceISBN.h
│ ├── NodoIndiceISBN.h
│ └── ArbolAVL_Auxiliar.cpp
├── GUI/ # Interfaces gráficas
│ ├── MainWindow.h/cpp
│ ├── Vistas/AVL/AVLViewer.h/cpp
│ ├── Vistas/B/BViewer.h/cpp
│ ├── Vistas/B+/BPlusViewer.h/cpp
│ ├── Vistas/Rendimiento/PruebaRendimiento.h/cpp
│ ├── Vistas/OrdenAlfabetico/ListadoAlfabetico.h/cpp
│ └── Vistas/Busqueda/BusquedaUnificada.h/cpp
├── Models/ # Modelos de datos
│ ├── Libro.h/cpp
│ ├── Catalogo.h/cpp
│ └── Nodo.h
├── csv/ # Procesamiento CSV
│ └── LectorCSV.h/cpp
├── include/ # Utilidades y exportadores
│ ├── Recorridos.h
│ ├── ExportadorDOT.h
│ ├── ExportadorDotB.h
│ └── ExportadorDotBPlus.h
└── cmake-build-*/ # Directorios de build (generados)
```

#  Formato de Archivo CSV
El sistema acepta archivos CSV con el siguiente formato:

csv
"Título","ISBN","Género","Año","Autor"

Ejemplo:
csv
"Cien años de soledad","978-84-376-0494-7","Realismo Mágico","1967","Gabriel García Márquez"
"1984","978-84-9759-327-5","Ciencia Ficción","1949","George Orwell"
"Don Quijote","978-84-376-0494-8","Clásico","1605","Miguel de Cervantes"

## Requisitos del formato:
5 campos por línea

Campos entre comillas dobles

ISBN formato: xxx-xx-xxx-xxxx-x

Año como número positivo

##  Funcionalidades
Gestión de Libros
Agregar libros manualmente o por CSV

️ Eliminar libros por ISBN

 Búsquedas avanzadas:

Por título (Árbol AVL)

Por ISBN (Índice ISBN)

Por género (Árbol B+)

Por rango de fechas (Árbol B)

## Visualizaciones
 Visualizar árbol AVL con títulos

 Visualizar árbol B con fechas

️ Visualizar árbol B+ con géneros

 Listado alfabético de todos los libros

## Exportación
 Exportar visualizaciones a SVG/PNG

 Generar archivos DOT para Graphviz

## ️ Uso del Sistema
### Cargar Archivo CSV
Menú Archivo → Cargar archivo CSV

Seleccionar archivo .csv válido

El sistema procesará y cargará los libros

Agregar Libro Manualmente
Menú Libros → Agregar libro

Completar todos los campos obligatorios

ISBN debe seguir formato estándar

## Buscar Libros
Menú Libros → Buscar un libro

Seleccionar tipo de búsqueda

Ingresar término de búsqueda

Los resultados aparecen en tabla ordenada

## Solución de Problemas
Error: "No se pudo abrir el archivo"
Verificar que el archivo existe

Verificar permisos de lectura

Asegurar formato .csv válido

Error: "Formato ISBN inválido"
Usar formato: xxx-xx-xxx-xxxx-x

Ejemplo: 978-84-376-0494-7

Error: "Graphviz no encontrado"
Instalar Graphviz: sudo apt install graphviz

Verificar instalación: dot -V

##  Características Técnicas
Complejidades
Inserción AVL: O(log n)

Búsqueda AVL: O(log n)

Búsqueda Árbol B: O(log n)

Búsqueda Árbol B+: O(log n)

Manejo de Memoria

Destructores virtuales

Manejo de excepciones

Liberación recursiva

