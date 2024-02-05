#Beck, B32_TTGOWeatherStation/main/CMakeLists.txt, 12/4/23b
# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS main.c Tasks.h Tasks.cpp B32_TTGOWeatherStation.cpp ani.h Animation.h Orbitron_Medium_20.h
    INCLUDE_DIRS        # optional, add here public include directories
    PRIV_INCLUDE_DIRS   # optional, add here private include directories
    REQUIRES            # optional, list the public requirements (component names)
    PRIV_REQUIRES       # optional, list the private requirements
)
