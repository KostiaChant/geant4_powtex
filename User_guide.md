# POWTEX Simulation - Usage Guide

## 1. Set Up Mamba Environment

### Initial Setup
You can create the Geant4 environment from the provided configuration file:

```bash
mamba env create -f cpp_environment.yml
```

This installs all required dependencies including:
- Geant4 v11.3.2
- ROOT framework
- CMake build tools
- Compiler tools (gcc, g++, gfortran)

### Activate Environment
Always activate this environment before building or running:

```bash
mamba activate cxx_geant4
```

## 2. Build the Code

Navigate to the project root and run the build script:

```bash
cd /path/to/geant4_powtex
./scripts/compile_Powtex_cmake.sh
```

This script will:
1. Clean the build directory
2. Run CMake configuration
3. Compile with make -j4
4. Copy the executable `Powtex` to the project root if successful

**Output:** Executable file `./Powtex` (and a copy in `./build/`)

## 3. Run the Simulation

### Method 1: Standard Batch Mode
Run with the corresponding macro file:
```bash
./Powtex macros/run.mac
```
You can specify an input file in the `run.mac` as
```mac
/powtex/source/inputFile path/to/the/input_file
```

### Method 2: Specify Custom Input File
Override the input file at runtime, e.g.:

```bash
./Powtex --input-file data/input/VitessDataPOWTEX/noutascii_reduced.dat macros/run.mac
```

Available sample files in `data/input/VitessDataPOWTEX/`:
- `noutascii_reduced.dat` - 100 events (quick test, visualization-friendly)
- `noutascii_biosample_r.dat` - 1.1M events (full dataset, ~8 hours on high-end CPU)
- `isotrop.dat` - trajectories of neutron isotropically scattered off a sphere with a diameter of 1 cm
- `isotrop_reduced.dat` - same as `isotrop.dat` but with smaller number of trajectories.

### Method 3: Interactive Mode (GUI)
For visualization and step-by-step debugging:

```bash
./Powtex
```

At the Geant4 prompt, you can set the input file interactively before running the macro:

```
Idle> /powtex/source/inputFile data/input/VitessDataPOWTEX/noutascii_reduced.dat
Idle> /control/execute macros/run_interactive.mac
```

**Important:** Use `run_interactive.mac` instead of `run.mac` when specifying input interactively. The `run.mac` file contains a hardcoded input file path that will override your interactive setting.

Alternatively, for visualization (input file is specified in macro):

```
Idle> /control/execute macros/vis.mac
```

## 4. Full Analysis Pipeline

Automate the complete simulation and analysis workflow:

```bash
./scripts/run_full_analysis.sh
```

This executes the following steps in sequence:
1. **Simulation** - Runs Geant4 with the `run.mac`
2. **ROOT tree creation** - Converts raw output to ROOT format
3. **Lookup table generation** - Builds voxel position table for TOF correction
4. **Physics analysis** - Processes detector data and applies calibration
5. **CSV export** - Exports detected events to CSV format

Output files are saved to `data/output/`:
- `powtex.root` - Raw detector tree
- `powtex_lookup.root` - Voxel position lookup table
- `powtex_new_cal.root` - Calibrated analysis results
- `detections.csv` - Event detections in CSV format

## 5. Input Data Configuration

The input file can be specified three ways (in order of priority):

1. **Command-line argument** (highest priority):
   ```bash
   ./Powtex --input-file /path/to/file.dat macros/run.mac
   ```

2. **Macro file command**:
   Edit `macros/run.mac` and change:
   ```
   /powtex/source/inputFile data/input/VitessDataPOWTEX/noutascii_reduced.dat
   ```

3. **Interactive UI command**:
   ```
   Idle> /powtex/source/inputFile /path/to/file.dat
   Idle> /control/execute macros/run_interactive.mac
   ```

## 6. Quick Troubleshooting

| Issue | Solution |
|-------|----------|
| `Powtex` command not found | Run from project root: `./Powtex` or full path: `./build/Powtex` |
| G4 environment variables missing | Activate environment: `mamba activate cxx_geant4` |
| ROOT not found | Already included in environment; ensure mamba env is activated |
| Compilation fails | Clean rebuild: `rm -rf build/* && ./scripts/compile_Powtex_cmake.sh` |
| Visualization gets stuck | Use `noutascii_reduced.dat` (100 events) instead of full dataset |

