#ifndef RecoTracker_LSTCore_interface_alpaka_LST_h
#define RecoTracker_LSTCore_interface_alpaka_LST_h

#include "RecoTracker/LSTCore/interface/alpaka/Common.h"
#include "../LSTESData.h" // YY: maybe can be changed back? 
#include "RecoTracker/LSTCore/interface/alpaka/LSTInputDeviceCollection.h"
#include "RecoTracker/LSTCore/interface/alpaka/TrackCandidatesDeviceCollection.h"
#include "RecoTracker/LSTCore/interface/TrackCandidatesHostCollection.h"
#include "RecoTracker/LSTCore/interface/LSTPrepareInput.h"

#include <cstdlib>
#include <cstdint>
#include <map>
#include <string>
#include <vector>
#include <numeric>
#include <alpaka/alpaka.hpp>

namespace ALPAKA_ACCELERATOR_NAMESPACE::lst {
  class LSTEvent;
  struct LSTRunCache;  // PIMPL: queue + ESData cache, defined in LST.cc

  class LST {
  public:
    LST() = default;
    ~LST();  // non-inline: defined in LST.cc, owns runCache_ lifetime

    void run(Queue& queue,
             bool verbose,
             const float ptCut,
             const uint16_t clustSizeCut,
             LSTESData<Device> const* deviceESData,
             LSTInputDeviceCollection const* lstInputDC,
             bool no_pls_dupclean,
             bool tc_pls_triplets);

#ifndef __NVCC__
    std::unique_ptr<TrackCandidatesBaseDeviceCollection> getTrackCandidates() {
      return std::move(trackCandidatesBaseDC_);
    }
#endif

    void run_SONIC(bool verbose,
                   std::map<std::string, const void*> inputs_name_buffer,
                   std::map<std::string, uint32_t> inputs_name_buffer_byte_size,
                   bool no_pls_dupclean,
                   bool tc_pls_triplets);

    const unsigned int& getOutput_nTrackCandidates() const { return output_nTrackCandidates_; }
    const std::vector<unsigned int>& getOutput_pixelSeedIndex() const { return output_pixelSeedIndex_; }
    const std::vector<int8_t>& getOutput_trackCandidateType() const { return output_trackCandidateType_; }
    const std::vector<unsigned int>& getOutput_hitIndices_flat() const { return output_hitIndices_flat_; }

    size_t getOutputBufferSize_nTrackCandidates() const { return sizeof(unsigned int); }
    size_t getOutputBufferSize_pixelSeedIndex() const { return output_pixelSeedIndex_.size() * sizeof(unsigned int); }
    size_t getOutputBufferSize_trackCandidateType() const { return output_trackCandidateType_.size() * sizeof(int8_t); }
    size_t getOutputBufferSize_hitIndices_flat() const { return output_hitIndices_flat_.size() * sizeof(unsigned int); }

  private:
#ifndef __NVCC__
    // Full type used by g++-compiled code (LSTRunnerCPU.cc, liblst_cuda.so).
    std::unique_ptr<TrackCandidatesBaseDeviceCollection> trackCandidatesBaseDC_;
#else
    // ABI-compatible placeholder for nvcc: sizeof(unique_ptr<T>) == sizeof(void*).
    // run_SONIC() in liblst_cuda.so always moves trackCandidatesBaseDC_ into a local
    // before returning, so this slot is null when our nvcc destructor runs — no leak.
    alignas(alignof(void*)) unsigned char trackCandidatesBaseDC_[sizeof(void*)];
#endif
    unsigned int output_nTrackCandidates_ = 0;
    std::vector<unsigned int> output_pixelSeedIndex_;
    std::vector<int8_t> output_trackCandidateType_;
    std::vector<unsigned int> output_hitIndices_flat_;
    // Raw pointer to PIMPL struct; sizeof(void*) in all compiler views — safe with
    // the __NVCC__ placeholder pattern above (no constraint-template instantiation).
    LSTRunCache* runCache_ = nullptr;
  };

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE::lst

#endif
