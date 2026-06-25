#ifndef RecoTracker_LSTCore_interface_TrackCandidatesDeviceCollection_h
#define RecoTracker_LSTCore_interface_TrackCandidatesDeviceCollection_h

#include "RecoTracker/LSTCore/interface/alpaka/Common.h"
#include "RecoTracker/LSTCore/interface/TrackCandidatesSoA.h"

#ifndef __NVCC__
// PortableCollection.h → PortableDeviceCollection.h has SoABlocks constructors with
// a C++20 constraint evaluation bug in nvcc: it evaluates the right-hand side of &&
// even when the left-hand side (requires { Layout::blocksNumber; }) is false.
// Only include when compiling with g++ where the constraint short-circuits correctly.
#include "DataFormats/Portable/interface/alpaka/PortableCollection.h"

namespace ALPAKA_ACCELERATOR_NAMESPACE::lst {
  using TrackCandidatesBaseDeviceCollection = PortableCollection<TrackCandidatesBaseSoA>;
  using TrackCandidatesExtendedDeviceCollection = PortableCollection<TrackCandidatesExtendedSoA>;
}  // namespace ALPAKA_ACCELERATOR_NAMESPACE::lst
#endif  // __NVCC__

#endif
