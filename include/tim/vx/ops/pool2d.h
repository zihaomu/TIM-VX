/****************************************************************************
*
*    Copyright (c) 2020 Vivante Corporation
*
*    Permission is hereby granted, free of charge, to any person obtaining a
*    copy of this software and associated documentation files (the "Software"),
*    to deal in the Software without restriction, including without limitation
*    the rights to use, copy, modify, merge, publish, distribute, sublicense,
*    and/or sell copies of the Software, and to permit persons to whom the
*    Software is furnished to do so, subject to the following conditions:
*
*    The above copyright notice and this permission notice shall be included in
*    all copies or substantial portions of the Software.
*
*    THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
*    IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
*    FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
*    AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
*    LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING
*    FROM, OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER
*    DEALINGS IN THE SOFTWARE.
*
*****************************************************************************/
#ifndef TIM_VX_OPS_POOL2D_H_
#define TIM_VX_OPS_POOL2D_H_

#include <array>

#include "tim/vx/operation.h"
#include "tim/vx/types.h"

namespace tim {
namespace vx {
namespace ops {

/**
 * ## Pool2d
 *
 * ### Classic Pool2d
 * 
 * Performs an 2-D pooling operation.
 *
 * - type : MAX, AVG, L2 or AVG_ANDROID.
 * - padding : AUTO, VALID or SAME.
 * - ksize : filter size.
 * - stride : stride along each spatial axis.
 * - round_type : CEILING or FLOOR.
 * 
 * ### Global Pool2d
 * 
 * - type : MAX, AVG, L2 or AVG_ANDROID.
 * - input_size : input size(only [W， H])
 * - round_type : CEILING or FLOOR.
 * 
 * ### Adaptive Pool2d
 * 
 * Same as torch.nn.AdaptiveXXXPool2d.
 * 
 * - type : MAX, AVG, L2 or AVG_ANDROID.
 * - input_size : input size(only [W， H])
 * - output_size : output size(only [W， H])
 * - round_type : CEILING or FLOOR.
 * 
 */

class Pool2d : public Operation {
 public:
  // for Classic Pool2d
  Pool2d(Graph* graph, PoolType type, PadType padding,
         const std::array<uint32_t, 2>& ksize,
         const std::array<uint32_t, 2>& stride,
         RoundType round_type = RoundType::FLOOR,
         DataLayout layout = DataLayout::WHCN);
  Pool2d(Graph* graph, PoolType type,
         const std::array<uint32_t, 4>& pad,
         const std::array<uint32_t, 2>& ksize,
         const std::array<uint32_t, 2>& stride,
         RoundType round_type = RoundType::FLOOR,
         DataLayout layout = DataLayout::WHCN);

  // for Global Pool2d
  Pool2d(Graph* graph, PoolType type,
         const std::array<uint32_t, 2>& input_size,
         RoundType round_type = RoundType::FLOOR,
         DataLayout layout = DataLayout::WHCN);

  // for Adaptive Pool2d
  Pool2d(Graph* graph, PoolType type,
         const std::array<uint32_t, 2>& input_size,
         const std::array<uint32_t, 2>& output_size,
         RoundType round_type = RoundType::FLOOR,
         DataLayout layout = DataLayout::WHCN);

  std::shared_ptr<Operation> Clone(std::shared_ptr<Graph>& graph) const override;
  void Init();

 protected:
  const PoolType type_;
  const PadType padding_;
  std::array<uint32_t, 2> ksize_;
  std::array<uint32_t, 2> stride_;
  const RoundType round_type_;
  const std::array<uint32_t, 4> pad_;
};

}  // namespace ops
}  // namespace vx
}  // namespace tim

#endif /* TIM_VX_OPS_POOL2D_H_ */