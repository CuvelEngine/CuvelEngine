//#include "voxelizer.hpp"
#include "voxelizer/voxelizer.hpp"
#include <iostream>


int main(int argc, char *argv[]) {

    if (argc < 3)
    {
        std::cout << "Use " << argv[0] << " file_path resolution" << std::endl;
        exit(1);
    }

    std::string filePath(argv[1]);

    char *str = argv[2];
    while (*str != 0)
    {
        if (!isdigit(*str++))
        {
            std::cout << "The resolution has to be an integer number" << std::endl;
            exit(1);
        }
    }

    int resolution = std::stoi(argv[2]);
    if (resolution < 1 || resolution > 1024)
    {
        std::cout << "The resolution has to be bigger than zero and smaller or equal than 1024" << std::endl;
        exit(1);
    }

    Voxelizer voxeliz = Voxelizer(filePath);
    voxeliz.voxelize(resolution);
}
