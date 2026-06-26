Introduction to CMSSW: http://cms-sw.github.io

---

# LST Standalone SONIC — CMSSW Fork

This repo forks the `RecoTracker/LSTCore` subpackage from CMSSW and applies
customizations for using **LSTCore as a standalone algorithm** for
Inference as-a-Service (SONIC).

---

## Build `lst_cuda.so` and `lst_cpu.so`

Get the container for building backend and building LST standalone. If you already have the container, skip this step. 

```bash
singularity pull --disable-cache docker://y19y19/tritonserver_builder_gcc13:v4
```

Clone the repo

```bash
git clone -b CMSSW_16_1_0_pre4 https://github.com/y19y19/LST_alpaka_standalone_SONIC.git lst_standalone
```

Operate within the container

```bash
singularity run --nv -e --no-home -B <path_to>/lst_standalone/:/workspace/lst_standalone/ -B /cvmfs/:/cvmfs/ <path_to>/tritonserver_builder_gcc13_v4.sif
cd /workspace/lst_standalone/RecoTracker/LSTCore/standalone
source setup.sh CMSSW_16_1_0_pre4 el8_amd64_gcc13
lst_make_tracklooper -m
```

The compiled libraries `lst_cuda.so` and `lst_cpu.so` will be located at:

```
RecoTracker/LSTCore/standalone/LST/
```

---

## Set Up This Repo from CMSSW

### 1. Set the CMSSW tag

```bash
export CMSSW_TAG=CMSSW_16_1_0_pre4
```

### 2. Clone with sparse checkout (LSTCore only)

```bash
git clone --filter=blob:none --no-checkout --depth 1 --sparse \
  --branch ${CMSSW_TAG} \
  https://github.com/cms-sw/cmssw.git cmssw
cd cmssw

git sparse-checkout set RecoTracker/LSTCore
git checkout
```

### 3. Fix `.gitignore`

Open `RecoTracker/LSTCore/standalone/.gitignore` and remove the `performance*` line:

```bash
vi RecoTracker/LSTCore/standalone/.gitignore
# remove: performance*
```

### 4. Add your fork as a remote

```bash
git remote add myfork git@github.com:y19y19/LST_alpaka_standalone_SONIC.git
```

### 5. Create an orphan branch with clean history

```bash
git checkout --orphan ${CMSSW_TAG}
```

### 6. Stage only LSTCore and make the base commit

```bash
git reset HEAD -- .
git add RecoTracker/LSTCore/
git commit -m "Base: CMSSW_16_1_0_pre4 RecoTracker/LSTCore (no history)"
```

### 7. Push to your fork

```bash
git push myfork refs/heads/${CMSSW_TAG}:refs/heads/${CMSSW_TAG}
```

For future development, git clone the branch from `myfork`.
