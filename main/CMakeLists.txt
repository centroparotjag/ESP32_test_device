# See the build system documentation in IDF programming guide
# for more information about component CMakeLists.txt files.

idf_component_register(
    SRCS main.c config_GPIO.c foo_I2C.c DHT11.c WS2812D.c DS18B20.c statistics.c BMP180.c LSM303DLHC.c measurement_physical_quantities.c # list the source files of this component
    INCLUDE_DIRS        # optional, add here public include directories
    PRIV_INCLUDE_DIRS   # optional, add here private include directories
    REQUIRES            # optional, list the public requirements (component names)
    PRIV_REQUIRES       # optional, list the private requirements
)