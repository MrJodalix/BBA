# Mit diesen Einstellungen soll Kompiliert werden. Änderungen sind nicht zulässig.
cmake_minimum_required(VERSION 3.1) 
project(bba)
find_package(OpenCV REQUIRED core highgui imgproc imgcodecs videoio)
file(GLOB SOURCE_FILES src/*) # dies ist eine schlechte Idee – CMake kann nicht feststellen, ob Dateien fehlen
add_executable(bba ${SOURCE_FILES})
set_property(TARGET bba PROPERTY CXX_STANDARD 17) # benötigt hinreichend neuen Compiler, z.B. MSVC 19.29.x aus MSVS 2019 6.11.x
target_include_directories(bba PRIVATE ${OpenCV_INCLUDE_DIRS})
target_link_libraries(bba ${OpenCV_LIBS})
if (CMAKE_CXX_COMPILER_ID MATCHES "Clang|GNU")
  target_compile_options(bba PRIVATE -Werror -Wall -Wextra) # alle Warnungen aktivieren
  target_compile_options(bba PRIVATE -Wvla) # C-style variable length arrays verbieten
  target_compile_options(bba PRIVATE -Wsign-compare)
  #target_compile_options(bba PRIVATE -Wconversion) # zu nervig
  #target_compile_options(bba PRIVATE -Wsign-conversion) # zu nervig
  target_compile_options(bba PRIVATE -fno-operator-names) # or und and funktionieren in MSVC nicht
elseif (MSVC)
  target_compile_options(bba PRIVATE /we4061) # fehlender case in switch mit default
  target_compile_options(bba PRIVATE /we4062) # fehlender case in switch ohne default
  target_compile_options(bba PRIVATE /we4388) # Vergleiche signed/unsigned (größer und kleiner)
  target_compile_options(bba PRIVATE /we4389) # Vergleiche signed/unsigned (Gleichheit und Ungleichheit)
  message(WARNING "Dies ist der MSVC. Nicht alle Warnungen sind aktiviert.")
else()
  message(WARNING "Dies ist kein bekannter Compiler. Warnungen sind nicht aktiviert.")
endif()