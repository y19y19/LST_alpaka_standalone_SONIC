Introduction to CMSSW: http://cms-sw.github.io

---

# LST Standalone SONIC — CMSSW Fork

This repo forks the `RecoTracker/LSTCore` subpackage from CMSSW and applies
customizations for using **LSTCore as a standalone algorithm** for
Inference as-a-Service (SONIC).

---

## Build `lst_cuda.so` and `lst_cpu.so`

Run these steps on a machine with access to `/cvmfs/`.

```bash
git clone -b CMSSW_16_1_0_pre4 https://github.com/y19y19/LST_alpaka_standalone_SONIC.git
cd LST_alpaka_standalone_SONIC/RecoTracker/LSTCore/standalone/
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
