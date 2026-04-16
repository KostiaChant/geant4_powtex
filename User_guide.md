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
**Note:** The relative path is important when specifying the input file. The instructions below assume execution of the binary from the project root using `./Powtex` or `./build/Powtex`.

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

### Method 3: Interactive Mode (UI) and Visualization
For visualization and step-by-step debugging:

```bash
./Powtex
```

Then execute the visualization macro (which includes a default input file):

```
Idle> /control/execute macros/vis.mac
```

The `vis.mac` file is pre-configured to use `noutascii_reduced.dat` (100 events), which is ideal for interactive visualization.

**Alternative:** For custom input files or different running conditions, you can still set the input file interactively:

```
Idle> /powtex/source/inputFile data/input/VitessDataPOWTEX/noutascii_reduced.dat
Idle> /control/execute macros/run_interactive.mac
```

**Important:** Use `run_interactive.mac` instead of `run.mac` when specifying input interactively. The `run.mac` file contains a hardcoded input file path that will override your interactive setting.

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
   /powtex/source/inputFile /path/to/file.dat
   ```

3. **Interactive UI command**:
   ```
   Idle> /powtex/source/inputFile /path/to/file.dat
   Idle> /control/execute macros/run_interactive.mac
   ```

### 5.1 Input File Format

The input file must be a plain text file with **9 space-separated columns per line**. 
Lines starting with `#` are treated as comments and skipped. Empty lines are also ignored.

| Column | Quantity | Unit | Description |
|--------|----------|------|-------------|
| 1 | Time-of-Flight (ToF) | milliseconds | Travel time of neutron (from its source) as it exits sample volume |
| 2 | Wavelength (λ) | Angstroms (Å) | De Broglie wavelength of neutron |
| 3 | Weight | dimensionless | Statistical weight (count rate) for this event |
| 4 | Position X | cm | VITESS sample coordinate X |
| 5 | Position Y | cm | VITESS sample coordinate Y |
| 6 | Position Z | cm | VITESS sample coordinate Z (beam axis) |
| 7 | Direction X | dimensionless | Unit vector component X |
| 8 | Direction Y | dimensionless | Unit vector component Y |
| 9 | Direction Z | dimensionless | Unit vector component Z |

**Remark**: quantities in the last three columns are called *direction cosines* in Vitess.

**Coordinate System Note:**
VITESS uses X as the beam axis, while Geant4 uses Z as the beam axis. The simulation automatically applies the transformation:
- Geant4 X ← VITESS Z
- Geant4 Y ← VITESS Y
- Geant4 Z ← VITESS X (+ optional sample_shift)

This transformation is applied to both position and direction vectors.

**Example Input File:**
```
# Neutron event data from VITESS
1.234 5.432 1.0 0.1 -0.2 0.05 0.0 0.1 0.995
1.245 5.401 1.2 0.15 -0.25 0.08 0.02 0.15 0.989
1.256 5.378 0.95 0.08 -0.18 0.03 -0.01 0.08 0.997
```

## 6. Quick Troubleshooting

| Issue | Solution |
|-------|----------|
| `Powtex` command not found | Run from project root: `./Powtex` or full path: `./build/Powtex` |
| G4 environment variables missing | Activate environment: `mamba activate cxx_geant4` |
| ROOT not found | Already included in environment; ensure mamba env is activated |
| Compilation fails | Clean rebuild: `rm -rf build/* && ./scripts/compile_Powtex_cmake.sh` |
| Visualization gets stuck | Use `noutascii_reduced.dat` (100 events) instead of full dataset |

