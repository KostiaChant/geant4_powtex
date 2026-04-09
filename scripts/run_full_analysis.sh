#!/bin/bash
# Full POWTEX analysis pipeline
# This script automates the complete simulation and analysis workflow

set -e  # Exit on error

echo "========================================"
echo "POWTEX Full Analysis Pipeline"
echo "========================================"
echo ""

# Check dependencies
echo "Checking dependencies..."

# Check if ROOT is available
if ! command -v root &> /dev/null; then
    echo "  ✗ ERROR: ROOT is not installed or not in PATH"
    echo ""
    echo "ROOT is required for data analysis. Please install it:"
    echo ""
    echo "Option 1: Install via conda (recommended):"
    echo "  conda activate cxx_geant4"
    echo "  conda install -c conda-forge root"
    echo ""
    echo "Option 2: Install from source:"
    echo "  See: https://root.cern/install/"
    echo ""
    echo "Option 3: Source existing ROOT installation:"
    echo "  source /path/to/root/bin/thisroot.sh"
    echo ""
    exit 1
else
    ROOT_VERSION=$(root-config --version 2>/dev/null || echo "unknown")
    echo "  ✓ ROOT found (version $ROOT_VERSION)"
fi

# Check if Geant4 environment variables are set
if [ -z "$G4ENSDFSTATEDATA" ]; then
    echo "  ⚠ WARNING: Geant4 environment not fully configured"
    echo "    Please activate your Geant4 environment:"
    echo "    conda activate cxx_geant4"
    echo ""
fi

echo ""

# Check if executable exists
if [ ! -f "./build/Powtex" ]; then
    echo "Error: Powtex executable not found in ./build/"
    echo "Please compile first using: ./scripts/compile_Powtex_cmake.sh"
    exit 1
fi

# Check if macro file exists
MACRO_FILE="macros/run.mac"
if [ ! -f "$MACRO_FILE" ]; then
    echo "Error: Macro file $MACRO_FILE not found"
    exit 1
fi

# Step 1: Run Geant4 simulation
echo "Step 1/5: Running Geant4 simulation..."
./build/Powtex "$MACRO_FILE"
if [ $? -eq 0 ]; then
    echo "  ✓ Simulation completed successfully"
else
    echo "  ✗ Simulation failed"
    exit 1
fi
echo ""

# Step 2: Convert raw data to ROOT format
echo "Step 2/5: Converting data to ROOT format..."
if [ ! -f "myfile_info.txt" ]; then
    echo "  ✗ Error: myfile_info.txt not found. Simulation may have failed."
    exit 1
fi
root -l -b -q 'analysis/CreatePowtexTree.C()'
if [ $? -eq 0 ] && [ -f "powtex.root" ]; then
    echo "  ✓ ROOT tree created successfully (powtex.root)"
else
    echo "  ✗ Failed to create ROOT tree"
    exit 1
fi
echo ""

# Step 3: Build lookup table
echo "Step 3/5: Building voxel lookup table..."
if [ ! -f "LookupTablePowtex.txt" ]; then
    echo "  ✗ Error: LookupTablePowtex.txt not found. Simulation may have failed."
    exit 1
fi
root -l -b -q 'analysis/ReadLookupTable.C()'
if [ $? -eq 0 ] && [ -f "powtex_lookup.root" ]; then
    echo "  ✓ Lookup table created successfully (powtex_lookup.root)"
else
    echo "  ✗ Failed to create lookup table"
    exit 1
fi
echo ""

# Step 4: Run physics analysis
echo "Step 4/5: Running physics analysis..."
root -l -b -q 'analysis/analysis_powtex.C()'
if [ $? -eq 0 ] && [ -f "powtex_new_cal.root" ]; then
    echo "  ✓ Physics analysis completed successfully"
    echo "  ✓ Output files:"
    echo "     - powtex.root (updated with powtex_new tree)"
    echo "     - powtex_new_cal.root (calibration data)"
else
    echo "  ✗ Physics analysis failed"
    exit 1
fi
echo ""

# Move output files to data/output directory
echo "Moving output files to data/output/..."
mkdir -p data/output
mv -f powtex.root data/output/ 2>/dev/null || true
mv -f powtex_lookup.root data/output/ 2>/dev/null || true
mv -f powtex_new_cal.root data/output/ 2>/dev/null || true
mv -f myfile_info.txt data/output/ 2>/dev/null || true
mv -f LookupTablePowtex.txt data/output/ 2>/dev/null || true
echo "  ✓ Output files moved to data/output/"
echo ""

# Step 5: Export detections to CSV
echo "Step 5/5: Exporting detections CSV..."
root -l -b -q 'analysis/Extract_branches.C()'
if [ $? -eq 0 ] && [ -f "detections.csv" ]; then
    mv -f detections.csv data/output/
    echo "  ✓ CSV export completed successfully"
    echo "  ✓ CSV file: data/output/detections.csv"
else
    echo "  ✗ Failed to export CSV detections"
    exit 1
fi
echo ""

echo "========================================"
echo "Analysis pipeline completed successfully!"
echo "========================================"
echo ""
echo "Results are in: data/output/"
echo ""
echo "CSV detections file: data/output/detections.csv"
