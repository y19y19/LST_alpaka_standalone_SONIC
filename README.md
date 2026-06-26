Introduction to CMSSW: http://cms-sw.github.io

---

# LST Standalone SONIC — CMSSW Fork

This repo forks the `RecoTracker/LSTCore` subpackage from CMSSW and applies
customizations for using **LSTCore as a standalone algorithm** for
Inference as-a-Service (SONIC).

---

## Build `lst_cuda.so` and `lst_cpu.so`

Get the container for building backend and building LST standlone. If you already have the container, skip this step. 

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

## This Branch

This branch contains changes to make the **LST Alpaka Triton backend compile successfully**.

To use it:

1. Copy the code from this branch into your backend's area.

2. Copy `lst_cuda.so` and `lst_cpu.so` — compiled from the `CMSSW_16_1_0_pre4` branch
   (see **Build** section above) — into the corresponding location:

```
RecoTracker/LSTCore/standalone/LST/
```
