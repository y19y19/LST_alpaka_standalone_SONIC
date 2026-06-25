#ifndef RecoTracker_LSTCore_interface_alpaka_LST_h
#define RecoTracker_LSTCore_interface_alpaka_LST_h

#include "RecoTracker/LSTCore/interface/alpaka/Common.h"
#include "RecoTracker/LSTCore/interface/LSTESData.h"
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

    std::unique_ptr<TrackCandidatesBaseDeviceCollection> getTrackCandidates() {
      return std::move(trackCandidatesBaseDC_);
    }

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
    std::unique_ptr<TrackCandidatesBaseDeviceCollection> trackCandidatesBaseDC_;
    unsigned int output_nTrackCandidates_ = 0;
    std::vector<unsigned int> output_pixelSeedIndex_;
    std::vector<int8_t> output_trackCandidateType_;
    std::vector<unsigned int> output_hitIndices_flat_;
    // Raw pointer to PIMPL struct; sized as a single pointer in all views.
    LSTRunCache* runCache_ = nullptr;
  };

}  // namespace ALPAKA_ACCELERATOR_NAMESPACE::lst

#endif
