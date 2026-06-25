#include "RecoTracker/LSTCore/interface/alpaka/LST.h"

#include "LSTEvent.h"

#include <format>
#include <optional>

using namespace ALPAKA_ACCELERATOR_NAMESPACE::lst;

#include "Math/Vector3D.h"
#include "Math/VectorUtil.h"
using XYZVector = ROOT::Math::XYZVector;

// PIMPL struct: holds per-instance cached resources to avoid per-event overhead.
// Defined here (g++ only) so alpaka/CMSSW types never appear in LST.h.
namespace ALPAKA_ACCELERATOR_NAMESPACE::lst {
struct LSTRunCache {
    std::optional<Queue> queue;
    std::unique_ptr<LSTESData<alpaka_common::DevHost>> hostESData;
    std::string ptCutString;
};
LST::~LST() { delete runCache_; }
}  // namespace ALPAKA_ACCELERATOR_NAMESPACE::lst

void LST::run(Queue& queue,
              bool verbose,
              float const ptCut,
              uint16_t const clustSizeCut,
              LSTESData<Device> const* deviceESData,
              LSTInputDeviceCollection const* lstInputDC,
              bool no_pls_dupclean,
              bool tc_pls_triplets) {
  auto event = LSTEvent(verbose, ptCut, clustSizeCut, queue, deviceESData);

  event.addInputToEvent(lstInputDC);
  event.addHitToEvent();
  event.addPixelSegmentToEventStart();
  event.createMiniDoublets();
  if (verbose) {
    alpaka::wait(queue);  // event calls are asynchronous: wait before printing
    printf("# of Mini-doublets produced: %d\n", event.getNumberOfMiniDoublets());
    printf("# of Mini-doublets produced barrel layer 1: %d\n", event.getNumberOfMiniDoubletsByLayerBarrel(0));
    printf("# of Mini-doublets produced barrel layer 2: %d\n", event.getNumberOfMiniDoubletsByLayerBarrel(1));
    printf("# of Mini-doublets produced barrel layer 3: %d\n", event.getNumberOfMiniDoubletsByLayerBarrel(2));
    printf("# of Mini-doublets produced barrel layer 4: %d\n", event.getNumberOfMiniDoubletsByLayerBarrel(3));
    printf("# of Mini-doublets produced barrel layer 5: %d\n", event.getNumberOfMiniDoubletsByLayerBarrel(4));
    printf("# of Mini-doublets produced barrel layer 6: %d\n", event.getNumberOfMiniDoubletsByLayerBarrel(5));
    printf("# of Mini-doublets produced endcap layer 1: %d\n", event.getNumberOfMiniDoubletsByLayerEndcap(0));
    printf("# of Mini-doublets produced endcap layer 2: %d\n", event.getNumberOfMiniDoubletsByLayerEndcap(1));
    printf("# of Mini-doublets produced endcap layer 3: %d\n", event.getNumberOfMiniDoubletsByLayerEndcap(2));
    printf("# of Mini-doublets produced endcap layer 4: %d\n", event.getNumberOfMiniDoubletsByLayerEndcap(3));
    printf("# of Mini-doublets produced endcap layer 5: %d\n", event.getNumberOfMiniDoubletsByLayerEndcap(4));
  }

  event.createSegmentsWithModuleMap();
  if (verbose) {
    alpaka::wait(queue);  // event calls are asynchronous: wait before printing
    printf("# of Segments produced: %d\n", event.getNumberOfSegments());
    printf("# of Segments produced layer 1-2:  %d\n", event.getNumberOfSegmentsByLayerBarrel(0));
    printf("# of Segments produced layer 2-3:  %d\n", event.getNumberOfSegmentsByLayerBarrel(1));
    printf("# of Segments produced layer 3-4:  %d\n", event.getNumberOfSegmentsByLayerBarrel(2));
    printf("# of Segments produced layer 4-5:  %d\n", event.getNumberOfSegmentsByLayerBarrel(3));
    printf("# of Segments produced layer 5-6:  %d\n", event.getNumberOfSegmentsByLayerBarrel(4));
    printf("# of Segments produced endcap layer 1:  %d\n", event.getNumberOfSegmentsByLayerEndcap(0));
    printf("# of Segments produced endcap layer 2:  %d\n", event.getNumberOfSegmentsByLayerEndcap(1));
    printf("# of Segments produced endcap layer 3:  %d\n", event.getNumberOfSegmentsByLayerEndcap(2));
    printf("# of Segments produced endcap layer 4:  %d\n", event.getNumberOfSegmentsByLayerEndcap(3));
    printf("# of Segments produced endcap layer 5:  %d\n", event.getNumberOfSegmentsByLayerEndcap(4));
  }

  event.createTriplets();
  if (verbose) {
    alpaka::wait(queue);  // event calls are asynchronous: wait before printing
    printf("# of T3s produced: %d\n", event.getNumberOfTriplets());
    printf("# of T3s produced layer 1-2-3: %d\n", event.getNumberOfTripletsByLayerBarrel(0));
    printf("# of T3s produced layer 2-3-4: %d\n", event.getNumberOfTripletsByLayerBarrel(1));
    printf("# of T3s produced layer 3-4-5: %d\n", event.getNumberOfTripletsByLayerBarrel(2));
    printf("# of T3s produced layer 4-5-6: %d\n", event.getNumberOfTripletsByLayerBarrel(3));
    printf("# of T3s produced endcap layer 1-2-3: %d\n", event.getNumberOfTripletsByLayerEndcap(0));
    printf("# of T3s produced endcap layer 2-3-4: %d\n", event.getNumberOfTripletsByLayerEndcap(1));
    printf("# of T3s produced endcap layer 3-4-5: %d\n", event.getNumberOfTripletsByLayerEndcap(2));
    printf("# of T3s produced endcap layer 1: %d\n", event.getNumberOfTripletsByLayerEndcap(0));
    printf("# of T3s produced endcap layer 2: %d\n", event.getNumberOfTripletsByLayerEndcap(1));
    printf("# of T3s produced endcap layer 3: %d\n", event.getNumberOfTripletsByLayerEndcap(2));
    printf("# of T3s produced endcap layer 4: %d\n", event.getNumberOfTripletsByLayerEndcap(3));
    printf("# of T3s produced endcap layer 5: %d\n", event.getNumberOfTripletsByLayerEndcap(4));
  }

  event.createQuintuplets();
  if (verbose) {
    alpaka::wait(queue);  // event calls are asynchronous: wait before printing
    printf("# of Quintuplets produced: %d\n", event.getNumberOfQuintuplets());
    printf("# of Quintuplets produced layer 1-2-3-4-5-6: %d\n", event.getNumberOfQuintupletsByLayerBarrel(0));
    printf("# of Quintuplets produced layer 2: %d\n", event.getNumberOfQuintupletsByLayerBarrel(1));
    printf("# of Quintuplets produced layer 3: %d\n", event.getNumberOfQuintupletsByLayerBarrel(2));
    printf("# of Quintuplets produced layer 4: %d\n", event.getNumberOfQuintupletsByLayerBarrel(3));
    printf("# of Quintuplets produced layer 5: %d\n", event.getNumberOfQuintupletsByLayerBarrel(4));
    printf("# of Quintuplets produced layer 6: %d\n", event.getNumberOfQuintupletsByLayerBarrel(5));
    printf("# of Quintuplets produced endcap layer 1: %d\n", event.getNumberOfQuintupletsByLayerEndcap(0));
    printf("# of Quintuplets produced endcap layer 2: %d\n", event.getNumberOfQuintupletsByLayerEndcap(1));
    printf("# of Quintuplets produced endcap layer 3: %d\n", event.getNumberOfQuintupletsByLayerEndcap(2));
    printf("# of Quintuplets produced endcap layer 4: %d\n", event.getNumberOfQuintupletsByLayerEndcap(3));
    printf("# of Quintuplets produced endcap layer 5: %d\n", event.getNumberOfQuintupletsByLayerEndcap(4));
  }

  event.addPixelSegmentToEventFinalize();

  event.pixelLineSegmentCleaning(no_pls_dupclean);

  event.createPixelQuintuplets();
  if (verbose) {
    alpaka::wait(queue);  // event calls are asynchronous: wait before printing
    printf("# of Pixel Quintuplets produced: %d\n", event.getNumberOfPixelQuintuplets());
  }

  event.createPixelTriplets();
  if (verbose) {
    alpaka::wait(queue);  // event calls are asynchronous: wait before printing
    printf("# of Pixel T3s produced: %d\n", event.getNumberOfPixelTriplets());
  }

  event.createQuadruplets();
  if (verbose) {
    alpaka::wait(queue);  // event calls are asynchronous: wait before printing
    printf("# of Quadruplets produced: %d\n", event.getNumberOfQuadruplets());
    printf("# of Quadruplets produced layer 1-2-3-4: %d\n", event.getNumberOfQuadrupletsByLayerBarrel(0));
    printf("# of Quadruplets produced layer 2: %d\n", event.getNumberOfQuadrupletsByLayerBarrel(1));
    printf("# of Quadruplets produced layer 3: %d\n", event.getNumberOfQuadrupletsByLayerBarrel(2));
    printf("# of Quadruplets produced layer 4: %d\n", event.getNumberOfQuadrupletsByLayerBarrel(3));
    printf("# of Quadruplets produced layer 5: %d\n", event.getNumberOfQuadrupletsByLayerBarrel(4));
    printf("# of Quadruplets produced layer 6: %d\n", event.getNumberOfQuadrupletsByLayerBarrel(5));
    printf("# of Quadruplets produced endcap layer 1: %d\n", event.getNumberOfQuadrupletsByLayerEndcap(0));
    printf("# of Quadruplets produced endcap layer 2: %d\n", event.getNumberOfQuadrupletsByLayerEndcap(1));
    printf("# of Quadruplets produced endcap layer 3: %d\n", event.getNumberOfQuadrupletsByLayerEndcap(2));
    printf("# of Quadruplets produced endcap layer 4: %d\n", event.getNumberOfQuadrupletsByLayerEndcap(3));
    printf("# of Quadruplets produced endcap layer 5: %d\n", event.getNumberOfQuadrupletsByLayerEndcap(4));
  }

  event.createTrackCandidates(no_pls_dupclean, tc_pls_triplets);
  if (verbose) {
    alpaka::wait(queue);  // event calls are asynchronous: wait before printing
    printf("# of TrackCandidates produced: %d\n", event.getNumberOfTrackCandidates());
    printf("        # of Pixel TrackCandidates produced: %d\n", event.getNumberOfPixelTrackCandidates());
    printf("        # of pT5 TrackCandidates produced: %d\n", event.getNumberOfPT5TrackCandidates());
    printf("        # of pT3 TrackCandidates produced: %d\n", event.getNumberOfPT3TrackCandidates());
    printf("        # of pLS TrackCandidates produced: %d\n", event.getNumberOfPLSTrackCandidates());
    printf("        # of T5 TrackCandidates produced: %d\n", event.getNumberOfT5TrackCandidates());
    printf("        # of T4 TrackCandidates produced: %d\n", event.getNumberOfT4TrackCandidates());
    lstWarning(std::format("[MEM] Total: {:.1f} MB", event.getMemoryAllocatedMB()));
  }

  trackCandidatesBaseDC_ = event.releaseTrackCandidatesBaseDeviceCollection();
}


void LST::run_SONIC(bool verbose,
                    std::map<std::string, const void*> inputs_name_buffer,
                    std::map<std::string, uint32_t> inputs_name_buffer_byte_size,
                    bool no_pls_dupclean,
                    bool tc_pls_triplets) {
    // Clear outputs from any previous invocation to avoid accumulation across events.
    output_nTrackCandidates_ = 0;
    output_pixelSeedIndex_.clear();
    output_trackCandidateType_.clear();
    output_hitIndices_flat_.clear();

    const float* see_px_ptr = static_cast<const float*>(inputs_name_buffer["see_px"]);
    const float* see_py_ptr = static_cast<const float*>(inputs_name_buffer["see_py"]);
    const float* see_pz_ptr = static_cast<const float*>(inputs_name_buffer["see_pz"]);
    const float* see_dxy_ptr = static_cast<const float*>(inputs_name_buffer["see_dxy"]);
    const float* see_dz_ptr = static_cast<const float*>(inputs_name_buffer["see_dz"]);
    const float* see_ptErr_ptr = static_cast<const float*>(inputs_name_buffer["see_ptErr"]);
    const float* see_etaErr_ptr = static_cast<const float*>(inputs_name_buffer["see_etaErr"]);
    const float* see_stateTrajGlbX_ptr = static_cast<const float*>(inputs_name_buffer["see_stateTrajGlbX"]);
    const float* see_stateTrajGlbY_ptr = static_cast<const float*>(inputs_name_buffer["see_stateTrajGlbY"]);
    const float* see_stateTrajGlbZ_ptr = static_cast<const float*>(inputs_name_buffer["see_stateTrajGlbZ"]);
    const float* see_stateTrajGlbPx_ptr = static_cast<const float*>(inputs_name_buffer["see_stateTrajGlbPx"]);
    const float* see_stateTrajGlbPy_ptr = static_cast<const float*>(inputs_name_buffer["see_stateTrajGlbPy"]);
    const float* see_stateTrajGlbPz_ptr = static_cast<const float*>(inputs_name_buffer["see_stateTrajGlbPz"]);
    const int* see_q_ptr = static_cast<const int*>(inputs_name_buffer["see_q"]);
    const int* see_hit_size_ptr = static_cast<const int*>(inputs_name_buffer["see_hit_size"]);
    const int* see_hitIdx_ptr = static_cast<const int*>(inputs_name_buffer["see_hitIdx"]);
    const int* see_hitType_ptr = static_cast<const int*>(inputs_name_buffer["see_hitType"]);
    const unsigned int* see_algo_ptr = static_cast<const unsigned int*>(inputs_name_buffer["see_algo"]);
    const unsigned int* ph2_detId_ptr = static_cast<const unsigned int*>(inputs_name_buffer["ph2_detId"]);
    const uint16_t* ph2_clustSize_ptr = static_cast<const uint16_t*>(inputs_name_buffer["ph2_clustSize"]);
    const float* ph2_x_ptr = static_cast<const float*>(inputs_name_buffer["ph2_x"]);
    const float* ph2_y_ptr = static_cast<const float*>(inputs_name_buffer["ph2_y"]);
    const float* ph2_z_ptr = static_cast<const float*>(inputs_name_buffer["ph2_z"]);

    const float* ptCut_ptr = static_cast<const float*>(inputs_name_buffer["ptCut"]);
    const uint16_t* clustSizeCut_ptr = static_cast<const uint16_t*>(inputs_name_buffer["clustSizeCut"]);

    int N_seed = inputs_name_buffer_byte_size["see_px"] / sizeof(float);
    std::vector<float> see_px(see_px_ptr, see_px_ptr + N_seed);
    std::vector<float> see_py(see_py_ptr, see_py_ptr + N_seed);
    std::vector<float> see_pz(see_pz_ptr, see_pz_ptr + N_seed);
    std::vector<float> see_dxy(see_dxy_ptr, see_dxy_ptr + N_seed);
    std::vector<float> see_dz(see_dz_ptr, see_dz_ptr + N_seed);
    std::vector<float> see_ptErr(see_ptErr_ptr, see_ptErr_ptr + N_seed);
    std::vector<float> see_etaErr(see_etaErr_ptr, see_etaErr_ptr + N_seed);
    std::vector<float> see_stateTrajGlbX(see_stateTrajGlbX_ptr, see_stateTrajGlbX_ptr + N_seed);
    std::vector<float> see_stateTrajGlbY(see_stateTrajGlbY_ptr, see_stateTrajGlbY_ptr + N_seed);
    std::vector<float> see_stateTrajGlbZ(see_stateTrajGlbZ_ptr, see_stateTrajGlbZ_ptr + N_seed);
    std::vector<float> see_stateTrajGlbPx(see_stateTrajGlbPx_ptr, see_stateTrajGlbPx_ptr + N_seed);
    std::vector<float> see_stateTrajGlbPy(see_stateTrajGlbPy_ptr, see_stateTrajGlbPy_ptr + N_seed);
    std::vector<float> see_stateTrajGlbPz(see_stateTrajGlbPz_ptr, see_stateTrajGlbPz_ptr + N_seed);

    std::vector<int> see_q(see_q_ptr, see_q_ptr + N_seed);
    std::vector<int> see_hit_size(see_hit_size_ptr, see_hit_size_ptr + N_seed);
    int N_hit = 0;
    for (int number : see_hit_size) N_hit += number;
    std::vector<int> see_hitIdx_flat(see_hitIdx_ptr, see_hitIdx_ptr + N_hit);
    std::vector<int> see_hitType_flat(see_hitType_ptr, see_hitType_ptr + N_hit);
    std::vector<std::vector<int>> see_hitIdx;
    std::vector<std::vector<int>> see_hitType;
    size_t index = 0;
    for (int size : see_hit_size) {
        see_hitIdx.emplace_back(see_hitIdx_flat.begin() + index, see_hitIdx_flat.begin() + index + size);
        see_hitType.emplace_back(see_hitType_flat.begin() + index, see_hitType_flat.begin() + index + size);
        index += size;
    }
    std::vector<unsigned int> see_algo(see_algo_ptr, see_algo_ptr + N_seed);
    int N_ph2 = inputs_name_buffer_byte_size["ph2_x"] / sizeof(float);
    std::vector<unsigned int> ph2_detId(ph2_detId_ptr, ph2_detId_ptr + N_ph2);
    std::vector<uint16_t> ph2_clustSize(ph2_clustSize_ptr, ph2_clustSize_ptr + N_ph2);
    std::vector<float> ph2_x(ph2_x_ptr, ph2_x_ptr + N_ph2);
    std::vector<float> ph2_y(ph2_y_ptr, ph2_y_ptr + N_ph2);
    std::vector<float> ph2_z(ph2_z_ptr, ph2_z_ptr + N_ph2);
    float ptCut = *ptCut_ptr;
    uint16_t clustSizeCut = *clustSizeCut_ptr;

    // Create the alpaka queue once per LST instance and reuse it across events.
    if (!runCache_) {
        runCache_ = new ALPAKA_ACCELERATOR_NAMESPACE::lst::LSTRunCache();
        ALPAKA_ACCELERATOR_NAMESPACE::Device devAcc = alpaka::getDevByIdx(ALPAKA_ACCELERATOR_NAMESPACE::Platform{}, 0u);
        runCache_->queue.emplace(devAcc);
    }
    ALPAKA_ACCELERATOR_NAMESPACE::Queue& queue_ = *runCache_->queue;

    LSTInputHostCollection lstInputHC =
      prepareInput(see_px,
                   see_py,
                   see_pz,
                   see_dxy,
                   see_dz,
                   see_ptErr,
                   see_etaErr,
                   see_stateTrajGlbX,
                   see_stateTrajGlbY,
                   see_stateTrajGlbZ,
                   see_stateTrajGlbPx,
                   see_stateTrajGlbPy,
                   see_stateTrajGlbPz,
                   see_q,
                   see_hitIdx,
                   see_hitType,
                   see_algo,
                   ph2_detId,
                   ph2_clustSize,
                   ph2_x,
                   ph2_y,
                   ph2_z,
                   ptCut,
                   queue_);

    // Load geometry from disk only on the first call or when ptCut category changes.
    // The H2D copy still happens every event; only the expensive disk read is cached.
    std::string ptCutString = (ptCut >= 0.8) ? "0.8" : "0.6";
    if (!runCache_->hostESData || runCache_->ptCutString != ptCutString) {
        runCache_->hostESData = loadAndFillESHost(ptCutString);
        runCache_->ptCutString = ptCutString;
    }

    auto deviceESData =
      cms::alpakatools::CopyToDevice<LSTESData<alpaka_common::DevHost>>::copyAsync(queue_, *runCache_->hostESData);
    // Copy input from Host to Device
    LSTInputDeviceCollection lstInputDC(queue_, lstInputHC->metadata().size());
    alpaka::memcpy(queue_, lstInputDC.buffer(), lstInputHC.buffer());
    alpaka::wait(queue_);

    // Run LST
    run(queue_,
        verbose,
        ptCut,
        clustSizeCut,
        &deviceESData,
        &lstInputDC,
        no_pls_dupclean,
        tc_pls_triplets
    );

    std::unique_ptr<TrackCandidatesBaseDeviceCollection> lstOutputDC_ptr = getTrackCandidates();

    TrackCandidatesBaseHostCollection lstOutputHC(queue_, lstOutputDC_ptr->size());
    alpaka::memcpy(queue_, lstOutputHC.buffer(), lstOutputDC_ptr->buffer());
    alpaka::wait(queue_);

    // Extract outputs from the host collection
    auto lstOutput_view = lstOutputHC.const_view();
    output_nTrackCandidates_ = lstOutput_view.nTrackCandidates();
    for (unsigned int i = 0; i < output_nTrackCandidates_; i++) {
        output_pixelSeedIndex_.push_back(lstOutput_view.pixelSeedIndex()[i]);
        output_trackCandidateType_.push_back(lstOutput_view.trackCandidateType()[i]);
        for (unsigned int layerSlot = 0; layerSlot < lst::Params_TC::kLayers; ++layerSlot) {
            for (unsigned int hitSlot = 0; hitSlot < lst::Params_TC::kHitsPerLayer; ++hitSlot) {
                output_hitIndices_flat_.push_back(lstOutput_view.hitIndices()[i][layerSlot][hitSlot]);
            }
        }
    }
}
