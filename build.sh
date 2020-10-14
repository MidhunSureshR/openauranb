# openauranb - Build script
# <rmidhunsuresh@gmail.com>

# Create output directory
echo -e "Creating bin directory\n"
mkdir -p bin
cd bin

echo -e "Trying to initialize cmake ...\n"
# Initialize cmake
if ! command cmake ../ 
then
    echo "You don't have cmake installed."
    echo "cmake is needed to compile openauranb."
    exit
fi

echo -e "\nBuilding project ...\n"
cmake --build . --target openauranb -- -j 6

