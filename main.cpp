#include <iostream>
#include <armadillo>
#include <fstream>

using namespace std;
using namespace arma;

void writeLammpsFile() {
    ivec atomTypes;
    atomTypes << 14 << 8 << 14; // Silicon, oxygen, silicon
    cout << atomTypes << endl;
    mat positions;
    positions << 1.0 << 2.5 << -1.5 << endr
              << 4.0 << -5.0 << 6.0 << endr
              << 7.0 << 2.0 << -3.0 << endr;
    cout << positions << endl;
    ofstream lammpsFile("outfile.lmp", ios::out | ios::binary);
    // Set up data for the timestep of this file
    int currentTimeStep = 0;
    int nAtomsTotal = positions.n_rows;
    // The system boundaries
    double xMin = -10.0;
    double xMax = 10.0;
    double yMin = -10.0;
    double yMax = 10.0;
    double zMin = -10.0;
    double zMax = 10.0;
    // Shearing is zero unless the system boundaries are sheared (yes that's "sheared",
    // not "shared")
    double xShear = 0.0;
    double yShear = 0.0;
    double zShear = 0.0;
    // nColumns is the number of data types you want to write. In our case we want to
    // write four - the atom type and the x, y and z components of the position.
    // If you want velocities, forces, etc., just add more columns and write more data.
    int nColumns = 1 + 3;
    // We could divide the data into chunks by the LAMMPS file format, but we don't - i.e. only
    // use one chunk. The chunk length is then the same as the number of atoms times the number
    // of columns.
    int nChunks = 1;
    int chunkLength = nAtomsTotal * nColumns;

    // Write all the above to the lammps file
    lammpsFile.write(reinterpret_cast<const char*>(&currentTimeStep), sizeof(int));
    lammpsFile.write(reinterpret_cast<const char*>(&nAtomsTotal), sizeof(int));
    lammpsFile.write(reinterpret_cast<const char*>(&xMin), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&xMax), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&yMin), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&yMax), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&zMin), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&zMax), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&xShear), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&yShear), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&zShear), sizeof(double));
    lammpsFile.write(reinterpret_cast<const char*>(&nColumns), sizeof(int));
    lammpsFile.write(reinterpret_cast<const char*>(&nChunks), sizeof(int));
    lammpsFile.write(reinterpret_cast<const char*>(&chunkLength), sizeof(int));

    // Write all the data for each atom to file
    for(uint i = 0; i < positions.n_rows; i++) {
        // IMPORTANT: Even atom numbers are usually integers, they must be written
        // as double according to the LAMMPS standard.
        double atomType = atomTypes(i);
        lammpsFile.write(reinterpret_cast<const char*>(&atomType), sizeof(double));
        // Write the x, y and z-components
        lammpsFile.write(reinterpret_cast<const char*>(&positions(i,0)), sizeof(double));
        lammpsFile.write(reinterpret_cast<const char*>(&positions(i,1)), sizeof(double));
        lammpsFile.write(reinterpret_cast<const char*>(&positions(i,2)), sizeof(double));
    }
    lammpsFile.close();
}

int main()
{
    writeLammpsFile();
    return 0;
}

