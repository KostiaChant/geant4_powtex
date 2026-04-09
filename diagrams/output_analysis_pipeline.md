# POWTEX Output And Analysis Pipeline

```mermaid
flowchart TB
  subgraph CFG["Primary input configuration paths"]
    CLI["Command line\n--input-file/-f <path>"]
    MAC["Macro command\n/powtex/source/inputFile <path>"]
  end

  subgraph USER_SIM["Project-specific simulation code (this repository)"]
    MAIN["Powtex.cc"]
    DET["PowtexConstruction + PowtexMantelConstruct.icc"]
    STEP["SteppingAction"]
    PGA["PrimaryGeneratorAction"]
    PG["PrimaryGenerator"]
    PGM["PrimaryGeneratorMessenger"]
    SRC["VITESS input\n*.dat"]
    MOVE14["Archive step\nmv outputs to data/output/"]
    MOVE5["CSV move\nmv detections.csv data/output/"]
  end

  subgraph G4["Geant4 default package components"]
    RM["G4RunManager"]
    PHYS["QGSP_BERT_HP"]
    KERNEL["Geant4 event/track stepping kernel"]
  end

  subgraph RAW["Runtime artifacts (workspace root)"]
    HITTXT["myfile_info.txt"]
    LOOKUPTXT["LookupTablePowtex.txt"]
    ROOT1_TMP["powtex.root\n(powtex_ev + powtex_new)"]
    ROOT2_TMP["powtex_lookup.root\n(lookup_tree)"]
    ROOT3_TMP["powtex_new_cal.root"]
    CSV_TMP["detections.csv"]
  end

  subgraph ROOT_PIPE["ROOT analysis scripts (project code)"]
    S1["analysis/CreatePowtexTree.C"]
    S2["analysis/ReadLookupTable.C"]
    S3["analysis/analysis_powtex.C"]
    S4["analysis/Extract_branches.C"]
  end

  subgraph PRODUCTS["Final files in data/output/"]
    HITTXT_OUT["data/output/myfile_info.txt"]
    LOOKUPTXT_OUT["data/output/LookupTablePowtex.txt"]
    ROOT1_OUT["data/output/powtex.root"]
    ROOT2_OUT["data/output/powtex_lookup.root"]
    ROOT3_OUT["data/output/powtex_new_cal.root"]
    CSV_OUT["data/output/detections.csv"]
  end

  BATCH["scripts/run_full_analysis.sh"]

  CLI --> MAIN
  MAIN --> RM
  MAIN --> PGA
  MAIN -.executes macro file.-> MAC

  RM --> PHYS
  RM --> DET
  RM --> PGA
  RM --> STEP

  PGA --> PG
  PG -.owns.-> PGM
  MAC --> PGM
  PGM --> PG
  SRC --> PG

  PHYS --> KERNEL
  DET --> KERNEL
  PG --> KERNEL
  KERNEL --> STEP

  STEP --> HITTXT
  DET --> LOOKUPTXT

  HITTXT --> S1
  LOOKUPTXT --> S2
  S1 --> ROOT1_TMP
  S2 --> ROOT2_TMP
  ROOT1_TMP --> S3
  ROOT2_TMP --> S3
  S3 --> ROOT1_TMP
  S3 --> ROOT3_TMP

  ROOT1_TMP --> MOVE14
  ROOT2_TMP --> MOVE14
  ROOT3_TMP --> MOVE14
  HITTXT --> MOVE14
  LOOKUPTXT --> MOVE14

  MOVE14 --> ROOT1_OUT
  MOVE14 --> ROOT2_OUT
  MOVE14 --> ROOT3_OUT
  MOVE14 --> HITTXT_OUT
  MOVE14 --> LOOKUPTXT_OUT

  ROOT1_OUT --> S4
  ROOT3_OUT --> S4
  S4 --> CSV_TMP
  CSV_TMP --> MOVE5
  MOVE5 --> CSV_OUT

  BATCH --> MAIN
  BATCH --> S1
  BATCH --> S2
  BATCH --> S3
  BATCH --> S4
  BATCH --> MOVE14
  BATCH --> MOVE5

  classDef user fill:#ffe9c9,stroke:#cc7a00,color:#2a1a00,stroke-width:1.5px;
  classDef g4 fill:#d9ecff,stroke:#2f6fad,color:#0b2a44,stroke-width:1.5px;
  classDef data fill:#fff7cc,stroke:#b89b00,color:#3a3000,stroke-width:1.5px;
  classDef cfg fill:#ececec,stroke:#5a5a5a,color:#202020,stroke-width:1.5px;

  class MAIN,DET,STEP,PGA,PG,PGM,SRC,S1,S2,S3,S4,BATCH,MOVE14,MOVE5 user;
  class RM,PHYS,KERNEL g4;
  class HITTXT,LOOKUPTXT,ROOT1_TMP,ROOT2_TMP,ROOT3_TMP,CSV_TMP,HITTXT_OUT,LOOKUPTXT_OUT,ROOT1_OUT,ROOT2_OUT,ROOT3_OUT,CSV_OUT data;
  class CLI,MAC cfg;
```

Legend:
- Orange nodes: explicitly coded in this repository.
- Blue nodes: Geant4 components provided by default.
- Yellow nodes: generated data artifacts (both temporary and final stored outputs).
- Gray nodes: runtime input-configuration commands for primary source selection.
- Precedence rule: command-line --input-file/-f value is locked and cannot be overridden later by macro commands.
- Step ordering in scripts/run_full_analysis.sh: steps 1-4 create/update ROOT files, then files are moved to data/output/, then Extract_branches.C reads from data/output and exports detections.csv.
