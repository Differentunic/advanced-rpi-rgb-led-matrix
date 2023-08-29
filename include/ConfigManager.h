#ifndef CONFIG_MANAGER_H
#define CONFIG_MANAGER_H

#include <string>
#include <led-matrix.h>

class ConfigManager {
public:
    ConfigManager();
    ~ConfigManager();

    void loadOrCreateConfig();

    std::string getMode();
    RGBMatrix::Options getMatrixOptions();

private:
    std::string mode;
    RGBMatrix::Options matrixOptions;
};

#endif // CONFIG_MANAGER_H
