Introduction to CMSSW: http://cms-sw.github.io

---

# LST Standalone SONIC — CMSSW Fork

This repo forks the `RecoTracker/LSTCore` subpackage from CMSSW and applies
customizations for using **LSTCore as a standalone algorithm** for
Inference as-a-Service (SONIC).

---

## Setup Procedure

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

### 3. Add your fork as a remote

```bash
git remote add myfork git@github.com:y19y19/LST_standalone_SONIC.git
```

### 4. Create an orphan branch with clean history

```bash
git checkout --orphan ${CMSSW_TAG}
```

### 5. Stage only LSTCore and make the base commit

```bash
git reset HEAD -- .
git add RecoTracker/LSTCore/
git commit -m "Base: CMSSW_16_1_0_pre4 RecoTracker/LSTCore (no history)"
```

### 6. Push to your fork

```bash
git push myfork refs/heads/${CMSSW_TAG}:refs/heads/${CMSSW_TAG}
```

### 7. Clean up the local tag

```bash
git tag -d CMSSW_16_1_0_pre4
```

### 8. Set upstream for future pushes

```bash
git push --set-upstream myfork CMSSW_16_1_0_pre4
```
