cmake_minimum_required(VERSION 3.10)

project(ProjALG)

# Nastavenie kompilátora a štandardu C++
set(CMAKE_CXX_STANDARD 17)
set(CMAKE_CXX_STANDARD_REQUIRED True)

# Ak používaš Mac s Apple Silicon (M1/M2/M3), nastav správnu architektúru
if(APPLE)
    set(CMAKE_OSX_ARCHITECTURES "arm64")
endif()

# Ak chceš použiť LLVM Clang namiesto predvoleného Clangu od Apple (voliteľné)
if(APPLE)
    # Nastavenie kompilátora na správnu cestu pre LLVM
    set(CMAKE_C_COMPILER /usr/local/opt/llvm/bin/clang)
    set(CMAKE_CXX_COMPILER /usr/local/opt/llvm/bin/clang++)

endif()

# Pridanie zdrojových súborov do projektu
add_executable(projekt
        main.cpp
        Set.cpp
        Loader.cpp
        KMeansDiscretization.cpp
        Set.cpp
        CMeansDiscretization.cpp
)

# Pre správnu kompatibilitu so štandardnou knižnicou na Macu (voliteľné)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -stdlib=libc++")


